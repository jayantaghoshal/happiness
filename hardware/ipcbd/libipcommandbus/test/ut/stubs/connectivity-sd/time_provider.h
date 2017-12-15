/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <chrono>
#include <functional>
#include <memory>
namespace Connectivity {
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
    class ProxyHandle;

  private:
    class PeriodicTimerSubscriptionHandle : public TimerSubscriptionHandle {
        // This is a helper class to enable periodic_timer when the implementation only supports single shot timer
      public:
        const std::shared_ptr<ProxyHandle> handle;
        explicit PeriodicTimerSubscriptionHandle(std::shared_ptr<ProxyHandle> handle);
        virtual ~PeriodicTimerSubscriptionHandle();
    };

    class ProxyHandle final : public std::enable_shared_from_this<ProxyHandle> {
      public:
        explicit ProxyHandle(std::chrono::milliseconds interval, std::function<void()> func,
                             ITimeProvider& timeProvider);
        virtual ~ProxyHandle() = default;
        void executeAndRegisterNextPeriod();

        const std::chrono::milliseconds interval;
        const std::function<void()> func;
        ITimeProvider& timeProvider;
        std::unique_ptr<TimerSubscriptionHandle> handle;
    };

  public:
    ITimeProvider() = default;
    virtual ~ITimeProvider() = default;
    ITimeProvider(const ITimeProvider&) = delete;
    ITimeProvider(ITimeProvider&&) = delete;
    ITimeProvider& operator=(const ITimeProvider&) = delete;
    ITimeProvider& operator=(ITimeProvider&&) = delete;

    virtual std::chrono::steady_clock::time_point steady_clock_now() const = 0;

    virtual std::chrono::system_clock::time_point system_clock_now() const = 0;

    virtual std::unique_ptr<TimerSubscriptionHandle> add_single_shot_timer(std::chrono::milliseconds time,
                                                                           std::function<void()> func) = 0;

    virtual std::unique_ptr<TimerSubscriptionHandle> add_periodic_timer(std::chrono::milliseconds interval,
                                                                        std::function<void()> func);
};
}
