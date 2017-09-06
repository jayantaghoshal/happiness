/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <functional>
#include "cedric/mock_cedric_localconfig.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "services/diagnostic_management_interfaces.h"
#include "services/diagnostic_management_service.h"
#include "services/diagnostic_management_stub.h"
#include "v0/org/volvo/connectivity/DiagnosticManagementStub.hpp"
#include "v0/org/volvo/connectivity/IpCommandBusTypes.hpp"

namespace Connectivity
{
namespace gen_conn = v0::org::volvo::connectivity;
using IPT = gen_conn::IpCommandBusTypes;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

namespace
{
void dummyResponseFunction(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error){};
}

class DiagnosticManagementStubAdapterMock : public gen_conn::DiagnosticManagementStubAdapter
{
public:
    MOCK_METHOD1(fireRemoteDiagnosticLinkAttributeChanged, void(const bool& remoteDiagnosticLink));
    MOCK_METHOD1(fireRemoteDiagnosticInfoAttributeChanged,
                 void(const gen_conn::IpCommandBusTypes::DiagMgm_RemoteDiagInfo& RemoteDiagnosticInfo));
    MOCK_METHOD1(fireCurrentJ2534SessionAttributeChanged, void(const bool& CurrentJ2534SessionLink));
    MOCK_METHOD1(fireCurrentDoIPStateAttributeChanged, void(const IPT::DiagMgm_DoIPState& CurrentDoIPState));
    MOCK_METHOD1(fireOpCurrentDoIPConnectionAttributeChanged,
                 void(const IPT::DiagMgm_CurrentDoIPConn& OpCurrentDoIPConnection));
    MOCK_METHOD0(deactivateManagedInstances, void());
};

class TestDiagnosticManagement : public ::testing::Test
{
public:
    TestDiagnosticManagement()
    {
        sut.Initialize(&msgDispatcher);
        sut.GetStub()->initStubAdapter(stubAdapter);
    }
    ~TestDiagnosticManagement()
    {
        sut.Uninitialize();
    }

    NiceMock<MockMessageDispatcher> msgDispatcher;
    std::shared_ptr<NiceMock<DiagnosticManagementStubAdapterMock>> stubAdapter =
        std::make_shared<NiceMock<DiagnosticManagementStubAdapterMock>>();
    const std::shared_ptr<CommonAPI::ClientId> client;

