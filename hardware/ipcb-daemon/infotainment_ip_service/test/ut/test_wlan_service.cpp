/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <functional>
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_ip_service.h"
#include "mocks/mock_wlan_service_stub.h"
#include "services/wlan_service.h"

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

class WlanServiceFixture : public ::testing::Test
{
public:
    WlanServiceFixture()
    {
    }

    virtual void SetUp()
    {
        fflush(stderr);
        ip_service_.setDispatcher(nullptr);
        wlan_service_.wlan_service_stub_ = p_wlan_service_stub_;
    }

    virtual void TearDown()
    {
    }

    WlanService wlan_service_;

    // The mock class (IpService) is the base class of the test class (WlanService).
    // (In fact IpService is used as a utility class, _not_ a base class in the classic sense with method overriding.)
    // So the test object and mock object are the one and same.
    // Just having this reference to be able to avoid confusion of whether context is test or mock...
    NiceMock<MockIpService> &ip_service_ = wlan_service_;

    std::shared_ptr<NiceMock<MockWlanStub>> p_wlan_service_stub_ = std::make_shared<NiceMock<MockWlanStub>>();
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

TEST_F(WlanServiceFixture, TestInitialize)
{
    NiceMock<MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    // (Doesn't work with reference, == operator missing:
    //  EXPECT_CALL(ip_service_, RegisterClient(wlan_service_.ip_bus_client_subscriptions));
    // )
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(true));

    bool ret = wlan_service_.Initialize(&dispatcher);
    ASSERT_EQ(ret, true);
}

TEST_F(WlanServiceFixture, TestInitializeFailRegister)
{
    NiceMock<MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    // (Doesn't work with reference, == operator missing:
    //  EXPECT_CALL(ip_service_, RegisterClient(wlan_service_.ip_bus_client_subscriptions));
    // )
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(false));

    bool ret = wlan_service_.Initialize(&dispatcher);
    ASSERT_EQ(ret, false);
}

TEST_F(WlanServiceFixture, TestUninitialize)
{
    wlan_service_.Uninitialize();
}

static void CapiCallbackWlanStaStatus(
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _outCurrentStaWlanMode,
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus
        _outCurrentConnectionStatus,
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ssid _currentUsedSsid,
    std::vector<::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation>
        _outAvailableNetworkInfoList)
{
}

TEST_F(WlanServiceFixture, TestWlanStaStatus)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::WlanStub::WlanStaStatusReply_t reply = CapiCallbackWlanStaStatus;

    EXPECT_CALL(
        ip_service_,
        SendGenericRequest(VccIpCmd::ServiceId::WLAN,
                           VccIpCmd::OperationId::WLANSTAStatus,
                           Message::Ecu::VCM,
                           LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                               std::shared_ptr<WlanService::WlanStaStatusCallerData> p_data =
                                   std::dynamic_pointer_cast<WlanService::WlanStaStatusCallerData>(p_caller_data);
                               EXPECT_NE(nullptr, p_data);
                               EXPECT_NE(nullptr, p_data->reply_);
                           })));

    wlan_service_.WlanStaStatus(client, reply);
}

