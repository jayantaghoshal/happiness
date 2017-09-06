/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <functional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_ip_service.h"
#include "mocks/mock_workshop_wlan_stub.hpp"
#include "services/workshop_wlan_service.hpp"

namespace Connectivity
{
namespace gen_conn = v0::org::volvo::connectivity;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

class WorkshopWlanServiceFixture : public ::testing::Test
{
public:
    WorkshopWlanServiceFixture()
    {
    }

    virtual void SetUp()
    {
        fflush(stderr);

        ip_service_.setDispatcher(nullptr);

        workshop_wlan_service_.workshop_wlan_stub_ = p_workshop_wlan_stub_;
    }

    virtual void TearDown()
    {
    }

    WorkshopWlanService workshop_wlan_service_;

    // The mock class (IpService) is the base class of the test class (TelematicsService).
    // (In fact IpService is used as a utility class, _not_ a base class in the classic sense with method overriding.)
    // So the test object and mock object are the one and same.
    // Just having this reference to be able to avoid confusion of whether context is test or mock...

    NiceMock<MockIpService> &ip_service_ = workshop_wlan_service_;

    std::shared_ptr<NiceMock<MockWorkshopWlanStub>> p_workshop_wlan_stub_ =
        std::make_shared<NiceMock<MockWorkshopWlanStub>>();
};

// Original wrapper used in the old transport layer unit tests:
// This setup lead to quite long failure logs with the actual fault being a little too well disguised.
// MATCHER_P(LambdaMatcher, func, "") { bool ret=false; *result_listener << func(arg, ret); return ret; }

// Instead we here catch the faults with EXPECT_EQ, EXPECT_TRUE, etc which give a compact and clear error message upon
// failure.
// It may look weird that this wrapper always returns true,
// but if an expect check fails in func, the parent test will automatically fail as well.

MATCHER_P(LambdaMatcher, func, "")
{
    func(arg);
    return true;
}

TEST_F(WorkshopWlanServiceFixture, Initialize)
{
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(true));

    bool ret = workshop_wlan_service_.Initialize(&dispatcher);

    ASSERT_EQ(ret, true);
}

TEST_F(WorkshopWlanServiceFixture, Initialize_Fail_Register)
{
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(false));

    bool ret = workshop_wlan_service_.Initialize(&dispatcher);

    ASSERT_EQ(ret, false);
}

TEST_F(WorkshopWlanServiceFixture, Uninitilize)
{
    workshop_wlan_service_.Uninitialize();
}

void CapiCallbackConnectWorkshopWlanSetRequestNoReturn()
{
}

TEST_F(WorkshopWlanServiceFixture, ConnectWorkshopWlanSetRequestNoReturn)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    v0_conn::WorkshopWlanStub::ConnectWorkshopWlanSetRequestNoReturnReply_t reply =
        CapiCallbackConnectWorkshopWlanSetRequestNoReturn;

    EXPECT_CALL(
        ip_service_,
        SendGenericSetRequestNoReturn(
            VccIpCmd::ServiceId::Connectivity,
            VccIpCmd::OperationId::ConnectWorkshopWlan,
            Message::Ecu::VCM,
            LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                std::shared_ptr<WorkshopWlanService::ConnectWorkshopWlanSetRequestNoReturnCallerData> p_data =
                    std::dynamic_pointer_cast<WorkshopWlanService::ConnectWorkshopWlanSetRequestNoReturnCallerData>(
                        p_caller_data);
                EXPECT_NE(nullptr, p_data);
                EXPECT_NE(nullptr, p_data->reply);
            })));

    workshop_wlan_service_.ConnectWorkshopWlanSetRequestNoReturn(client, reply);
}

void CapiCallbackUpdateDaCMode(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors error)
{
}
TEST_F(WorkshopWlanServiceFixture, UpdateDaCMode)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    v0_conn::WorkshopWlanStub::UpdateDaCModeReply_t reply = CapiCallbackUpdateDaCMode;

    v0_conn::WorkshopWlan::DaCMode update_dac_mode = v0_conn::WorkshopWlan::DaCMode::MANUAL;

    workshop_wlan_service_.UpdateDaCMode(client, update_dac_mode, reply);
}

TEST_F(WorkshopWlanServiceFixture, cbConnectWorkshopWlanNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpConnectWorkshopWlan_Notification notification = Icb_OpConnectWorkshopWlan_Notification_Create(session);

    notification->connectionResult = e_Icb_WorkshopWlanConnectionResult_connected;

    EXPECT_CALL(ip_service_,
                DecodeMessageConnectWorkshopWlanNotification(
                    _, _, Icb_OpConnectWorkshopWlan_Notification_Create, Icb_OpConnectWorkshopWlan_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notification), Return(true)));

    workshop_wlan_service_.cbConnectWorkshopWlanNotification(msg);
}

TEST_F(WorkshopWlanServiceFixture, cbWorkshopWlanDetectedNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWorkshopWlanDetected_Notification notification = Icb_OpWorkshopWlanDetected_Notification_Create(session);

    notification->mode = v0_conn::WorkshopWlan::DaCMode::MANUAL;
    notification->nomadicDevicePresent = false;

    EXPECT_CALL(
        ip_service_,
        DecodeMessageWorkshopWlanDetectedNotification(
            _, _, Icb_OpWorkshopWlanDetected_Notification_Create, Icb_OpWorkshopWlanDetected_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notification), Return(true)));

    workshop_wlan_service_.cbWorkshopWlanDetectedNotification(msg);
}
}  // namespace Connectivity
