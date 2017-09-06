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
#include "mocks/mock_telematics_service_stub.h"
#include "services/telematics_service.h"

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

class TelematicsServiceFixture : public ::testing::Test
{
public:
    TelematicsServiceFixture()
    {
    }

    virtual void SetUp()
    {
        fflush(stderr);

        ip_service_.setDispatcher(nullptr);

        telematics_service_.telematics_service_stub_ = p_telematics_service_stub_;
    }

    virtual void TearDown()
    {
    }

    TelematicsService telematics_service_;

    // The mock class (IpService) is the base class of the test class (TelematicsService).
    // (In fact IpService is used as a utility class, _not_ a base class in the classic sense with method overriding.)
    // So the test object and mock object are the one and same.
    // Just having this reference to be able to avoid confusion of whether context is test or mock...

    NiceMock<MockIpService> &ip_service_ = telematics_service_;

    std::shared_ptr<NiceMock<MockTelematicsStub>> p_telematics_service_stub_ =
        std::make_shared<NiceMock<MockTelematicsStub>>();
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

TEST_F(TelematicsServiceFixture, TestInitialize)
{
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    // (Doesn't work with reference, == operator missing:
    //  EXPECT_CALL(ip_service_, RegisterClient(wlan_service_.ip_bus_client_subscriptions));
    // )
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(true));

    bool ret = telematics_service_.Initialize(&dispatcher);

    ASSERT_EQ(ret, true);
}

TEST_F(TelematicsServiceFixture, TestInitializeFailRegister)
{
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    // (Doesn't work with reference, == operator missing:
    //  EXPECT_CALL(ip_service_, RegisterClient(wlan_service_.ip_bus_client_subscriptions));
    // )
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(false));

    bool ret = telematics_service_.Initialize(&dispatcher);

    ASSERT_EQ(ret, false);
}

TEST_F(TelematicsServiceFixture, TestUninitialize)
{
    telematics_service_.Uninitialize();
}

void CapiCallbackSoHPacketSendSetRequestNoReturn()
{
}

TEST_F(TelematicsServiceFixture, TestSoHPacketSendSetRequestNoReturn)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    const gen_conn::IpCommandBusTypes::SBTrigger trigger = gen_conn::IpCommandBusTypes::SBTrigger::kTriggerDiagnostic;
    const std::string text = "some stuff";
    const gen_conn::IpCommandBusTypes::SBWarning mainCause = gen_conn::IpCommandBusTypes::SBWarning(0, "Catastrophe");
    std::vector<gen_conn::IpCommandBusTypes::SBWarning> warnings;

    const gen_conn::IpCommandBusTypes::SBWarning mainCause1 = gen_conn::IpCommandBusTypes::SBWarning(1, "Fire");
    const gen_conn::IpCommandBusTypes::SBWarning mainCause2 = gen_conn::IpCommandBusTypes::SBWarning(2, "Flood");

    warnings.push_back(mainCause1);
    warnings.push_back(mainCause2);

    gen_conn::TelematicsStub::SoHPacketSendSetRequestNoReturnReply_t reply =
        CapiCallbackSoHPacketSendSetRequestNoReturn;

    EXPECT_CALL(ip_service_,
                SendSetRequestNoReturnOpSoHPacket(
                    _,
                    Icb_OpSoHPacketSend_SetRequestNoReturn_Encode,
                    Icb_OpSoHPacketSend_SetRequestNoReturn_EncodedSize,
                    VccIpCmd::ServiceId::Telematics,
                    VccIpCmd::OperationId::SoHPacketSend,
                    Message::Ecu::VCM,
                    LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                        std::shared_ptr<TelematicsService::SoHPacketSendSetRequestNoReturnCallerData> p_data =
                            std::dynamic_pointer_cast<TelematicsService::SoHPacketSendSetRequestNoReturnCallerData>(
                                p_caller_data);
                        EXPECT_NE(nullptr, p_data);
                        EXPECT_NE(nullptr, p_data->reply_);
                    })));

    telematics_service_.SoHPacketSendSetRequestNoReturn(client, trigger, text, mainCause, warnings, reply);
}

void CapiCallbackSubscriptionActivationSetRequest(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                  v0_conn::IpCommandBusTypes::TEM_ActivationStatus _status)
{
}