TEST_F(WlanServiceFixture, TestWlanStaStatusResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<WlanService::WlanStaStatusCallerData> p_caller_data(
        std::make_shared<WlanService::WlanStaStatusCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWLANSTAStatus_Response resp = Icb_OpWLANSTAStatus_Response_Create(session);
    resp->currentWLANSTAMode = e_Icb_WLANMode_sta;
    resp->currentConnectionStatus = e_Icb_WLANConnectionStatus_connected;
    char current_ssid[] = "testing123";
    ASN_OctetString_AssociateText(resp->currentUsedSSID, current_ssid);
    Icb_ArrayOf_AvailableNetworkInformation_SetSize(resp->availableNetworkInfoList, session, 2);

    char ssid1[] = "ssid1";
    ASN_OctetString_AssociateText(resp->availableNetworkInfoList->data_array[1]->sSID, ssid1);
    resp->availableNetworkInfoList->data_array[1]->signalStrength = 313;
    resp->availableNetworkInfoList->data_array[1]->securityType = e_Icb_WLANSecurityType_wPA2_PSK;
    resp->availableNetworkInfoList->data_array[1]->rememberedSSID = true;

    EXPECT_CALL(
        ip_service_,
        DecodeResponseWlanStaStatus(_, _, _, Icb_OpWLANSTAStatus_Response_Create, Icb_OpWLANSTAStatus_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_ = [current_ssid, ssid1, &b_callback_called](
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _outCurrentStaWlanMode,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus _outCurrentConnectionStatus,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _currentUsedSsid,
        std::vector<::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation>
            _outAvailableNetworkInfoList) {
        b_callback_called = true;
        // fprintf(stderr, "b_callback_called (callback)=%u\n", b_callback_called);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kSta, _outCurrentStaWlanMode);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kConnected,
                  _outCurrentConnectionStatus);
        ASSERT_EQ(current_ssid, _currentUsedSsid);

        ASSERT_EQ(static_cast<std::size_t>(2), _outAvailableNetworkInfoList.size());
        ASSERT_EQ(ssid1, _outAvailableNetworkInfoList[1].getSsid());
        ASSERT_EQ(static_cast<uint32_t>(313), _outAvailableNetworkInfoList[1].getSignalStrength());
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpa2Psk,
                  _outAvailableNetworkInfoList[1].getSecurityType());
        ASSERT_EQ(true, _outAvailableNetworkInfoList[1].getRememberedSsid());
    };

    wlan_service_.cbWlanStaStatusResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(WlanServiceFixture, TestWlanStaStatusResponseErrorStat)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<WlanService::WlanStaStatusCallerData> p_caller_data(
        std::make_shared<WlanService::WlanStaStatusCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseWlanStaStatus(_, _, _, _, _)).Times(0);

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_ = [&b_callback_called](
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _outCurrentStaWlanMode,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus _outCurrentConnectionStatus,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _currentUsedSsid,
        std::vector<::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation>
            _outAvailableNetworkInfoList) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    wlan_service_.cbWlanStaStatusResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(WlanServiceFixture, TestWlanStaStatusResponseDecodeError)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<WlanService::WlanStaStatusCallerData> p_caller_data(
        std::make_shared<WlanService::WlanStaStatusCallerData>());

    // ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    // ASN_Session session  = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    // Icb_OpWLANSTAStatus_Response resp = Icb_OpWLANSTAStatus_Response_Create(session);

    EXPECT_CALL(
        ip_service_,
        DecodeResponseWlanStaStatus(_, _, _, Icb_OpWLANSTAStatus_Response_Create, Icb_OpWLANSTAStatus_Response_Decode))
        .WillOnce(Return(false));

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_ = [&b_callback_called](
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _outCurrentStaWlanMode,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus _outCurrentConnectionStatus,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _currentUsedSsid,
        std::vector<::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation>
            _outAvailableNetworkInfoList) {
        b_callback_called = true;
        // fprintf(stderr, "b_callback_called (callback)=%u\n", b_callback_called);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    wlan_service_.cbWlanStaStatusResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(WlanServiceFixture, TestWlanStaStatusNotification)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWLANSTAStatus_Notification resp = Icb_OpWLANSTAStatus_Notification_Create(session);
    resp->notifyConnectionStatus = e_Icb_WLANConnectionStatus_connected;
    Icb_ArrayOf_AvailableNetworkInformation_SetSize(resp->availableNetworkInfoList, session, 2);

    char ssid1[] = "ssid1";
    ASN_OctetString_AssociateText(resp->availableNetworkInfoList->data_array[1]->sSID, ssid1);
    resp->availableNetworkInfoList->data_array[1]->signalStrength = 313;
    resp->availableNetworkInfoList->data_array[1]->securityType = e_Icb_WLANSecurityType_wPA2_PSK;
    resp->availableNetworkInfoList->data_array[1]->rememberedSSID = true;

    EXPECT_CALL(ip_service_,
                DecodeMessageWlanStaStatus(
                    _, _, Icb_OpWLANSTAStatus_Notification_Create, Icb_OpWLANSTAStatus_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(resp), Return(true)));

    EXPECT_CALL(
        *p_wlan_service_stub_,
        fireWlanStaStatusNotificationEvent(
            LambdaMatcher(
                [](const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus
                       &_outWlanConnectionStatus) {
                    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kConnected,
                              _outWlanConnectionStatus);
                }),
            LambdaMatcher([ssid1](
                const std::vector<
                    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation>
                    &_outAvailableNetworkInfoList) {
                EXPECT_EQ(static_cast<std::size_t>(2), _outAvailableNetworkInfoList.size());
                EXPECT_EQ(ssid1, _outAvailableNetworkInfoList[1].getSsid());
                EXPECT_EQ(static_cast<uint32_t>(313), _outAvailableNetworkInfoList[1].getSignalStrength());
                EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpa2Psk,
                          _outAvailableNetworkInfoList[1].getSecurityType());
                EXPECT_EQ(true, _outAvailableNetworkInfoList[1].getRememberedSsid());
            })));

    wlan_service_.cbWlanStaStatusNotification(msg);
}

