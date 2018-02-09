/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <pthread.h>
#include <sys/timerfd.h>
#include <cerrno>
#include <set>
#include <system_error>
#include <thread>

#include "IDispatcher.h"
#include "epoll_queue.h"

#define LOG_TAG "tarmac.dispatcher"
#include <cutils/log.h>

/***********************************************************************************
 * Implements an event/main loop class that can handle to dispatch lambdas as well *
 * as monitoring file descriptors for available in-data.                           *
 ***********************************************************************************/

namespace tarmac {
namespace eventloop {

using Task = std::function<void()>;

class Dispatcher : public IDispatcher {
  public:
    explicit Dispatcher(bool auto_start_on_new_thread);

    ~Dispatcher() override;

    void Enqueue(std::function<void()>&& f) override;

    JobId EnqueueWithDelay(std::chrono::microseconds delay, std::function<void()>&& f,
                           bool cyclic_timer = false) override;

    bool Cancel(JobId jobid) override;

    void AddFd(int fd, std::function<void()>&& f, uint32_t events = EPOLLIN) override;
    void RemoveFd(int fd) override;

    void Stop() final;
    void Join() override;
    void RunUntil(std::function<bool()> stop_condition) override;

  private:
    void Start();

    EPollQueue queue_;
    bool stop_ = false;
    std::thread eventthread_;
    JobId next_job_id_ = 0;
    mutable std::mutex mutex_;
    std::map<JobId, int> delayed_jobs_;
};

// static helpers
IDispatcher& IDispatcher::GetDefaultDispatcher() {
    static std::shared_ptr<IDispatcher> dispatcher = CreateDispatcher();

    return *dispatcher;
}

void IDispatcher::EnqueueTask(std::function<void()>&& f) { GetDefaultDispatcher().Enqueue(std::move(f)); }

IDispatcher::JobId IDispatcher::EnqueueTaskWithDelay(std::chrono::microseconds delay, std::function<void()>&& f,
                                                     bool cyclic_timer) {
    return GetDefaultDispatcher().EnqueueWithDelay(delay, std::move(f), cyclic_timer);
}

std::shared_ptr<IDispatcher> IDispatcher::CreateDispatcher(bool auto_start_on_new_thread) {
    return std::shared_ptr<IDispatcher>(new Dispatcher(auto_start_on_new_thread));
}

Dispatcher::Dispatcher(bool auto_start_on_new_thread) {
    if (auto_start_on_new_thread) {
        eventthread_ = std::thread([this]() { Start(); });
    }
}

// destructor
Dispatcher::~Dispatcher() { Stop(); }

void Dispatcher::Enqueue(std::function<void()>&& f) { queue_.enqueue(std::move(f)); }

IDispatcher::JobId Dispatcher::EnqueueWithDelay(std::chrono::microseconds delay, std::function<void()>&& f,
                                                bool cyclic_timer) {
    struct itimerspec ts;
    int tfd, usec;
    JobId this_id;

    usec = delay.count();
    ts.it_interval.tv_sec = cyclic_timer ? usec / 1000000 : 0;
    ts.it_interval.tv_nsec = cyclic_timer ? (usec % 1000000) * 1000 : 0;
    ts.it_value.tv_sec = usec / 1000000;
    ts.it_value.tv_nsec = (usec % 1000000) * 1000;

    tfd = timerfd_create(CLOCK_MONOTONIC, 0);

    {  // lock scope
        std::lock_guard<std::mutex> lock(mutex_);
        this_id = next_job_id_++;      // obtain the new jobid
        delayed_jobs_[this_id] = tfd;  // and add this id to the set of running timers
    }

    timerfd_settime(tfd, 0, &ts, nullptr);

    AddFd(tfd, [this, tfd, f, this_id, cyclic_timer]() {
        bool dispatch_job_now = false;
        {  // lock scope
            std::lock_guard<std::mutex> lock(mutex_);
            auto iter = delayed_jobs_.find(this_id);
            // see if job shall continue, which means it has NOT been cancelled
            dispatch_job_now = (iter != delayed_jobs_.end());
            if (dispatch_job_now && !cyclic_timer) delayed_jobs_.erase(iter);  // job has not been cancelled
        }
        if (dispatch_job_now) {
            uint64_t dummy;
            int r = read(tfd, &dummy, sizeof(dummy));
            ALOGE_IF(r < 0, "read failed: %s", strerror(errno));
            f();
            if (!cyclic_timer) {
                RemoveFd(tfd);
                close(tfd);
            }
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

void Dispatcher::AddFd(int fd, std::function<void()>&& f, uint32_t events) { queue_.addFd(fd, std::move(f), events); }

void Dispatcher::RemoveFd(int fd) { queue_.removeFd(fd); }

// private
void Dispatcher::Start() {
    RunUntil([&]() { return stop_; });
}

void Dispatcher::RunUntil(std::function<bool()> stop_condition) {
    while (!stop_condition()) {
        std::vector<Task> tasks = queue_.dequeue();  // Blocking call
        if (!stop_) {
            // do callbacks for all tasks
            for (const Task& t : tasks) {
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
}  // namespace eventloop
}  // namespace tarmac
