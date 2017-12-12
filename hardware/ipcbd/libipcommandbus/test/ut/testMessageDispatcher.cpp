/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

//#include <gmock/gmock.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ivi-logging.h>

#include <cstdint>

#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/Pdu.h"
#include "ipcommandbus/idiagnostics_client.h"
#include "ipcommandbus/net_serializer.h"

#include "MockIdiagnosticsClient.h"
#include "MockTransportServices.h"
#include "cedric/cedric_localconfig.h"
#include "connectivity-sd/mock_thread_dispatcher.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Ref;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

using namespace Connectivity;
using namespace VccIpCmd;

LocalconfigStub local_config_stub;

class MessageDispatcherFixture : public ::testing::Test {
  public:
    MessageDispatcherFixture() {
        logging::ConsoleLogContext::setGlobalLogLevel(logging::LogLevel::None);

        EXPECT_CALL(transport, registerIncomingResponseCallback(_)).WillOnce(SaveArg<0>(&respMsgCb));
        EXPECT_CALL(transport, registerIncomingRequestCallback(_)).WillOnce(SaveArg<0>(&reqMsgCb));
        EXPECT_CALL(transport, registerIncomingNotificationCallback(_)).WillOnce(SaveArg<0>(&notMsgCb));
        EXPECT_CALL(transport, registerErrorOnRequestCallback(_)).WillOnce(SaveArg<0>(&errMsgCb));
        ON_CALL(transport, getThreadDispatcher()).WillByDefault(ReturnRef(mockThreadDispatcher));
        dispatcher = std::make_unique<Connectivity::MessageDispatcher>(
                &transport, &mockThreadDispatcher);  // Must be created after EXPECT_CALLS on transport
        dispatcher->setDiagnostics(&diagnostics);
    }

  protected:
    void verifyAndClear(void) {
        Mock::VerifyAndClearExpectations(&transport);
        Mock::VerifyAndClearExpectations(&diagnostics);
    }
    MockThreadDispatcher mockThreadDispatcher;
    NiceMock<MockTransportServices> transport;

    MockIDiagnosticsClient diagnostics;

    std::unique_ptr<Connectivity::MessageDispatcher> dispatcher;

    std::function<void(Message &)> respMsgCb;
    std::function<bool(Message &)> reqMsgCb;
    std::function<void(Message &)> notMsgCb;
    std::function<void(Message &, ITransportServices::ErrorType)> errMsgCb;

    std::vector<uint8_t> expectErrData = {
            0x00,
            0x01,  // service id
            0x00,
            0x02,  // operation id
            0x00, 0x00, 0x00,
            0x0B,  // length
            0x01,
            0x02,  // service handle id
            static_cast<uint8_t>(VccIpCmd::OperationType::REQUEST), 0xAA,
            0x02,                                                   // protocol version
            static_cast<uint8_t>(VccIpCmd::OperationType::ERROR),   // operation type
            static_cast<uint8_t>(VccIpCmd::DataType::NOT_ENCODED),  // data type
            0x00,                                                   // reserved
            /* data */
            static_cast<uint8_t>(ITransportServices::INVALID_OPERATION_ID),  // error code
            0x00,
            0x00  // error info
    };

    Connectivity::Message createMsg(VccIpCmd::ServiceId srvId, VccIpCmd::OperationId opId,
                                    VccIpCmd::OperationType opType, uint8_t seqNr,
                                    std::vector<uint8_t> &&data = std::vector<uint8_t>(),
                                    VccIpCmd::DataType dataType = VccIpCmd::DataType::ENCODED) {
        Pdu pdu;
        pdu.createHeader(srvId, opId, opType, dataType, seqNr);
        pdu.setPayload(std::move(data));
        Connectivity::Message msg(std::move(pdu));

        return msg;
    }

  public:
    /**
     * Accessor methods to private members of MessageDispatcher
     */
    MessageDispatcher::RequestsMap getRequestMap() { return dispatcher->m_requestsMap; }
};

