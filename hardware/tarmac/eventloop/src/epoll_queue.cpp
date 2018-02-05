/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "epoll_queue.h"

#define LOG_TAG "tarmac.epollqueue"
#include <cutils/log.h>

#if !defined(EFD_SEMAPHORE)
#define EFD_SEMAPHORE (1 << 0)
#endif

using tarmac::eventloop::EPollQueue;
using tarmac::eventloop::Task;

EPollQueue::EPollQueue() : epollfd_(epoll_create1(0)), eventfd_((eventfd(0, EFD_SEMAPHORE))) {
    epoll_event event;
    event.events = EPOLLIN;
    event.data.ptr = nullptr;  // this indicates a "normal" enqueue task
    int r = epoll_ctl(epollfd_, EPOLL_CTL_ADD, eventfd_, &event);
    if (r == -1) {
        ALOGE("EPOLL_CTL_ADD failed: %s", strerror(errno));
        throw std::system_error(errno, std::system_category());
    }
}

void EPollQueue::enqueue(Task&& t) {
    {
        // lock scope
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.emplace(t);
    }
    const uint64_t dummy = 1;
    int r = write(eventfd_, &dummy, sizeof(dummy));
    ALOGE_IF(r < 0, "write failed: %s", strerror(errno));
}

void EPollQueue::addFd(int fd, Task&& t, uint32_t events) {
    int r = 0;
    int action;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        action = (fdTasks_.find(fd) != fdTasks_.end()) ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
    }
    epoll_event event;
    event.events = events;
    event.data.fd = fd;
    r = epoll_ctl(epollfd_, action, fd, &event);
    if (r == -1) {
        ALOGE("EPOLL_CTL_ADD fd failed: %s", strerror(errno));
        throw std::system_error(errno, std::system_category());
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        fdTasks_[fd] = t;
    }
}

bool EPollQueue::removeFd(int fd) {
    epoll_event event;
    event.data.fd = fd;
    int r = epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event);
    if (r == -1) {
        ALOGE("EPOLL_CTL_DEL fd failed: %s", strerror(errno));
        throw std::system_error(errno, std::system_category());
    }
    uint16_t removed;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        removed = fdTasks_.erase(fd);
    }
    return removed > 0;
}

std::vector<Task> EPollQueue::dequeue() {
    std::vector<Task> result;
    epoll_event events[MAX_EVENTS];
    int r = 0;
    do {
        r = epoll_wait(epollfd_, events, MAX_EVENTS, -1);  // -1 -> wait forever
        // Check the following stackoverflow QA as to why this loop is here:
        // https://stackoverflow.com/questions/6870158/epoll-wait-fails-due-to-eintr-how-to-remedy-this
    } while (r < 0 && errno == EINTR);
    if (r < 0) {
        ALOGE("epoll_wait failed: %s", strerror(errno));
        throw std::system_error(errno, std::system_category());
    }
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
    return result;
}
