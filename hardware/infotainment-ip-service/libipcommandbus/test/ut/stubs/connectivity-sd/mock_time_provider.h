/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once
#include <chrono>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include "time_provider.h"
namespace Connectivity
{
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

class TimeProviderStub final : public ITimeProvider
{
    class InternalSubscriptionHandle;
    class SubscriptionData;
    class UserSubscriptionHandle;

    class UserSubscriptionHandle final : public TimerSubscriptionHandle
    {
    public:
        explicit UserSubscriptionHandle(std::shared_ptr<TimeProviderStub::SubscriptionData> meta);
        ~UserSubscriptionHandle();

    private:
        std::shared_ptr<TimeProviderStub::SubscriptionData> meta;
    };

    class SubscriptionData final
    {
    public:
        explicit SubscriptionData(std::function<void()> func);
        void fire();
        void unsubscribe();

    private:
        bool expired{false};
        const std::function<void()> func;
        std::recursive_mutex fireMutex;
    };

    class InternalSubscriptionHandle final
    {
    public:
        explicit InternalSubscriptionHandle(std::chrono::steady_clock::time_point fire_at,
                                            std::weak_ptr<TimeProviderStub::SubscriptionData> meta);
        const std::chrono::steady_clock::time_point fire_at;
        std::weak_ptr<TimeProviderStub::SubscriptionData> meta;
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
    std::chrono::system_clock::time_point system_clock_now() const override;
    virtual std::unique_ptr<TimerSubscriptionHandle> add_single_shot_timer(std::chrono::milliseconds time,
                                                                           std::function<void()> func) override;
    void sleep_for(const std::chrono::milliseconds duration);
};
}
