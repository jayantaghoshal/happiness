#pragma once

#include <chrono>
#include <cstdint>
#include <functional>

namespace tarmac {
namespace eventloop {
class IDispatcher {
  public:
    // An opaque type that identifies delayed jobs
    using JobId = uint64_t;

    // Get the default dispatcher
    static IDispatcher &GetDefaultDispatcher();

    // Helper function for easy job scheduling
    static void EnqueueTask(std::function<void()> &&f);

    // Create a new dispatcher instance with the provided priority
    static std::unique_ptr<IDispatcher> CreateDispatcher();

    virtual ~IDispatcher() = default;

    // Enqueue a job to be executed asap
    virtual void Enqueue(std::function<void()> &&f) = 0;

    // Enqueue a job for later execution. The returned value is an id that can be used in Cancel(...)
    virtual JobId EnqueueWithDelay(std::chrono::microseconds delay, std::function<void()> &&f) = 0;

    // Cancel a job that has been enqueued with EnqueueWithDelay()
    // return value:
    //    true  -> the job was cancelled before it was dispatched
    //    false -> the job couldn't be cancelled, it was probably already dispatched or previously cancelled
    virtual bool Cancel(JobId jobid) = 0;

    // Add and remove file descriptor monitoring for available in-data
    virtual void AddFd(int fd, std::function<void()> &&f) = 0;
    virtual void RemoveFd(int fd) = 0;

    // Stop the dispatcher thread. The dispatcher can no longer be used after this.
    virtual void Stop() = 0;

    // Join with the thread that handles this loop
    virtual void Join() = 0;
};
}  // end eventloop
}  // end tarmac
