/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <chrono>
#include <memory>
#include <thread>
#include "IDispatcher.h"

namespace tarmac {
namespace eventloop {

class WatchDog {
  public:
    virtual ~WatchDog() = default;
    static std::unique_ptr<WatchDog> Create();
    static std::unique_ptr<WatchDog> Create(std::function<void(std::thread::id)>&& assertion_function);

    // Enables watchdog monitoring of a Dispatcher instance to ensure the dispatcher is never blocked too long
    // inside application callbacks (such as those passed to AddFd or EnqueueWithDelay).
    // If such a callback does not return within maxTime, the watchdog will by default crash the application, unless
    // a custom assertion_function was used when creating the WatchDog.
    virtual void Watch(std::shared_ptr<IDispatcher> dispatcher, std::chrono::steady_clock::duration maxTime) = 0;
};

}  // namespace eventloop
}  // namespace tarmac