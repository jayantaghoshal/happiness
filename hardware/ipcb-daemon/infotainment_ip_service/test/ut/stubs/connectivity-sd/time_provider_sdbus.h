/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once

#if defined(__COVERITY__) && !defined(__INCLUDE_LEVEL__)
/* Systemd's use of gcc's __INCLUDE_LEVEL__ extension macro appears to confuse
 * Coverity. Here's a kludge to unconfuse it.
 */
#define __INCLUDE_LEVEL__ 2
#endif  // __COVERITY__

#include <systemd/sd-event.h>
#include <chrono>
#include <functional>
#include <memory>
#include "connectivity-sd/time_provider.h"

namespace Connectivity
{
class TimeProviderSdBus final : public ITimeProvider
{
public:
    TimeProviderSdBus(sd_event& sdEvent);

    std::chrono::steady_clock::time_point steady_clock_now() const override;
    std::chrono::system_clock::time_point system_clock_now() const override;
    std::unique_ptr<TimerSubscriptionHandle> add_single_shot_timer(std::chrono::milliseconds time,
                                                                   std::function<void()> func) override;

private:
    sd_event& sdEvent;
};
}
