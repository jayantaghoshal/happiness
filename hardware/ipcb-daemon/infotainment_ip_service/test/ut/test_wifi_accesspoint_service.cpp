/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <services/wifi_accesspoint.h>
#include "mocks/MockMessageDispatcher.h"
#include "util/type_conversion_helpers.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::SetArgPointee;

namespace gen_conn = ::v0::org::volvo::connectivity;

namespace
{
ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
ASN_BYTE stream_buffer[ASN_STREAM_SIZE];
ASN_Stream stream = ASN_Stream_Create(stream_buffer, sizeof(stream_buffer));

ASN_BYTE respond_session_buffer[ASN_SESSION_SIZE + 2048];
ASN_Session respond_session = ASN_Session_Create(respond_session_buffer, sizeof(respond_session_buffer));
// ASN_BYTE respond_stream_buffer[ASN_STREAM_SIZE];
// ASN_Stream respond_stream = ASN_Stream_Create(stream_buffer, sizeof(stream_buffer));
}

namespace Connectivity
{
class MockWifiAccesspointStubAdapter final : public gen_conn::WifiAccesspointStubAdapter
{
public:
    MOCK_METHOD1(
        fireNotifyConnectedDevicesListChangedEvent,
        void(const std::vector<v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WLANConnDevice>
                 &_connectedDevices));

    void deactivateManagedInstances()
    {
    }
};

ACTION_P2(ForwardToMessageDispatcher, forwardTo_MockMessageDispatcherPtr, fromEcu)
{
    Message msgCopy = arg0;
    msgCopy.ecu = fromEcu;
    forwardTo_MockMessageDispatcherPtr->simulateMessageReceived(msgCopy, nullptr);
}

class WifiAccesspointTestFixture : public ::testing::Test
{
public:
    WifiAccesspointTestFixture() : stubAdapter{std::make_shared<MockWifiAccesspointStubAdapter>()}
    {
        sut.GetStub()->initStubAdapter(stubAdapter);
        sut.Initialize(&ihuMessageDispatcherMock_);
        vcmIpService.setDispatcher(&vcmMessageDispatcherMock_);

        ON_CALL(ihuMessageDispatcherMock_, sendMessage(_, _))
            .WillByDefault(ForwardToMessageDispatcher(&vcmMessageDispatcherMock_, Message::Ecu::IHU));
        ON_CALL(vcmMessageDispatcherMock_, sendMessage(_, _))
            .WillByDefault(ForwardToMessageDispatcher(&ihuMessageDispatcherMock_, Message::Ecu::VCM));
    }

    NiceMock<MockMessageDispatcher> ihuMessageDispatcherMock_;
    NiceMock<MockMessageDispatcher> vcmMessageDispatcherMock_;
    IpService vcmIpService;

