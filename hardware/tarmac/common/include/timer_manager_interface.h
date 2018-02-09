/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <functional>
#include <memory>
#include "time_provider_interface.h"

namespace tarmac {
namespace timeprovider {

class TimerSubscriptionHandle {
  public:
    TimerSubscriptionHandle() = default;
    virtual ~TimerSubscriptionHandle() = default;

    TimerSubscriptionHandle(const TimerSubscriptionHandle&) = delete;
    TimerSubscriptionHandle(TimerSubscriptionHandle&&) = delete;
    TimerSubscriptionHandle& operator=(const TimerSubscriptionHandle&) = delete;
    TimerSubscriptionHandle& operator=(TimerSubscriptionHandle&&) = delete;
};

class TimerManagerInterface : public TimerProviderInterface {
  public:
    virtual ~TimerManagerInterface() = default;

    virtual std::unique_ptr<TimerSubscriptionHandle> AddSingleShotTimer(std::chrono::milliseconds delay,
                                                                        std::function<void()> func) = 0;

    virtual std::unique_ptr<TimerSubscriptionHandle> AddPeriodicTimer(std::chrono::milliseconds interval,
                                                                      std::function<void()> func) = 0;
};
}  // namespace timeprovider
}  // namespace tarmac
