/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "ipcommandbus/TimeoutInfo.h"

#include <cmath>

namespace Connectivity
{
TimeoutInfo::TimeoutInfo()
{
}

TimeoutInfo::TimeoutInfo(std::chrono::milliseconds bTimeout, uint32_t mRetries, float mult)
    : baseTimeout_(bTimeout), maxRetries_(mRetries), multiplier_(mult), timeoutValue_(bTimeout), retry_(0)
{
}

bool TimeoutInfo::increaseTimeout()
{
    retry_++;
    if (retry_ > maxRetries_)
    {
        return false;
    }
    timeoutValue_ = std::chrono::milliseconds{std::lround(timeoutValue_.count() * multiplier_)};
    return true;
}

std::chrono::milliseconds TimeoutInfo::getTimeoutValue(void)
{
    return timeoutValue_;
}

void TimeoutInfo::reset(void)
{
    retry_ = 0;
    timeoutValue_ = baseTimeout_;
}

}  // Connectivity