    ASN_BYTE m_session_buffer_msgd[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_msgd = ASN_Session_Create(m_session_buffer_msgd, sizeof(m_session_buffer_msgd));

    DiagnosticManagementService sut;
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

TEST_F(TestDiagnosticManagement, RequestRemoteDiagnostics_WhenReceivingFromHmi_WillSendToMessageDispatcher)
{
    EXPECT_CALL(msgDispatcher,
                sendMessage(LambdaMatcher([](const Message& msg) {
                                EXPECT_EQ(Message::Ecu::VCM, msg.ecu);
                                EXPECT_EQ(VccIpCmd::OperationId::RequestRemoteDiagnostics, msg.pdu.header.operation_id);
                                EXPECT_EQ(VccIpCmd::ServiceId::DiagnosticManagement, msg.pdu.header.service_id);
                                // TODO: Verify payload?
                            }),
                            _))
        .Times(1);

    sut.GetStub()->RequestRemoteDiagnostics(client, true, dummyResponseFunction);
}

TEST_F(TestDiagnosticManagement, TextUserToOperator_WhenReceivingFromHmi_WillSendToMessageDispatcher)
{
    EXPECT_CALL(msgDispatcher,
                sendMessage(LambdaMatcher([](const Message& msg) {
                                EXPECT_EQ(Message::Ecu::VCM, msg.ecu);
                                EXPECT_EQ(VccIpCmd::OperationId::TextFromUserToOperator, msg.pdu.header.operation_id);
                                EXPECT_EQ(VccIpCmd::ServiceId::DiagnosticManagement, msg.pdu.header.service_id);
                                // TODO: Verify payload?
                            }),
                            _))
        .Times(1);

    sut.GetStub()->TextFromUserToOperator(client, 4, "Hello!", dummyResponseFunction);
}

TEST_F(TestDiagnosticManagement, AbortRemoteDiagnostics_WhenReceivingFromHmi_WillSendToMessageDispatcher)
{
    EXPECT_CALL(msgDispatcher,
                sendMessage(LambdaMatcher([](const Message& msg) {
                                EXPECT_EQ(Message::Ecu::VCM, msg.ecu);
                                EXPECT_EQ(VccIpCmd::OperationId::AbortRemoteDiagnostics, msg.pdu.header.operation_id);
                                EXPECT_EQ(VccIpCmd::ServiceId::DiagnosticManagement, msg.pdu.header.service_id);
                                // TODO: Verify payload?
                            }),
                            _))
        .Times(1);

    sut.GetStub()->AbortRemoteDiagnostics(client, true, dummyResponseFunction);
}

TEST_F(TestDiagnosticManagement, UpdateDacMode_WhenReceivingFromHmi_WillSendToMessageDispatcher)
{
    EXPECT_CALL(msgDispatcher,
                sendMessage(LambdaMatcher([](const Message& msg) {
                                EXPECT_EQ(Message::Ecu::VCM, msg.ecu);
                                EXPECT_EQ(VccIpCmd::OperationId::UpdateDaCMode, msg.pdu.header.operation_id);
                                EXPECT_EQ(VccIpCmd::ServiceId::DiagnosticManagement, msg.pdu.header.service_id);
                                // TODO: Verify payload?
                            }),
                            _))
        .Times(1);

    sut.GetStub()->UpdateDaCMode(client, IPT::DiagMgm_UpdateDacMode::semiautomatic, dummyResponseFunction);
}

TEST_F(TestDiagnosticManagement, cbRemoteDiagnosticLink_WhenReceivingFromDispatcher_WillSendToHmi)
{
    EXPECT_CALL(*stubAdapter, fireRemoteDiagnosticLinkAttributeChanged(true));

    Icb_OpRemoteDiagnosticLink_Notification pMsg = Icb_OpRemoteDiagnosticLink_Notification_Create(m_session_msgd);
    pMsg->remoteDiagLink = true;

    Message m = createMessage(pMsg,
                              Icb_OpRemoteDiagnosticLink_Notification_EncodedSize,
                              Icb_OpRemoteDiagnosticLink_Notification_Encode,
                              VccIpCmd::ServiceId::DiagnosticManagement,
                              VccIpCmd::OperationId::RemoteDiagnosticLink,
                              VccIpCmd::OperationType::NOTIFICATION,
                              VccIpCmd::DataType::ENCODED,
                              0,
                              Message::Ecu::VCM);
    msgDispatcher.simulateMessageReceived(m, nullptr);
    EXPECT_TRUE(sut.GetStub()->getRemoteDiagnosticLinkAttribute(client));
}

TEST_F(TestDiagnosticManagement, cbRemoteDiagnosticInfo_WhenReceivingFromDispatcher_WillSendToHmi)
{
    EXPECT_CALL(
        *stubAdapter,
        fireRemoteDiagnosticInfoAttributeChanged(IPT::DiagMgm_RemoteDiagInfo((IPT::DiagMgm_RemoteDiagInfo::started))));

    Icb_OpRemoteDiagnosticInfo_Notification pMsg = Icb_OpRemoteDiagnosticInfo_Notification_Create(m_session_msgd);
    pMsg->remoteDiagInfo = Icb_RemoteDiagInfo_t::e_Icb_RemoteDiagInfo_started;

    Message m = createMessage(pMsg,
                              Icb_OpRemoteDiagnosticInfo_Notification_EncodedSize,
                              Icb_OpRemoteDiagnosticInfo_Notification_Encode,
                              VccIpCmd::ServiceId::DiagnosticManagement,
                              VccIpCmd::OperationId::RemoteDiagnosticInfo,
                              VccIpCmd::OperationType::NOTIFICATION,
                              VccIpCmd::DataType::ENCODED,
                              0,
                              Message::Ecu::VCM);

    msgDispatcher.simulateMessageReceived(m, nullptr);
    EXPECT_EQ(IPT::DiagMgm_RemoteDiagInfo::started, sut.GetStub()->getRemoteDiagnosticInfoAttribute(client));
}

TEST_F(TestDiagnosticManagement, cbCurrentJ2534Session_WhenReceivingFromDispatcher_WillSendToHmi)
{
    EXPECT_CALL(*stubAdapter, fireCurrentJ2534SessionAttributeChanged(false));

    Icb_OpCurrentJ2534Session_Notification pMsg = Icb_OpCurrentJ2534Session_Notification_Create(m_session_msgd);
    pMsg->sessionStatus = false;

    Message m = createMessage(pMsg,
                              Icb_OpCurrentJ2534Session_Notification_EncodedSize,
                              Icb_OpCurrentJ2534Session_Notification_Encode,
                              VccIpCmd::ServiceId::DiagnosticManagement,
                              VccIpCmd::OperationId::CurrentJ2534Session,
                              VccIpCmd::OperationType::NOTIFICATION,
                              VccIpCmd::DataType::ENCODED,
                              0,
                              Message::Ecu::VCM);

    msgDispatcher.simulateMessageReceived(m, nullptr);
    EXPECT_FALSE(sut.GetStub()->getCurrentJ2534SessionAttribute(client));
}

TEST_F(TestDiagnosticManagement, CurrentDOIPState_WhenReceivingFromDispatcher_WillSendToHmi)
{
    IPT::DiagMgm_DoIPState expected(true, IPT::DiagMgm_DoIPMode::remote, IPT::DiagMgm_DoIPPhase::announcement);

    EXPECT_CALL(*stubAdapter, fireCurrentDoIPStateAttributeChanged(expected));
    Icb_OpCurrentDoIPState_Notification pMsg = Icb_OpCurrentDoIPState_Notification_Create(m_session_msgd);
    pMsg->doIPState = true;
    pMsg->doIPPhase = Icb_DoIPPhase_t::e_Icb_DoIPPhase_announcement;
    pMsg->doIPMode = Icb_DoIPMode_t::e_Icb_DoIPMode_remote;

    Message m = createMessage(pMsg,
                              Icb_OpCurrentDoIPState_Notification_EncodedSize,
                              Icb_OpCurrentDoIPState_Notification_Encode,
                              VccIpCmd::ServiceId::DiagnosticManagement,
                              VccIpCmd::OperationId::CurrentDoIPState,
                              VccIpCmd::OperationType::NOTIFICATION,
                              VccIpCmd::DataType::ENCODED,
                              0,
                              Message::Ecu::VCM);

    msgDispatcher.simulateMessageReceived(m, nullptr);
    EXPECT_EQ(expected, sut.GetStub()->getCurrentDoIPStateAttribute(client));
}

TEST_F(TestDiagnosticManagement, CurrentDOIPConnection_WhenReceivingFromDispatcher_WillSendToHmi)
{
    EXPECT_CALL(*stubAdapter,
                fireOpCurrentDoIPConnectionAttributeChanged(
                    IPT::DiagMgm_CurrentDoIPConn(IPT::DiagMgm_CurrentDoIPConn::ethernetp2p)));

    Icb_OpCurrentDoIPConnection_Notification pMsg = Icb_OpCurrentDoIPConnection_Notification_Create(m_session_msgd);
    pMsg->currentDoIPConn = Icb_CurrentDoIPConn_t::e_Icb_CurrentDoIPConn_ethernetp2p;

    Message m = createMessage(pMsg,
                              Icb_OpCurrentDoIPConnection_Notification_EncodedSize,
                              Icb_OpCurrentDoIPConnection_Notification_Encode,
                              VccIpCmd::ServiceId::DiagnosticManagement,
                              VccIpCmd::OperationId::CurrentDoIPConnection,
                              VccIpCmd::OperationType::NOTIFICATION,
                              VccIpCmd::DataType::ENCODED,
                              0,
                              Message::Ecu::VCM);

    msgDispatcher.simulateMessageReceived(m, nullptr);
    EXPECT_EQ(IPT::DiagMgm_CurrentDoIPConn::ethernetp2p, sut.GetStub()->getOpCurrentDoIPConnectionAttribute(client));
}
}
