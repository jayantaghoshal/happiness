/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "CommonAPI/CommonAPI.hpp"
#include "ipcommandbus/Message.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_commonapi_runtime.hpp"
#include "mocks/mock_ip_service.h"
#include "mocks/mock_parking_climate_proxy.h"
#include "services/climate_timer_proxy.h"

#include <functional>
#include <map>
#include <memory>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgReferee;

extern int g_climate_timer_function_calls;
extern int g_climate_timer_available_calls;
extern bool g_climate_timer_proxy_available;

namespace Connectivity
{
class ClimateTimerProxyFixture : public ::testing::Test
{
public:
    ClimateTimerProxyFixture()
    {
        // climate_timer_proxy_ = new ClimateTimerProxy();
        mock_runtime_ = CommonAPI::MockRuntime::get();
    }
    virtual void SetUp()
    {
        fflush(stderr);
        ON_CALL(climate_timer_proxy_, RegisterServer(_)).WillByDefault(Return(true));
    }
    virtual void TearDown()
    {
        g_climate_timer_function_calls = 0;
        g_climate_timer_function_calls = 0;
        climate_timer_proxy_.climate_timer_available_ = false;
    }
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;
    std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
        std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");
    ClimateTimerProxy climate_timer_proxy_;
    std::shared_ptr<CommonAPI::MockRuntime> mock_runtime_;
    void setClimateTimerAvailable(bool b)
    {
        climate_timer_proxy_.climate_timer_available_ = b;
    }

    // Access private functions
    void ParkingClimateServicesAvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
    {
        climate_timer_proxy_.ParkingClimateServicesAvailabilityCallback(status);
    }

    bool getProxyAvailable()
    {
        return climate_timer_proxy_.climate_timer_available_;
    }

    void cbGenericSettingSynchReq(Connectivity::Message &msg)
    {
        climate_timer_proxy_.cbGenericSettingSynchReq(msg);
    }

    void cbGenericSettingSynchSetReq(Connectivity::Message &msg)
    {
        climate_timer_proxy_.cbGenericSettingSynchSetReq(msg);
    }

    void cbGenericSettingSynchNotificationReq(Connectivity::Message &msg)
    {
        climate_timer_proxy_.cbGenericSettingSynchNotificationReq(msg);
    }

    void TimerListChangedNotificationCallback(const v0_climate::ParkingClimate::ParkingTimers &timerList)
    {
        climate_timer_proxy_.TimerListChangedNotificationCallback(timerList);
    }

    void addClimateTimerSubscriber(const Connectivity::IpService::NotificationSubscriber &sub)
    {
        climate_timer_proxy_.climate_timer_subscriber_.push_back(sub);
    }

    void sendGenericSettingSynchResponse(const CommonAPI::CallStatus &callstatus,
                                         Icb_DateTime _time,
                                         Icb_ArrayOf_Setting _settings,
                                         const Message::Ecu msg_target_ecu,
                                         const VccIpCmd::SenderHandleId sender_handle_id)

    {
        climate_timer_proxy_.sendGenericSettingSynchResponse(
            callstatus, _time, _settings, msg_target_ecu, sender_handle_id);
    }

    bool ParkingTimersEqual(const v0_climate::ParkingClimate::ParkingTimers &timersA,
                            const v0_climate::ParkingClimate::ParkingTimers &timersB)
    {
        return climate_timer_proxy_.ParkingTimersEqual(timersA, timersB);
    }

protected:
    void verifyAndClear(void)
    {
    }
};

TEST_F(ClimateTimerProxyFixture, TestInitialize)
{
    mock_runtime_->available = true;

    EXPECT_CALL(climate_timer_proxy_, RegisterServer(_)).WillOnce(Return(false));
    EXPECT_FALSE(climate_timer_proxy_.Initialize(&dispatcher, common_api_main_loop_context_));

    EXPECT_CALL(climate_timer_proxy_, RegisterServer(_)).WillOnce(Return(true));
    EXPECT_TRUE(climate_timer_proxy_.Initialize(&dispatcher, common_api_main_loop_context_));
}

/**
 * Test: AvailabilityCallback
 * Expected: Status is AVAILABLE and proxy is available => telematics_proxy_ = true
 */
TEST_F(ClimateTimerProxyFixture, AvailabilityCallback_Status_Available_And_Proxy_Available)
{
    mock_runtime_->available = true;
    climate_timer_proxy_.Initialize(&dispatcher, common_api_main_loop_context_);
    g_climate_timer_proxy_available = false;
    this->ParkingClimateServicesAvailabilityCallback(CommonAPI::AvailabilityStatus::NOT_AVAILABLE);
    EXPECT_EQ(0, g_climate_timer_available_calls);

    g_climate_timer_proxy_available = true;
    this->ParkingClimateServicesAvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(1, g_climate_timer_available_calls);

    g_climate_timer_proxy_available = true;
    this->ParkingClimateServicesAvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(2, g_climate_timer_available_calls);
}

TEST_F(ClimateTimerProxyFixture, TestcbGenericSettingSynchReq)
{
    mock_runtime_->available = true;
    climate_timer_proxy_.Initialize(&dispatcher, common_api_main_loop_context_);

    setClimateTimerAvailable(false);
    EXPECT_CALL(climate_timer_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::Telematics,
                                 Connectivity::VccIpCmd::OperationId::GenericSettingSynch,
                                 Connectivity::Message::Ecu::TEM,
                                 8));
    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::TEM;
    msg.pdu.header.sender_handle_id = 8;
    this->cbGenericSettingSynchReq(msg);