    const std::shared_ptr<CommonAPI::ClientId> clientIdPtr;
    const std::shared_ptr<MockWifiAccesspointStubAdapter> stubAdapter;
    WifiAccessPointService sut;
};

TEST_F(WifiAccesspointTestFixture,
       GetWifiApSettings_CalledFromCommonApi_IPCBRequestSent_OnReply_CommonapiCallbackCalled)
{
    const std::string result_ssid = "newPassword";
    const std::string result_password = "newPassword";
    const int result_nrConnectedDevices = 7;

    auto getStatusRequestHandler = [&](Message &msg) {
        ASN_Session_Reset(respond_session);
        EXPECT_EQ(VccIpCmd::ServiceId::WLAN, msg.pdu.header.service_id);
        EXPECT_EQ(VccIpCmd::OperationId::WLANAPStatusSettings, msg.pdu.header.operation_id);

        ASN_Session_Reset(respond_session);
        Icb_OpWLANAPStatusSettings_Response resp = Icb_OpWLANAPStatusSettings_Response_Create(respond_session);
        resp->currentPassword =
            InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(respond_session, result_password);
        resp->currentSSID = InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(respond_session, result_ssid);
        resp->currentUsedFrequencyBand = Icb_WLANFrequencyBand_t::e_Icb_WLANFrequencyBand_highFrequencyBand;
        resp->numberOfConnectedDevices = result_nrConnectedDevices;
        resp->wLANAPCurrentStatus = Icb_WLANAPCurrentStatus_t::e_Icb_WLANAPCurrentStatus_enabled;

        vcmIpService.SendResponse(resp,
                                  Icb_OpWLANAPStatusSettings_Response_Encode,
                                  Icb_OpWLANAPStatusSettings_Response_EncodedSize,
                                  VccIpCmd::ServiceId::WLAN,
                                  VccIpCmd::OperationId::WLANAPStatusSettings,
                                  msg.ecu,
                                  msg.pdu.header.sender_handle_id);
    };

    vcmIpService.RegisterServer({IpService::ServerIpCommandBusService{VccIpCmd::ServiceId::WLAN,
                                                                      VccIpCmd::OperationId::WLANAPStatusSettings,
                                                                      getStatusRequestHandler,
                                                                      nullptr,
                                                                      nullptr}});

    bool commonApiGetStatusCallbackCalled = false;
    sut.GetStub()->GetWlanApStatusSettings(
        clientIdPtr,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanApCurrentStatus _WLANAPCurrentStatus,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _ssid,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanAPPassword _password,
            uint8_t _numberOfConnectedDevices,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanFrequencyBand _currentUsedFrequencyBand) {
            EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
            EXPECT_EQ(result_ssid, _ssid);
            EXPECT_EQ(result_password, _password);
            EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanApCurrentStatus::Literal::kEnabled,
                      _WLANAPCurrentStatus);
            EXPECT_EQ(result_nrConnectedDevices, _numberOfConnectedDevices);
            EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanFrequencyBand::Literal::kHighFrequencyBand,
                      _currentUsedFrequencyBand);
            commonApiGetStatusCallbackCalled = true;
        });

    EXPECT_TRUE(commonApiGetStatusCallbackCalled);
}

TEST_F(WifiAccesspointTestFixture, GetWifiApSettings_CalledFromCommonApi_NoIPCBResponse_NoCommonapiCallbackCalled)
{
    // Note that we are not waiting for timeout here
    bool commonApiGetStatusCallbackCalled = false;
    sut.GetStub()->GetWlanApStatusSettings(
        clientIdPtr,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanApCurrentStatus _WLANAPCurrentStatus,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _ssid,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanAPPassword _password,
            uint8_t _numberOfConnectedDevices,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanFrequencyBand _currentUsedFrequencyBand) {
            commonApiGetStatusCallbackCalled = true;
        });

    EXPECT_FALSE(commonApiGetStatusCallbackCalled);
}

TEST_F(WifiAccesspointTestFixture,
       GetWifiApSettings_CalledFromCommonApi_IPCBRequestSent_OnReplyWithCorruptData_CommonapiCallbackCalledWithError)
{
    auto getStatusRequestHandler = [&](Message &msg) {
        ASN_Session_Reset(respond_session);
        EXPECT_EQ(VccIpCmd::ServiceId::WLAN, msg.pdu.header.service_id);
        EXPECT_EQ(VccIpCmd::OperationId::WLANAPStatusSettings, msg.pdu.header.operation_id);

        ASN_Session_Reset(respond_session);

        // Reply with a message with empty payload to force message decode failure
        vcmIpService.SendGenericResponse(VccIpCmd::ServiceId::WLAN,
                                         VccIpCmd::OperationId::WLANAPStatusSettings,
                                         msg.ecu,
                                         msg.pdu.header.sender_handle_id,
                                         nullptr);
    };

    vcmIpService.RegisterServer({IpService::ServerIpCommandBusService{VccIpCmd::ServiceId::WLAN,
                                                                      VccIpCmd::OperationId::WLANAPStatusSettings,
                                                                      getStatusRequestHandler,
                                                                      nullptr,
                                                                      nullptr}});

    bool commonApiGetStatusCallbackCalled = false;
    sut.GetStub()->GetWlanApStatusSettings(
        clientIdPtr,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanApCurrentStatus _WLANAPCurrentStatus,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _ssid,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanAPPassword _password,
            uint8_t _numberOfConnectedDevices,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanFrequencyBand _currentUsedFrequencyBand) {
            EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
            commonApiGetStatusCallbackCalled = true;
        });

    EXPECT_TRUE(commonApiGetStatusCallbackCalled);
}

