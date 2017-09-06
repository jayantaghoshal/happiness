/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "IpService.h"
#include "cedric/mock_cedric_localconfig.h"
#include "mocks/MockMessageDispatcher.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

using namespace Connectivity;

MockLocalconfig* mock_local_config;

class IpServiceDerivative : public IpService
{
public:
    bool Prot_RegisterClient(const std::vector<ClientIpCommandBusService>& table)
    {
        return RegisterClient(table);
    }

    bool Prot_RegisterServer(const std::vector<ServerIpCommandBusService>& table)
    {
        return RegisterServer(table);
    }
};

class IpServiceFixture : public ::testing::Test
{
public:
    /**
     * Fixture Constructor
     */
    IpServiceFixture()
    {
    }

    /**
     * Set up fixture
     */
    virtual void SetUp()
    {
        ipService_.setDispatcher(&messageDispatcher_);
    }

    /**
     * Tear down fixture
     */
    virtual void TearDown()
    {
    }

    IpServiceDerivative ipService_;
    NiceMock<MockMessageDispatcher> messageDispatcher_;
};

// Original wrapper used in the old transport layer unit tests:
// This setup lead to quite long failure logs with the actual fault being a little too well disguised.
// MATCHER_P(LambdaMatcher, func, "") { bool ret=false; *result_listener << func(arg, ret); return ret; }

// Instead we here catch the faults with EXPECT_EQ, EXPECT_TRUE, etc which give a compact and clear error message upon
// failure.
// It may look weird that this wrapper always returns true, but if an expect check fails in func, the parent test will
// automatically fail as well.
MATCHER_P(LambdaMatcher, func, "")
{
    func(arg);
    return true;
}

TEST_F(IpServiceFixture, RegisterClient)
{
    std::vector<IpService::ClientIpCommandBusService> ip_cmdbus_services;
    ip_cmdbus_services = {
        {Message::IHU, VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::LocalStorage, nullptr, nullptr, false},
        {Message::IHU,
         VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::LocalStorage,
         [&](Message& message, std::shared_ptr<MessageDispatcher::CallerData> callerData) {
             (void)message;
             (void)callerData;
         },
         [&](Message& message) { (void)message; },
         false},
        {Message::VCM,
         VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::LocalStorage,
         [&](Message& message, std::shared_ptr<MessageDispatcher::CallerData> callerData) {
             (void)message;
             (void)callerData;
         },
         nullptr,
         false},
        {Message::VCM,
         VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::LocalStorage,
         nullptr,
         [&](Message& message) { (void)message; },
         true},
    };

    EXPECT_CALL(
        messageDispatcher_,
        registerResponseCallback_mocked(ip_cmdbus_services[1].service_id, ip_cmdbus_services[1].operation_id, _))
        .Times(2);
    EXPECT_CALL(messageDispatcher_,
                registerNotificationCyclicCallback_mocked(
                    ip_cmdbus_services[1].service_id, ip_cmdbus_services[1].operation_id, _))
        .Times(2);
    EXPECT_CALL(
        messageDispatcher_,
        registerNotificationCallback_mocked(ip_cmdbus_services[1].service_id, ip_cmdbus_services[1].operation_id, _))
        .Times(2);

    EXPECT_CALL(messageDispatcher_, sendMessage(_, _)).Times(1);
    EXPECT_EQ(true, ipService_.Prot_RegisterClient(ip_cmdbus_services));
}

TEST_F(IpServiceFixture, RegisterServer)
{
    std::vector<IpService::ServerIpCommandBusService> ip_cmdbus_services;
    ip_cmdbus_services = {
        {VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::LocalStorage, nullptr, nullptr, nullptr},
        {VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::LocalStorage,
         [&](Message& message) { (void)message; },
         nullptr,
         nullptr},
        {VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::LocalStorage,
         [&](Message& message) { (void)message; },
         [&](Message& message) { (void)message; },
         nullptr},
        {VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::LocalStorage,
         [&](Message& message) { (void)message; },
         [&](Message& message) { (void)message; },
         [&](Message& message) { (void)message; }},
    };

    EXPECT_CALL(messageDispatcher_,
                registerRequestCallback_mocked(ip_cmdbus_services[1].service_id, ip_cmdbus_services[1].operation_id, _))
        .Times(3);
    EXPECT_CALL(
        messageDispatcher_,
        registerSetRequestCallback_mocked(ip_cmdbus_services[1].service_id, ip_cmdbus_services[1].operation_id, _))
        .Times(2);
    EXPECT_CALL(messageDispatcher_,
                registerNotificationRequestCallback_mocked(
                    ip_cmdbus_services[1].service_id, ip_cmdbus_services[1].operation_id, _))
        .Times(1);

    EXPECT_EQ(true, ipService_.Prot_RegisterServer(ip_cmdbus_services));
}

TEST_F(IpServiceFixture, ResponseOnRequest)
{
    VccIpCmd::ServiceId service = VccIpCmd::ServiceId::CommonPhoneTelematics;
    VccIpCmd::OperationId opid = VccIpCmd::OperationId::CallHandling;

    MessageDispatcher::ResponseMessageCallback msgReceived = nullptr;
    std::shared_ptr<MessageDispatcher::CallerData> callerData = nullptr;

    bool called = false;
    {
        EXPECT_CALL(messageDispatcher_, registerResponseCallback_mocked(service, opid, _))
            .WillOnce(::testing::SaveArg<2>(&msgReceived));
        EXPECT_CALL(messageDispatcher_, sendMessage(_, _)).WillOnce(::testing::SaveArg<1>(&callerData));
        ASN_Session_Reset(ipService_.m_session_msgd);
        Icb_OpWLANAPSSID_SetRequest request = Icb_OpWLANAPSSID_SetRequest_Create(ipService_.m_session_msgd);

        auto responseHandler = [&](std::shared_ptr<MessageDispatcher::CallerData> callerData,
                                   Icb_OpWLANAPSSID_Response response) {
            (void)callerData;
            (void)response;
            called = true;
        };

        ipService_.SendSetRequestWithReply<Icb_OpWLANAPSSID_SetRequest, Icb_OpWLANAPSSID_Response>(
            request,
            Icb_OpWLANAPSSID_SetRequest_EncodedSize,
            Icb_OpWLANAPSSID_SetRequest_Encode,
            Icb_OpWLANAPSSID_Response_Decode,
            Icb_OpWLANAPSSID_Response_Create,
            service,
            opid,
            Message::Ecu::VCM,
            responseHandler);
    }

    ASSERT_TRUE(bool(msgReceived));
    ASSERT_TRUE(callerData != nullptr);
    Message dummy;
    dummy.pdu.payload.resize(1000);  // Fill with junk so decode message doesn't complain

    callerData->errorType = ITransportServices::ErrorType::OK;
    msgReceived(dummy, callerData);
    EXPECT_TRUE(called);
}
