/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "connectivity-sd/time_provider.h"

namespace Connectivity
{
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ITimeProvider
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<TimerSubscriptionHandle> ITimeProvider::add_periodic_timer(std::chrono::milliseconds interval,
                                                                           std::function<void()>     func)
{
    std::shared_ptr<ProxyHandle> proxy = std::make_shared<ITimeProvider::ProxyHandle>(interval, func, *this);
    std::unique_ptr<PeriodicTimerSubscriptionHandle> rval(new PeriodicTimerSubscriptionHandle(proxy));
    proxy->handle = add_single_shot_timer(interval, std::bind(&ProxyHandle::executeAndRegisterNextPeriod, proxy));
    return rval;
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PeriodicTimerSubscriptionHandle
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ITimeProvider::PeriodicTimerSubscriptionHandle::PeriodicTimerSubscriptionHandle(std::shared_ptr<ProxyHandle> handle)
    : handle(handle)
{
}

ITimeProvider::PeriodicTimerSubscriptionHandle::~PeriodicTimerSubscriptionHandle()
{
    handle->handle.reset(nullptr);
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ProxyHandle
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ITimeProvider::ProxyHandle::ProxyHandle(std::chrono::milliseconds interval,
                                        std::function<void()>     func,
                                        ITimeProvider&            timeProvider)
    : interval(interval)
    , func(func)
    , timeProvider(timeProvider)
{
}

void ITimeProvider::ProxyHandle::executeAndRegisterNextPeriod()
{
    func();
    std::unique_ptr<TimerSubscriptionHandle> keepRefToProxy = std::move(handle);
    handle                                                  = timeProvider.add_single_shot_timer(
        interval, std::bind(&ProxyHandle::executeAndRegisterNextPeriod, this->shared_from_this()));
}
}
