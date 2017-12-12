/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ivi-logging.h>

#include "MockIdiagnosticsClient.h"
#include "MockUdpSocket.h"
#include "connectivity-sd/mock_thread_dispatcher.h"
#include "connectivity-sd/mock_time_provider.h"
#include "ipcommandbus/Pdu.h"
#include "ipcommandbus/TransportServices.h"
#include "ipcommandbus/idiagnostics_client.h"

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

using namespace Connectivity;
using namespace VccIpCmd;

class TransportServicesFixture : public ::testing::Test {
  public:
    TransportServicesFixture() : transport{timeProvider, toIpcbThreadDispatcher, Message::Ecu::IHU} {
        // Fetch the callback
        EXPECT_CALL(sock, registerReadReadyCb(_)).WillOnce(SaveArg<0>(&readyReadCb));

        transport.setSocket(&sock);
        transport.setBroadcastSocket(&sock_bcast);
        transport.setDiagnostics(&diagnostics, &toAppthreadDispatcher);
    }

  public:
    /**
     * Pass-through methods for private members of
     * class Connectivity::TransportServices.
     */
    void messageTimeout(TrackMessage &tm) {
        (void)tm;
        timeProvider.sleep_for(std::chrono::milliseconds(800));
    }

  public:
    /**
     * Accessor methods for private and protected members
     * of class Connectivity::TransportServices.
     */
    uint8_t getProtocolVersion() { return transport.PROTOCOL_VERSION; }

    void setRetry(int value, TrackMessage &tm) { tm.wfr.retry_ = value; }

    void setMaxRetries(int value, TrackMessage &tm) { tm.wfr.maxRetries_ = value; }

  protected:
    void verifyAndClear(void) {
        Mock::VerifyAndClearExpectations(&sock);
        Mock::VerifyAndClearExpectations(&diagnostics);
    }

    void getResponseSendAck(const Message &msg) {
        std::vector<uint8_t> respData, expectData;
        Connectivity::Pdu respPdu, ackPdu;
        respPdu.header = msg.pdu.header;
        respPdu.header.operation_type = OperationType::RESPONSE;  ///< Set this based on the request type later on?
        ackPdu.header = msg.pdu.header;
        ackPdu.header.operation_type = OperationType::ACK;

        respPdu.toData(respData);
        ackPdu.toData(expectData);

        EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(respData), SetArgReferee<1>(Message::VCM)));

        EXPECT_CALL(sock, writeTo(expectData, _));

        // Ensure that message is delivered to upper layers
        bool messageDelivered = false;
        transport.registerIncomingResponseCallback([&messageDelivered](Connectivity::Message &m) {
            (void)(m);
            messageDelivered = true;
        });

        // Execute: Trigger read ack
        readyReadCb();
        ASSERT_EQ(true, messageDelivered);
    }

    std::function<void(void)> readyReadCb;

    Connectivity::TimeProviderStub timeProvider;
    Connectivity::MockThreadDispatcher toIpcbThreadDispatcher;
    Connectivity::MockThreadDispatcher toAppthreadDispatcher;
    Connectivity::TransportServices transport;
    NiceMock<MockUdpSocket> sock;
    NiceMock<MockUdpSocket> sock_bcast;
    MockIDiagnosticsClient diagnostics;

    std::vector<uint8_t> requestData_ = {0x00,
                                         0xA3,
                                         0x03,
                                         0x0F,
                                         0x00,
                                         0x00,
                                         0x00,
                                         0x08,
                                         0xA3,
                                         0x0F,
                                         static_cast<uint8_t>(OperationType::REQUEST),
                                         0xAA,
                                         0x02,
                                         static_cast<uint8_t>(OperationType::REQUEST),
                                         0x00,
                                         0x00};

    std::vector<uint8_t> requestAck_ = {0x00,
                                        0xA3,
                                        0x03,
                                        0x0F,
                                        0x00,
                                        0x00,
                                        0x00,
                                        0x08,
                                        0xA3,
                                        0x0F,
                                        static_cast<uint8_t>(OperationType::REQUEST),
                                        0xAA,
                                        0x02,
                                        static_cast<uint8_t>(OperationType::ACK),
                                        0x00,
                                        0x00};

    std::vector<uint8_t> requestResponse_ = {0x00,
                                             0xA3,
                                             0x03,
                                             0x0F,
                                             0x00,
                                             0x00,
                                             0x00,
                                             0x08,
                                             0xA3,
                                             0x0F,
                                             static_cast<uint8_t>(OperationType::REQUEST),
                                             0xAA,
                                             0x02,
                                             static_cast<uint8_t>(OperationType::RESPONSE),
                                             0x00,
                                             0x00};

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
};

