/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef VOC_MOCKS_MOCK_CLIMATE_TIMER_PROXY_H_
#define VOC_MOCKS_MOCK_CLIMATE_TIMER_PROXY_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "climate_timer_proxy.hpp"

namespace Connectivity
{
class MockClimateTimerProxy;
extern ::testing::NiceMock<MockClimateTimerProxy>* g_mocked_climate_timer_proxy_;

class MockClimateTimerProxy
{
public:
    MOCK_METHOD0(Initialize, bool(void));
    MOCK_METHOD1(SendRequestTimerList, void(v0_climate::ParkingClimate::ParkingTimers timerList));
    // MOCK_METHOD1(ParkingClimateServicesAvailabilityCallback, void(const CommonAPI::AvailabilityStatus &status));
    MOCK_METHOD2(RequestTimerListCallback,
                 void(const CommonAPI::CallStatus& callstatus, const v0_climate::CommonTypes::ReturnCode& errorCode));

    ~MockClimateTimerProxy()
    {
        g_mocked_climate_timer_proxy_ = nullptr;
    }
};
}  // namespace Connectivity

#endif  // VOC_MOCKS_MOCK_CLIMATE_TIMER_PROXY_H_