TEST_F(WlanServiceFixture, TestWlanStaStatusNotificationDecodeError)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;

    EXPECT_CALL(ip_service_,
                DecodeMessageWlanStaStatus(
                    _, _, Icb_OpWLANSTAStatus_Notification_Create, Icb_OpWLANSTAStatus_Notification_Decode))
        .WillOnce(Return(false));

    EXPECT_CALL(*p_wlan_service_stub_, fireWlanStaStatusNotificationEvent(_, _)).Times(0);

    wlan_service_.cbWlanStaStatusNotification(msg);
}

void CapiCallbackWlanMode(::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                          ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _outWlanMode)
{
}

TEST_F(WlanServiceFixture, TestWlanModeRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::WlanStub::WlanModeRequestReply_t reply = CapiCallbackWlanMode;

    EXPECT_CALL(ip_service_,
                SendGenericRequest(VccIpCmd::ServiceId::WLAN,
                                   VccIpCmd::OperationId::WLANMode,
                                   Message::Ecu::VCM,
                                   LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                                       std::shared_ptr<WlanService::WlanModeCallerData> p_data =
                                           std::dynamic_pointer_cast<WlanService::WlanModeCallerData>(p_caller_data);
                                       EXPECT_NE(nullptr, p_data);
                                       EXPECT_NE(nullptr, p_data->reply_request_);
                                       EXPECT_EQ(nullptr, p_data->reply_set_request_);
                                   })));

    wlan_service_.WlanModeRequest(client, reply);
}

TEST_F(WlanServiceFixture, TestWlanModeSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::WlanStub::WlanModeSetRequestReply_t reply = CapiCallbackWlanMode;

    const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode requested_wlan_mode =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kStaWs;

    EXPECT_CALL(
        ip_service_,
        SendSetRequestWlanMode(_,
                               Icb_OpWLANMode_SetRequest_Encode,
                               Icb_OpWLANMode_SetRequest_EncodedSize,
                               VccIpCmd::ServiceId::WLAN,
                               VccIpCmd::OperationId::WLANMode,
                               Message::Ecu::VCM,
                               LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                                   std::shared_ptr<WlanService::WlanModeCallerData> p_data =
                                       std::dynamic_pointer_cast<WlanService::WlanModeCallerData>(p_caller_data);
                                   EXPECT_NE(nullptr, p_data);
                                   EXPECT_NE(nullptr, p_data->reply_set_request_);
                                   EXPECT_EQ(nullptr, p_data->reply_request_);
                               })));

    wlan_service_.WlanModeSetRequest(client, requested_wlan_mode, reply);
}