MATCHER_P(LambdaMatcher, func, "") {
    bool ret = false;
    *result_listener << func(arg, ret);
    return ret;
}

/**
 * Test that the ready read callback is assigned in the Transport ctor.
 * Expect:
 *      Set callback upon provided socket is called.
 */
TEST_F(TransportServicesFixture, ReadCallbackAssignment) {
    auto pSock = std::make_shared<NiceMock<MockUdpSocket> >();

    std::function<void(void)> readyReadCb;

    EXPECT_CALL(sock, registerReadReadyCb(_)).WillOnce(SaveArg<0>(&readyReadCb));
    Connectivity::TransportServices transport{timeProvider, toIpcbThreadDispatcher, Message::Ecu::IHU};
    transport.setSocket(&sock);
}

/**
 * Send request sequence on client side
 * Expect:
 *      1. Shall send data on socket
 *      2. After receiving an ACK and a response an ack shall be sent
 */
TEST_F(TransportServicesFixture, SendRequest_Success) {
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xAA);
    msg.ecu = Message::VCM;
    msg.pdu.header.protocol_version = 2;
    Connectivity::Message respAndAck;
    respAndAck.pdu.header = msg.pdu.header;

    // 1. send request
    // 2. receive ack
    // 3. receive response
    // 4. send ack

    // 1. Validate outgoing message
    EXPECT_CALL(sock, writeTo(requestData_, _));
    // EXPECT_CALL(timer, start(_,_));
    // Execute
    transport.sendMessage(std::move(msg));

    this->verifyAndClear();

    // 2. Receive ACK
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestAck_), SetArgReferee<1>(Message::VCM)));
    // Execute: Trigger read ack
    readyReadCb();
    this->verifyAndClear();

    // 3. Receive response
    // 4. Expect an ACK to be sent

    this->getResponseSendAck(respAndAck);
}

/**
 * Send request sequence on client side with an ACK timeout.
 * Expect:
 *      1. Shall send data on socket.
 *      2. First ACK shall timeout.
 *      3. Shall resend data on socket
 *      4. After receiving second ACK a response and ACK shall be sent
 */
TEST_F(TransportServicesFixture, SendRequest_ACK_Timeout) {
    // Create request message
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xAA);
    msg.ecu = Message::VCM;
    msg.pdu.header.protocol_version = 2;
    Connectivity::Message respAndAck(std::move(msg.pdu));

    // 1. Send the request
    EXPECT_CALL(sock, writeTo(requestData_, _));
    // EXPECT_CALL(timer, start(500, _)).WillOnce(SaveArg<1>(&msg1TimeoutCb));
    // Execute
    transport.sendMessage(std::move(msg));
    this->verifyAndClear();

    // 2. The ACK will time out
    // 3. Resend of message
    EXPECT_CALL(sock, writeTo(requestData_, _));
    // EXPECT_CALL(timer, restart(750));
    timeProvider.sleep_for(std::chrono::milliseconds(800));
    this->verifyAndClear();

    // 4. Ack received
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestAck_), SetArgReferee<1>(Message::VCM)));
    // EXPECT_CALL(timer, restart(1000));
    readyReadCb();
    this->verifyAndClear();

    this->getResponseSendAck(respAndAck);
}

/**
 * Send request on client side, but ACK times out until [default] number of resends finish.
 * Expect:
 *      1. Shall send data on socket.
 *      2. ACK shall timeout.
 *      3. Shall resend data on socket
 *      4. Repeat from 1 default number of times.
 *      5. Report error to higher layers.
 */
