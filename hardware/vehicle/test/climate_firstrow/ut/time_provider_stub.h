/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "timer_manager_interface.h"

#include <chrono>
#include <functional>
#include <list>
#include <memory>
#include <mutex>

/*


|------------------------
|    TimeProviderStub   |
|                       |
|------------------------
          |
          V
|------------------------ weak |------------------------ shared|------------------------
|  Internal             |----->|  SubscriptionData     |<------| UserSubscriptionHandle|
|                       |      |                       |       |                       |
|------------------------      |------------------------       |------------------------
                                                                          ^
                                                                          | unique
                                                              |------------------------
                                                              | Application           |
                                                              |                       |
                                                              |------------------------

*/

class TimeProviderStub final : public tarmac::timeprovider::TimerManagerInterface {
    class SubscriptionData;
    class ProxyHandle;

    class UserSubscriptionHandle final : public tarmac::timeprovider::TimerSubscriptionHandle {
      public:
        explicit UserSubscriptionHandle(std::shared_ptr<TimeProviderStub::SubscriptionData> meta);
        ~UserSubscriptionHandle();

      private:
        std::shared_ptr<TimeProviderStub::SubscriptionData> meta;
    };

    class SubscriptionData final {
      public:
        explicit SubscriptionData(std::function<void()> func);
        void fire();
        void unsubscribe();

      private:
        bool expired{false};
        const std::function<void()> func;
        std::recursive_mutex fireMutex;
    };

    class InternalSubscriptionHandle final {
      public:
        explicit InternalSubscriptionHandle(std::chrono::steady_clock::time_point fire_at,
                                            std::weak_ptr<TimeProviderStub::SubscriptionData> meta);
        const std::chrono::steady_clock::time_point fire_at;
        std::weak_ptr<TimeProviderStub::SubscriptionData> meta;
    };

  private:
    class PeriodicTimerSubscriptionHandle : public tarmac::timeprovider::TimerSubscriptionHandle {
        // This is a helper class to enable periodic_timer when the implementation only supports single shot timer
      public:
        const std::shared_ptr<ProxyHandle> handle;
        explicit PeriodicTimerSubscriptionHandle(std::shared_ptr<ProxyHandle> handle);
        virtual ~PeriodicTimerSubscriptionHandle();
    };

    class ProxyHandle final : public std::enable_shared_from_this<ProxyHandle> {
      public:
        explicit ProxyHandle(std::chrono::milliseconds interval,
                             std::function<void()> func,
                             TimerManagerInterface& timeProvider);
        virtual ~ProxyHandle() = default;
        void executeAndRegisterNextPeriod();

        const std::chrono::milliseconds interval;
        const std::function<void()> func;
        tarmac::timeprovider::TimerManagerInterface& timeProvider;
        std::unique_ptr<tarmac::timeprovider::TimerSubscriptionHandle> handle;
    };

  private:
    std::chrono::steady_clock::time_point steady_now{std::chrono::steady_clock::time_point::duration::zero()};
    std::chrono::system_clock::time_point system_now{std::chrono::system_clock::time_point::duration::zero()};

    std::list<InternalSubscriptionHandle> timers;
    std::recursive_mutex timersMutex;

  public:
    explicit TimeProviderStub() = default;
    virtual ~TimeProviderStub() = default;
    std::chrono::steady_clock::time_point steady_clock_now() const override;
    void sleep_for(std::chrono::milliseconds duration);
    std::unique_ptr<tarmac::timeprovider::TimerSubscriptionHandle> AddSingleShotTimer(
            std::chrono::milliseconds time,
            std::function<void()> func) override;
    std::unique_ptr<tarmac::timeprovider::TimerSubscriptionHandle> AddPeriodicTimer(
            std::chrono::milliseconds interval,
            std::function<void()> func) override;
};