TEST_F(WlanServiceFixture, TestWlanModeResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<WlanService::WlanModeCallerData> p_caller_data(std::make_shared<WlanService::WlanModeCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWLANMode_Response resp = Icb_OpWLANMode_Response_Create(session);
    resp->wLANMode = e_Icb_WLANMode_ap;

    EXPECT_CALL(ip_service_,
                DecodeResponseWlanMode(_, _, _, Icb_OpWLANMode_Response_Create, Icb_OpWLANMode_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_request_ = [&b_callback_called](
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _outWlanMode) {
        b_callback_called = true;
        // fprintf(stderr, "b_callback_called (callback)=%u\n", b_callback_called);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kAp, _outWlanMode);
    };

    wlan_service_.cbWlanModeResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(WlanServiceFixture, TestWlanModeResponseErrorStat)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<WlanService::WlanModeCallerData> p_caller_data(std::make_shared<WlanService::WlanModeCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseWlanMode(_, _, _, _, _)).Times(0);

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_request_ = [&b_callback_called](
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _outWlanMode) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    wlan_service_.cbWlanModeResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(WlanServiceFixture, TestWlanModeResponseDecodeError)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<WlanService::WlanModeCallerData> p_caller_data(std::make_shared<WlanService::WlanModeCallerData>());

    // ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    // ASN_Session session  = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    // Icb_OpWlanMode_Response resp = Icb_OpWlanMode_Response_Create(session);

    EXPECT_CALL(ip_service_,
                DecodeResponseWlanMode(_, _, _, Icb_OpWLANMode_Response_Create, Icb_OpWLANMode_Response_Decode))
        .WillOnce(Return(false));

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_request_ = [&b_callback_called](
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _outWlanMode) {
        b_callback_called = true;
        // fprintf(stderr, "b_callback_called (callback)=%u\n", b_callback_called);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    wlan_service_.cbWlanModeResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(WlanServiceFixture, TestWlanModeNotification)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWLANMode_Notification notif = Icb_OpWLANMode_Notification_Create(session);
    notif->wLANMode = e_Icb_WLANMode_staWS;

    EXPECT_CALL(ip_service_,
                DecodeMessageWlanMode(_, _, Icb_OpWLANMode_Notification_Create, Icb_OpWLANMode_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notif), Return(true)));

    EXPECT_CALL(*p_wlan_service_stub_,
                fireWlanModeNotificationEvent(
                    LambdaMatcher([](const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode &_wlanMode) {
                        EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kStaWs, _wlanMode);
                    })));

    wlan_service_.cbWlanModeNotification(msg);
}

TEST_F(WlanServiceFixture, TestWlanModeNotificationDecodeError)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;

    EXPECT_CALL(ip_service_,
                DecodeMessageWlanMode(_, _, Icb_OpWLANMode_Notification_Create, Icb_OpWLANMode_Notification_Decode))
        .WillOnce(Return(false));

    EXPECT_CALL(*p_wlan_service_stub_, fireWlanModeNotificationEvent(_)).Times(0);

    wlan_service_.cbWlanModeNotification(msg);
}

static void CapiCallbackWlanStaConnect(
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus
        _outCurrentWlanConnectStatus,
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ssid _outCurrentSsid)
{
}

TEST_F(WlanServiceFixture, TestWlanStaConnectSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::WlanStub::WlanStaConnectSetRequestReply_t reply = CapiCallbackWlanStaConnect;

    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid requested_ssid = "volvo740glt";
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanExternalPassword requested_passwd = "Gyllenhammar";

    EXPECT_CALL(ip_service_,
                SendSetRequestWlanStaConnect(
                    _,
                    Icb_OpWLANSTAConnect_SetRequest_Encode,
                    Icb_OpWLANSTAConnect_SetRequest_EncodedSize,
                    VccIpCmd::ServiceId::WLAN,
                    VccIpCmd::OperationId::WLANSTAConnect,
                    Message::Ecu::VCM,
                    LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                        std::shared_ptr<WlanService::WlanStaConnectCallerData> p_data =
                            std::dynamic_pointer_cast<WlanService::WlanStaConnectCallerData>(p_caller_data);
                        EXPECT_NE(nullptr, p_data);
                        EXPECT_NE(nullptr, p_data->reply_set_request_);
                    })));

    wlan_service_.WlanStaConnectSetRequest(client, requested_ssid, requested_passwd, reply);
}

TEST_F(WlanServiceFixture, TestWlanStaConnectResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<WlanService::WlanStaConnectCallerData> p_caller_data(
        std::make_shared<WlanService::WlanStaConnectCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWLANSTAConnect_Response resp = Icb_OpWLANSTAConnect_Response_Create(session);

    char ssid1[] = "ssid1";
    ASN_OctetString_AssociateText(resp->currentSSID, ssid1);
    resp->wLANConnectStatus = e_Icb_WLANConnectStatus_wrongPassword;

    EXPECT_CALL(ip_service_,
                DecodeResponseWlanStaConnect(
                    _, _, _, Icb_OpWLANSTAConnect_Response_Create, Icb_OpWLANSTAConnect_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_set_request_ = [ssid1, &b_callback_called](
        const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors error,
        const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus status,
        const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid ssid) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, error);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kWrongPassword, status);
        ASSERT_EQ(ssid1, ssid);
    };

    wlan_service_.cbWlanStaConnectResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(WlanServiceFixture, TestWlanStaConnectNotification)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWLANSTAConnect_Response notif = Icb_OpWLANSTAConnect_Response_Create(session);
    char ssid1[] = "ssid1";
    ASN_OctetString_AssociateText(notif->currentSSID, ssid1);
    notif->wLANConnectStatus = e_Icb_WLANConnectStatus_connected;

    EXPECT_CALL(
        ip_service_,
        DecodeMessageWlanStaConnect(_, _, Icb_OpWLANSTAConnect_Response_Create, Icb_OpWLANSTAConnect_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notif), Return(true)));

    EXPECT_CALL(
        *p_wlan_service_stub_,
        fireWlanStaConnectNotificationEvent(
            LambdaMatcher(
                [ssid1](const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid &ssid) { EXPECT_EQ(ssid1, ssid); }),
            LambdaMatcher([](const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus &connect_status) {
                EXPECT_EQ(e_Icb_WLANConnectStatus_connected, connect_status);
            })));

    wlan_service_.cbWlanStaConnectNotification(msg);
}