TEST_F(TransportServicesFixture, SendRequest_ACK_Timeout_No_Resends) {
    // Create request message
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xAA);
    msg.ecu = Message::VCM;
    // NOTE: Exact number just for testing purposes
    const uint32_t MAX_RETRIES = 7;

    std::function<void(void)> msg1TimeoutCb;

    bool errorCalled = false;

    transport.registerErrorOnRequestCallback([&errorCalled](Message &msg, TransportServices::ErrorType et) {

        ASSERT_EQ(VccIpCmd::OperationType::ERROR, msg.pdu.header.operation_type);
        ASSERT_EQ(static_cast<uint8_t>(TransportServices::ErrorCode::TIMEOUT_ERROR),
                  (msg.pdu.payload[0] >> 3) & 0x0f);  // Bit 3-6 contains error code.
        ASSERT_EQ(TransportServices::LOCAL_TIMEOUT, et);
        errorCalled = true;
    });

    // 1. Send the request
    EXPECT_CALL(sock, writeTo(requestData_, _));
    // EXPECT_CALL(timer, start(500, _)).WillOnce(SaveArg<1>(&msg1TimeoutCb));
    // Execute
    transport.sendMessage(std::move(msg));
    this->verifyAndClear();

    uint32_t timeout_ms = 510;
    for (uint32_t i = 0; i < MAX_RETRIES; ++i) {
        // 2. The ACK will time out
        // 3. Resend of message
        EXPECT_CALL(sock, writeTo(requestData_, _));
        // EXPECT_CALL(timer, restart(_));
        timeProvider.sleep_for(std::chrono::milliseconds(timeout_ms));
        ASSERT_FALSE(errorCalled);
        this->verifyAndClear();
        timeout_ms *= 1.5;
    }

    // No more retries
    EXPECT_CALL(sock, writeTo(_, _)).Times(0);
    timeProvider.sleep_for(std::chrono::milliseconds(timeout_ms));
    // EXPECT_CALL(timer, restart(_)).Times(0);
    ASSERT_TRUE(errorCalled);
    this->verifyAndClear();
}

/**
 * Send request on client side. Response timeout which will reset the ACK timeout counter.
 * Expect:
 *      1. Shall send data on socket.
 *      2. ACK timeout, resend data on socket
 *      3. Receive ACK
 *      4. Response timeout, resend data on socket: Ack timeout shall now be reset
 *      5. Receive ACK, response timeout shall now be increased
 */
TEST_F(TransportServicesFixture, SendRequest_Resp_Timeout_Reset_Ack) {
    // Create request message
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xAA);
    msg.ecu = Message::VCM;

    // 1. Send the request
    EXPECT_CALL(sock, writeTo(requestData_, _));
    transport.sendMessage(std::move(msg));
    this->verifyAndClear();

    // 2. ACK timeout, resend data on socket
    EXPECT_CALL(sock, writeTo(requestData_, _));
    timeProvider.sleep_for(std::chrono::milliseconds(800));
    this->verifyAndClear();

    // 3. Receive ACK
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestAck_), SetArgReferee<1>(Message::VCM)));
    readyReadCb();
    this->verifyAndClear();

    // 4. Response timeout, resend data on socket: Ack timeout shall now be reset
    EXPECT_CALL(sock, writeTo(requestData_, _));
    timeProvider.sleep_for(std::chrono::milliseconds(1100));
    this->verifyAndClear();

    // 5. Receive ACK, response timeout shall now be increased
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestAck_), SetArgReferee<1>(Message::VCM)));
    readyReadCb();
    this->verifyAndClear();
}

/**
 * Send request sequence on client side
 * Expect:
 *      1. Shall send data on socket
 *      2. After receiving an ACK and a response an ack shall be sent
 */
TEST_F(TransportServicesFixture, SendRequest_NewRequestOnResponse) {
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xAA);
    msg.ecu = Message::VCM;

    // 1. send request
    // 2. receive ack
    // 3. receive response
    // 4. send ack

    // 1. Validate outgoing message
    EXPECT_CALL(sock, writeTo(requestData_, _));
    // EXPECT_CALL(timer, start(_,_));
    // Execute
    transport.sendMessage(std::move(msg));
    this->verifyAndClear();

    // 2. Receive ACK
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestAck_), SetArgReferee<1>(Message::VCM)));
    // Execute: Trigger read ack
    readyReadCb();

    // 3. Receive response

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestResponse_), SetArgReferee<1>(Message::VCM)));

    // 4. Expect new request and ACK to be sent

    std::vector<uint8_t> expectData = {0x00,
                                       0xA3,
                                       0x03,
                                       0x0F,
                                       0x00,
                                       0x00,
                                       0x00,
                                       0x08,
                                       0xA3,
                                       0x0F,
                                       static_cast<uint8_t>(OperationType::REQUEST),
                                       0xBB,
                                       0x02,
                                       static_cast<uint8_t>(OperationType::REQUEST),
                                       0x00,
                                       0x00};
    EXPECT_CALL(sock, writeTo(expectData, _));
    EXPECT_CALL(sock, writeTo(requestAck_, _));

    // Create a response function that immediately sends a new message
    transport.registerIncomingResponseCallback([&](Connectivity::Message &m) {
        (void)(m);
        Connectivity::Message msg2 =
                createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xBB);
        msg2.ecu = Message::VCM;

        transport.sendMessage(std::move(msg2));
    });

    // Execute: Trigger response and ACK
    readyReadCb();
}

