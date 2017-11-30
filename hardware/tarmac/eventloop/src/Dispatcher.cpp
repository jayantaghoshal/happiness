#include <cutils/log.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <cerrno>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <vector>

#include "IDispatcher.h"

#undef LOG_TAG
#define LOG_TAG "tarmac.dispatcher"

#if !defined(EFD_SEMAPHORE)
#define EFD_SEMAPHORE (1 << 0)
#endif

/***********************************************************************************
 * Implements an event/main loop class that can handle to dispatch lambdas as well *
 * as monitoring file descriptors for available in-data.                           *
 ***********************************************************************************/

namespace tarmac {
namespace eventloop {

using Task = std::function<void()>;

class EPollQueue {
  public:
    EPollQueue() {
        epollfd_ = epoll_create1(0);
        ALOGE_IF(epollfd_ < 0, "Failed to create epollfd: %s", strerror(errno));

        eventfd_ = eventfd(0, EFD_SEMAPHORE);
        ALOGE_IF(eventfd_ < 0, "Failed to create eventfd: %s", strerror(errno));

        epoll_event event;
        event.events = EPOLLIN;
        event.data.ptr = nullptr;  // this indicates a "normal" enqueue task
        int r = epoll_ctl(epollfd_, EPOLL_CTL_ADD, eventfd_, &event);
        ALOGE_IF(r != 0, "EPOLL_CTL_ADD failed: %s", strerror(errno));
    }

    virtual ~EPollQueue() {
        close(eventfd_);
        close(epollfd_);
    }

    void enqueue(Task &&t) {
        {  // lock scope
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.emplace(t);
        }
        const uint64_t dummy = 1;
        int r = write(eventfd_, &dummy, sizeof(dummy));
        ALOGE_IF(r < 0, "write failed: %s", strerror(errno));
    }

    void addFd(int fd, Task &&t) {
        std::lock_guard<std::mutex> lock(mutex_);
        int r = 0;

        // fd already added so lets remove it first
        if (fdTasks_.find(fd) != fdTasks_.end()) {
            removeFd(fd);
        }

        epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = fd;
        r = epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);
        ALOGE_IF(r != 0, "EPOLL_CTL_ADD fd failed: %s", strerror(errno));

        if (r == 0) {
            fdTasks_[fd] = t;
        }
    }

    bool removeFd(int fd) {
        epoll_event event;
        event.data.fd = fd;
        int r = epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event);
        ALOGE_IF(r != 0, "EPOLL_CTL_DEL fd failed: %s", strerror(errno));
        std::lock_guard<std::mutex> lock(mutex_);
        auto removed = fdTasks_.erase(fd);
        return removed > 0;
    }

    std::vector<Task> dequeue(void) {
        std::vector<Task> result;
        epoll_event events[MAX_EVENTS];
        int r = epoll_wait(epollfd_, events, MAX_EVENTS, -1);  // -1 -> wait forever
        if (r > 0) {
            // we got some stuff to work on
            for (int i = 0; i < r; ++i) {
                if (events[i].data.ptr == nullptr) {
                    // this is a "normal" enqueued task, lets get them all
                    int cnt = 0;
                    {  // lock scope
                        std::lock_guard<std::mutex> lock(mutex_);
                        while (!queue_.empty()) {
                            result.push_back(queue_.front());
                            queue_.pop();
                            cnt++;
                        }
                    }
                    // now read the same amount of times
                    uint64_t dummy;
                    while (cnt > 0) {
                        int r = read(eventfd_, &dummy, sizeof(dummy));  // one read for each write above
                        ALOGE_IF(r < 0, "read failed: %s", strerror(errno));
                        cnt--;
                    }
                } else {
                    std::lock_guard<std::mutex> lock(mutex_);
                    auto iter = fdTasks_.find(events[i].data.fd);
                    if (iter != fdTasks_.end()) {
                        result.push_back(iter->second);
                    }
                }
            }
        } else if (r < 0) {
            ALOGE("epoll_wait failed: %s", strerror(errno));
        }
        return result;
    }

  private:
    std::queue<Task> queue_;
    std::map<int, Task> fdTasks_;
    mutable std::mutex mutex_;
    int eventfd_;
    int epollfd_;
    const unsigned int MAX_EVENTS = 5;
};

class Dispatcher : public IDispatcher {
  public:
    Dispatcher();

    virtual ~Dispatcher();

