/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ipcommandbus/TimeoutInfo.h"
#include <cmath>

namespace Connectivity {
TimeoutInfo::TimeoutInfo()
    : TimeoutInfo(LocalconfigParameters::getInstance().getDefaultAckTimeout(),
                  LocalconfigParameters::getInstance().getDefaultAckNumRetries()) {}

TimeoutInfo::TimeoutInfo(std::chrono::milliseconds timeout, uint32_t max_retries)
    : baseTimeout_(timeout),
      maxRetries_(max_retries),
      multiplier_(LocalconfigParameters::getInstance().getDefaultAckMultiplier()),
      timeoutValue_(timeout),
      retry_(0) {}

bool TimeoutInfo::increaseTimeout() {
    retry_++;
    if (retry_ > maxRetries_) {
        return false;
    }
    // Note: While overflow is a potential issue here, the realistic values used
    // in production will still be far below that range.
    timeoutValue_ = std::chrono::milliseconds{std::lround(timeoutValue_.count() * multiplier_)};
    return true;
}

std::chrono::milliseconds TimeoutInfo::getTimeoutValue(void) { return timeoutValue_; }

void TimeoutInfo::reset(void) {
    retry_ = 0;
    timeoutValue_ = baseTimeout_;
}

}  // Connectivity