/**
 * Send request sequence on client side and respond with a ERROR.
 * Expect:
 *      1. Send request
 *      2. Response is ERROR
 */
TEST_F(TransportServicesFixture, SendRequest_ErrorInsteadOfAck) {
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xAA);
    msg.ecu = Message::VCM;
    msg.pdu.header.protocol_version = 2;

    bool errorCalled = false;
    transport.registerErrorOnRequestCallback([&errorCalled](Message &, TransportServices::ErrorType et) {
        ASSERT_EQ(TransportServices::REMOTE_ERROR, et);
        errorCalled = true;
    });

    // 1. Send request
    EXPECT_CALL(sock, writeTo(requestData_, _));
    // Execute
    transport.sendMessage(std::move(msg));
    this->verifyAndClear();

    // 2. Receive ERROR

    std::vector<uint8_t> expectErrData;
    expectErrData = {0x00,
                     0xA3,
                     0x03,
                     0x0F,
                     0x00,
                     0x00,
                     0x00,
                     0x0B,
                     0xA3,
                     0x0F,
                     static_cast<uint8_t>(OperationType::REQUEST),
                     0xAA,
                     0x02,
                     static_cast<uint8_t>(OperationType::ERROR),
                     static_cast<uint8_t>(DataType::NOT_ENCODED),
                     0x00,
                     static_cast<uint8_t>(TransportServices::INVALID_SERVICE_ID),
                     0x00,
                     0x00};

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(expectErrData), SetArgReferee<1>(Message::VCM)));
    // Execute: Trigger read error
    readyReadCb();
    this->verifyAndClear();
    ASSERT_TRUE(errorCalled);
}

/**
 * Send request sequence on client side and respond ack and then ERROR.
 * Expect:
 *      1. Send request
 *      2. Receive ACK
 *      3. Receive ERROR instead of response
 */
TEST_F(TransportServicesFixture, SendRequest_ErrorInsteadOfResponse) {
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xAA);
    msg.ecu = Message::VCM;
    msg.pdu.header.protocol_version = 2;

    bool errorCalled = false;
    transport.registerErrorOnRequestCallback([&errorCalled](Message &, TransportServices::ErrorType et) {
        ASSERT_EQ(TransportServices::REMOTE_ERROR, et);
        errorCalled = true;
    });

    // 1. Validate outgoing message
    EXPECT_CALL(sock, writeTo(requestData_, _));
    // Execute
    transport.sendMessage(std::move(msg));
    this->verifyAndClear();

    // 2. Receive ACK
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestAck_), SetArgReferee<1>(Message::VCM)));
    // Execute: Trigger read ack
    readyReadCb();
    this->verifyAndClear();

    std::vector<uint8_t> expectErrData;
    expectErrData = {0x00,
                     0xA3,
                     0x03,
                     0x0F,
                     0x00,
                     0x00,
                     0x00,
                     0x0B,
                     0xA3,
                     0x0F,
                     static_cast<uint8_t>(OperationType::REQUEST),
                     0xAA,
                     0x02,
                     static_cast<uint8_t>(OperationType::ERROR),
                     static_cast<uint8_t>(DataType::NOT_ENCODED),
                     0x00,
                     static_cast<uint8_t>(TransportServices::INVALID_OPERATION_ID),
                     0x00,
                     0x00};

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(expectErrData), SetArgReferee<1>(Message::VCM)));
    // Execute: Trigger read error
    readyReadCb();
    this->verifyAndClear();
    ASSERT_TRUE(errorCalled);
}

/**
 * Send request sequence on client side and respond with a busy ERROR.
 * Expect:
 *      1. Send request
 *      2. Response is busy (ERROR)
 */
