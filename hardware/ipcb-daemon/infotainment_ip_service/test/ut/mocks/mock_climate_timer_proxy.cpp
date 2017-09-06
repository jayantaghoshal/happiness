/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "mock_climate_timer_proxy.hpp"

namespace Connectivity
{
::testing::NiceMock<MockClimateTimerProxy>* g_mocked_climate_timer_proxy_ = nullptr;

// Mock method implementation
bool ClimateTimerProxy::Initialize(void)
{
    EXPECT_TRUE(nullptr != g_mocked_climate_timer_proxy_);
    return g_mocked_climate_timer_proxy_->Initialize();
}

void ClimateTimerProxy::SendRequestTimerList(v0_climate::ParkingClimate::ParkingTimers timerList)
{
    EXPECT_TRUE(nullptr != g_mocked_climate_timer_proxy_);
    g_mocked_climate_timer_proxy_->SendRequestTimerList(timerList);
}

void ClimateTimerProxy::RequestTimerListCallback(const CommonAPI::CallStatus& callstatus,
                                                 const v0_climate::CommonTypes::ReturnCode& errorCode)
{
    EXPECT_TRUE(nullptr != g_mocked_climate_timer_proxy_);
    g_mocked_climate_timer_proxy_->RequestTimerListCallback(callstatus, errorCode);
}

// v0_climate::ParkingClimate::ParkingTimers ClimateTimerProxy::GetTimerList()
//{
//    EXPECT_TRUE(nullptr != g_mocked_climate_timer_proxy_);
//    g_mocked_climate_timer_proxy_->GetTimerList();
//}
}  // namespace Connectivity