    climate_timer_proxy_.Initialize(&dispatcher, common_api_main_loop_context_);
    g_climate_timer_proxy_available = true;
    setClimateTimerAvailable(true);
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
    Icb_OpGenericSettingSynch_Request req = Icb_OpGenericSettingSynch_Request_Create(session);
    req->settingIDs = {};
    req->time = {};
    // EXPECT_CALL(climate_timer_proxy_, DecodeMessageGenericSettingRequest(_,_,_,_)).WillOnce(DoAll(
    // SetArgReferee<1>(req), Return(true)));
    // this->cbGenericSettingSynchReq(msg);

    msg.pdu.header.sender_handle_id = 9;
    EXPECT_CALL(climate_timer_proxy_, DecodeMessageGenericSettingRequest(_, _, _, _)).WillOnce(Return(false));
    EXPECT_CALL(climate_timer_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::Telematics,
                                 Connectivity::VccIpCmd::OperationId::GenericSettingSynch,
                                 Connectivity::Message::Ecu::TEM,
                                 9));
    this->cbGenericSettingSynchReq(msg);
}

TEST_F(ClimateTimerProxyFixture, TestcbGenericSettingSynchSetReq)
{
    mock_runtime_->available = true;
    climate_timer_proxy_.Initialize(&dispatcher, common_api_main_loop_context_);

    setClimateTimerAvailable(false);
    EXPECT_CALL(climate_timer_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::Telematics,
                                 Connectivity::VccIpCmd::OperationId::GenericSettingSynch,
                                 Connectivity::Message::Ecu::TEM,
                                 8));
    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::TEM;
    msg.pdu.header.sender_handle_id = 8;
    this->cbGenericSettingSynchSetReq(msg);

    setClimateTimerAvailable(true);
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
    Icb_OpGenericSettingSynch_SetRequest req = Icb_OpGenericSettingSynch_SetRequest_Create(session);
    req->settings = {};
    req->time = {};
    // EXPECT_CALL(climate_timer_proxy_, DecodeMessageGenericSettingSetRequest(_,_,_,_)).WillOnce(DoAll(
    // SetArgReferee<1>(req), Return(true)));
    // this->cbGenericSettingSynchSetReq(msg);

    msg.pdu.header.sender_handle_id = 9;
    EXPECT_CALL(climate_timer_proxy_, DecodeMessageGenericSettingSetRequest(_, _, _, _)).WillOnce(Return(false));
    EXPECT_CALL(climate_timer_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::Telematics,
                                 Connectivity::VccIpCmd::OperationId::GenericSettingSynch,
                                 Connectivity::Message::Ecu::TEM,
                                 9));
    this->cbGenericSettingSynchSetReq(msg);
}

TEST_F(ClimateTimerProxyFixture, TestcbGenericSettingSynchNotificationReq)
{
    setClimateTimerAvailable(false);
    EXPECT_CALL(climate_timer_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::Telematics,
                                 Connectivity::VccIpCmd::OperationId::GenericSettingSynch,
                                 Connectivity::Message::Ecu::UNKNOWN,
                                 1));
    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::UNKNOWN;
    msg.pdu.header.sender_handle_id = 1;
    this->cbGenericSettingSynchNotificationReq(msg);

    setClimateTimerAvailable(true);
    EXPECT_CALL(climate_timer_proxy_,
                SendResponseGenericSetting(_,
                                           _,
                                           _,
                                           Connectivity::VccIpCmd::ServiceId::Telematics,
                                           Connectivity::VccIpCmd::OperationId::GenericSettingSynch,
                                           Connectivity::Message::Ecu::UNKNOWN,
                                           1));
    this->cbGenericSettingSynchNotificationReq(msg);
}

TEST_F(ClimateTimerProxyFixture, TestTimerListChangedNotificationCallback)
{
    v0_climate::ParkingClimate::ParkingTimers timerList;
    v0_climate::ParkingClimate::ParkingTimerStruct element;
    v0_climate::ParkingClimate::DateTimeStruct date_element;
    v0_climate::ParkingClimate::WeekdaysVec week_vector;
    v0_climate::ParkingClimate::WeekdayEnum week_element;
    element.setActive(v0_climate::ParkingClimate::TimerStateEnum::OFF);

    date_element.setDay(30);
    date_element.setHour(2);
    date_element.setMinute(12);
    date_element.setYear(2016);

    element.setTimerId(1);

    week_element = v0_climate::ParkingClimate::WeekdayEnum::FRIDAY;
    week_vector.push_back(week_element);
    element.setWeekdays(week_vector);

    timerList.push_back(element);

    EXPECT_CALL(climate_timer_proxy_, sendMessage(_, _)).Times(1);
    this->TimerListChangedNotificationCallback(timerList);
}

TEST_F(ClimateTimerProxyFixture, TestParkingTimersEqual)
{
    v0_climate::ParkingClimate::ParkingTimers timerList;
    v0_climate::ParkingClimate::ParkingTimerStruct element;
    v0_climate::ParkingClimate::DateTimeStruct date_element;
    v0_climate::ParkingClimate::WeekdaysVec week_vector;
    v0_climate::ParkingClimate::WeekdayEnum week_element;
    element.setActive(v0_climate::ParkingClimate::TimerStateEnum::OFF);

    date_element.setDay(30);
    date_element.setHour(2);
    date_element.setMinute(12);
    date_element.setYear(2016);

    element.setTimerId(1);

    week_element = v0_climate::ParkingClimate::WeekdayEnum::FRIDAY;
    week_vector.push_back(week_element);
    element.setWeekdays(week_vector);

    timerList.push_back(element);

    EXPECT_FALSE(this->ParkingTimersEqual(timerList, {}));
    EXPECT_TRUE(this->ParkingTimersEqual(timerList, timerList));
}
}
