/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_WORKSHOP_WLAN_STUB_HPP_
#define MOCK_WORKSHOP_WLAN_STUB_HPP_

#include <sstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <v0/org/volvo/connectivity/IpCommandBusTypes.hpp>
#include <v0/org/volvo/connectivity/WorkshopWlanStub.hpp>

#include "services/workshop_wlan_stub.hpp"

namespace v0_conn = v0::org::volvo::connectivity;

class MockWorkshopWlanStub : public Connectivity::WorkshopWlanStub
{
public:
    MockWorkshopWlanStub() : WorkshopWlanStub(nullptr)
    {
    }

    MockWorkshopWlanStub(Connectivity::IWorkshopWlanService *service) : Connectivity::WorkshopWlanStub(service)
    {
    }

    MOCK_METHOD1(fireConnectWorkshopWlanNotificationEvent,
                 void(const v0_conn::WorkshopWlan::WorkshopWlanConnectionResult &connection_result));

    MOCK_METHOD2(fireWorkshopWlanDetectedEvent,
                 void(const v0_conn::WorkshopWlan::DaCMode &mode, const bool &nomadic_device_present));
};

#endif  // MOCK_WORKSHOP_WLAN_STUB_HPP_
