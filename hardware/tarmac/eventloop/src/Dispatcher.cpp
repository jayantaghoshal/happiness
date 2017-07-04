#include <map>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <cutils/log.h>
#include <errno.h>
#include <pthread.h>

#include "IDispatcher.h"

#define LOG_TAG "dispatcher"

#if !defined(EFD_SEMAPHORE)
#define EFD_SEMAPHORE (1 << 0)
#endif

/***********************************************************************************
 * Implements an event/main loop class that can handle to dispatch lambdas as well *
 * as monitoring file descriptors for available in-data.                           *
 ***********************************************************************************/

namespace tarmac
{
namespace eventloop
{

typedef std::function<void()> Task;

class EPollQueue
{
public:
    EPollQueue() {
        epollfd_ = epoll_create1(0);
        ALOGE_IF(epollfd_<0, "Failed to create epollfd: %s", strerror(errno));

        eventfd_ = eventfd(0, EFD_SEMAPHORE);
        ALOGE_IF(eventfd_<0, "Failed to create eventfd: %s", strerror(errno));

        epoll_event event;
        event.events   = EPOLLIN;
        event.data.ptr = nullptr; // this indicates a "normal" enqueue task
        int r = epoll_ctl (epollfd_, EPOLL_CTL_ADD, eventfd_, &event);
        ALOGE_IF(r!=0, "EPOLL_CTL_ADD failed: %s", strerror(errno));
    }

    virtual ~EPollQueue() {
        close(eventfd_);
        close(epollfd_);
    }

    void enqueue(Task&& t)
    {
        { // lock scope
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.emplace(t);
        }
        const uint64_t dummy = 1;
        write(eventfd_, &dummy, sizeof(dummy));
    }

    void addFd(int fd, Task&& t)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        int r=0;
        if ( fdTasks_.find(fd)==fdTasks_.end() ) {
            // fd has not previously been added so lets do it
            epoll_event event;
            event.events  = EPOLLIN;
            event.data.fd = fd;
            r = epoll_ctl (epollfd_, EPOLL_CTL_ADD, fd, &event);
            ALOGE_IF(r!=0, "EPOLL_CTL_ADD fd failed: %s", strerror(errno));
        }
        if (r==0) {
            fdTasks_[fd] = t;
        }
    }

    void removeFd(int fd)
    {
        epoll_event event;
        event.data.fd = fd;
        int r = epoll_ctl (epollfd_, EPOLL_CTL_DEL, fd, &event);
        ALOGE_IF(r!=0, "EPOLL_CTL_DEL fd failed: %s", strerror(errno));
        std::lock_guard<std::mutex> lock(mutex_);
        fdTasks_.erase(fd);
    }

    std::vector<Task> dequeue(void)
    {
        std::vector<Task> result;
        epoll_event events[MAX_EVENTS];
        int r = epoll_wait(epollfd_, events, MAX_EVENTS, -1); // -1 -> wait forever
        if (r>0) {
            // we got some stuff to work on
            for (int i=0 ; i<r ; ++i) {
                if (events[i].data.ptr==nullptr) {
                    // this is a "normal" enqueued task, lets get them all
                    int cnt = 0;
                    { // lock scope
                        std::lock_guard<std::mutex> lock(mutex_);
                        while (!queue_.empty())
                        {
                            result.push_back(queue_.front());
                            queue_.pop();
                            cnt++;
                        }
                    }
                    // now read the same amount of times
                    uint64_t dummy;
                    while(cnt>0) {
                        read(eventfd_, &dummy, sizeof(dummy)); // one read for each write above
                        cnt--;
                    }
                } else {
                    std::lock_guard<std::mutex> lock(mutex_);
                    auto iter = fdTasks_.find(events[i].data.fd);
                    if ( iter != fdTasks_.end() ) {
                        result.push_back(iter->second);
                    }
                }
            }
        } else if (r<0) {
            ALOGE("epoll_wait failed: %s", strerror(errno));
        }
        return result;
    }

private:
    std::queue<Task>   queue_;
    std::map<int,Task> fdTasks_;
    mutable std::mutex mutex_;
    int                eventfd_;
    int                epollfd_;
    const unsigned int MAX_EVENTS = 5;
};

class Dispatcher : public IDispatcher
{
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

    EPollQueue  queue_;
    bool        stop_;
    std::thread eventthread_;
};

// static helpers
IDispatcher& IDispatcher::GetDefaultDispatcher()
{
    static std::unique_ptr<IDispatcher> dispatcher = CreateDispatcher();

    return *dispatcher.get();
}

void IDispatcher::EnqueueTask(std::function<void()> &&f) {
    GetDefaultDispatcher().Enqueue(std::move(f));
}

std::unique_ptr<IDispatcher> IDispatcher::CreateDispatcher() {
    return std::unique_ptr<IDispatcher>(new Dispatcher());
}

// constructor
Dispatcher::Dispatcher()
    : stop_(false),
      eventthread_([this]() { Start(); })
{
}

// destructor
Dispatcher::~Dispatcher() {
    Stop();
}

void Dispatcher::Enqueue(std::function<void()> &&f) {
    queue_.enqueue(std::move(f));
}

IDispatcher::JobId Dispatcher::EnqueueWithDelay(std::chrono::microseconds delay, std::function<void()> &&f) {
    // TODO!!!!!
    // for the moment we just dispatch directly but that MUST be fixed
    queue_.enqueue(std::move(f));
    return 0;
}

bool Dispatcher::Cancel(JobId jobid) {
    // TODO
    return true;
}

void Dispatcher::AddFd(int fd, std::function<void()> &&f) {
    queue_.addFd(fd, std::move(f));
}

void Dispatcher::RemoveFd(int fd) {
    queue_.removeFd(fd);
}


// private
void Dispatcher::Start() {
    while (!stop_)
    {
        std::vector<Task> tasks = queue_.dequeue(); // Blocking call
        if (!stop_)
        {
            // do callbacks for all tasks
            for (Task t : tasks) {
                t();
            }
        }
    }
}

void Dispatcher::Stop() {
    stop_ = true;
    Enqueue([]() {}); // add a "stop pill" to the queue to make it wakeup and stop asap
    Join();
}

void Dispatcher::Join() {
    if (eventthread_.joinable()) {
        eventthread_.join();
    }
}

}
}