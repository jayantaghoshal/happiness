/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <sys/epoll.h>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>

namespace tarmac {
namespace eventloop {
class IDispatcher {
  public:
    // An opaque type that identifies delayed jobs
    using JobId = uint64_t;

    // Get the default dispatcher
    static IDispatcher& GetDefaultDispatcher();

    // Helper function for easy job scheduling
    static void EnqueueOnDefaultDispatcher(std::function<void()>&& f);

    // Helper function for easy job scheduling of delayed or cyclic task
    // User is responsible to cancel the timer at shutdown, e.g. in dtor when cyclic timer is used
    static JobId EnqueueWithDelayOnDefaultDispatcher(std::chrono::microseconds delay,
                                                     std::function<void()>&& f,
                                                     bool cyclic_timer = false);

    // Create a new dispatcher instance with the provided priority
    static std::shared_ptr<IDispatcher> CreateDispatcher(bool auto_start_on_new_thread = true);

    virtual ~IDispatcher() = default;

    // Enqueue a job to be executed asap
    virtual void Enqueue(std::function<void()>&& f) = 0;

    // Enqueue a job for later execution. The returned value is an id that can be used in Cancel(...).
    // User is responsible to cancel the timer at shutdown, e.g. in dtor when cyclic timer is used
    virtual JobId EnqueueWithDelay(std::chrono::microseconds delay, std::function<void()>&& f) = 0;
    virtual JobId EnqueueWithDelayCyclic(std::chrono::microseconds delay, std::function<void()>&& f) = 0;

    // Cancel a job that has been enqueued with EnqueueWithDelay()
    // return value:
    //    true  -> the job was cancelled before it was dispatched
    //    false -> the job couldn't be cancelled, it was probably already dispatched or previously cancelled
    virtual bool Cancel(JobId jobid) = 0;

    // Add and remove file descriptor monitoring for available in-data
    virtual void AddFd(int fd, std::function<void()>&& f) = 0;  // Uses EPOLLIN if no events are specified
    virtual void AddFd(int fd, std::function<void()>&& f, uint32_t events) = 0;
    virtual void RemoveFd(int fd) = 0;

    // Consumes the event-queue synchronously forever until stop_condition is fulfilled.
    // stop_condition is evaluated at the end of processing a undefined chunk of events
    // This method must only be called when dispatcher is created with auto_start_on_new_thread=false
    // Main use case of this function would be for testing
    virtual void RunUntil(std::function<bool()> stop_condition) = 0;

    // Stop the dispatcher thread. The dispatcher can no longer be used after this.
    virtual void Stop() = 0;

    // Join with the thread that handles this loop
    virtual void Join() = 0;
};
}  // namespace eventloop
}  // namespace tarmac
