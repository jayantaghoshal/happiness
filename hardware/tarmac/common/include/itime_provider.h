/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <chrono>
#include <functional>
#include <memory>

class TimerSubscriptionHandle {
  public:
    TimerSubscriptionHandle() = default;
    virtual ~TimerSubscriptionHandle() = default;
    TimerSubscriptionHandle(const TimerSubscriptionHandle&) = delete;
    TimerSubscriptionHandle(TimerSubscriptionHandle&&) = delete;
    TimerSubscriptionHandle& operator=(const TimerSubscriptionHandle&) = delete;
    TimerSubscriptionHandle& operator=(TimerSubscriptionHandle&&) = delete;
};

class ITimeProvider {
  public:
    virtual ~ITimeProvider() = default;

    virtual std::chrono::steady_clock::time_point steady_clock_now() const = 0;

    virtual std::chrono::system_clock::time_point system_clock_now() const = 0;

    virtual std::unique_ptr<TimerSubscriptionHandle> add_single_shot_timer(std::chrono::milliseconds time,
                                                                           std::function<void()> func) = 0;

    virtual std::unique_ptr<TimerSubscriptionHandle> add_periodic_timer(std::chrono::milliseconds interval,
                                                                        std::function<void()> func) = 0;
};