MATCHER_P(LambdaMatcher, func, "") {
    bool ret = false;
    *result_listener << func(arg, ret);
    return ret;
}

TEST_F(MessageDispatcherFixture, ReceiveResponse) {
    bool msgReceived = false;

    dispatcher->registerResponseCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                         [&msgReceived](Message &m, std::shared_ptr<MessageDispatcher::CallerData> c) {
                                             (void)(m);
                                             (void)(c);
                                             msgReceived = true;
                                         });

    // We must first 'send' a request so that there is a match upon receiving the response.
    // (Otherwise the service specific callback will intentionally not be called.)
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData = std::make_shared<MessageDispatcher::CallerData>();
    Message msgReq =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04);
    dispatcher->sendMessage(std::move(msgReq), pCallerData);

    // 'Receive' response
    Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::RESPONSE, 0x04);
    msg.pdu.header.sender_handle_id = msgReq.pdu.header.sender_handle_id;

    // Expectation to make SetInvalidData execute once.
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);

    respMsgCb(msg);

    ASSERT_EQ(true, msgReceived);
}

TEST_F(MessageDispatcherFixture, ReceiveResponse_NoReceiver) {
    bool msgReceived = false;

    dispatcher->registerResponseCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                         [&msgReceived](Message &m, std::shared_ptr<MessageDispatcher::CallerData> c) {
                                             (void)(m);
                                             (void)(c);
                                             msgReceived = true;
                                         });

    Message msg = createMsg(ServiceId::Positioning, OperationId::GNSSPositionData, OperationType::RESPONSE, 0x04);

    // Expectation to make SetInvalidData execute once.
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);

    respMsgCb(msg);

    ASSERT_NE(true, msgReceived);
}

TEST_F(MessageDispatcherFixture, ReceiveRequest) {
    bool msgReceived = false;

    dispatcher->registerRequestCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                        [&msgReceived](Message &m) {
                                            (void)(m);
                                            msgReceived = true;
                                        });

    Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04);

    // Expectation to make SetInvalidData execute once.
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);

    reqMsgCb(msg);

    ASSERT_EQ(true, msgReceived);
}

TEST_F(MessageDispatcherFixture, ReceiveRequest_NoReceiver) {
    bool msgReceived = false;

    dispatcher->registerRequestCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                        [&msgReceived](Message &m) {
                                            (void)(m);
                                            msgReceived = true;
                                        });

    Message msg;

    msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::NOTIFICATION, 0x04);
    msg.ecu = Message::Ecu::VCM;
    EXPECT_CALL(transport,
                sendError(Message::Ecu::VCM, Ref(msg.pdu), ITransportServices::ErrorCode::INVALID_OPERATION_TYPE,
                          static_cast<std::uint16_t>(OperationType::NOTIFICATION)));
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);
    reqMsgCb(msg);
    ASSERT_NE(true, msgReceived);

    msg = createMsg(ServiceId::Connectivity, OperationId::GNSSPositionData, OperationType::REQUEST, 0x04);
    msg.ecu = Message::Ecu::VCM;
    EXPECT_CALL(transport,
                sendError(Message::Ecu::VCM, Ref(msg.pdu), ITransportServices::ErrorCode::INVALID_OPERATION_ID,
                          static_cast<std::uint16_t>(OperationId::GNSSPositionData)));
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);
    reqMsgCb(msg);
    ASSERT_NE(true, msgReceived);

    msg = createMsg(ServiceId::Positioning, OperationId::GNSSPositionData, OperationType::REQUEST, 0x04);
    msg.ecu = Message::Ecu::VCM;
    EXPECT_CALL(transport, sendError(Message::Ecu::VCM, Ref(msg.pdu), ITransportServices::ErrorCode::INVALID_SERVICE_ID,
                                     static_cast<std::uint16_t>(ServiceId::Positioning)));
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);
    reqMsgCb(msg);
    ASSERT_NE(true, msgReceived);
}