TEST_F(TransportServicesFixture, SendRequest_BusyError) {
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::REQUEST, 0xAA);
    msg.ecu = Message::VCM;
    msg.pdu.header.protocol_version = 2;

    std::vector<uint8_t> expectErrData;
    expectErrData = {0x00,
                     0x01,
                     0x00,
                     0x02,
                     0x00,
                     0x00,
                     0x00,
                     0x0B,
                     0x01,
                     0x02,
                     static_cast<uint8_t>(OperationType::REQUEST),
                     0xAA,
                     0x02,
                     static_cast<uint8_t>(OperationType::ERROR),
                     static_cast<uint8_t>(DataType::NOT_ENCODED),
                     0x00,
                     static_cast<uint8_t>(TransportServices::BUSY_ERROR),
                     0x00,
                     0x00};

    transport.registerErrorOnRequestCallback([](Message &, TransportServices::ErrorType) { ASSERT_TRUE(false); });

    // 1. Send request
    EXPECT_CALL(sock, writeTo(requestData_, _));
    // Execute
    transport.sendMessage(std::move(msg));
    this->verifyAndClear();

    // 2. Receive busy ERROR
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(expectErrData), SetArgReferee<1>(Message::VCM)));

    // Execute
    readyReadCb();
    this->verifyAndClear();

    // Timeout shall now resend
    EXPECT_CALL(sock, writeTo(requestData_, _));
    timeProvider.sleep_for(std::chrono::milliseconds(800));
    this->verifyAndClear();
}

/**
 * Incoming request server side with immediate response (I.e. response is send in request upcall)
 * Expect:
 *      1. Incoming request
 *      2. Request callback send response
 *      3. An ACK will be sent by TransportServices
 *      4. Response is sent
 *      5. Request callback return true, indicating that request is supported
 *      6. ACK already sent, so transport will not send it again..
 *      7. Receive response ACK
 */
TEST_F(TransportServicesFixture, IncomingRequestImmResp_Success) {
    // Register a request function that immediately sends a response
    transport.registerIncomingRequestCallback([&](Connectivity::Message &m) {
        Connectivity::Message msg;
        msg.pdu.header = m.pdu.header;
        msg.pdu.header.operation_type = OperationType::RESPONSE;
        msg.ecu = Message::VCM;
        // 2. Request callback send response
        transport.sendMessage(std::move(msg));
        // 5. Request callback return true, indicating that request is supported
        return true;
    });

    // We now expect the following in sequence
    // 1. Incoming request
    // 3. An ACK will be sent by TransportServices
    // 4. Response is sent
    {
        Sequence seq;

        EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestData_), SetArgReferee<1>(Message::VCM)));

        EXPECT_CALL(sock, writeTo(requestAck_, _));

        EXPECT_CALL(sock, writeTo(requestResponse_, _));
    }
    // Execute: Trigger: read request, send ack, send response
    readyReadCb();

    // 7. Receive response ACK
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestAck_), SetArgReferee<1>(Message::VCM)));
    // Execute: Trigger read ack
    readyReadCb();
}

/**
 * Incoming request server side with response later on
 * Expect:
 *      1. Incoming request
 *      2. Request callback return true, indicating that request is supported
 *      3. An ACK will be sent by TransportServices
 *      4. Response is sent
 *      5. Receive response ACK
 */
TEST_F(TransportServicesFixture, IncomingRequestDelayedResp_Success) {
    Pdu requestPdu;
    // Register a request function that immediately sends a response
    transport.registerIncomingRequestCallback([&requestPdu](Connectivity::Message &m) {
        // 2. Request callback return true, indicating that request is supported
        requestPdu = m.pdu;
        return true;
    });

    // We now expect the following in sequence
    // 1. Incoming request
    // 3. An ACK will be sent by TransportServices
    {
        Sequence seq;

        EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestData_), SetArgReferee<1>(Message::VCM)));

        EXPECT_CALL(sock, writeTo(requestAck_, _));
    }
    // Execute: Trigger: read request, send ack, send response
    readyReadCb();

    EXPECT_CALL(sock, writeTo(requestResponse_, _));

    // 4. Response is sent
    Connectivity::Message msg;
    msg.ecu = Message::VCM;
    msg.pdu = requestPdu;
    msg.pdu.header.operation_type = OperationType::RESPONSE;
    transport.sendMessage(std::move(msg));

    // 5. Receive response ACK
    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestAck_), SetArgReferee<1>(Message::VCM)));
    // Execute: Trigger read ack
    readyReadCb();
}

/**
 * Incoming request server side that is not supported
 * Expect:
 *      1. Incoming request
 *      2. Send error message from request callback
 *      3. Request callback return false, indicating that request is not supported.
 */
