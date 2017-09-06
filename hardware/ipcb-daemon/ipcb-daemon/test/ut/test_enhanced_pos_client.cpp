/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "CommonAPI/CommonAPI.hpp"
#include "connectivity-sd/mock_time_provider.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_commonapi_runtime.hpp"
#include "services/enhanced_pos_client.h"
#include "util/type_conversion_helpers.h"

#include <map>
#include <memory>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

namespace ePos = org::genivi::EnhancedPositionService;

class EnhancedPosClientFixture : public ::testing::Test
{
public:
    EnhancedPosClientFixture() : enhancedPosClient{timeProvider}
    {
        mockRuntime = CommonAPI::MockRuntime::get();
    }

    Connectivity::EnhancedPosClient enhancedPosClient;
    Connectivity::TimeProviderStub timeProvider;
    std::shared_ptr<CommonAPI::MockRuntime> mockRuntime;
    std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
        std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");

    // Access methods for private members of Connectivity::EnhancedPosClient
    std::shared_ptr<org::genivi::EnhancedPositionService::EnhancedPositionProxy<>> getProxy()
    {
        return enhancedPosClient.p_proxy_;
    }

    const Icb_DRPositionData &getDrPosData() const
    {
        return enhancedPosClient.p_dr_position_data_;
    }

    void addSubscriber(const Connectivity::IpService::NotificationSubscriber &sub)
    {
        enhancedPosClient.dr_position_subscribers_.push_back(sub);
    }

    // Pass-through methods to private calls of Connectivity::EnhancedPosClient

    void PosUpdateCallback(const ePos::EnhancedPositionServiceTypes::Bitmask bitMask)
    {
        enhancedPosClient.PosUpdateCallback(bitMask);
    }

    void GetPosAsyncCallback(const CommonAPI::CallStatus &callStatus,
                             const ePos::EnhancedPositionServiceTypes::Timestamp &timestampMs,
                             const ePos::EnhancedPositionServiceTypes::PositionInfo &posInfo)
    {
        enhancedPosClient.GetPosAsyncCallback(callStatus, timestampMs, posInfo);
    }

    void GetTimeAsyncCallback(const CommonAPI::CallStatus &call_status,
                              const ePos::EnhancedPositionServiceTypes::Timestamp &timestamp_ms,
                              const ePos::EnhancedPositionServiceTypes::TimeInfo &time_info)
    {
        enhancedPosClient.GetTimeAsyncCallback(call_status, timestamp_ms, time_info);
    }

    void AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
    {
        enhancedPosClient.AvailabilityCallback(status);
    }

    void DrTimerEvent()
    {
        timeProvider.sleep_for(std::chrono::milliseconds(1100));
        // enhancedPosClient.DrTimerEvent();
    }

    void cbDrPositionReq(Connectivity::Message &msg)
    {
        enhancedPosClient.cbDrPositionReq(msg);
    }

    void cbDrPositionNotificationReq(Connectivity::Message &msg)
    {
        enhancedPosClient.cbDrPositionNotificationReq(msg);
    }
};

TEST_F(EnhancedPosClientFixture, TestInitialize)
{
    mockRuntime->available = true;

    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;

    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(false));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(true));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);
}

extern int g_GetPositionInfoAsync_calls;
extern int g_GetTimeAsync_calls;

TEST_F(EnhancedPosClientFixture, TestPosUpdateCallback)
{
    g_GetPositionInfoAsync_calls = 0;
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;
    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(false));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    const ePos::EnhancedPositionServiceTypes::Bitmask bitMask(0xfeedfacedeadbeef);
    this->PosUpdateCallback(bitMask);
    EXPECT_EQ(g_GetPositionInfoAsync_calls, 1);
}

