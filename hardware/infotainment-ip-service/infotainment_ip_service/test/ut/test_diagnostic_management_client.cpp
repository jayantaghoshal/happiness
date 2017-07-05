/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <CommonAPI/CommonAPI.hpp>
#include <functional>
#include "cedric/mock_cedric_localconfig.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_CommonApiAttribute.h"
#include "mocks/mock_diagnostic_management_auth_proxy.h"
#include "services/diagnostic_management_client.h"
#include "v0/org/volvo/connectivity/DiagnosticManagementAuthenticationProxyBase.hpp"
#include "v0/org/volvo/connectivity/IpCommandBusTypes.hpp"

#include <future>

using namespace testing;

MockLocalconfig *mock_local_config;

namespace
{
auto ReturnCallSuccess =
    Return(ByMove(std::async(std::launch::deferred, []() { return CommonAPI::CallStatus::SUCCESS; })));
}

namespace Connectivity
{
namespace gen_conn = v0::org::volvo::connectivity;
using IPT = gen_conn::IpCommandBusTypes;

class TestDiagnosticManagementClient : public ::testing::Test
{
public:
    TestDiagnosticManagementClient()
    {
        mock_local_config = new NiceMock<MockLocalconfig>();

        ON_CALL(*proxy, getProxyStatusEvent()).WillByDefault(ReturnRef(proxyStatus));
    }

    void SetUp()
    {
        createSut();
    }

protected:
    void createSut()
    {
        sut = std::make_unique<DiagnosticManagementClient>();
        sut->Initialize(&msgDispatcher, proxy);
    }

    NiceMock<MockMessageDispatcher> msgDispatcher;
    std::shared_ptr<NiceMock<DiagnosticManagementAuthProxyMock>> proxy =
        std::make_shared<NiceMock<DiagnosticManagementAuthProxyMock>>();

    MockCommonApiEvent<CommonAPI::AvailabilityStatus> proxyStatus;
    const std::shared_ptr<CommonAPI::ClientId> client;
    const CommonAPI::ConnectionId_t clientId = "sdfjkldsfkljmdlkssdmklfj";

