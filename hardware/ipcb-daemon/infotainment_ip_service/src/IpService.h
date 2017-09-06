/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef IPSERVICE_H
#define IPSERVICE_H

#undef LOG_TAG
#define LOG_TAG "Infotainment_IP_Service.ips"

#include <functional>
#include <list>
#include <set>

#include "ipcommandbus/Message.h"
#include "ipcommandbus/MessageDispatcher.h"
#include <cutils/log.h>
#include "ipcommandbus/vcc_pdu_header.h"
#include "util/type_conversion_helpers.h"

#ifdef __cplusplus
extern "C" {
#include "infotainmentIpBus.h"
#include "pl/asn_base/asn_base.h"
}
#endif

namespace MyAssert
{
// NOTE: The macros below has not been declared (or using) inline functions for the following reasons:
// 1) The code including the if-statement can not be an inline functions since it's only if the condition
//    fails, a return from the _calling_ function shall be made. This will not be possible
//    if the code block in the macro is put into another function.
// 2) If the log-call plus the assert is put in an inline function, a compilation error occurs:
//    error: 'getDefaultContext' was not declared in this scope
//

/// Macro: If \c condition is false, a log is generated before the assertion, and finally returns void
///
/// @param[in] condition Condition to evaluate
#define ASSERT_OR_RETURN_VOID(condition)                                                               \
    if (!(condition))                                                                                  \
    {                                                                                                  \
        ALOGE("Assert (%s) in %s @ line %d", #condition, __FUNCTION__, __LINE__); \
        assert(true);                                                                                  \
        return;                                                                                        \
    }

/// Macro: If \c condition is false, a log is generated before the assertion, and finally returns void
///
/// @param[in] condition Condition to evaluate
/// @param[in] v1 uint32_t-value to put into log for debugging purposes
#define ASSERT_OR_RETURN_VOID1(condition, v1)                               \
    if (!(condition))                                                       \
    {                                                                       \
        ALOGE("Assert (%s) in %s @ line %d [%08X/%u]", \
                                   #condition,                              \
                                   __FUNCTION__,                            \
                                   __LINE__,                                \
                                   static_cast<uint32_t>(v1),               \
                                   static_cast<uint32_t>(v1));              \
        assert(true);                                                       \
        return;                                                             \
    }

/// Macro: If \c condition is false, a log is generated before the assertion, and finally returns void
///
/// @param[in] condition Condition to evaluate
/// @param[in] v1 uint32_t-value to put into log for debugging purposes
/// @param[in] v2 uint32_t-value to put into log for debugging purposes
#define ASSERT_OR_RETURN_VOID2(condition, v1, v2)                                   \
    if (!(condition))                                                               \
    {                                                                               \
        ALOGE("Assert (%s) in %s @ line %d [%08X/%u %08X/%u]", \
                                   #condition,                                      \
                                   __FUNCTION__,                                    \
                                   __LINE__,                                        \
                                   static_cast<uint32_t>(v1),                       \
                                   static_cast<uint32_t>(v1),                       \
                                   static_cast<uint32_t>(v2),                       \
                                   static_cast<uint32_t>(v2));                      \
        assert(true);                                                               \
        return;                                                                     \
    }

/// Macro: If \c condition is false, a log is generated before the assertion, and finally returns void
///
/// @param[in] condition Condition to evaluate
/// @param[in] v1 uint32_t-value to put into log for debugging purposes
/// @param[in] v2 uint32_t-value to put into log for debugging purposes
/// @param[in] v3 uint32_t-value to put into log for debugging purposes
#define ASSERT_OR_RETURN_VOID3(condition, v1, v2, v3)                                       \
    if (!(condition))                                                                       \
    {                                                                                       \
        ALOGE("Assert (%s) in %s @ line %d [%08X/%u %08X/%u %08X/%u]", \
                                   #condition,                                              \
                                   __FUNCTION__,                                            \
                                   __LINE__,                                                \
                                   static_cast<uint32_t>(v1),                               \
                                   static_cast<uint32_t>(v1),                               \
                                   static_cast<uint32_t>(v2),                               \
                                   static_cast<uint32_t>(v2),                               \
                                   static_cast<uint32_t>(v3),                               \
                                   static_cast<uint32_t>(v3));                              \
        assert(true);                                                                       \
        return;                                                                             \
    }

/// Macro: If \c condition is false, a log is generated before the assertion, and finally returns \c rval
///
/// @param[in] condition Condition to evaluate
/// @param[in] rval Value to return
/// @param[in] v1 uint32_t-value to put into log for debugging purposes
/// @param[in] v2 uint32_t-value to put into log for debugging purposes
/// @param[in] v3 uint32_t-value to put into log for debugging purposes
#define ASSERT_OR_RETURN(condition, rval)                                                              \
    if (!(condition))                                                                                  \
    {                                                                                                  \
        ALOGE("Assert (%s) in %s @ line %d", #condition, __FUNCTION__, __LINE__); \
        assert(true);                                                                                  \
        return (rval);                                                                                 \
    }

/// Macro: If \c condition is false, a log is generated before the assertion, and finally returns \c rval
///
/// @param[in] condition Condition to evaluate
/// @param[in] rval Value to return
/// @param[in] v1 uint32_t-value to put into log for debugging purposes
#define ASSERT_OR_RETURN1(condition, rval, v1)                              \
    if (!(condition))                                                       \
    {                                                                       \
        ALOGE("Assert (%s) in %s @ line %d [%08X/%u]", \
                                   #condition,                              \
                                   __FUNCTION__,                            \
                                   __LINE__,                                \
                                   static_cast<uint32_t>(v1),               \
                                   static_cast<uint32_t>(v1));              \
        assert(true);                                                       \
        return (rval);                                                      \
    }

/// Macro: If \c condition is false, a log is generated before the assertion, and finally returns \c rval
///
/// @param[in] condition Condition to evaluate
/// @param[in] rval Value to return
/// @param[in] v1 uint32_t-value to put into log for debugging purposes
/// @param[in] v2 uint32_t-value to put into log for debugging purposes
#define ASSERT_OR_RETURN2(condition, rval, v1, v2)                                  \
    if (!(condition))                                                               \
    {                                                                               \
        ALOGE("Assert (%s) in %s @ line %d [%08X/%u %08X/%u]", \
                                   #condition,                                      \
                                   __FUNCTION__,                                    \
                                   __LINE__,                                        \
                                   static_cast<uint32_t>(v1),                       \
                                   static_cast<uint32_t>(v1),                       \
                                   static_cast<uint32_t>(v2),                       \
                                   static_cast<uint32_t>(v2));                      \
        assert(true);                                                               \
        return (rval);                                                              \
    }

/// Macro: If \c condition is false, a log is generated before the assertion, and finally returns \c rval
///
/// @param[in] condition Condition to evaluate
/// @param[in] rval Value to return
/// @param[in] v1 uint32_t-value to put into log for debugging purposes
/// @param[in] v2 uint32_t-value to put into log for debugging purposes
/// @param[in] v3 uint32_t-value to put into log for debugging purposes
#define ASSERT_OR_RETURN3(condition, rval, v1, v2, v3)                                      \
    if (!(condition))                                                                       \
    {                                                                                       \
        ALOGE("Assert (%s) in %s @ line %d [%08X/%u %08X/%u %08X/%u]", \
                                   #condition,                                              \
                                   __FUNCTION__,                                            \
                                   __LINE__,                                                \
                                   static_cast<uint32_t>(v1),                               \
                                   static_cast<uint32_t>(v1),                               \
                                   static_cast<uint32_t>(v2),                               \
                                   static_cast<uint32_t>(v2),                               \
                                   static_cast<uint32_t>(v3),                               \
                                   static_cast<uint32_t>(v3));                              \
        assert(true);                                                                       \
        return (rval);                                                                      \
    }
}  // namespace MyAssert

namespace Connectivity
{
class IpService
{
public:
    void setDispatcher(MessageDispatcher *dispatcher);

    typedef std::function<void(Message &msg)> MessageCbType;
    typedef std::function<void(Message &, std::shared_ptr<MessageDispatcher::CallerData>)> ResponseMessageCbType;

    /// Container for a client service
    struct ClientIpCommandBusService
    {
        Message::Ecu ecu;                     ///< Server ECU
        VccIpCmd::ServiceId service_id;       ///< Service Id
        VccIpCmd::OperationId operation_id;   ///< Operation Id
        ResponseMessageCbType messageCbResp;  ///< Callback for response/error messages
        MessageCbType messageCbNotification;  ///< Callback for notifications
        bool bSendNotificationRequest;        ///< Flag indicating if a NotificationRequest shall be sent
    };

    /// Container for a server service
    struct ServerIpCommandBusService
    {
        VccIpCmd::ServiceId service_id;          ///< Service Id
        VccIpCmd::OperationId operation_id;      ///< Operation Id
        MessageCbType messageCbReq;              ///< Callback for incoming Requests messages
        MessageCbType messageCbSetReq;           ///< Callback for incoming SetRequest messages
        MessageCbType messageCbNotificationReq;  ///< Callback for incoming Notification.Request messages
    };

    struct NotificationSubscriber
    {
    public:
        NotificationSubscriber(const Message &msg);

        NotificationSubscriber(VccIpCmd::ServiceId serviceId, VccIpCmd::OperationId operationId, Message::Ecu ecu);

        NotificationSubscriber();

        VccIpCmd::ServiceId serviceId;
        VccIpCmd::OperationId operationId;
        Message::Ecu ecu;
    };

    typedef std::list<NotificationSubscriber> NotificationSubscriberList;

public:
    /// Name of service
    std::string service_name_ = "<not set>";

    /// Register a set of client operations according to the given \c table.
    ///
    /// @param[in] table Table with operations to register
    bool RegisterClient(const std::vector<ClientIpCommandBusService> &table);

    /// Register a set of local operations this service supports
    ///
    /// @param[in] table Table with operations to register
    bool RegisterServer(const std::vector<ServerIpCommandBusService> &table);

    void SendGenericRequest(const VccIpCmd::ServiceId serviceId,
                            const VccIpCmd::OperationId operationId,
                            const Message::Ecu ecu,
                            std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    void SendGenericSetRequestNoReturn(const VccIpCmd::ServiceId serviceId,
                                       const VccIpCmd::OperationId operationId,
                                       const Message::Ecu ecu,
                                       std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    void SendGenericResponse(const VccIpCmd::ServiceId serviceId,
                             const VccIpCmd::OperationId operationId,
                             const Message::Ecu ecu,
                             const VccIpCmd::SenderHandleId senderHandleId,
                             std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    /// This support function will prepare a PDU based on the given input and send it to the message dispatcher
    ///
    /// @param[in] pMsg                 A pointer to an ASN1 message (struct)
    /// @param[in] encodeFunc           Pointer to function which encodes the given message
    /// @param[in] encodedSizeFunc      Pointer to function which returns the size of the encoded message
    /// @param[in] serviceId            Service Id
    /// @param[in] operationId          Operation Id
    /// @param[in] operationType        Operation Type
    /// @param[in] dataType             Data Type
    /// @param[in] senderHandleId       Sender Handle Id
    /// @param[in] ecu                  ECU to send the PDU to
    /// @param[in] pCallerData          (Optional?) CallerData
    template <class MsgTypePtr>
    void PreparePduAndSend(MsgTypePtr pMsg,
                           ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                           U32 (*encodedSizeFunc)(MsgTypePtr),
                           const VccIpCmd::ServiceId serviceId,
                           const VccIpCmd::OperationId operationId,
                           const VccIpCmd::OperationType operationType,
                           const VccIpCmd::DataType dataType,
                           const VccIpCmd::SenderHandleId senderHandleId,  // TODO: This is not the full SenderHandleId,
                                                                           // only last 8bit sequenceID are used,
                                                                           // document properly
                           const Message::Ecu ecu,
                           std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    template <typename ResponseMsgTypePtr>
    void ResponseHandlerWithDecode(ASN_Result ResponseDecodeFunc(ResponseMsgTypePtr, ASN_Session, ASN_Stream),
                                   ResponseMsgTypePtr ResponseCreateFunc(ASN_Session),
                                   Message &msg,
                                   std::shared_ptr<MessageDispatcher::CallerData> callerData);

    template <typename RequestMsgTypePtr, typename ResponseMsgTypePtr>
    void SendSetRequestWithReply(
        RequestMsgTypePtr pMsg,
        U32 RequestEncodeSizeFunc(RequestMsgTypePtr),
        ASN_Result RequestEncodeFunc(RequestMsgTypePtr, ASN_Stream),
        ASN_Result ResponseDecodeFunc(ResponseMsgTypePtr, ASN_Session, ASN_Stream),
        ResponseMsgTypePtr ResponseCreateFunc(ASN_Session),
        const VccIpCmd::ServiceId serviceId,
        const VccIpCmd::OperationId operationId,
        const Message::Ecu ecu,
        std::function<void(std::shared_ptr<MessageDispatcher::CallerData>, ResponseMsgTypePtr)> responseCallback);

    template <typename ResponseMsgTypePtr>
    void SendGenericRequestWithReply(
        ASN_Result ResponseDecodeFunc(ResponseMsgTypePtr, ASN_Session, ASN_Stream),
        ResponseMsgTypePtr ResponseCreateFunc(ASN_Session),
        const VccIpCmd::ServiceId serviceId,
        const VccIpCmd::OperationId operationId,
        const Message::Ecu ecu,
        std::function<void(std::shared_ptr<MessageDispatcher::CallerData>, ResponseMsgTypePtr)> responseCallback);

    // Must be used when sending set request messages!
    template <class MsgTypePtr>
    void SendSetRequest(MsgTypePtr pMsg,
                        ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                        U32 (*encodedSizeFunc)(MsgTypePtr),
                        const VccIpCmd::ServiceId serviceId,
                        const VccIpCmd::OperationId operationId,
                        const Message::Ecu ecu,
                        std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    // Must be used when sending set request no return messages!
    template <class MsgTypePtr>
    void SendSetRequestNoReturn(MsgTypePtr pMsg,
                                ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                                U32 (*encodedSizeFunc)(MsgTypePtr),
                                const VccIpCmd::ServiceId serviceId,
                                const VccIpCmd::OperationId operationId,
                                const Message::Ecu ecu,
                                std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    // Must be used when sending response messages!
    template <class MsgTypePtr>
    void SendResponse(MsgTypePtr pMsg,
                      ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                      U32 (*encodedSizeFunc)(MsgTypePtr),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      const VccIpCmd::SenderHandleId senderHandleId);

    // Must be used when sending error response messages!
    void SendGenericError(Icb_OpGeneric_Error pMsg,
                          const VccIpCmd::ServiceId serviceId,
                          const VccIpCmd::OperationId operationId,
                          const Message::Ecu ecu,
                          const uint32_t senderHandleId);

    template <class MsgTypePtr>
    void SendError(MsgTypePtr pMsg,
                   ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                   U32 (*encodedSizeFunc)(MsgTypePtr),
                   const VccIpCmd::ServiceId serviceId,
                   const VccIpCmd::OperationId operationId,
                   const Message::Ecu ecu,
                   const uint32_t senderHandleId);

    // Use for all other message types.
    // TODO: Move to private section, and make message type specific wrappers.
    //       In case we ahead realize the need of type specific handling...
    template <class MsgTypePtr>
    void SendEncodedMsg(MsgTypePtr pMsg,
                        ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                        U32 (*encodedSizeFunc)(MsgTypePtr),
                        const VccIpCmd::ServiceId serviceId,
                        const VccIpCmd::OperationId operationId,
                        const VccIpCmd::OperationType operationType,
                        const Message::Ecu ecu,
                        const uint8_t seqNr);

    // TODO: Direct use of these this low level senders from service level should really be avoided.
    //       Move to private section and update calls (and remove unnecessary message assembly stuff)
    //       DO ABSOLUTELY NOT USE FOR NEW FEATURES!!
    void broadcastMessage(Message &&msg);

    template <class MsgTypePtr>
    bool DecodeResponse(Message &encodedMsg,
                        std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                        MsgTypePtr &pDecodedMsg,
                        MsgTypePtr (*createFunc)(ASN_Session session),
                        ASN_Result (*decodeFunc)(MsgTypePtr ThisPtr, ASN_Session session, ASN_Stream stream));

    template <class MsgTypePtr>
    bool DecodeMessage(Message &encodedMsg,
                       MsgTypePtr &pDecodedMsg,
                       MsgTypePtr (*createFunc)(ASN_Session session),
                       ASN_Result (*decodeFunc)(MsgTypePtr ThisPtr, ASN_Session session, ASN_Stream stream));

    void registerResponseCallback(VccIpCmd::ServiceId serviceId,
                                  VccIpCmd::OperationId operationId,
                                  MessageDispatcher::ResponseMessageCallback messageCb);

    void registerRequestCallback(VccIpCmd::ServiceId serviceId,
                                 VccIpCmd::OperationId operationId,
                                 MessageDispatcher::MessageCallback messageCb);

    void registerSetRequestCallback(VccIpCmd::ServiceId serviceId,
                                    VccIpCmd::OperationId operationId,
                                    MessageDispatcher::MessageCallback messageCb);

    // TODO: Why do we distinguish between NOTIFICATION and NOTIFICATION_CYCLIC? Application shouldn't care?
    void registerNotificationCallback(VccIpCmd::ServiceId serviceId,
                                      VccIpCmd::OperationId operationId,
                                      MessageDispatcher::MessageCallback messageCb);
    void registerNotificationCyclicCallback(VccIpCmd::ServiceId serviceId,
                                            VccIpCmd::OperationId operationId,
                                            MessageDispatcher::MessageCallback messageCb);

    void registerNotificationRequestCallback(VccIpCmd::ServiceId serviceId,
                                             VccIpCmd::OperationId operationId,
                                             MessageDispatcher::MessageCallback messageCb);

    // Note: There is by design no 'registerErrorCallback'.
    //       Errors associated with earlier sent requests are returned trough response callback.
    //       (Errors associated with earlier sent responses are currently not handled above TransportServices level...)

    uint8_t getNewSequenceId();

    MessageDispatcher *m_dispatcher = nullptr;
    uint8_t m_sequenceId = 0;

    // Helpers for ASN1 codec
    // NOTE! For use when called by MessageDispatcher (thread) only (when receiving messages)!
    ASN_BYTE m_session_buffer_msgd[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_msgd = ASN_Session_Create(m_session_buffer_msgd, sizeof(m_session_buffer_msgd));
    ASN_BYTE m_stream_buffer_msgd[ASN_STREAM_SIZE];
    ASN_Stream m_stream_msgd = ASN_Stream_Create(m_stream_buffer_msgd, sizeof(m_stream_buffer_msgd));

private:
    template <typename ResponseMsgTypePtr>
    class CallerDataWithCallback final : public MessageDispatcher::CallerData
    {
    public:
        CallerDataWithCallback(
            std::function<void(std::shared_ptr<MessageDispatcher::CallerData>, ResponseMsgTypePtr)> responseCallback)
            : responseCallback{responseCallback}
        {
        }
        const std::function<void(std::shared_ptr<MessageDispatcher::CallerData>, ResponseMsgTypePtr)> responseCallback;
    };

    void sendMessage(Message &&msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData = nullptr);
    void sendMessage(Message::Ecu ecu, Message &&msg);
    void PreparePduHeader(Pdu &pdu,
                          const VccIpCmd::ServiceId serviceId,
                          const VccIpCmd::OperationId operationId,
                          const VccIpCmd::OperationType operationType,
                          const VccIpCmd::DataType dataType,
                          const VccIpCmd::SenderHandleId senderHandleId);

    void CreateAndSendPduMessage(const Message::Ecu ecu,
                                 Pdu &pdu,
                                 std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    // Used by SendXxxWithResponse-functions for automatic registration of response callbacks.
    std::set< std::tuple<VccIpCmd::ServiceId, VccIpCmd::OperationId> > registeredResponseHandlers;
};

template <class MsgTypePtr>
void IpService::PreparePduAndSend(MsgTypePtr pMsg,
                                  ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                                  U32 (*encodedSizeFunc)(MsgTypePtr),
                                  const VccIpCmd::ServiceId serviceId,
                                  const VccIpCmd::OperationId operationId,
                                  const VccIpCmd::OperationType operationType,
                                  const VccIpCmd::DataType dataType,
                                  const VccIpCmd::SenderHandleId senderHandleId,
                                  const Message::Ecu ecu,
                                  std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    ASSERT_OR_RETURN_VOID3(pMsg != nullptr, serviceId, operationId, operationType);
    ASSERT_OR_RETURN_VOID3(encodedSizeFunc != nullptr, serviceId, operationId, operationType);
    ASSERT_OR_RETURN_VOID3(encodeFunc != nullptr, serviceId, operationId, operationType);

    // Prepare header
    Pdu pdu;
    PreparePduHeader(pdu, serviceId, operationId, operationType, dataType, senderHandleId);

    // Allocate stream buffer
    std::vector<uint8_t> bufStream(ASN_STREAM_SIZE);

    // Prepare payload
    // - Align the size of the ASN1 payload buffer on 8-bit boundary
    const U32 encodedSizeInBits = encodedSizeFunc(pMsg);
    const U32 encodedSizeInBytes = (encodedSizeInBits + 7) / 8;
    std::vector<uint8_t> bufPayload(encodedSizeInBytes);

    ASN_Stream stream = ASN_Stream_Create(&bufStream.front(), bufStream.size());
    ASSERT_OR_RETURN_VOID3(stream != nullptr, serviceId, operationId, operationType);

    ASN_Stream_AttachBuffer(stream, &bufPayload.front(), bufPayload.size());

    ASN_Result res = encodeFunc(pMsg, stream);
    if (res != ASN_RESULT_OK)
    {
        ALOGE(
            "[%s] %s: ASN1 encoding failed (ASN error '%s' @ InfotainmentIpBus.c: %u) for %s.%s (%04X.%04X.%02X-%08X) "
            "DT=%d",
            service_name_.c_str(),
            __FUNCTION__,
            InfotainmentIpService::Utils::AsnErrCodeToString(res->error).c_str(),
            res->linenumber,
            toString(serviceId, operationId).c_str(),
            toString(operationType),
            (int)serviceId,
            (int)operationId,
            (int)operationType,
            pdu.header.sender_handle_id,
            (int)dataType);
        return;  // TODO: Report error to upper layers????????
    }

    // .. and move it to pdu
    pdu.setPayload(std::move(bufPayload));

    CreateAndSendPduMessage(ecu, pdu, pCallerData);
}

template <typename ResponseMsgTypePtr>
void IpService::ResponseHandlerWithDecode(ASN_Result ResponseDecodeFunc(ResponseMsgTypePtr, ASN_Session, ASN_Stream),
                                          ResponseMsgTypePtr ResponseCreateFunc(ASN_Session),
                                          Message &msg,
                                          std::shared_ptr<MessageDispatcher::CallerData> callerData)
{
    std::shared_ptr<CallerDataWithCallback<ResponseMsgTypePtr>> callerDataResp =
        std::dynamic_pointer_cast<CallerDataWithCallback<ResponseMsgTypePtr>>(callerData);
    if (callerDataResp == nullptr)
        return;

    ResponseMsgTypePtr resp;
    bool decodeOk = DecodeResponse(msg, callerData, resp, ResponseCreateFunc, ResponseDecodeFunc);
    if (decodeOk)
    {
        if (callerDataResp->responseCallback)
            callerDataResp->responseCallback(callerData, resp);
    }
    else
    {
        if (callerData->errorType == TransportServices::ErrorType::OK)
            callerData->errorType = TransportServices::ErrorType::PAYLOAD_DECODE_ERROR;
        if (callerDataResp->responseCallback)
            callerDataResp->responseCallback(callerData, resp);
    }
}

template <typename RequestMsgTypePtr, typename ResponseMsgTypePtr>
void IpService::SendSetRequestWithReply(
    RequestMsgTypePtr pMsg,
    U32 RequestEncodeSizeFunc(RequestMsgTypePtr),
    ASN_Result RequestEncodeFunc(RequestMsgTypePtr, ASN_Stream),
    ASN_Result ResponseDecodeFunc(ResponseMsgTypePtr, ASN_Session, ASN_Stream),
    ResponseMsgTypePtr ResponseCreateFunc(ASN_Session),
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::function<void(std::shared_ptr<MessageDispatcher::CallerData>, ResponseMsgTypePtr)> responseCallback)
{
    auto callerData = std::make_shared<CallerDataWithCallback<ResponseMsgTypePtr>>(responseCallback);

    const auto emplaceStatus = registeredResponseHandlers.emplace(std::make_tuple(serviceId, operationId));
    const bool newInsertion = emplaceStatus.second;
    if (newInsertion)
    {
        registerResponseCallback(serviceId,
                                 operationId,
                                 [ResponseDecodeFunc, ResponseCreateFunc, this](
                                     Message &msg, std::shared_ptr<MessageDispatcher::CallerData> callerData) {
                                     ResponseHandlerWithDecode(ResponseDecodeFunc, ResponseCreateFunc, msg, callerData);
                                 });
    }
    SendSetRequest(pMsg, RequestEncodeFunc, RequestEncodeSizeFunc, serviceId, operationId, ecu, callerData);
}

template <typename ResponseMsgTypePtr>
void IpService::SendGenericRequestWithReply(
    ASN_Result ResponseDecodeFunc(ResponseMsgTypePtr, ASN_Session, ASN_Stream),
    ResponseMsgTypePtr ResponseCreateFunc(ASN_Session),
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::function<void(std::shared_ptr<MessageDispatcher::CallerData>, ResponseMsgTypePtr)> responseCallback)
{
    auto callerData = std::make_shared<CallerDataWithCallback<ResponseMsgTypePtr>>(responseCallback);

    const auto emplaceStatus = registeredResponseHandlers.emplace(std::make_tuple(serviceId, operationId));
    const bool newInsertion = emplaceStatus.second;
    if (newInsertion)
    {
        registerResponseCallback(serviceId,
                                 operationId,
                                 [ResponseDecodeFunc, ResponseCreateFunc, this](
                                     Message &msg, std::shared_ptr<MessageDispatcher::CallerData> callerData) {
                                     ResponseHandlerWithDecode(ResponseDecodeFunc, ResponseCreateFunc, msg, callerData);
                                 });
    }
    SendGenericRequest(serviceId, operationId, ecu, callerData);
}

// MsgTypePtr - Icb_XXX message type pointer (e.g. Icb_OpDeadReckonedPosition_Response).
// EncodeFunc - Encode function for message type (e.g. Icb_OpDeadReckonedPosition_Response_Encode).
// EncodeFunc - Encoded size function for message type (e.g. Icb_OpDeadReckonedPosition_Response_EncodedSize).
template <class MsgTypePtr>
void IpService::SendSetRequest(MsgTypePtr pMsg,
                               ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                               U32 (*encodedSizeFunc)(MsgTypePtr),
                               const VccIpCmd::ServiceId serviceId,
                               const VccIpCmd::OperationId operationId,
                               const Message::Ecu ecu,
                               std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    return PreparePduAndSend(pMsg,
                             encodeFunc,
                             encodedSizeFunc,
                             serviceId,
                             operationId,
                             VccIpCmd::OperationType::SETREQUEST,
                             VccIpCmd::DataType::ENCODED,
                             getNewSequenceId(),
                             ecu,
                             pCallerData);
}

// MsgTypePtr - Icb_XXX message type pointer (e.g. Icb_OpDeadReckonedPosition_Response).
// EncodeFunc - Encode function for message type (e.g. Icb_OpDeadReckonedPosition_Response_Encode).
// EncodeFunc - Encoded size function for message type (e.g. Icb_OpDeadReckonedPosition_Response_EncodedSize).
template <class MsgTypePtr>
void IpService::SendSetRequestNoReturn(MsgTypePtr pMsg,
                                       ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                                       U32 (*encodedSizeFunc)(MsgTypePtr),
                                       const VccIpCmd::ServiceId serviceId,
                                       const VccIpCmd::OperationId operationId,
                                       const Message::Ecu ecu,
                                       std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    return PreparePduAndSend(pMsg,
                             encodeFunc,
                             encodedSizeFunc,
                             serviceId,
                             operationId,
                             VccIpCmd::OperationType::SETREQUEST_NORETURN,
                             VccIpCmd::DataType::ENCODED,
                             getNewSequenceId(),
                             ecu,
                             pCallerData);
}

// MsgTypePtr - Icb_XXX message type pointer (e.g. Icb_OpDeadReckonedPosition_Response).
// EncodeFunc - Encode function for message type (e.g. Icb_OpDeadReckonedPosition_Response_Encode).
// EncodeFunc - Encoded size function for message type (e.g. Icb_OpDeadReckonedPosition_Response_EncodedSize).
// senderHandleId - SenderHandleId of original request (/ set request / notification request) that this response belongs
// to.
template <class MsgTypePtr>
void IpService::SendResponse(MsgTypePtr pMsg,
                             ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                             U32 (*encodedSizeFunc)(MsgTypePtr),
                             const VccIpCmd::ServiceId serviceId,
                             const VccIpCmd::OperationId operationId,
                             const Message::Ecu ecu,
                             const VccIpCmd::SenderHandleId senderHandleId)
{
    return PreparePduAndSend(pMsg,
                             encodeFunc,
                             encodedSizeFunc,
                             serviceId,
                             operationId,
                             VccIpCmd::OperationType::RESPONSE,
                             VccIpCmd::DataType::ENCODED,
                             senderHandleId,
                             ecu,
                             nullptr);
}

// MsgTypePtr - Icb_XXX message type pointer (e.g. Icb_OpDeadReckonedPosition_Response).
// EncodeFunc - Encode function for message type (e.g. Icb_OpDeadReckonedPosition_Response_Encode).
// EncodeFunc - Encoded size function for message type (e.g. Icb_OpDeadReckonedPosition_Response_EncodedSize).
// senderHandleId - SenderHandleId of original request (/ set request / notification request) that this response belongs
// to.
template <class MsgTypePtr>
void IpService::SendError(MsgTypePtr pMsg,
                          ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                          U32 (*encodedSizeFunc)(MsgTypePtr),
                          const VccIpCmd::ServiceId serviceId,
                          const VccIpCmd::OperationId operationId,
                          const Message::Ecu ecu,
                          const uint32_t senderHandleId)
{
    return PreparePduAndSend(pMsg,
                             encodeFunc,
                             encodedSizeFunc,
                             serviceId,
                             operationId,
                             VccIpCmd::OperationType::ERROR,
                             VccIpCmd::DataType::ENCODED,
                             senderHandleId,
                             ecu,
                             nullptr);
}

// MsgTypePtr - Icb_XXX message type pointer (e.g. Icb_OpDeadReckonedPosition_Response).
// EncodeFunc - Encode function for message type (e.g. Icb_OpDeadReckonedPosition_Response_Encode).
// EncodeFunc - Encoded size function for message type (e.g. Icb_OpDeadReckonedPosition_Response_EncodedSize).
template <class MsgTypePtr>
void IpService::SendEncodedMsg(MsgTypePtr pMsg,
                               ASN_Result (*encodeFunc)(MsgTypePtr, ASN_Stream),
                               U32 (*encodedSizeFunc)(MsgTypePtr),
                               const VccIpCmd::ServiceId serviceId,
                               const VccIpCmd::OperationId operationId,
                               const VccIpCmd::OperationType operationType,
                               const Message::Ecu ecu,
                               const uint8_t seqNr)
{
    return PreparePduAndSend(pMsg,
                             encodeFunc,
                             encodedSizeFunc,
                             serviceId,
                             operationId,
                             operationType,
                             VccIpCmd::DataType::ENCODED,
                             seqNr,
                             ecu,
                             nullptr);
}

// Helper for response receive callbacks.
// Performs common consistency checks and decoding work needed for response message payload.
//
// MsgTypePtr - Icb_XXX message type pointer (e.g. Icb_OpDeadReckonedPosition_Response).
// CreateFunc - Structure creation function for message type (e.g. Icb_OpDeadReckonedPosition_Response_Create).
// DecodeFunc - Decode function for message type (e.g. Icb_OpDeadReckonedPosition_Response_Decode).
//
// return - true  = Decoding went ok.
//          false = Decoding failed or not a response message (error/timeout).
//                  In case decoding of response message failed,
//                  pCallerData->errorType will get assigned value PAYLOAD_DECODE_ERROR.
template <class MsgTypePtr>
bool IpService::DecodeResponse(Message &encodedMsg,
                               std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                               MsgTypePtr &pDecodedMsg,
                               MsgTypePtr (*createFunc)(ASN_Session session),
                               ASN_Result (*decodeFunc)(MsgTypePtr ThisPtr, ASN_Session session, ASN_Stream stream))
{
    pDecodedMsg = nullptr;
    bool bRet = true;

    if (pCallerData->errorType != TransportServices::ErrorType::OK)
    {
        if (pCallerData->errorInfo.exists__optional__errorInfo)
        {
            ALOGW(
                "[%s] %s: Skipping payload decode due to error / timeout: %08X - errorType: %u, errorCode: %u, "
                "errorInfo: %u",
                service_name_.c_str(),
                __FUNCTION__,
                encodedMsg.pdu.header.sender_handle_id,
                static_cast<uint32_t>(pCallerData->errorType),
                pCallerData->errorInfo.errorCode,
                pCallerData->errorInfo.errorInfo);
        }
        else
        {
            ALOGW(
                "[%s] %s: Skipping payload decode due to error / timeout: %08X - errorType: %u, errorCode: %u",
                service_name_.c_str(),
                __FUNCTION__,
                encodedMsg.pdu.header.sender_handle_id,
                static_cast<uint32_t>(pCallerData->errorType),
                pCallerData->errorInfo.errorCode);
        }
        bRet = false;
    }
    else if (!DecodeMessage(encodedMsg, pDecodedMsg, createFunc, decodeFunc))
    {
        pCallerData->errorType = TransportServices::ErrorType::PAYLOAD_DECODE_ERROR;
        bRet = false;
    }

    return bRet;
}

// Helper for receive callbacks for all other message types.
// Performs common consistency checks and decoding work needed for message payload.
//
// MsgTypePtr - Icb_XXX message type pointer (e.g. Icb_OpDeadReckonedPosition_Request).
// CreateFunc - Structure creation function for message type (e.g. Icb_OpDeadReckonedPosition_Request_Create).
// DecodeFunc - Decode function for message type (e.g. Icb_OpDeadReckonedPosition_Request_Decode).
//
// pDecodeMsg   - Return of structure representing the decoded message.
//                NOTE!! The structure is allocated in our asn session buffer (m_session_msgd)
//                       and gets invalidated next time the function is called!
// return - true  = Decoding went ok.
//          false = Decoding failed.
template <class MsgTypePtr>
bool IpService::DecodeMessage(Message &encodedMsg,
                              MsgTypePtr &pDecodedMsg,
                              MsgTypePtr (*createFunc)(ASN_Session session),
                              ASN_Result (*decodeFunc)(MsgTypePtr ThisPtr, ASN_Session session, ASN_Stream stream))
{
    pDecodedMsg = nullptr;
    bool bRet = true;

    ASSERT_OR_RETURN3(createFunc != nullptr,
                      false,
                      encodedMsg.pdu.header.service_id,
                      encodedMsg.pdu.header.operation_id,
                      encodedMsg.pdu.header.operation_type);
    ASSERT_OR_RETURN3(decodeFunc != nullptr,
                      false,
                      encodedMsg.pdu.header.service_id,
                      encodedMsg.pdu.header.operation_id,
                      encodedMsg.pdu.header.operation_type);

    ASN_Session_Reset(m_session_msgd);
    pDecodedMsg = createFunc(m_session_msgd);

    if (!pDecodedMsg)
    {
        ALOGE("[%s] ERROR: Session buffer is too small for message type (operation id: 0x%04X)",
                                   service_name_.c_str(),
                                   (int)encodedMsg.pdu.header.operation_id);
        bRet = false;
    }
    else
    {
        // Attach the PDU payload to the ASN bitstream buffer
        ASN_Stream_AttachBuffer(m_stream_msgd, encodedMsg.pdu.payload.data(), encodedMsg.pdu.payload.size());

        const ASN_Result ASN_retcode = decodeFunc(pDecodedMsg, m_session_msgd, m_stream_msgd);
        if (ASN_retcode)
        {
            ALOGE(
                "[%s] Payload decoding failed (ASN error '%s' @ InfotainmentIpBus.c: %u) %04X.%04X.%02d 0x%08X(size: "
                "%d)",
                service_name_.c_str(),
                InfotainmentIpService::Utils::AsnErrCodeToString(ASN_retcode->error).c_str(),
                ASN_retcode->linenumber,
                (int)encodedMsg.pdu.header.service_id,
                (int)encodedMsg.pdu.header.operation_id,
                (int)encodedMsg.pdu.header.operation_type,
                encodedMsg.pdu.header.sender_handle_id,
                (int)encodedMsg.pdu.payload.size());

            bRet = false;
        }
    }

    // Detach the (possible) PDU payload from the ASN bitstream buffer
    ASN_Stream_Reset(m_stream_msgd);

    return bRet;
}

}  // Connectivity

#endif  // IPSERVICE_H