TEST_F(MessageDispatcherFixture, SendMessage) {
    MockThreadDispatcher mtd;
    Message expectedMsg =
            createMsg(ServiceId::Positioning, OperationId::GNSSPositionData, OperationType::RESPONSE, 0x04);

    EXPECT_CALL(transport, getThreadDispatcher()).WillOnce(ReturnRef(mtd));

    EXPECT_CALL(transport,
                sendMessage_mocked(LambdaMatcher([&expectedMsg](const Connectivity::Message &msg, bool &ret) {
                    if (msg.pdu.header.service_id != expectedMsg.pdu.header.service_id) {
                        ret = false;
                        return "ServiceId mismatch";
                    }
                    if (msg.pdu.header.operation_id != expectedMsg.pdu.header.operation_id) {
                        ret = false;
                        return "OperationId mismatch";
                    }
                    if (msg.pdu.header.operation_type != expectedMsg.pdu.header.operation_type) {
                        ret = false;
                        return "OperationType mismatch";
                    }
                    if (msg.pdu.header.sender_handle_id != expectedMsg.pdu.header.sender_handle_id) {
                        ret = false;
                        return "SenderHandleId mismatch";
                    }
                    ret = true;
                    return "";
                })))
            .Times(1);

    dispatcher->sendMessage(std::move(expectedMsg));
}

TEST_F(MessageDispatcherFixture, TestNotification) {
    bool msgReceived = false;

    dispatcher->registerNotificationCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                             [&msgReceived](Message &m) {
                                                 (void)(m);
                                                 msgReceived = true;
                                             });

    Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::NOTIFICATION, 0x04);

    // Expectation to make SetInvalidData execute once.
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);

    notMsgCb(msg);

    ASSERT_EQ(true, msgReceived);
}

TEST_F(MessageDispatcherFixture, TestNotification_no_receiver) {
    bool msgReceived = false;

    dispatcher->registerNotificationCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                             [&msgReceived](Message &m) {
                                                 (void)(m);
                                                 msgReceived = true;
                                             });

    Message msg = createMsg(ServiceId::Positioning, OperationId::GNSSPositionData, OperationType::NOTIFICATION, 0x04);

    // Expectation to make SetInvalidData execute once.
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);

    notMsgCb(msg);

    ASSERT_EQ(msgReceived, false);
}

TEST_F(MessageDispatcherFixture, TestError) {
    bool msgReceived = false;

    dispatcher->registerRequestCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                        [&msgReceived](Message &m) {
                                            (void)(m);
                                            msgReceived = true;
                                        });

    Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::ERROR, 0x04);

    errMsgCb(msg, ITransportServices::ErrorType::REMOTE_ERROR);

    ASSERT_EQ(msgReceived, false);
}

TEST_F(MessageDispatcherFixture, TestError_no_receiver) {
    bool msgReceived = false;

    dispatcher->registerRequestCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                        [&msgReceived](Message &m) {
                                            (void)(m);
                                            msgReceived = true;
                                        });

    Message msg = createMsg(ServiceId::Positioning, OperationId::GNSSPositionData, OperationType::NOTIFICATION, 0x04);

    errMsgCb(msg, ITransportServices::ErrorType::REMOTE_ERROR);

    ASSERT_EQ(msgReceived, false);
}

TEST_F(MessageDispatcherFixture, TestError_with_earlier_request_no_payload) {
    bool msgReceived = false;

    dispatcher->registerRequestCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                        [&msgReceived](Message &m) {
                                            (void)(m);
                                            msgReceived = true;
                                        });

    Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04);

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData = std::make_shared<MessageDispatcher::CallerData>();
    pCallerData->errorType = ITransportServices::ErrorType::REMOTE_ERROR;
    pCallerData->errorInfo = {true, 0xdead, 0xbeef};

    dispatcher->sendMessage(std::move(msg), pCallerData);
    errMsgCb(msg, ITransportServices::ErrorType::REMOTE_ERROR);

    ASSERT_EQ(msgReceived, false);
}