TEST_F(TransportServicesFixture, IncomingRequest_NotSupported) {
    // Register a request function that immediately sends a response
    transport.registerIncomingRequestCallback([&](Connectivity::Message &m) {
        Connectivity::Message msg;
        msg.ecu = Message::VCM;
        msg.pdu.header = m.pdu.header;
        msg.pdu.header.operation_type = OperationType::ERROR;
        // 2. Send error message from request callback
        transport.sendMessage(std::move(msg));

        // 3. Request callback return false, indicating that request is not supported
        return false;
    });

    std::vector<uint8_t> expectErrData;

    // We now expect the following in sequence
    // 1. Incoming request
    // 3. An ACK will be sent by TransportServices
    // 4. Response is sent
    {
        Sequence seq;

        EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(requestData_), SetArgReferee<1>(Message::VCM)));

        expectErrData = {0x00,
                         0xA3,
                         0x03,
                         0x0F,
                         0x00,
                         0x00,
                         0x00,
                         0x08,
                         0xA3,
                         0x0F,
                         static_cast<uint8_t>(OperationType::REQUEST),
                         0xAA,
                         0x02,
                         static_cast<uint8_t>(OperationType::ERROR),
                         0x00,
                         0x00};

        // Shall not send any other data than the ERROR
        EXPECT_CALL(sock, writeTo(_, _)).Times(0);
        // The ERROR
        EXPECT_CALL(sock, writeTo(expectErrData, _));
    }

    // Execute: Trigger: read request, send ack, send response
    readyReadCb();
}

TEST_F(TransportServicesFixture, TestHandleInData_ReadError) {
    EXPECT_CALL(sock, read(_, _));

    readyReadCb();
}

TEST_F(TransportServicesFixture, TestHandleInData_BufferTooSmall) {
    std::vector<uint8_t> readData = {0, 0, 0};

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    readyReadCb();
}

TEST_F(TransportServicesFixture, TestHandleInData_HeaderSanityChecks) {
    std::vector<uint8_t> readData;

    // Create a PDU inside the read buffer
    Pdu pdu;

    // Wrong sender_handle_id
    pdu.createHeader(ServiceId::Connectivity, OperationId::LocalStorage, OperationType::REQUEST, DataType::ENCODED, 1);
    pdu.header.sender_handle_id = 0x00ffff00;
    pdu.header.protocol_version = this->getProtocolVersion();
    pdu.toData(readData);

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    readyReadCb();

    // Wrong protocol version
    readData.clear();
    pdu.createHeader(ServiceId::Connectivity, OperationId::LocalStorage, OperationType::REQUEST, DataType::ENCODED, 1);
    pdu.header.protocol_version = 0xff;
    pdu.toData(readData);

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    // Expectation to make SetInvalidData execute once.
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);

    readyReadCb();

    // TODO: Wrong payload size. Is this testable?
    //    readData.clear();
    //    pdu.createHeader(ServiceId::Connectivity, OperationId::LocalStorage, OperationType::REQUEST,
    //    DataType::ENCODED, 1);
    //    pdu.header.protocol_version = this->getProtocolVersion();
    //    pdu.toData(readData);
    //
    //    // Expectation to make read() execute once.
    //    EXPECT_CALL(sock, hasPendingDatagrams()).WillOnce(Return(true)).WillOnce(Return(false));
    //
    //    EXPECT_CALL(sock, read(_, _, _))
    //        .WillOnce(DoAll(
    //                SetArgReferee<0>(readData),
    //                SetArgReferee<1>(vcmIp),
    //                Return(readData.size()) ));
    //
    //    readyReadCb();
}

TEST_F(TransportServicesFixture, TestprocessIncomingPdu_Setrequest_Noreturn) {
    std::vector<uint8_t> readData;

    // Create a PDU inside the read buffer
    Pdu pdu;

    // Wrong sender_handle_id
    pdu.createHeader(ServiceId::Connectivity, OperationId::LocalStorage, OperationType::SETREQUEST_NORETURN,
                     DataType::ENCODED, 1);
    pdu.header.protocol_version = getProtocolVersion();
    pdu.toData(readData);

    // Execute without incoming request callback

    transport.registerIncomingRequestCallback([&](Connectivity::Message &msg) {
        (void)msg;
        return false;
    });

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    readyReadCb();

    // Execute with incoming request callback

    transport.registerIncomingRequestCallback([&](Connectivity::Message &msg) {
        (void)msg;
        return true;
    });

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    readyReadCb();
}

TEST_F(TransportServicesFixture, TestprocessIncomingPdu_Notification) {
    std::vector<uint8_t> readData;

    // Create a PDU inside the read buffer
    Pdu pdu;

    // Wrong sender_handle_id
    pdu.createHeader(ServiceId::Connectivity, OperationId::LocalStorage, OperationType::NOTIFICATION, DataType::ENCODED,
                     1);
    pdu.header.protocol_version = getProtocolVersion();
    pdu.toData(readData);

    // Execute for Notification without callback

    transport.registerIncomingNotificationCallback(nullptr);

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    readyReadCb();

    // Execute for Notification with callback

    transport.registerIncomingNotificationCallback([&](Connectivity::Message &msg) {
        (void)msg;
        return true;
    });

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    readyReadCb();
}