    ASN_BYTE m_session_buffer_msgd[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_msgd = ASN_Session_Create(m_session_buffer_msgd, sizeof(m_session_buffer_msgd));

    std::unique_ptr<DiagnosticManagementClient> sut;
};

template <class MsgTypePtr>
Message createMessage(MsgTypePtr pMsg,
                      U32 encodedSizeFunc(MsgTypePtr msg),
                      ASN_Result encodeFunc(MsgTypePtr ThisPtr, ASN_Stream stream),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const VccIpCmd::OperationType operationType,
                      const VccIpCmd::DataType dataType,
                      const VccIpCmd::SenderHandleId senderHandleId,
                      const Message::Ecu ecu)
{
    Pdu pdu;
    pdu.createHeader(serviceId, operationId, operationType, VccIpCmd::DataType::NOT_ENCODED, senderHandleId);

    const uint32_t kBufAlign = 8;
    const U32 encodedSize = (((encodedSizeFunc(pMsg) / kBufAlign) + 1) * kBufAlign);
    std::vector<uint8_t> bufStream(((ASN_STREAM_SIZE / kBufAlign) + 1) * kBufAlign);
    std::vector<uint8_t> bufPayload(encodedSize);

    ASN_Stream stream = ASN_Stream_Create(&bufStream.front(), bufStream.size());
    ASN_Stream_AttachBuffer(stream, &bufPayload.front(), bufPayload.size());

    (void)encodeFunc(pMsg, stream);
    pdu.setPayload(std::move(bufPayload));
    Message message(std::move(pdu));
    message.ecu = ecu;
    return message;
}

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

TEST_F(TestDiagnosticManagementClient, TestRequestRemoteDiagnostics_SuccessfulReturn)
{
    const CommonAPI::CallStatus callStatusRetVal = CommonAPI::CallStatus::SUCCESS;
    const IPT::DiagMgm_UserAuthCredentials userAuthRetVal = IPT::DiagMgm_UserAuthCredentials::accept;

    CommonAPI::CallInfo ci;

    EXPECT_CALL(*proxy, RequestUserAuthenticationAsync("byggare bob", "fix & trix bil AB", 9876, _, _))
        .WillOnce(DoAll(InvokeArgument<3>(callStatusRetVal, userAuthRetVal), ReturnCallSuccess));

    Icb_OpRequestUserAuthentication_Request pMsg = Icb_OpRequestUserAuthentication_Request_Create(m_session_msgd);
    pMsg->operatorName = InfotainmentIpService::Utils::CapiStringTypeToAsnUtf8String(m_session_msgd, "byggare bob");
    pMsg->serviceOrg = InfotainmentIpService::Utils::CapiStringTypeToAsnUtf8String(m_session_msgd, "fix & trix bil AB");
    pMsg->sessionSafetyCode = 9876;

    auto m = createMessage(pMsg,
                           Icb_OpRequestUserAuthentication_Request_EncodedSize,
                           Icb_OpRequestUserAuthentication_Request_Encode,
                           VccIpCmd::ServiceId::DiagnosticManagement,
                           VccIpCmd::OperationId::RequestUserAuthentication,
                           VccIpCmd::OperationType::REQUEST,
                           VccIpCmd::DataType::ENCODED,
                           0x2au,
                           Message::Ecu::VCM);

    EXPECT_CALL(msgDispatcher,
                sendMessage(LambdaMatcher([](const Message &msg) {
                                EXPECT_EQ(Message::Ecu::VCM, msg.ecu);
                                EXPECT_EQ(VccIpCmd::OperationId::RequestUserAuthentication,
                                          msg.pdu.header.operation_id);
                                EXPECT_EQ(VccIpCmd::ServiceId::DiagnosticManagement, msg.pdu.header.service_id);
                                EXPECT_EQ(VccIpCmd::OperationType::RESPONSE, msg.pdu.header.operation_type);
                                EXPECT_EQ(0x2au, msg.pdu.header.sender_handle_id & 0xFFu);
                            }),
                            _));
    msgDispatcher.simulateMessageReceived(m, nullptr);
}

TEST_F(TestDiagnosticManagementClient, TestRequestRemoteDiagnostics_CommonApiFail)
{
    const CommonAPI::CallStatus callStatusRetVal = CommonAPI::CallStatus::REMOTE_ERROR;
    const IPT::DiagMgm_UserAuthCredentials userAuthRetVal = IPT::DiagMgm_UserAuthCredentials::accept;

    CommonAPI::CallInfo ci;

    EXPECT_CALL(*proxy, RequestUserAuthenticationAsync("byggare bob", "fix & trix bil AB", 9876, _, _))
        .WillOnce(DoAll(InvokeArgument<3>(callStatusRetVal, userAuthRetVal), ReturnCallSuccess));

    Icb_OpRequestUserAuthentication_Request pMsg = Icb_OpRequestUserAuthentication_Request_Create(m_session_msgd);
    pMsg->operatorName = InfotainmentIpService::Utils::CapiStringTypeToAsnUtf8String(m_session_msgd, "byggare bob");
    pMsg->serviceOrg = InfotainmentIpService::Utils::CapiStringTypeToAsnUtf8String(m_session_msgd, "fix & trix bil AB");
    pMsg->sessionSafetyCode = 9876;

    Message m = createMessage(pMsg,
                              Icb_OpRequestUserAuthentication_Request_EncodedSize,
                              Icb_OpRequestUserAuthentication_Request_Encode,
                              VccIpCmd::ServiceId::DiagnosticManagement,
                              VccIpCmd::OperationId::RequestUserAuthentication,
                              VccIpCmd::OperationType::REQUEST,
                              VccIpCmd::DataType::ENCODED,
                              0x2au,
                              Message::Ecu::VCM);

    EXPECT_CALL(msgDispatcher,
                sendMessage(LambdaMatcher([](const Message &msg) {
                                EXPECT_EQ(Message::Ecu::VCM, msg.ecu);
                                EXPECT_EQ(VccIpCmd::OperationId::RequestUserAuthentication,
                                          msg.pdu.header.operation_id);
                                EXPECT_EQ(VccIpCmd::ServiceId::DiagnosticManagement, msg.pdu.header.service_id);
                                EXPECT_EQ(VccIpCmd::OperationType::ERROR, msg.pdu.header.operation_type);
                                EXPECT_EQ(0x2au, msg.pdu.header.sender_handle_id & 0xFFu);
                            }),
                            _))
        .Times(1);

    msgDispatcher.simulateMessageReceived(m, nullptr);
}

TEST_F(TestDiagnosticManagementClient, TestTextFromOperatorToUser_SuccessfulReturn)
{
    const CommonAPI::CallStatus callStatusRetVal = CommonAPI::CallStatus::SUCCESS;

    CommonAPI::CallInfo ci;

    EXPECT_CALL(*proxy, TextFromOperatorToUserAsync("hej hej hej", _, _))
        .WillOnce(DoAll(InvokeArgument<1>(callStatusRetVal), ReturnCallSuccess));

    Icb_OpTextFromOperatorToUser_Notification pMsg = Icb_OpTextFromOperatorToUser_Notification_Create(m_session_msgd);
    pMsg->textChatMessage = InfotainmentIpService::Utils::CapiStringTypeToAsnUtf8String(m_session_msgd, "hej hej hej");

    Message m = createMessage(pMsg,
                              Icb_OpTextFromOperatorToUser_Notification_EncodedSize,
                              Icb_OpTextFromOperatorToUser_Notification_Encode,
                              VccIpCmd::ServiceId::DiagnosticManagement,
                              VccIpCmd::OperationId::TextFromOperatorToUser,
                              VccIpCmd::OperationType::NOTIFICATION,
                              VccIpCmd::DataType::ENCODED,
                              0x05u,
                              Message::Ecu::VCM);

    msgDispatcher.simulateMessageReceived(m, nullptr);
}

class TestDiagnosticManagementClientConstructor : public TestDiagnosticManagementClient
{
    void SetUp() override{};
};

TEST_F(TestDiagnosticManagementClientConstructor, Constructor_WillRegisterCallbacks)
{
    EXPECT_CALL(msgDispatcher,
                registerRequestCallback_mocked(
                    VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::RequestUserAuthentication, _));
    EXPECT_CALL(msgDispatcher,
                registerNotificationCallback_mocked(
                    VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::TextFromOperatorToUser, _));
    createSut();
}

}  // namespace Connectivity