TEST_F(TelematicsServiceFixture, TestSubscriptionActivationSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    const v0_conn::IpCommandBusTypes::TEM_ActivationStatusChange new_status =
        v0_conn::IpCommandBusTypes::TEM_ActivationStatusChange::kActivate;
    const uint32_t activation_code = 1;
    const v0_conn::TelematicsStub::SubscriptionActivationSetRequestReply_t reply =
        CapiCallbackSubscriptionActivationSetRequest;

    EXPECT_CALL(ip_service_,
                SendSetRequestOpSubscriptionActivation(
                    _,
                    Icb_OpSubscriptionActivation_SetRequest_Encode,
                    Icb_OpSubscriptionActivation_SetRequest_EncodedSize,
                    VccIpCmd::ServiceId::Telematics,
                    VccIpCmd::OperationId::SubscriptionActivation,
                    Message::Ecu::TEM,
                    LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                        std::shared_ptr<TelematicsService::SubscriptionActivationSetRequestCallerData> p_data =
                            std::dynamic_pointer_cast<TelematicsService::SubscriptionActivationSetRequestCallerData>(
                                p_caller_data);
                        EXPECT_NE(nullptr, p_data);
                        EXPECT_NE(nullptr, p_data->reply_);
                    })));

    telematics_service_.SubscriptionActivationSetRequest(client, new_status, activation_code, reply);
}

void CapiCallbackTelematicSettingsSetRequest(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                             v0_conn::IpCommandBusTypes::TEM_TelematicSettings _outTelematicSettings)
{
}

TEST_F(TelematicsServiceFixture, TestTelematicSettingsSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    v0_conn::IpCommandBusTypes::TEM_TelematicSettings telematicSettings;
    const v0_conn::TelematicsStub::TelematicSettingsSetRequestReply_t reply = CapiCallbackTelematicSettingsSetRequest;

    EXPECT_CALL(
        ip_service_,
        SendSetRequestOpTelematicSettings(
            _,
            Icb_OpTelematicSettings_SetRequest_Encode,
            Icb_OpTelematicSettings_SetRequest_EncodedSize,
            VccIpCmd::ServiceId::Telematics,
            VccIpCmd::OperationId::TelematicSettings,
            Message::Ecu::TEM,
            LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                std::shared_ptr<TelematicsService::TelematicSettingsSetRequestCallerData> p_data =
                    std::dynamic_pointer_cast<TelematicsService::TelematicSettingsSetRequestCallerData>(p_caller_data);
                EXPECT_NE(nullptr, p_data);
                EXPECT_NE(nullptr, p_data->reply_);
            })));

    telematics_service_.TelematicSettingsSetRequest(client, telematicSettings, reply);
}

void CapiCallbackECallSettingsSetRequest(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                         v0_conn::IpCommandBusTypes::TEM_PreferredEcalls _inECallSettings)
{
}

TEST_F(TelematicsServiceFixture, TestECallSettingsSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    v0_conn::IpCommandBusTypes::TEM_PreferredEcalls eCallSettings;
    const v0_conn::TelematicsStub::ECallSettingsSetRequestReply_t reply = CapiCallbackECallSettingsSetRequest;

    EXPECT_CALL(
        ip_service_,
        SendSetRequestOpECallSettings(
            _,
            Icb_OpEcallSettings_SetRequest_Encode,
            Icb_OpEcallSettings_SetRequest_EncodedSize,
            VccIpCmd::ServiceId::Telematics,
            VccIpCmd::OperationId::EcallSettings,
            Message::Ecu::TEM,
            LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                std::shared_ptr<TelematicsService::ECallSettingsSetRequestCallerData> p_data =
                    std::dynamic_pointer_cast<TelematicsService::ECallSettingsSetRequestCallerData>(p_caller_data);
                EXPECT_NE(nullptr, p_data);
                EXPECT_NE(nullptr, p_data->reply_);
            })));

    telematics_service_.ECallSettingsSetRequest(client, eCallSettings, reply);
}

void CapiCallbackPositionDataRequest(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                     v0_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition _position)
{
}

TEST_F(TelematicsServiceFixture, TestPositionDataRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    v0_conn::TelematicsStub::PositionDataRequestReply_t reply = CapiCallbackPositionDataRequest;

    EXPECT_CALL(ip_service_,
                SendGenericRequest(VccIpCmd::ServiceId::Telematics,
                                   VccIpCmd::OperationId::PositionData,
                                   Message::Ecu::TEM,
                                   LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                                       std::shared_ptr<TelematicsService::PositionDataRequestCallerData> p_data =
                                           std::dynamic_pointer_cast<TelematicsService::PositionDataRequestCallerData>(
                                               p_caller_data);
                                       EXPECT_NE(nullptr, p_data);
                                       EXPECT_NE(nullptr, p_data->reply_);
                                   })));

    telematics_service_.PositionDataRequest(client, reply);
}

