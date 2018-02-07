/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <map>
#include <mutex>
#include <queue>
#include <vector>
#include "file_descriptor.h"

namespace tarmac {
namespace eventloop {

using Task = std::function<void()>;

class EPollQueue {
  public:
    EPollQueue();

    void enqueue(Task&& t);

    void addFd(int fd, Task&& t, uint32_t events);

    bool removeFd(int fd);

    std::vector<Task> dequeue();

  private:
    std::queue<Task> queue_;
    std::map<int, Task> fdTasks_;
    mutable std::mutex mutex_;
    FileDescriptor epollfd_;
    FileDescriptor eventfd_;
    const unsigned int MAX_EVENTS = 5;
};

}  // namespace eventloop
}  // namespace tarmac
