/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_TIMEOUTINFO_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_TIMEOUTINFO_H_

#include <chrono>
#include <cstdint>
#include "ipcommandbus/local_config_parameters.h"
#include "ipcommandbus/VccIpCmdApi.h"

#ifdef ENABLE_TESTS
class TransportServicesFixture;
class TimeoutInfoFixture;
#endif  // ENABLE_TESTS

namespace Connectivity
{
class TimeoutInfo
{
  public:
    // Constructor for ACKs
    TimeoutInfo();
    // Constructor for responses
    TimeoutInfo(VccIpCmd::CombinedId id);
    // General constructor
    TimeoutInfo(std::chrono::milliseconds baseTimeout, uint32_t maxRetries, float multiplier);

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