void CapiCallbackServiceActivationSetRequest(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                             std::string responseOk)
{
}

TEST_F(TelematicsServiceFixture, TestServiceActivationSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    v0_conn::TelematicsStub::ServiceActivationSetRequestReply_t reply = CapiCallbackServiceActivationSetRequest;

    EXPECT_CALL(
        ip_service_,
        SendSetRequestOpServiceActivation(
            _,
            Icb_OpServiceActivation_SetRequest_Encode,
            Icb_OpServiceActivation_SetRequest_EncodedSize,
            VccIpCmd::ServiceId::Telematics,
            VccIpCmd::OperationId::ServiceActivation,
            Message::Ecu::TEM,
            LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                std::shared_ptr<TelematicsService::ServiceActivationSetRequestCallerData> p_data =
                    std::dynamic_pointer_cast<TelematicsService::ServiceActivationSetRequestCallerData>(p_caller_data);
                EXPECT_NE(nullptr, p_data);
                EXPECT_NE(nullptr, p_data->reply_);
            })));

    v0_conn::IpCommandBusTypes::TEM_OnCallService service;
    v0_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting action;
    telematics_service_.ServiceActivationSetRequest(client, service, action, reply);
}

void CapiCallbackSVTStatusRequest(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                  v0_conn::IpCommandBusTypes::SecurityStatus svtStatus)
{
}

TEST_F(TelematicsServiceFixture, TestSVTStatusRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    v0_conn::TelematicsStub::SVTStatusRequestReply_t reply = CapiCallbackSVTStatusRequest;

    EXPECT_CALL(ip_service_,
                SendGenericRequest(VccIpCmd::ServiceId::Telematics,
                                   VccIpCmd::OperationId::SVTStatus,
                                   Message::Ecu::TEM,
                                   LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                                       std::shared_ptr<TelematicsService::SVTStatusRequestCallerData> p_data =
                                           std::dynamic_pointer_cast<TelematicsService::SVTStatusRequestCallerData>(
                                               p_caller_data);
                                       EXPECT_NE(nullptr, p_data);
                                       EXPECT_NE(nullptr, p_data->reply_);
                                   })));

    telematics_service_.SVTStatusRequest(client, reply);
}

void CapiCallbackTNStatusRequest(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                 v0_conn::IpCommandBusTypes::SecurityStatus svtStatus)
{
}

TEST_F(TelematicsServiceFixture, TestTNStatusRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    v0_conn::TelematicsStub::TNStatusRequestReply_t reply = CapiCallbackTNStatusRequest;

    EXPECT_CALL(ip_service_,
                SendGenericRequest(VccIpCmd::ServiceId::Telematics,
                                   VccIpCmd::OperationId::TNStatus,
                                   Message::Ecu::TEM,
                                   LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                                       std::shared_ptr<TelematicsService::TNStatusRequestCallerData> p_data =
                                           std::dynamic_pointer_cast<TelematicsService::TNStatusRequestCallerData>(
                                               p_caller_data);
                                       EXPECT_NE(nullptr, p_data);
                                       EXPECT_NE(nullptr, p_data->reply_);
                                   })));

    telematics_service_.TNStatusRequest(client, reply);
}