TEST_F(WifiAccesspointTestFixture,
       SetWlanSSID_SetRequestCommonApi_SentToMsgDispatcher_OnMsgDispatcherReply_ReplyOnCommonApi)
{
    const std::string ssidToSet = "newSsid";

    auto setSsidRequestHandler = [&](Message &msg) {
        ASN_Session_Reset(respond_session);
        Icb_OpWLANAPSSID_SetRequest receivedSetReq;
        vcmIpService.DecodeMessage(
            msg, receivedSetReq, Icb_OpWLANAPSSID_SetRequest_Create, Icb_OpWLANAPSSID_SetRequest_Decode);

        EXPECT_STREQ(ssidToSet.c_str(), ASN_OctetString_GetText(receivedSetReq->wLANnewSSID));

        ASN_Session_Reset(respond_session);
        Icb_OpWLANAPSSID_Response resp = Icb_OpWLANAPSSID_Response_Create(respond_session);
        resp->wLANcurrentSSID =
            InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(respond_session, ssidToSet);
        vcmIpService.SendResponse(resp,
                                  Icb_OpWLANAPSSID_Response_Encode,
                                  Icb_OpWLANAPSSID_Response_EncodedSize,
                                  VccIpCmd::ServiceId::WLAN,
                                  VccIpCmd::OperationId::WLANAPSSID,
                                  msg.ecu,
                                  msg.pdu.header.sender_handle_id);
    };

    vcmIpService.RegisterServer({IpService::ServerIpCommandBusService{
        VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPSSID, nullptr, setSsidRequestHandler, nullptr}});

    bool called = false;
    sut.GetStub()->SetWlanApSsid(clientIdPtr,
                                 ssidToSet,
                                 [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                     gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _newSsidApplied) {
                                     EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
                                     EXPECT_EQ(ssidToSet, _newSsidApplied);
                                     called = true;
                                 });
    EXPECT_TRUE(called);
}
TEST_F(WifiAccesspointTestFixture,
       SetWlanPassword_SetRequestCommonApi_SentToMsgDispatcher_OnMsgDispatcherReply_ReplyOnCommonApi)
{
    const std::string passWordToSet = "newPassword";

    auto setPasswordRequestHandler = [&](Message &msg) {
        ASN_Session_Reset(respond_session);
        Icb_OpWLANAPPasswd_SetRequest receivedSetReq;
        vcmIpService.DecodeMessage(
            msg, receivedSetReq, Icb_OpWLANAPPasswd_SetRequest_Create, Icb_OpWLANAPPasswd_SetRequest_Decode);

        EXPECT_STREQ(passWordToSet.c_str(), ASN_OctetString_GetText(receivedSetReq->wLANNewPassword));

        ASN_Session_Reset(respond_session);
        Icb_OpWLANAPPasswd_Response resp = Icb_OpWLANAPPasswd_Response_Create(respond_session);
        resp->wLANCurrentPassword =
            InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(respond_session, passWordToSet);
        vcmIpService.SendResponse(resp,
                                  Icb_OpWLANAPPasswd_Response_Encode,
                                  Icb_OpWLANAPPasswd_Response_EncodedSize,
                                  VccIpCmd::ServiceId::WLAN,
                                  VccIpCmd::OperationId::WLANAPPasswd,
                                  msg.ecu,
                                  msg.pdu.header.sender_handle_id);
    };

    vcmIpService.RegisterServer({IpService::ServerIpCommandBusService{
        VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPPasswd, nullptr, setPasswordRequestHandler, nullptr}});

    bool commonApiSetWlanApPasswordCallbackCalled = false;
    sut.GetStub()->SetWlanApPassword(
        clientIdPtr,
        passWordToSet,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanAPPassword _newPassApplied) {
            EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
            EXPECT_EQ(passWordToSet, _newPassApplied);
            commonApiSetWlanApPasswordCallbackCalled = true;
        });

    EXPECT_TRUE(commonApiSetWlanApPasswordCallbackCalled);
}

