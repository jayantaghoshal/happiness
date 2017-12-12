/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_TIMEOUTINFO_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_TIMEOUTINFO_H_

#include <chrono>
#include <cstdint>
#include "ipcommandbus/local_config_parameters.h"

#ifdef ENABLE_TESTS
class TransportServicesFixture;
class TimeoutInfoFixture;
#endif  // ENABLE_TESTS

namespace Connectivity {
class TimeoutInfo {
  public:
    // Constructor for ACKs
    TimeoutInfo();
    // General constructor
    TimeoutInfo(std::chrono::milliseconds timeout, uint32_t max_retries);

    bool increaseTimeout();
    std::chrono::milliseconds getTimeoutValue(void);
    void reset(void);

  private:
    std::chrono::milliseconds baseTimeout_;
    uint32_t maxRetries_;
    float multiplier_;

    std::chrono::milliseconds timeoutValue_;
    uint32_t retry_;

#ifdef ENABLE_TESTS
    friend TransportServicesFixture;
    friend class ::TimeoutInfoFixture;
#endif  // ENABLE_TESTS
};

}  // Connectivity

#endif  // _CONNECTIVITY_LIBIPCOMMANDBUS_TIMEOUTINFO_H_
