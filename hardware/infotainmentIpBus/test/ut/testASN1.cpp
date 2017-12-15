/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

// Note that the auto-generated ASN1 files are in C, not C++

extern "C" {
#include "infotainmentIpBus.h"
}
extern "C" {
#include "pl/asn_base/asn_base.h"
}
// using ::testing::_;
// using ::testing::NiceMock;
// using ::testing::Mock;
// using ::testing::DoAll;
// using ::testing::Return;
// using ::testing::SetArgReferee;
// using ::testing::Field;
// using ::testing::AllOf;

#define LOG_TAG "iipbus.ut"
#include <cutils/log.h>

class ASN1Test : public ::testing::Test {
  public:
    ASN_Result ASN_retcode;
    static const U32 m_EncodeBufSize = 2048;
    static const U32 m_DecodeBufSize = 2048;
    static const U32 m_SessionBufSize = ASN_SESSION_SIZE + m_DecodeBufSize;
    ASN_BYTE m_stream_buffer[ASN_STREAM_SIZE];
    ASN_BYTE m_encode_buffer[m_EncodeBufSize];
    ASN_BYTE m_decode_buffer[m_SessionBufSize];
    ASN_Stream m_stream = ASN_Stream_Create(m_stream_buffer, ASN_STREAM_SIZE);
    ASN_Session m_session = ASN_Session_Create(m_decode_buffer, m_SessionBufSize);
    ASN_BYTE* m_encodedBufPtr = nullptr;
    U32 m_encodedBufLen = 0;

    virtual void SetUp() {
        m_encodedBufPtr = nullptr;
        ASN_retcode = NULL;
    }

    virtual void TearDown() {}

  protected:
    void verifyAndClear(void) {}
};

// MATCHER_P(LambdaMatcher, func, "") { bool ret=false; *result_listener << func(arg, ret); return ret; }

/**
 * Encode one message.
 * Expect:
 *      Encode function returns OK (NULL)
 */
TEST_F(ASN1Test, EncodeOk) {
    Icb_OpSendToCarConfirmation_SetRequest p = NULL;

    ASN_Stream_Reset(m_stream);
    ASN_Session_Reset(m_session);

    ASN_Stream_AttachBuffer(m_stream, m_encode_buffer, m_EncodeBufSize);

    p = Icb_OpSendToCarConfirmation_SetRequest_Create(m_session);
    p->confirmation = e_Icb_SendToCarConfirmation_addedAsWaypoint;
    p->confirmedId = 1234;

    ASN_retcode = Icb_OpSendToCarConfirmation_SetRequest_Encode(p, m_stream);
    ASSERT_TRUE(ASN_retcode == ASN_RESULT_OK);
    EXPECT_EQ(NULL, ASN_retcode);

    m_encodedBufPtr = ASN_Stream_GetBuffer(m_stream, &m_encodedBufLen);
}

/**
 * Encode one message.
 * Expect:
 *      Encode function returns an Error with code ASN_ERROR_VALUE_NOT_WITHIN_RANGE.
 */
TEST_F(ASN1Test, EncodeError) {
    Icb_OpSendToCarConfirmation_SetRequest p = NULL;

    ASN_Stream_Reset(m_stream);
    ASN_Session_Reset(m_session);

    ASN_Stream_AttachBuffer(m_stream, m_encode_buffer, m_EncodeBufSize);

    p = Icb_OpSendToCarConfirmation_SetRequest_Create(m_session);
    p->confirmation = 10;
    p->confirmedId = 1234;

    ASN_retcode = Icb_OpSendToCarConfirmation_SetRequest_Encode(p, m_stream);
    ASSERT_TRUE(ASN_retcode != ASN_RESULT_OK) << ASN_retcode;
    EXPECT_EQ(ASN_ERROR_VALUE_NOT_WITHIN_RANGE, ASN_retcode->error) << "error = " << ASN_retcode->error;

    m_encodedBufPtr = ASN_Stream_GetBuffer(m_stream, &m_encodedBufLen);
}

/**
 * Decode one message.
 * Expect:
 *      Decode function returns OK (NULL)
 */
TEST_F(ASN1Test, DecodeOk) {
    Icb_OpSendToCarConfirmation_SetRequest p = NULL;
    ASN_BYTE encode_buffer[] = {0x10, 0x00, 0x00, 0x4d, 0x20};

    /* ASN_Stream_Reset(m_stream); */
    /* ASN_Session_Reset(m_session); */

    p = Icb_OpSendToCarConfirmation_SetRequest_Create(m_session);

    ASN_Stream_AttachBuffer(m_stream, encode_buffer, sizeof(encode_buffer));

    ASN_retcode = Icb_OpSendToCarConfirmation_SetRequest_Decode(p, m_session, m_stream);
    ASSERT_TRUE(ASN_retcode == ASN_RESULT_OK) << ASN_retcode;

    ASSERT_EQ(e_Icb_SendToCarConfirmation_addedAsWaypoint, p->confirmation);
    ASSERT_EQ(1234U, p->confirmedId);
}

/**
 * Decode one message.
 * Expect:
 *      Decode function returns an Error with code ASN_ERROR_VALUE_NOT_WITHIN_RANGE.
 */
TEST_F(ASN1Test, DecodeError) {
    Icb_OpSendToCarConfirmation_SetRequest p = NULL;
    ASN_BYTE encode_buffer[] = {0xee, 0xab, 0x10, 0xab};  // INVALID bitstream

    p = Icb_OpSendToCarConfirmation_SetRequest_Create(m_session);

    ASN_Stream_AttachBuffer(m_stream, encode_buffer, sizeof(encode_buffer));

    ASN_retcode = Icb_OpSendToCarConfirmation_SetRequest_Decode(p, m_session, m_stream);
    ASSERT_TRUE(ASN_retcode != ASN_RESULT_OK) << ASN_retcode;
    EXPECT_EQ(ASN_ERROR_VALUE_NOT_WITHIN_RANGE, ASN_retcode->error) << "error = " << ASN_retcode->error;

    ASSERT_EQ(6U, p->confirmation);  // Not a valid enum
    ASSERT_EQ(0U, p->confirmedId);
}
