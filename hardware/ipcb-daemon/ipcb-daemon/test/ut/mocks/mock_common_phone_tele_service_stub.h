/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#ifndef MOCK_COMMON_PHONE_TELEMATICS_SERVICE_STUB_H_
#define MOCK_COMMON_PHONE_TELEMATICS_SERVICE_STUB_H_

#include <sstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <v0/org/volvo/connectivity/CommonPhoneTelematics.hpp>
#include <v0/org/volvo/connectivity/IpCommandBusTypes.hpp>

#include "services/common_phone_tele_service_stub.h"

namespace v0_conn = v0::org::volvo::connectivity;

class MockCommonPhoneTelematicsStub : public Connectivity::CommonPhoneTelematicsStub
{
public:
    MockCommonPhoneTelematicsStub() : CommonPhoneTelematicsStub(nullptr)
    {
    }

    MockCommonPhoneTelematicsStub(Connectivity::ICommonPhoneTelematicsService *service)
        : Connectivity::CommonPhoneTelematicsStub(service)
    {
    }
};

#endif  // MOCK_COMMON_PHONE_TELEMATICS_SERVICE_STUB_H_