static void CapiCallbackWlanStaDisconnect(
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
    ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus
        _outCurrentWlanDisconnectStatus)
{
}

TEST_F(WlanServiceFixture, TestWlanStaDisconnect)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::WlanStub::WlanStaDisconnectRequestReply_t reply = CapiCallbackWlanStaDisconnect;

    EXPECT_CALL(
        ip_service_,
        SendGenericRequest(VccIpCmd::ServiceId::WLAN,
                           VccIpCmd::OperationId::WLANSTADisconnect,
                           Message::Ecu::VCM,
                           LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                               std::shared_ptr<WlanService::WlanStaDisconnectCallerData> p_data =
                                   std::dynamic_pointer_cast<WlanService::WlanStaDisconnectCallerData>(p_caller_data);
                               EXPECT_NE(nullptr, p_data);
                               EXPECT_NE(nullptr, p_data->reply_);
                           })));

    wlan_service_.WlanStaDisconnectRequest(client, reply);
}

TEST_F(WlanServiceFixture, TestWlanStaDisconnectResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<WlanService::WlanStaDisconnectCallerData> p_caller_data(
        std::make_shared<WlanService::WlanStaDisconnectCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWLANSTADisconnect_Response resp = Icb_OpWLANSTADisconnect_Response_Create(session);

    resp->wLANConnectStatus = e_Icb_WLANConnectStatus_disconnected;

    EXPECT_CALL(ip_service_,
                DecodeResponseWlanStaDisconnect(
                    _, _, _, Icb_OpWLANSTADisconnect_Response_Create, Icb_OpWLANSTADisconnect_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_ = [&b_callback_called](
        ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors error,
        ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus status) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, error);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected, status);
    };

    wlan_service_.cbWlanStaDisconnectResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(WlanServiceFixture, TestWlanChannelFreqInfoNotification)
{
    Message msg;
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpWLANChannelFreqInfo_Notification notif = Icb_OpWLANChannelFreqInfo_Notification_Create(session);
    notif->wLANChannelInfo->wlan24GhzChannelNumber = 1;
    notif->wLANChannelInfo->wLANChWidth = e_Icb_WLANChannelWidthData_width_20MHz;

    EXPECT_CALL(ip_service_,
                DecodeMessageWlanChannelFreqInfo(
                    _, _, Icb_OpWLANChannelFreqInfo_Notification_Create, Icb_OpWLANChannelFreqInfo_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notif), Return(true)));

    EXPECT_CALL(*p_wlan_service_stub_,
                fireWlanChannelFreqInfoAttributeChanged(
                    gen_conn::Wlan::WlanChannelFreqInfo(1, gen_conn::Wlan::WlanChannelWidthData::Literal::Width20MHz)));

    wlan_service_.cbWlanChannelFreqInfoNotification(msg);
}

TEST_F(WlanServiceFixture, TestWlanChannelFreqInfoNotificationDecodeError)
{
    Message msg;

    EXPECT_CALL(ip_service_,
                DecodeMessageWlanChannelFreqInfo(
                    _, _, Icb_OpWLANChannelFreqInfo_Notification_Create, Icb_OpWLANChannelFreqInfo_Notification_Decode))
        .WillOnce(Return(false));

    EXPECT_CALL(*p_wlan_service_stub_, fireWlanChannelFreqInfoAttributeChanged(_)).Times(0);

    wlan_service_.cbWlanChannelFreqInfoNotification(msg);
}
}