TEST_F(TelematicsServiceFixture, TestSubscriptionActivationResponse)
{
    Message msg;
    std::shared_ptr<TelematicsService::SubscriptionActivationSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::SubscriptionActivationSetRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpSubscriptionActivation_Response response = Icb_OpSubscriptionActivation_Response_Create(session);

    response->status = e_Icb_ActivationStatus_activated_provisioned;
    v0_conn::IpCommandBusTypes::TEM_ActivationStatus status;

    EXPECT_CALL(
        ip_service_,
        DecodeResponseSubscriptionActivation(
            _, _, _, Icb_OpSubscriptionActivation_Response_Create, Icb_OpSubscriptionActivation_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(response), Return(true)));

    bool callback_called = false;

    p_caller_data->reply_ = [status, &callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                       v0_conn::IpCommandBusTypes::TEM_ActivationStatus _status) {
        callback_called = true;

        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
        EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kActivatedProvisioned, _status);
    };

    telematics_service_.cbTelematicsSubscriptionActivationResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestSubscriptionActivationResponseError)
{
    Message msg;
    std::shared_ptr<TelematicsService::SubscriptionActivationSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::SubscriptionActivationSetRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseSubscriptionActivation(_, _, _, _, _)).Times(0);

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_ActivationStatus _status) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    telematics_service_.cbTelematicsSubscriptionActivationResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestSubscriptionActivationResponseDecodeError)
{
    Message msg;
    std::shared_ptr<TelematicsService::SubscriptionActivationSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::SubscriptionActivationSetRequestCallerData>());

    EXPECT_CALL(
        ip_service_,
        DecodeResponseSubscriptionActivation(
            _, _, _, Icb_OpSubscriptionActivation_Response_Create, Icb_OpSubscriptionActivation_Response_Decode))
        .WillOnce(Return(false));

    bool callback_called = false;
    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_ActivationStatus _status) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    telematics_service_.cbTelematicsSubscriptionActivationResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestPositionDataResponse)
{
    Message msg;
    std::shared_ptr<TelematicsService::PositionDataRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::PositionDataRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPositionData_Response response = Icb_OpPositionData_Response_Create(session);

    v0_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition position;

    EXPECT_CALL(
        ip_service_,
        DecodeResponsePositionData(_, _, _, Icb_OpPositionData_Response_Create, Icb_OpPositionData_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(response), Return(true)));

    bool callback_called = false;

    p_caller_data->reply_ = [position, &callback_called](
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        v0_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition _position) {
        callback_called = true;

        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    telematics_service_.cbTelematicsPositionDataResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestPositionDataResponseError)
{
    Message msg;
    std::shared_ptr<TelematicsService::PositionDataRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::PositionDataRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponsePositionData(_, _, _, _, _)).Times(0);

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition _position) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    telematics_service_.cbTelematicsPositionDataResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestPositionDataResponseDecodeError)
{
    Message msg;
    std::shared_ptr<TelematicsService::PositionDataRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::PositionDataRequestCallerData>());

    EXPECT_CALL(
        ip_service_,
        DecodeResponsePositionData(_, _, _, Icb_OpPositionData_Response_Create, Icb_OpPositionData_Response_Decode))
        .WillOnce(Return(false));

    bool callback_called = false;
    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition _position) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    telematics_service_.cbTelematicsPositionDataResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestServiceActivationResponse)
{
    Message msg;
    std::shared_ptr<TelematicsService::ServiceActivationSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::ServiceActivationSetRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpServiceActivation_Response response = Icb_OpServiceActivation_Response_Create(session);

    EXPECT_CALL(ip_service_,
                DecodeResponseServiceActivation(
                    _, _, _, Icb_OpServiceActivation_Response_Create, Icb_OpServiceActivation_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(response), Return(true)));

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               std::string _responseOk) {
        callback_called = true;

        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    telematics_service_.cbTelematicsServiceActivationResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestServiceActivationResponseError)
{
    Message msg;
    std::shared_ptr<TelematicsService::ServiceActivationSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::ServiceActivationSetRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseServiceActivation(_, _, _, _, _)).Times(0);

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               std::string _responseOk) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    telematics_service_.cbTelematicsServiceActivationResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestServiceActivationResponseDecodeError)
{
    Message msg;
    std::shared_ptr<TelematicsService::ServiceActivationSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::ServiceActivationSetRequestCallerData>());

    EXPECT_CALL(ip_service_,
                DecodeResponseServiceActivation(
                    _, _, _, Icb_OpServiceActivation_Response_Create, Icb_OpServiceActivation_Response_Decode))
        .WillOnce(Return(false));

    bool callback_called = false;
    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               std::string _responseOk) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    telematics_service_.cbTelematicsServiceActivationResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestSVTStatusResponse)
{
    Message msg;
    std::shared_ptr<TelematicsService::SVTStatusRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::SVTStatusRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpSVTStatus_Response response = Icb_OpSVTStatus_Response_Create(session);

    EXPECT_CALL(ip_service_,
                DecodeResponseSVTStatus(_, _, _, Icb_OpSVTStatus_Response_Create, Icb_OpSVTStatus_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(response), Return(true)));

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::SecurityStatus svtStatus) {
        callback_called = true;

        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    telematics_service_.cbTelematicsSVTStatusResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestSVTStatusResponseError)
{
    Message msg;
    std::shared_ptr<TelematicsService::SVTStatusRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::SVTStatusRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseSVTStatus(_, _, _, _, _)).Times(0);

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::SecurityStatus svtStatus) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    telematics_service_.cbTelematicsSVTStatusResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestSVTStatusResponseDecodeError)
{
    Message msg;
    std::shared_ptr<TelematicsService::SVTStatusRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::SVTStatusRequestCallerData>());

    EXPECT_CALL(ip_service_,
                DecodeResponseSVTStatus(_, _, _, Icb_OpSVTStatus_Response_Create, Icb_OpSVTStatus_Response_Decode))
        .WillOnce(Return(false));

    bool callback_called = false;
    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::SecurityStatus svtStatus) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    telematics_service_.cbTelematicsSVTStatusResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestTNStatusResponse)
{
    Message msg;
    std::shared_ptr<TelematicsService::TNStatusRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::TNStatusRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpTNStatus_Response response = Icb_OpTNStatus_Response_Create(session);

    EXPECT_CALL(ip_service_,
                DecodeResponseTNStatus(_, _, _, Icb_OpTNStatus_Response_Create, Icb_OpTNStatus_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(response), Return(true)));

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::SecurityStatus TNStatus) {
        callback_called = true;

        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    telematics_service_.cbTelematicsTNStatusResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestTNStatusResponseError)
{
    Message msg;
    std::shared_ptr<TelematicsService::TNStatusRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::TNStatusRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseTNStatus(_, _, _, _, _)).Times(0);

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::SecurityStatus TNStatus) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    telematics_service_.cbTelematicsTNStatusResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestTNStatusResponseDecodeError)
{
    Message msg;
    std::shared_ptr<TelematicsService::TNStatusRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::TNStatusRequestCallerData>());

    EXPECT_CALL(ip_service_,
                DecodeResponseTNStatus(_, _, _, Icb_OpTNStatus_Response_Create, Icb_OpTNStatus_Response_Decode))
        .WillOnce(Return(false));

    bool callback_called = false;
    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::SecurityStatus TNStatus) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    telematics_service_.cbTelematicsTNStatusResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestTelematicSettingResponse)
{
    Message msg;

    std::shared_ptr<TelematicsService::TelematicSettingsSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::TelematicSettingsSetRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpTelematicSettings_Response response = Icb_OpTelematicSettings_Response_Create(session);

    EXPECT_CALL(ip_service_,
                DecodeResponseTelematicSetting(
                    _, _, _, Icb_OpTelematicSettings_Response_Create, Icb_OpTelematicSettings_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(response), Return(true)));

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_TelematicSettings telematicSettings) {
        callback_called = true;

        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    telematics_service_.cbTelematicSettingstResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestTelematicSettingResponseError)
{
    Message msg;
    std::shared_ptr<TelematicsService::TelematicSettingsSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::TelematicSettingsSetRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseTelematicSetting(_, _, _, _, _)).Times(0);

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_TelematicSettings telematicSettings) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    telematics_service_.cbTelematicSettingstResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestTelematicSettingResponseDecodeError)
{
    Message msg;
    std::shared_ptr<TelematicsService::TelematicSettingsSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::TelematicSettingsSetRequestCallerData>());

    EXPECT_CALL(ip_service_,
                DecodeResponseTelematicSetting(
                    _, _, _, Icb_OpTelematicSettings_Response_Create, Icb_OpTelematicSettings_Response_Decode))
        .WillOnce(Return(false));

    bool callback_called = false;
    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_TelematicSettings telematicSettings) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    telematics_service_.cbTelematicSettingstResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestECallSettingResponse)
{
    Message msg;

    std::shared_ptr<TelematicsService::ECallSettingsSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::ECallSettingsSetRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpEcallSettings_Response response = Icb_OpEcallSettings_Response_Create(session);

    EXPECT_CALL(
        ip_service_,
        DecodeResponseECallSettings(_, _, _, Icb_OpEcallSettings_Response_Create, Icb_OpEcallSettings_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(response), Return(true)));

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_PreferredEcalls preferredEcalls) {
        callback_called = true;

        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    telematics_service_.cbECallSettingsResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestECallSettingResponseError)
{
    Message msg;
    std::shared_ptr<TelematicsService::ECallSettingsSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::ECallSettingsSetRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseECallSettings(_, _, _, _, _)).Times(0);

    bool callback_called = false;

    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_PreferredEcalls preferredEcalls) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    telematics_service_.cbECallSettingsResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestECallSettingResponseDecodeError)
{
    Message msg;
    std::shared_ptr<TelematicsService::ECallSettingsSetRequestCallerData> p_caller_data(
        std::make_shared<TelematicsService::ECallSettingsSetRequestCallerData>());

    EXPECT_CALL(
        ip_service_,
        DecodeResponseECallSettings(_, _, _, Icb_OpEcallSettings_Response_Create, Icb_OpEcallSettings_Response_Decode))
        .WillOnce(Return(false));

    bool callback_called = false;
    p_caller_data->reply_ = [&callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               v0_conn::IpCommandBusTypes::TEM_PreferredEcalls preferredEcalls) {
        callback_called = true;
        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    telematics_service_.cbECallSettingsResponse(msg, p_caller_data);

    EXPECT_EQ(callback_called, true);
}

TEST_F(TelematicsServiceFixture, TestSoHPackageUploadedNotification)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpSoHPackageUploaded_Notification response = Icb_OpSoHPackageUploaded_Notification_Create(session);

    char packet_id[] = "0";

    ASN_OctetString_AssociateText(response->packetID, packet_id);
    response->result = e_Icb_GenericOkStatus_ok;

    EXPECT_CALL(ip_service_,
                DecodeMessageOpSoHPacketSend(
                    _, _, Icb_OpSoHPackageUploaded_Notification_Create, Icb_OpSoHPackageUploaded_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(response), Return(true)));

    telematics_service_.cbTelematicsSoHPackageUploadedNotification(msg);
}