TEST_F(TransportServicesFixture, TestprocessIncomingPdu_Invalid_operation_type) {
    std::vector<uint8_t> readData;

    // Create a PDU inside the read buffer
    Pdu pdu;

    // Wrong sender_handle_id
    pdu.createHeader(ServiceId::Connectivity, OperationId::LocalStorage, OperationType::UNDEFINED, DataType::ENCODED,
                     1);
    pdu.header.protocol_version = getProtocolVersion();
    pdu.toData(readData);

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    // Expectation to make SetInvalidData execute once.
    EXPECT_CALL(diagnostics, SetInvalidData(_, _)).Times(1);

    readyReadCb();
}

/**
 * Test that ack is not sent on receiving error pdu
 */

TEST_F(TransportServicesFixture, TestprocessIncomingPdu_error_handle_no_ack) {
    std::vector<uint8_t> readData;
    // Create a PDU inside the read buffer
    Pdu pdu;

    // Wrong sender_handle_id
    pdu.createHeader(ServiceId::Connectivity, OperationId::LocalStorage, OperationType::ERROR, DataType::ENCODED, 1);
    pdu.header.protocol_version = getProtocolVersion();
    pdu.toData(readData);

    EXPECT_CALL(sock, writeTo(_, _)).Times(0);

    readyReadCb();
}

TEST_F(TransportServicesFixture, TesthandleIncomingAck_no_matching_message) {
    std::vector<uint8_t> readData;

    // Create a PDU inside the read buffer
    Pdu pdu;

    pdu.createHeader(ServiceId::Connectivity, OperationId::LocalStorage, OperationType::ACK, DataType::ENCODED, 1);
    pdu.header.protocol_version = getProtocolVersion();
    pdu.toData(readData);

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    readyReadCb();
}

TEST_F(TransportServicesFixture, TesthandleIncomingAck_set_request_no_return) {
    Connectivity::Message msg = createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource,
                                          OperationType::SETREQUEST_NORETURN, 0xAA);
    msg.ecu = Message::VCM;
    msg.pdu.header.protocol_version = getProtocolVersion();

    transport.sendMessage(std::move(msg));

    std::vector<uint8_t> readData;
    msg.pdu.header.operation_type = VccIpCmd::OperationType::ACK;
    msg.pdu.toData(readData);

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    transport.registerIncomingRequestCallback([&](Connectivity::Message &msg) {
        (void)msg;
        return true;
    });

    readyReadCb();
}

/**
 * Test handleIncomingAck with NOTIFICATION
 */
TEST_F(TransportServicesFixture, TesthandleIncomingAck_notification) {
    Connectivity::Message msg =
            createMsg(ServiceId::Connectivity, OperationId::CurrentInternetSource, OperationType::NOTIFICATION, 0xAB);
    msg.ecu = Message::VCM;
    msg.pdu.header.protocol_version = getProtocolVersion();

    transport.sendMessage(std::move(msg));

    std::vector<uint8_t> readData;
    msg.pdu.header.operation_type = VccIpCmd::OperationType::ACK;
    msg.pdu.toData(readData);

    EXPECT_CALL(sock, read(_, _)).WillOnce(DoAll(SetArgReferee<0>(readData), SetArgReferee<1>(Message::VCM)));

    transport.registerIncomingNotificationCallback([&](Connectivity::Message &msg) {
        (void)msg;
        return true;
    });

    readyReadCb();
}

/**
 * Test SendRequest with notification.
 */
TEST_F(TransportServicesFixture, Test_SendRequest_Notification) {
    Pdu notificationPdu;
    Connectivity::Message msg;
    msg.ecu = Message::VCM;
    msg.pdu = notificationPdu;
    msg.pdu.header.operation_type = OperationType::NOTIFICATION;
    EXPECT_CALL(sock, writeTo(_, _)).Times(1);
    transport.sendMessage(std::move(msg));
}

/**
 * Test SendRequest with an undefined operation type
 */
TEST_F(TransportServicesFixture, test_SendRequest_Undefined) {
    Pdu notificationPdu;
    Connectivity::Message msg;
    msg.ecu = Message::VCM;
    msg.pdu = notificationPdu;
    msg.pdu.header.operation_type = OperationType::UNDEFINED;
    EXPECT_CALL(sock, writeTo(_, _)).Times(0);
    transport.sendMessage(std::move(msg));
    msg.pdu.header.operation_type = OperationType::ACK;
    EXPECT_CALL(sock, writeTo(_, _)).Times(0);
    transport.sendMessage(std::move(msg));
}