TEST_F(WifiAccesspointTestFixture,
       GetWlanApConnectedDevicesFromCommonApi_SentToMsgDispatcher_OnMsgDispatcherReply_ReplyOnCommonApi)
{
    const std::vector<std::string> result_connectedClients = {
        "hej",  // Ascii
        "sdflkjsdlfkm",
        "åölaäöpldas",                      // Non ascii
        "象形字",                           // Unicode
        "",                                 // Empty
        "12345678901234567890123456789012"  // Max length 32 bytes
    };
    ASSERT_GE(8LU, result_connectedClients.size());

    auto getConnectedDevicesRequestHandler = [&](Message &msg) {
        EXPECT_EQ(VccIpCmd::OperationId::WLANAPConnDev, msg.pdu.header.operation_id);

        ASN_Session_Reset(respond_session);
        Icb_OpWLANAPConnDev_Response resp = Icb_OpWLANAPConnDev_Response_Create(respond_session);
        resp->wLANCurrentConnDevices = Icb_ArrayOf_WLANConnDevice_Create(respond_session);
        Icb_ArrayOf_WLANConnDevice_SetSize(
            resp->wLANCurrentConnDevices, respond_session, result_connectedClients.size());
        for (size_t i = 0; i < result_connectedClients.size(); i++)
        {
            resp->wLANCurrentConnDevices->data_array[i]->deviceName =
                InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(respond_session,
                                                                             result_connectedClients[i]);
        }
        vcmIpService.SendResponse(resp,
                                  Icb_OpWLANAPConnDev_Response_Encode,
                                  Icb_OpWLANAPConnDev_Response_EncodedSize,
                                  VccIpCmd::ServiceId::WLAN,
                                  VccIpCmd::OperationId::WLANAPConnDev,
                                  msg.ecu,
                                  msg.pdu.header.sender_handle_id);
    };

    vcmIpService.RegisterServer({IpService::ServerIpCommandBusService{VccIpCmd::ServiceId::WLAN,
                                                                      VccIpCmd::OperationId::WLANAPConnDev,
                                                                      getConnectedDevicesRequestHandler,
                                                                      nullptr,
                                                                      nullptr}});

    bool commonApiCallbackCalled = false;
    sut.GetStub()->GetConnectedDevicesList(
        clientIdPtr,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WLANConnDevice> _connectedDevices) {
            EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);

            ASSERT_EQ(result_connectedClients.size(), _connectedDevices.size());
            EXPECT_EQ(result_connectedClients, _connectedDevices);
            commonApiCallbackCalled = true;
        });

    EXPECT_TRUE(commonApiCallbackCalled);
}

TEST_F(WifiAccesspointTestFixture, ConnectedDevicesBroadcastReceived_NotificationSentToCommonApi)
{
    const std::vector<std::string> result_connectedClients = {
        "hej",  // Ascii
        "sdflkjsdlfkm",
        "åölaäöpldas",                      // Non ascii
        "象形字",                           // Unicode
        "",                                 // Empty
        "12345678901234567890123456789012"  // Max length 32 bytes
    };
    ASSERT_GE(8LU, result_connectedClients.size());

    EXPECT_CALL(*stubAdapter, fireNotifyConnectedDevicesListChangedEvent(result_connectedClients));

    {
        ASN_Session_Reset(respond_session);
        Icb_OpWLANAPConnDev_Notification resp = Icb_OpWLANAPConnDev_Notification_Create(respond_session);
        resp->wLANCurrentConnDevices = Icb_ArrayOf_WLANConnDevice_Create(respond_session);
        Icb_ArrayOf_WLANConnDevice_SetSize(
            resp->wLANCurrentConnDevices, respond_session, result_connectedClients.size());
        for (size_t i = 0; i < result_connectedClients.size(); i++)
        {
            resp->wLANCurrentConnDevices->data_array[i]->deviceName =
                InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(respond_session,
                                                                             result_connectedClients[i]);
        }

        vcmIpService.SendEncodedMsg(resp,
                                    Icb_OpWLANAPConnDev_Notification_Encode,
                                    Icb_OpWLANAPConnDev_Notification_EncodedSize,
                                    VccIpCmd::ServiceId::WLAN,
                                    VccIpCmd::OperationId::WLANAPConnDev,
                                    VccIpCmd::OperationType::NOTIFICATION,
                                    Message::Ecu::IHU,
                                    0);
    }
}

}  // namespace
