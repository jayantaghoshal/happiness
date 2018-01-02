/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/**
 * Unit test of modules in VS;
 *
 * Production code under test:
 *   src/message_inject.cpp
 *   src/vsm.cpp
 */

#include <avmp.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <message_api.h>
#include <misc_mock.h>
#include <vsm.h>
#include <vsm_inject.h>
#include <vsm_inject_mock.h>
#include <vsm_sink_mock.h>

#include <vipcomm/VipFramework.h>

using ::testing::_;
using ::testing::Mock;
using ::testing::Pointee;

namespace vipcomm {
void getFileCrc(uint16_t &com_arxml, uint16_t &swc_arxml, uint16_t &rte_type, uint16_t &com_cfg) {
    com_arxml = 1;
    swc_arxml = 2;
    rte_type = 3;
    com_cfg = 4;
}
}

/* Global variables used in production code */

std::atomic<std::chrono::steady_clock::time_point> lastAvmpHeartbeat;
std::atomic<VersionHandshakeStatus> avmpVersionCheckOk{VersionHandshakeStatus::NotReceived};

uint16_t comCrc = 1;
uint16_t swcCrc = 2;
uint16_t rteTypeCrc = 3;
uint16_t comCfgCrc = 4;

extern "C" {
extern int __real_open(const char *pathname, int flags, mode_t mode);
extern ssize_t __real_read(int fd, void *buf, size_t count);
}

/* Create a mock of the messageSend-function */
class MessageSend {
  public:
    virtual void messageSend(Message_Send_T *msg_data) = 0;
};

class MessageSendMock : public MessageSend {
  public:
    MOCK_METHOD1(messageSend, void(Message_Send_T *));
};

static MessageSendMock *messageSendMockPtr;

class HisipMessageInjectTestFixture : public ::testing::Test {
  public:
    virtual void SetUp() {
        vsm_inject_mock_init(&vsmInjectMock);
        vsm_sink_mock_init(&vsmSinkMock);
        messageSendMockPtr = &messageSendMock;

        avmpVersionCheckOk = VersionHandshakeStatus::NotReceived;
    }

    virtual void TearDown() {}

  protected:
    VsmInjectMock vsmInjectMock;
    VsmSinkMock vsmSinkMock;
    MessageSendMock messageSendMock;
};

void messageSend(Message_Send_T *msg_data) { messageSendMockPtr->messageSend(msg_data); }

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_nullPtrMessage_shallBeIgnored) {
    avmpMessageInject(nullptr, 0);
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenValidSignal_shallCallVsmInject) {
    // Valid signal
    uint8_t data[] = {0xd, 3, 123, 45};

    EXPECT_CALL(vsmInjectMock, vsm_inject_inject(_, _, _, _)).Times(1);
    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenInvalidCtrlMsg_shallLogDiscardedMsg) {
    // Invalid control message
    uint8_t data[] = {0xd, 0x80, 123, 45};

    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenHeartbeatCtrlMsg_shallCallMessageSend) {
    // Heartbeat control message
    uint8_t data[] = {0x2, 0x80, 123};

    EXPECT_CALL(messageSendMock, messageSend(_)).Times(1);
    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenHeartbeatWrongLenCtrlMsg_shallNotCallMessageSend) {
    // Heartbeat control message with invalid length
    uint8_t data[] = {0x2, 0x80, 123, 4};

    EXPECT_CALL(messageSendMock, messageSend(_)).Times(0);
    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenVersionCheckMsg_shallCallVsmSinkInit) {
    // Correct version check control message
    uint8_t data[] = {0x3,
                      0x80,
                      avmp::avmpVersion,
                      static_cast<uint8_t>(comCrc & 0x00FF),
                      static_cast<uint8_t>(comCrc >> 8),
                      static_cast<uint8_t>(swcCrc & 0x00FF),
                      static_cast<uint8_t>(swcCrc >> 8),
                      static_cast<uint8_t>(rteTypeCrc & 0x00FF),
                      static_cast<uint8_t>(rteTypeCrc >> 8),
                      static_cast<uint8_t>(comCfgCrc & 0x00FF),
                      static_cast<uint8_t>(comCfgCrc >> 8)};

    EXPECT_CALL(vsmSinkMock, vsm_sink_init()).Times(1);
    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenVersionCheckMsgInvalid_shallNotCallVsmSinkInit) {
    const uint8_t invalidAvmpVersion = 1;

    // Invalid version check control message
    uint8_t data[] = {0x3,
                      0x80,
                      invalidAvmpVersion,
                      static_cast<uint8_t>(comCrc & 0x00FF),
                      static_cast<uint8_t>(comCrc >> 8),
                      static_cast<uint8_t>(swcCrc & 0x00FF),
                      static_cast<uint8_t>(swcCrc >> 8),
                      static_cast<uint8_t>(rteTypeCrc & 0x00FF),
                      static_cast<uint8_t>(rteTypeCrc >> 8),
                      static_cast<uint8_t>(comCfgCrc & 0x00FF),
                      static_cast<uint8_t>(comCfgCrc >> 8)};

    EXPECT_CALL(vsmSinkMock, vsm_sink_init()).Times(0);
    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenVersionCheckWrongLenCtrlMsg_shallNotCallVsmSinkInit) {
    // Version check control message with invalid length
    uint8_t data[] = {0x3, 0x80, 123, 4};

    EXPECT_CALL(vsmSinkMock, vsm_sink_init()).Times(0);
    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenDataWithLen0_shallLogMessageWithLen0) {
    // Message with length 0 (invalid length)
    uint8_t data[] = {0xd};

    avmpMessageInject(data, 0);
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenDataWithLen2_shallLogMessageWithLen1) {
    // Message with length 1 (invalid length)
    uint8_t data[] = {0xd};

    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenDataWithLen2_shallLogMessageWithLen2) {
    // Message with length 2 (invalid length)
    uint8_t data[] = {0xd, 3};

    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenDataWithLen3_shallLogMessageWithLen3) {
    // Message with length 3
    uint8_t data[] = {0xd, 3, 123};

    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenDataWithLen4_shallLogMessageWithLen4) {
    // Message with length 4
    uint8_t data[] = {0xd, 3, 123, 45};

    avmpMessageInject(data, sizeof(data));
}

TEST_F(HisipMessageInjectTestFixture, avmpMessageInject_givenDataWithLen5_shallLogMessageWithLen5) {
    // Message with length 5
    uint8_t data[] = {0xd, 3, 123, 45, 6};

    avmpMessageInject(data, sizeof(data));
}

///////////////////// Tests for the vsm.cpp module ///////////////////////////////////
class VsmTestFixture : public ::testing::Test {
  public:
    virtual void SetUp() {
        vsm_inject_mock_init(&vsmInjectMock);
        misc_mock_init(&miscMock);
        messageSendMockPtr = &messageSendMock;
        miscMock.selectReturnValue = 0;
        miscMock.unlinkReturnValue = 0;
        miscMock.mkfifoReturnValue = 0;
        errno = 0;
    }

    virtual void TearDown() {}

  protected:
    VsmInjectMock vsmInjectMock;
    MessageSendMock messageSendMock;
    MiscMock miscMock;
};

TEST_F(VsmTestFixture, vsmInit_noPresumptions_shallCallVsmInjectInit) {
    EXPECT_CALL(vsmInjectMock, vsm_inject_init()).Times(1);
    vsm_init();
}

TEST_F(VsmTestFixture, vsmDeInit_noPresumptions_shallDoNothing) { vsm_deInit(); }
int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