TEST_F(MessageDispatcherFixture, TestError_with_earlier_request_no_payload_no_extra_errorinfo) {
    bool msgReceived = false;

    dispatcher->registerRequestCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                        [&msgReceived](Message &m) {
                                            (void)(m);
                                            msgReceived = true;
                                        });

    Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04);

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData = std::make_shared<MessageDispatcher::CallerData>();
    pCallerData->errorType = ITransportServices::ErrorType::REMOTE_ERROR;
    pCallerData->errorInfo = {false, 0xdead, 0xbeef};

    dispatcher->sendMessage(std::move(msg), pCallerData);
    errMsgCb(msg, ITransportServices::ErrorType::REMOTE_ERROR);

    ASSERT_EQ(msgReceived, false);
}

TEST_F(MessageDispatcherFixture, TestError_with_earlier_request_incl_correct_size_payload) {
    bool msgReceived = false;

    dispatcher->registerRequestCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                        [&msgReceived](Message &m) {
                                            (void)(m);
                                            msgReceived = true;
                                        });

    std::vector<uint8_t> payload;
    payload.reserve(5);
    NetSerializer serializer(payload);
    serializer.write_uint8(static_cast<uint8_t>(ITransportServices::ErrorCode::INVALID_OPERATION_ID));
    serializer.write_uint32(0xdeadbeef);  // ErrorInfo
    serializer.write_uint32(0xfeedface);  // ErrorInfo

    Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04,
                            std::move(payload), VccIpCmd::DataType::NOT_ENCODED);
    msg.ecu = Message::IHU;

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData = std::make_shared<MessageDispatcher::CallerData>();
    pCallerData->errorType = ITransportServices::ErrorType::REMOTE_ERROR;
    pCallerData->errorInfo = {true, 0xdead, 0xbeef};

    dispatcher->sendMessage(std::move(msg), pCallerData);

    msg.pdu.payload = payload;
    errMsgCb(msg, ITransportServices::ErrorType::REMOTE_ERROR);

    ASSERT_EQ(msgReceived, false);
}

TEST_F(MessageDispatcherFixture, TestError_with_earlier_request_incl_incorrect_size_payload) {
    bool msgReceived = false;

    dispatcher->registerRequestCallback(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                        [&msgReceived](Message &m) {
                                            (void)(m);
                                            msgReceived = true;
                                        });

    std::vector<uint8_t> payload;
    payload.reserve(3);
    NetSerializer serializer(payload);
    serializer.write_uint8(static_cast<uint8_t>(ITransportServices::ErrorCode::INVALID_OPERATION_ID));
    serializer.write_uint16(0xdead);  // ErrorInfo

    Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04,
                            std::move(payload), VccIpCmd::DataType::NOT_ENCODED);
    msg.ecu = Message::IHU;

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData = std::make_shared<MessageDispatcher::CallerData>();
    pCallerData->errorType = ITransportServices::ErrorType::REMOTE_ERROR;
    pCallerData->errorInfo = {true, 0xdead, 0xbeef};

    dispatcher->sendMessage(std::move(msg), pCallerData);

    msg.pdu.payload = payload;
    errMsgCb(msg, ITransportServices::ErrorType::REMOTE_ERROR);

    ASSERT_EQ(msgReceived, false);
}

TEST_F(MessageDispatcherFixture, TestSendMessage_existing_sequence_number) {
    Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04);
    Message msg2 = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04);
    msg.ecu = Message::IHU;
    msg2.ecu = Message::IHU;

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData = std::make_shared<MessageDispatcher::CallerData>();
    pCallerData->errorType = ITransportServices::ErrorType::REMOTE_ERROR;
    pCallerData->errorInfo = {true, 0xdead, 0xbeef};

    dispatcher->sendMessage(std::move(msg), pCallerData);
    int maps = this->getRequestMap().size();
    EXPECT_EQ(maps, 1);
    dispatcher->sendMessage(std::move(msg2), pCallerData);
    maps = this->getRequestMap().size();
    EXPECT_EQ(maps, 1);

    // Cover corner coverage case of no caller data for a request (no way to test this, really)
    Message msg3 = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0x04);
    msg3.ecu = Message::IHU;
    dispatcher->sendMessage(std::move(msg3));
}
