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

constexpr uint32_t MAX_EVENTS_PROCESSED_AT_ONCE = 16;
constexpr int FILE_DESCRIPTOR_FOR_NON_FD_TASK_QUEUE = -1;

using tarmac::eventloop::EPollQueue;
using tarmac::eventloop::Task;

EPollQueue::EPollQueue() : epollfd_(epoll_create1(EPOLL_CLOEXEC)), eventfd_((eventfd(0, EFD_SEMAPHORE))) {
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = FILE_DESCRIPTOR_FOR_NON_FD_TASK_QUEUE;
    int r = epoll_ctl(epollfd_, EPOLL_CTL_ADD, eventfd_, &event);
    if (r == -1) {
        ALOGE("EPOLL_CTL_ADD failed: %s", strerror(errno));
        throw std::system_error(errno, std::system_category());
    }
}

void EPollQueue::enqueue(Task&& t) {
    {
        // lock scope
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        tasks_normal_.emplace(t);
    }
    const uint64_t dummy = 1;
    int r = write(eventfd_, &dummy, sizeof(dummy));
    ALOGE_IF(r < 0, "write failed: %s", strerror(errno));
}

void EPollQueue::addFd(int fd, Task&& t, uint32_t events) {
    int r = 0;
    int action;
    {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        action = (tasks_fd_.find(fd) != tasks_fd_.end()) ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
    }
    epoll_event event;
    event.events = events;
    event.data.fd = fd;
    r = epoll_ctl(epollfd_, action, fd, &event);
    if (r == -1) {
        ALOGE("EPOLL_CTL_ADD/MOD fd failed: %s", strerror(errno));
        throw std::system_error(errno, std::system_category());
    }
    {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        tasks_fd_[fd] = t;
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
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        removed = tasks_fd_.erase(fd);
    }
    return removed > 0;
}

std::vector<Task> EPollQueue::dequeue() {
    std::vector<Task> result;
    epoll_event events[MAX_EVENTS_PROCESSED_AT_ONCE];

    int r = 0;
    do {
        r = epoll_wait(epollfd_, events, MAX_EVENTS_PROCESSED_AT_ONCE, -1);  // -1 -> wait forever
        // Check the following stackoverflow QA as to why this loop is here:
        // https://stackoverflow.com/questions/6870158/epoll-wait-fails-due-to-eintr-how-to-remedy-this
    } while (r < 0 && errno == EINTR);

    if (r < 0) {
        ALOGE("epoll_wait failed: %s", strerror(errno));
        throw std::system_error(errno, std::system_category());
    }

    // we got some stuff to work on
    for (int i = 0; i < r; ++i) {
        const epoll_event& event = events[i];

        if (event.data.fd == FILE_DESCRIPTOR_FOR_NON_FD_TASK_QUEUE) {
            // this is a "normal" enqueued task, lets get them all
            int popped_task_normal_cnt = 0;
            {  // lock scope
                std::lock_guard<std::mutex> lock(tasks_mutex_);
                while (!tasks_normal_.empty()) {
                    result.push_back(tasks_normal_.front());
                    tasks_normal_.pop();
                    popped_task_normal_cnt++;
                }
            }
            // now read the same amount of times
            uint64_t dummy;
            while (popped_task_normal_cnt > 0) {
                int r = read(eventfd_, &dummy, sizeof(dummy));  // one read for each write above
                ALOGE_IF(r < 0, "read failed: %s", strerror(errno));
                popped_task_normal_cnt--;
            }
        } else {
            // this is fd associated Task, lets find the right handler for it.
            std::lock_guard<std::mutex> lock(tasks_mutex_);
            auto iter = tasks_fd_.find(events[i].data.fd);
            if (iter != tasks_fd_.end()) {
                result.push_back(iter->second);
            }
        }
    }
    return result;
}