/**
 * Test ErrorTypeToCString
 */
TEST_F(TransportServicesFixture, TestErrorTypeToCString) {
    EXPECT_STREQ("OK", transport.ErrorTypeToCString(TransportServices::ErrorType::OK));
    EXPECT_STREQ("REMOTE_ERROR", transport.ErrorTypeToCString(TransportServices::ErrorType::REMOTE_ERROR));
    EXPECT_STREQ("LOCAL_TIMEOUT", transport.ErrorTypeToCString(TransportServices::ErrorType::LOCAL_TIMEOUT));
    EXPECT_STREQ("PAYLOAD_DECODE_ERROR",
                 transport.ErrorTypeToCString(TransportServices::ErrorType::PAYLOAD_DECODE_ERROR));
    EXPECT_STREQ("Unknown error type", transport.ErrorTypeToCString(static_cast<TransportServices::ErrorType>(55)));
}

/**
 * Test MessageTimeout in different ways
 */
TEST_F(TransportServicesFixture, TestmessageTimeout) {
    // TODO: Rewrite this test without exposing internals
}

/**
 * Test invalid pdu length in Pdu::fromData
 */
TEST_F(TransportServicesFixture, TestPdu_from_data_invalid_pdu_length) {
    std::vector<uint8_t> data;
    Pdu pdu;

    pdu.header.operation_type = VccIpCmd::OperationType::ACK;
    pdu.header.length = 9;
    pdu.toData(data);

    EXPECT_EQ(pdu.fromData(data), false);
}

TEST_F(TransportServicesFixture, ServiceIDUnknown) {
    const std::vector<uint8_t> invalid_service_id_data = {0x12, 0x34, 0x03, 0x08, 0,    0, 0, 0x09, 0x34,
                                                          0x08, 0x02, 0x7d, 0x02, 0x02, 0, 0, 0};

    const std::vector<uint8_t> error_data = {0x12, 0x34, 0x03, 0x08, 0, 0, 0,    0x0b, 0x34, 0x08,
                                             0x02, 0x7d, 0x02, 0xe0, 0, 0, 0x88, 0x91, 0xa0};

    EXPECT_CALL(sock, read(_, _))
            .WillOnce(DoAll(SetArgReferee<0>(invalid_service_id_data), SetArgReferee<1>(Message::VCM)));

    EXPECT_CALL(sock, writeTo(error_data, Message::VCM));

    readyReadCb();
}

TEST_F(TransportServicesFixture, SenderHandleIDDoesNotMatchLowerServiceIDByte) {
    const std::vector<uint8_t> invalid_sender_handle_id_data = {0,    0xa3, 0x03, 0x08, 0,    0, 0, 0x09, 0xb4,
                                                                0x08, 0x02, 0x7d, 0x02, 0x02, 0, 0, 0};

    const std::vector<uint8_t> error_data = {0,    0xa3, 0x03, 0x08, 0,    0, 0, 0x09, 0xb4,
                                             0x08, 0x02, 0x7d, 0x02, 0xe0, 0, 0, 0};

    EXPECT_CALL(sock, read(_, _))
            .WillOnce(DoAll(SetArgReferee<0>(invalid_sender_handle_id_data), SetArgReferee<1>(Message::VCM)));

    EXPECT_CALL(sock, writeTo(error_data, Message::VCM));

    readyReadCb();
}

TEST_F(TransportServicesFixture, SenderHandleIDDoesNotMatchLowerOperationIDByte) {
    const std::vector<uint8_t> invalid_sender_handle_id_data = {0,    0xa3, 0x03, 0x08, 0,    0, 0, 0x09, 0xa3,
                                                                0x09, 0x02, 0x7d, 0x02, 0x02, 0, 0, 0};

    const std::vector<uint8_t> error_data = {0,    0xa3, 0x03, 0x08, 0,    0, 0, 0x09, 0xa3,
                                             0x09, 0x02, 0x7d, 0x02, 0xe0, 0, 0, 0};

    EXPECT_CALL(sock, read(_, _))
            .WillOnce(DoAll(SetArgReferee<0>(invalid_sender_handle_id_data), SetArgReferee<1>(Message::VCM)));

    EXPECT_CALL(sock, writeTo(error_data, Message::VCM));

    readyReadCb();
}