TEST_F(TelematicsServiceFixture, TestSubscriptionActvationNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpSubscriptionActivation_Notification notification = Icb_OpSubscriptionActivation_Notification_Create(session);

    notification->status = e_Icb_ActivationStatus_activated_provisioned;

    EXPECT_CALL(
        ip_service_,
        DecodeMessageOpSubscriptionActivationNotification(
            _, _, Icb_OpSubscriptionActivation_Notification_Create, Icb_OpSubscriptionActivation_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notification), Return(true)));

    telematics_service_.cbTelematicsSubscriptionActivationNotification(msg);
}

TEST_F(TelematicsServiceFixture, TestRescueStatusNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpRescueStatus_Notification notification = Icb_OpRescueStatus_Notification_Create(session);

    notification->raStatus = e_Icb_RescueStatus_active;
    notification->eaStatus = e_Icb_RescueStatus_active;
    notification->sdnStatus = e_Icb_RescueStatus_active;
    notification->voiceStatus = e_Icb_VoiceStatus_connectedCall;
    notification->voiceSource = e_Icb_SourceStatus_trusted;
    notification->buttonStatus = e_Icb_ButtonStatus_onCallPressed;
    notification->psapConfirmStatus = e_Icb_PSAPStatus_confirmRequired;
    notification->backupAudioStatus = e_Icb_GenericOkStatus_ok;

    EXPECT_CALL(ip_service_,
                DecodeMessageOpRescueStatusNotification(
                    _, _, Icb_OpRescueStatus_Notification_Create, Icb_OpRescueStatus_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notification), Return(true)));

    telematics_service_.cbTelematicsRescueStatusNotification(msg);
}

TEST_F(TelematicsServiceFixture, TestSVTStatusNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpSVTStatus_Notification notification = Icb_OpSVTStatus_Notification_Create(session);

    notification->svtStatus = e_Icb_SecurityStatus_standby;

    EXPECT_CALL(ip_service_,
                DecodeMessageOpSVTStatusNotification(
                    _, _, Icb_OpSVTStatus_Notification_Create, Icb_OpSVTStatus_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notification), Return(true)));

    telematics_service_.cbTelematicsSVTStatusNotification(msg);
}

TEST_F(TelematicsServiceFixture, TestTNStatusNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpTNStatus_Notification notification = Icb_OpTNStatus_Notification_Create(session);

    notification->tnStatus = e_Icb_SecurityStatus_standby;

    EXPECT_CALL(ip_service_,
                DecodeMessageOpTNStatusNotification(
                    _, _, Icb_OpTNStatus_Notification_Create, Icb_OpTNStatus_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notification), Return(true)));

    telematics_service_.cbTelematicsTNStatusNotification(msg);
}

}  // namespace Connectivity