    virtual void Enqueue(std::function<void()> &&f);

    virtual JobId EnqueueWithDelay(std::chrono::microseconds delay, std::function<void()> &&f);

    virtual bool Cancel(JobId jobid);

    virtual void AddFd(int fd, std::function<void()> &&f);
    virtual void RemoveFd(int fd);

    virtual void Stop();
    virtual void Join();

  private:
    void Start();

    EPollQueue queue_;
    bool stop_;
    std::thread eventthread_;
    JobId next_job_id_;
    mutable std::mutex mutex_;
    std::map<JobId, int> delayed_jobs_;
};

// static helpers
IDispatcher &IDispatcher::GetDefaultDispatcher() {
    static std::unique_ptr<IDispatcher> dispatcher = CreateDispatcher();

    return *dispatcher.get();
}

void IDispatcher::EnqueueTask(std::function<void()> &&f) { GetDefaultDispatcher().Enqueue(std::move(f)); }

std::unique_ptr<IDispatcher> IDispatcher::CreateDispatcher() { return std::unique_ptr<IDispatcher>(new Dispatcher()); }

// constructor
Dispatcher::Dispatcher() : stop_(false), eventthread_([this]() { Start(); }), next_job_id_(0) {}

// destructor
Dispatcher::~Dispatcher() { Stop(); }

void Dispatcher::Enqueue(std::function<void()> &&f) { queue_.enqueue(std::move(f)); }

IDispatcher::JobId Dispatcher::EnqueueWithDelay(std::chrono::microseconds delay, std::function<void()> &&f) {
    struct itimerspec ts;
    int tfd, usec;
    JobId this_id;

    usec = delay.count();
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = usec / 1000000;
    ts.it_value.tv_nsec = (usec % 1000000) * 1000;

    tfd = timerfd_create(CLOCK_MONOTONIC, 0);

    {  // lock scope
        std::lock_guard<std::mutex> lock(mutex_);
        this_id = next_job_id_++;      // obtain the new jobid
        delayed_jobs_[this_id] = tfd;  // and add this id to the set of running timers
    }

    timerfd_settime(tfd, 0, &ts, NULL);

    AddFd(tfd, [this, tfd, f, this_id]() {
        bool dispatch_job_now = false;
        {  // lock scope
            std::lock_guard<std::mutex> lock(mutex_);
            auto iter = delayed_jobs_.find(this_id);
            // see if job shall continue, which means it has NOT been cancelled
            dispatch_job_now = (iter != delayed_jobs_.end());
            if (dispatch_job_now) delayed_jobs_.erase(iter);  // job has not been cancelled
        }
        if (dispatch_job_now) {
            uint64_t dummy;
            int r = read(tfd, &dummy, sizeof(dummy));
            ALOGE_IF(r < 0, "read failed: %s", strerror(errno));
            f();
            RemoveFd(tfd);
            close(tfd);
        }
    });
    return this_id;
}

bool Dispatcher::Cancel(JobId jobid) {
    bool dispatch_job_now = false;
    int tfd = -1;
    {  // lock scope
        std::lock_guard<std::mutex> lock(mutex_);
        auto iter = delayed_jobs_.find(jobid);
        dispatch_job_now = (iter != delayed_jobs_.end());
        if (dispatch_job_now) {
            tfd = iter->second;
            delayed_jobs_.erase(iter);
        }
    }
    if (dispatch_job_now && queue_.removeFd(tfd)) {
        close(tfd);
        return true;
    }

    return false;
}

void Dispatcher::AddFd(int fd, std::function<void()> &&f) { queue_.addFd(fd, std::move(f)); }

void Dispatcher::RemoveFd(int fd) { queue_.removeFd(fd); }

// private
void Dispatcher::Start() {
    while (!stop_) {
        std::vector<Task> tasks = queue_.dequeue();  // Blocking call
        if (!stop_) {
            // do callbacks for all tasks
            for (const Task &t : tasks) {
                t();
            }
        }
    }
}

void Dispatcher::Stop() {
    stop_ = true;
    Enqueue([]() {});  // add a "stop pill" to the queue to make it wakeup and stop asap
    Join();
}

void Dispatcher::Join() {
    if (eventthread_.joinable() && (std::this_thread::get_id() != eventthread_.get_id())) {
        // Only call join if joinable and when we are not calling Join() from the dispatcher thread
        eventthread_.join();
    }
}
}  // end eventloop
}  // end tarmac