TEST_F(EnhancedPosClientFixture, TestGetPosAsyncCallback)
{
    g_GetTimeAsync_calls = 0;
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;
    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(false));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    ePos::EnhancedPositionServiceTypes::PositionInfo posInfo;

    this->GetPosAsyncCallback(CommonAPI::CallStatus::INVALID_VALUE,
                              0x123456789abc,
                              const_cast<const ePos::EnhancedPositionServiceTypes::PositionInfo &>(posInfo));

    EXPECT_EQ(g_GetTimeAsync_calls, 0);

    this->GetPosAsyncCallback(CommonAPI::CallStatus::SUCCESS,
                              0x123456789abc,
                              const_cast<const ePos::EnhancedPositionServiceTypes::PositionInfo &>(posInfo));

    EXPECT_EQ(g_GetTimeAsync_calls, 1);

    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::LATITUDE] = 1.12345;
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::LONGITUDE] = -5.4321;
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::HEADING] = 123.0;
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::SPEED] = 50.0;
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::HDOP] = 9.0;
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::USED_SATELLITES] = static_cast<uint64_t>(7);
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::GNSS_FIX_STATUS] = static_cast<uint64_t>(8);
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::DR_STATUS] =
        static_cast<uint64_t>(e_Icb_DeadReckoningType_drNoMapMatch);
    this->GetPosAsyncCallback(CommonAPI::CallStatus::SUCCESS,
                              0x123456789abc,
                              const_cast<const ePos::EnhancedPositionServiceTypes::PositionInfo &>(posInfo));
    EXPECT_EQ(getDrPosData()->longLat->latitude, InfotainmentIpService::Utils::DegreesDToFixedPoint32(1.12345));
    EXPECT_EQ(getDrPosData()->longLat->longitude, InfotainmentIpService::Utils::DegreesDToFixedPoint32(-5.4321));
    EXPECT_EQ(getDrPosData()->heading, 123U);
    EXPECT_EQ(getDrPosData()->speedKmph, U32(50 * 3.6));
    EXPECT_EQ(getDrPosData()->hdopX10, U32(9 * 10));
    EXPECT_EQ(getDrPosData()->numSat, 7U);
    EXPECT_EQ(getDrPosData()->drType, e_Icb_DeadReckoningType_drNoMapMatch);
    EXPECT_EQ(getDrPosData()->fixType, InfotainmentIpService::Utils::FixStatusToIpcbType(8));
    EXPECT_EQ(g_GetTimeAsync_calls, 2);

    // Test ceiling values
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::SPEED] = 300.0;
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::HDOP] = 30.0;
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::USED_SATELLITES] = static_cast<uint64_t>(200);
    posInfo[ePos::EnhancedPositionServiceTypes::PositionInfoKey::DR_STATUS] =
        static_cast<uint64_t>(e_Icb_DeadReckoningType_noDr);
    this->GetPosAsyncCallback(CommonAPI::CallStatus::SUCCESS,
                              0x123456789abc,
                              const_cast<const ePos::EnhancedPositionServiceTypes::PositionInfo &>(posInfo));
    EXPECT_EQ(getDrPosData()->speedKmph, U32(300 * 3.6));
    EXPECT_EQ(getDrPosData()->hdopX10, U32(25.5 * 10.0));
    EXPECT_EQ(getDrPosData()->numSat, 200U);
    EXPECT_EQ(getDrPosData()->drType, e_Icb_DeadReckoningType_noDr);
    EXPECT_EQ(g_GetTimeAsync_calls, 3);
}

TEST_F(EnhancedPosClientFixture, TestGetTimeAsyncCallback)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;
    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(false));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    ePos::EnhancedPositionServiceTypes::TimeInfo timeInfo;

    this->GetTimeAsyncCallback(CommonAPI::CallStatus::INVALID_VALUE,
                               0x123456789abc,
                               const_cast<const ePos::EnhancedPositionServiceTypes::TimeInfo &>(timeInfo));

    // Verify that values are same as during initialization.
    EXPECT_EQ(getDrPosData()->fixTime->year, 2015U);
    EXPECT_EQ(getDrPosData()->fixTime->month, 1U);
    EXPECT_EQ(getDrPosData()->fixTime->day, 1U);
    EXPECT_EQ(getDrPosData()->fixTime->hour, 0U);
    EXPECT_EQ(getDrPosData()->fixTime->minute, 0U);
    EXPECT_EQ(getDrPosData()->fixTime->second, 0U);

    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::YEAR] = static_cast<uint64_t>(2000);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::MONTH] = static_cast<uint64_t>(1);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::DAY] = static_cast<uint64_t>(1);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::HOUR] = static_cast<uint64_t>(0);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::MINUTE] = static_cast<uint64_t>(0);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::SECOND] = static_cast<uint64_t>(0);

    this->GetTimeAsyncCallback(CommonAPI::CallStatus::SUCCESS,
                               0x123456789abc,
                               const_cast<const ePos::EnhancedPositionServiceTypes::TimeInfo &>(timeInfo));

    // Verify that values are same as minimum ceiling values.
    EXPECT_EQ(getDrPosData()->fixTime->year, 2000U);
    EXPECT_EQ(getDrPosData()->fixTime->month, 1U);
    EXPECT_EQ(getDrPosData()->fixTime->day, 1U);
    EXPECT_EQ(getDrPosData()->fixTime->hour, 0U);
    EXPECT_EQ(getDrPosData()->fixTime->minute, 0U);
    EXPECT_EQ(getDrPosData()->fixTime->second, 0U);

    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::YEAR] = static_cast<uint64_t>(2127);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::MONTH] = static_cast<uint64_t>(12);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::DAY] = static_cast<uint64_t>(31);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::HOUR] = static_cast<uint64_t>(23);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::MINUTE] = static_cast<uint64_t>(59);
    timeInfo[ePos::EnhancedPositionServiceTypes::TimeInfoKey::SECOND] = static_cast<uint64_t>(59);

    this->GetTimeAsyncCallback(CommonAPI::CallStatus::SUCCESS,
                               0x123456789abc,
                               const_cast<const ePos::EnhancedPositionServiceTypes::TimeInfo &>(timeInfo));

    // Verify that values are same as maximum ceiling values.
    EXPECT_EQ(getDrPosData()->fixTime->year, 2127U);
    EXPECT_EQ(getDrPosData()->fixTime->month, 12U);
    EXPECT_EQ(getDrPosData()->fixTime->day, 31U);
    EXPECT_EQ(getDrPosData()->fixTime->hour, 23U);
    EXPECT_EQ(getDrPosData()->fixTime->minute, 59U);
    EXPECT_EQ(getDrPosData()->fixTime->second, 59U);
}

