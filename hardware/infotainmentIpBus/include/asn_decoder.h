#ifndef INFOTAINMENT_IP_BUS_ASN_DECODER_H
#define INFOTAINMENT_IP_BUS_ASN_DECODER_H

#define LOG_TAG "InfotainmentIPBus.decode"

extern "C" {
    #include "infotainmentIpBus.h"
    #include "pl/asn_base/asn_base.h"
  }

#include <cutils/log.h>
#include "type_conversion_helpers.h"

namespace InfotainmentIpBus
{
namespace Utils
{

template <class MsgTypePtr>
bool DecodeMessage(std::vector<uint8_t> payload,
                   ASN_Session& m_session_msgd,
                   MsgTypePtr &pDecodedMsg,
                   MsgTypePtr (*createFunc)(ASN_Session session),
                   ASN_Result (*decodeFunc)(MsgTypePtr ThisPtr, ASN_Session session, ASN_Stream stream))
{
    ASN_Session_Reset(m_session_msgd);
    pDecodedMsg = createFunc(m_session_msgd);

    if (!pDecodedMsg)
    {
        ALOGE("ERROR: Session buffer is too small!");

        return false;
    }
    else
    {
        ASN_BYTE m_stream_buffer_msgd[ASN_STREAM_SIZE];
        ASN_Stream m_stream_msgd = ASN_Stream_Create(m_stream_buffer_msgd, sizeof(m_stream_buffer_msgd));

        // Attach the PDU payload to the ASN bitstream buffer
        ASN_Stream_AttachBuffer(m_stream_msgd, payload.data(), payload.size());

        const ASN_Result ASN_retcode = decodeFunc(pDecodedMsg, m_session_msgd, m_stream_msgd);
        if (ASN_retcode)
        {
            ALOGE(
                "Payload decoding failed (ASN error '%s' @ InfotainmentIpBus.c: %u)(size: %d)",
                AsnErrCodeToString(ASN_retcode->error).c_str(),
                ASN_retcode->linenumber,
                (int)payload.size());

            return false;
        }
    }

    return true;
}
}  // namespace Utils
}  // namespace InfotainmentIpBus
#endif //INFOTAINMENT_IP_BUS_ASN_DECODER_H