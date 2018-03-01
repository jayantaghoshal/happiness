/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#define ASN_LOG_TAG "InfotainmentIPBus.decode"

extern "C" {
#include "infotainmentIpBus.h"
#include "pl/asn_base/asn_base.h"
}

#include <cutils/log.h>
#include "type_conversion_helpers.h"

namespace InfotainmentIpBus {
namespace Utils {

template <class MsgTypePtr>
bool DecodeMessage(std::vector<uint8_t> payload,
                   ASN_Session& m_session_msgd,
                   MsgTypePtr& pDecodedMsg,
                   MsgTypePtr (*createFunc)(ASN_Session session),
                   ASN_Result (*decodeFunc)(MsgTypePtr ThisPtr, ASN_Session session, ASN_Stream stream)) {
    ASN_Session_Reset(m_session_msgd);
    pDecodedMsg = createFunc(m_session_msgd);

    if (!pDecodedMsg) {
        ALOG(LOG_ERROR, ASN_LOG_TAG, "ERROR: Session buffer is too small!");

        return false;
    } else {
        ASN_BYTE m_stream_buffer_msgd[ASN_STREAM_SIZE];
        ASN_Stream m_stream_msgd = ASN_Stream_Create(m_stream_buffer_msgd, sizeof(m_stream_buffer_msgd));

        // Attach the PDU payload to the ASN bitstream buffer
        ASN_Stream_AttachBuffer(m_stream_msgd, payload.data(), payload.size());

        const ASN_Result ASN_retcode = decodeFunc(pDecodedMsg, m_session_msgd, m_stream_msgd);
        if (ASN_retcode) {
            ALOG(LOG_ERROR,
                 ASN_LOG_TAG,
                 "Payload decoding failed (ASN error '%s' @ InfotainmentIpBus.c: %u)(size: %d)",
                 AsnErrCodeToString(ASN_retcode->error).c_str(),
                 ASN_retcode->linenumber,
                 (int)payload.size());

            return false;
        }
    }

    return true;
}

template <class MsgTypePtr>
void encodeMessage(MsgTypePtr pMsg,
                   ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                   U32 (*encodedSizeFunc)(MsgTypePtr),
                   std::vector<uint8_t>* bufPayload) {
    // Prepare payload
    // - Align the size of the ASN1 payload buffer on 8-bit boundary
    const U32 encodedSizeInBits = encodedSizeFunc(pMsg);
    const U32 encodedSizeInBytes = (encodedSizeInBits + 7) / 8;
    bufPayload->resize(encodedSizeInBytes);

    std::vector<uint8_t> streambuf(ASN_SESSION_SIZE);
    ASN_Stream stream = ASN_Stream_Create(&streambuf.front(), streambuf.size());
    ASN_Stream_AttachBuffer(stream, &bufPayload->front(), bufPayload->size());

    ASN_Result res = encodeFunc(pMsg, stream);
    if (res != ASN_RESULT_OK) {
        ALOG(LOG_ERROR,
             ASN_LOG_TAG,
             "%s: ASN1 encoding failed (ASN error '%s' @ InfotainmentIpBus.c: %u)",
             __FUNCTION__,
             InfotainmentIpBus::Utils::AsnErrCodeToString(res->error).c_str(),
             res->linenumber);
        return;  // TODO: Report error to upper layers????????
    }
}

/* Example of how to use encoder!!

void Test() {

    Icb_OpCurrentInternetSource_SetRequest_t msg;

    msg.currentinternetsource = 1;

    Msg message;

    //TODO: Fill header

    //Encode payload
    std::vector<uint8_t> payload;
    InfotainmentIpBus::Utils::encodeMessage(&msg, Icb_OpCurrentInternetSource_SetRequest_Encode,
        Icb_OpCurrentInternetSource_SetRequest_EncodedSize, &payload);
    message.pdu.payload = std::move(payload);

    //Send message
    ipcbServer_.get()->sendMessage(message);
}

*/

/*
void createHeader(Connectivity::VccIpCmd::ServiceId serviceId,
    Connectivity::VccIpCmd::OperationId operationId,
    Connectivity::VccIpCmd::OperationType operationType,
    Connectivity::VccIpCmd::DataType dataType,
    uint8_t sequenceId)
{
    header.service_id = serviceId;
    header.operation_id = operationId;
    header.length = VCCPDUHeader::DATA_SIZE - 8;
    header.sender_handle_id = ((static_cast<uint16_t>(serviceId) & 0xFF) << 24) |
            ((static_cast<uint16_t>(operationId) & 0xFF) << 16) |
            ((static_cast<uint8_t>(operationType) & 0xFF) << 8) | sequenceId;
    header.protocol_version = 0;  // Protocol version will be set by TransportServicesupon send
    header.operation_type = operationType;
    header.data_type = dataType;
    header.process_flag_and_reserved = 0;
}
*/

}  // namespace Utils
}  // namespace InfotainmentIpBus

#undef LOG_TAG