extern bool g_enhancedPositionProxyAvailable;
extern int g_enhancedPositionProxyAvailable_calls;

TEST_F(EnhancedPosClientFixture, TestAvailabilityCallback)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;
    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(false));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    g_enhancedPositionProxyAvailable_calls = 0;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::NOT_AVAILABLE);
    EXPECT_EQ(g_enhancedPositionProxyAvailable_calls, 0);

    g_enhancedPositionProxyAvailable = false;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(g_enhancedPositionProxyAvailable_calls, 1);

    g_enhancedPositionProxyAvailable = true;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(g_enhancedPositionProxyAvailable_calls, 2);
}

TEST_F(EnhancedPosClientFixture, TestDrTimerEvent)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;
    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(false));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    Connectivity::IpService::NotificationSubscriber subscriber1(Connectivity::VccIpCmd::ServiceId::Telematics,
                                                                Connectivity::VccIpCmd::OperationId::PositionData,
                                                                Connectivity::Message::Ecu::IHU);
    Connectivity::IpService::NotificationSubscriber subscriber2(
        Connectivity::VccIpCmd::ServiceId::Positioning,
        Connectivity::VccIpCmd::OperationId::DeadReckonedPosition,
        Connectivity::Message::Ecu::VCM);
    this->addSubscriber(subscriber1);
    this->addSubscriber(subscriber2);
    //
    //
    //
    //    Connectivity::Message msg1(VccIpCmd::ServiceId::Telematics,
    //            VccIpCmd::OperationId::PositionData,
    //            VccIpCmd::OperationType::RESPONSE,
    //            1);
    //    msg1.ecu = Message::Ecu::IHU;
    //
    //    Connectivity::Message msg2(VccIpCmd::ServiceId::Positioning,
    //            VccIpCmd::OperationId::DeadReckonedPosition,
    //            VccIpCmd::OperationType::REQUEST,
    //            1);
    //    msg1.ecu = Message::Ecu::VCM;

    EXPECT_CALL(enhancedPosClient, getNewSequenceId()).Times(3);
    EXPECT_CALL(enhancedPosClient, sendMessage(_, _)).Times(3);
    //    EXPECT_CALL(enhancedPosClient, sendMessage(msg2, nullptr)).Times(1);
    DrTimerEvent();
}

TEST_F(EnhancedPosClientFixture, TestcbDrPositionReq)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;
    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(false));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    EXPECT_CALL(enhancedPosClient, sendMessage(_, _)).Times(1);
    Connectivity::Message msg;
    cbDrPositionReq(msg);
}

TEST_F(EnhancedPosClientFixture, TestcbDrPositionNotificationReq)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;
    EXPECT_CALL(enhancedPosClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(enhancedPosClient, RegisterServer(_)).WillOnce(Return(false));
    EXPECT_CALL(enhancedPosClient, getNewSequenceId()).WillOnce(Return(1));
    enhancedPosClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    EXPECT_CALL(enhancedPosClient, sendMessage(_, _)).Times(1);
    Connectivity::Message msg;
    cbDrPositionNotificationReq(msg);
}
