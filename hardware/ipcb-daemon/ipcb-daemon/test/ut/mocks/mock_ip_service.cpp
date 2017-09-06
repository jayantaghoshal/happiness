/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define _MOCK_IP_SERVICE_CPP_

#include "mock_ip_service.h"

namespace Connectivity
{
bool IpService::RegisterClient(const std::vector<ClientIpCommandBusService> &table)
{
    return static_cast<MockIpService *>(this)->RegisterClient(table);
}

bool IpService::RegisterServer(const std::vector<ServerIpCommandBusService> &table)
{
    return static_cast<MockIpService *>(this)->RegisterServer(table);
}

void IpService::setDispatcher(MessageDispatcher *dispatcher)
{
    static_cast<MockIpService *>(this)->setDispatcher(dispatcher);
}

// This is a private function in (real) IpService.cpp being used by template functions in (real) IpService.h.
// Thus we must have an (empty) implementation to be able to compile.
// Never supposed to execute though when running unit tests for service layers!
void IpService::sendMessage(Message &&msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    static_cast<MockIpService *>(this)->sendMessage(msg, pCallerData);
}

void IpService::broadcastMessage(Message &&msg)
{
    static_cast<MockIpService *>(this)->broadcastMessage(msg);
}

// This is a private function in (real) IpService.cpp being used by template functions in (real) IpService.h.
// Thus we must have an (empty) implementation to be able to compile.
// Never supposed to execute though when running unit tests for service layers!
void IpService::sendMessage(Message::Ecu ecu, Message &&msg)
{
    fprintf(stderr, "Not supposed to reach this stage in service layer tests...");
    assert(0);
}

void IpService::SendGenericRequest(const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    static_cast<MockIpService *>(this)->SendGenericRequest(serviceId, operationId, ecu, pCallerData);
}

void IpService::SendGenericSetRequestNoReturn(const VccIpCmd::ServiceId serviceId,
                                              const VccIpCmd::OperationId operationId,
                                              const Message::Ecu ecu,
                                              std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    static_cast<MockIpService *>(this)->SendGenericRequest(serviceId, operationId, ecu, pCallerData);
}

void IpService::SendGenericResponse(const VccIpCmd::ServiceId serviceId,
                                    const VccIpCmd::OperationId operationId,
                                    const Message::Ecu ecu,
                                    const VccIpCmd::SenderHandleId senderHandleId,
                                    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    static_cast<MockIpService *>(this)->SendGenericResponse(serviceId, operationId, ecu, senderHandleId, pCallerData);
}

uint8_t IpService::getNewSequenceId()
{
    return static_cast<MockIpService *>(this)->getNewSequenceId();
}

void IpService::CreateAndSendPduMessage(const Message::Ecu ecu,
                                        Pdu &pdu,
                                        std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    // Create message and set pdu.
    Message message(std::move(pdu));
    message.ecu = ecu;

    // Send the message
    sendMessage(std::move(message), pCallerData);
}

void IpService::registerResponseCallback(VccIpCmd::ServiceId serviceId,
                                         VccIpCmd::OperationId operationId,
                                         MessageDispatcher::ResponseMessageCallback messageCb)
{
    assert(m_dispatcher);
    return m_dispatcher->registerResponseCallback(serviceId, operationId, messageCb);
}

void IpService::registerRequestCallback(VccIpCmd::ServiceId serviceId,
                                        VccIpCmd::OperationId operationId,
                                        MessageDispatcher::MessageCallback messageCb)
{
    assert(m_dispatcher);
    return m_dispatcher->registerRequestCallback(serviceId, operationId, messageCb);
}

void IpService::registerSetRequestCallback(VccIpCmd::ServiceId serviceId,
                                           VccIpCmd::OperationId operationId,
                                           MessageDispatcher::MessageCallback messageCb)
{
    assert(m_dispatcher);
    return m_dispatcher->registerSetRequestCallback(serviceId, operationId, messageCb);
}

void IpService::registerNotificationCallback(VccIpCmd::ServiceId serviceId,
                                             VccIpCmd::OperationId operationId,
                                             MessageDispatcher::MessageCallback messageCb)
{
    assert(m_dispatcher);
    return m_dispatcher->registerNotificationCallback(serviceId, operationId, messageCb);
}

void IpService::registerNotificationCyclicCallback(VccIpCmd::ServiceId serviceId,
                                                   VccIpCmd::OperationId operationId,
                                                   MessageDispatcher::MessageCallback messageCb)
{
    assert(m_dispatcher);
    return m_dispatcher->registerNotificationCyclicCallback(serviceId, operationId, messageCb);
}

void IpService::registerNotificationRequestCallback(VccIpCmd::ServiceId serviceId,
                                                    VccIpCmd::OperationId operationId,
                                                    MessageDispatcher::MessageCallback messageCb)
{
    assert(m_dispatcher);
    return m_dispatcher->registerNotificationRequestCallback(serviceId, operationId, messageCb);
}

// =========================================================================================
// SendSetRequest() specializations

template <>
void MockIpService::SendSetRequest(
    Icb_OpWLANMode_SetRequest pMsg,
    ASN_Result (*encodeFunc)(Icb_OpWLANMode_SetRequest, ASN_Stream),  // Icb_OpWLANMode_SetRequest_Encode
    U32 (*encodedSizeFunc)(Icb_OpWLANMode_SetRequest),                // Icb_OpWLANMode_SetRequest_EncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestWlanMode(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(
    Icb_OpWLANSTAConnect_SetRequest pMsg,
    ASN_Result (*encodeFunc)(Icb_OpWLANSTAConnect_SetRequest, ASN_Stream),  // Icb_OpWLANSTAConnect_SetRequest_Encode
    U32 (*encodedSizeFunc)(Icb_OpWLANSTAConnect_SetRequest),  // Icb_OpWLANSTAConnect_SetRequest_EncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestWlanStaConnect(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(
    Icb_OpCancelDownload_SetRequest pMsg,
    ASN_Result (*encodeFunc)(Icb_OpCancelDownload_SetRequest, ASN_Stream),  // Icb_OpCancelDownload_SetRequest_Encode
    U32 (*encodedSizeFunc)(Icb_OpCancelDownload_SetRequest),  // Icb_OpCancelDownload_SetRequest_EncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestCancelDownload(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(
    Icb_OpDeletedSoftware_SetRequest pMsg,
    ASN_Result (*encodeFunc)(Icb_OpDeletedSoftware_SetRequest, ASN_Stream),  // Icb_OpDeletedSoftware_SetRequest_Encode
    U32 (*encodedSizeFunc)(Icb_OpDeletedSoftware_SetRequest),  // Icb_OpDeletedSoftware_SetRequest_EncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestDeletedSoftware(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(
    Icb_OpDownloadSoftware_SetRequest pMsg,
    ASN_Result (*encodeFunc)(Icb_OpDownloadSoftware_SetRequest,
                             ASN_Stream),                       // Icb_OpDownloadSoftware_SetRequest_Encode
    U32 (*encodedSizeFunc)(Icb_OpDownloadSoftware_SetRequest),  // Icb_OpDownloadSoftware_SetRequest_EncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestDownloadSoftware(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(
    Icb_OpCurrentInternetSource_SetRequest pMsg,
    ASN_Result (*encodeFunc)(Icb_OpCurrentInternetSource_SetRequest,
                             ASN_Stream),  // Icb_OpCurrentInternetSource_SetRequest_Encode
    U32 (*encodedSizeFunc)(
        Icb_OpCurrentInternetSource_SetRequest),  // Icb_OpCurrentInternetSource_SetRequest_EncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestCurrentInternetSource(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(
    Icb_OpInternetGateway_SetRequest pMsg,
    ASN_Result (*encodeFunc)(Icb_OpInternetGateway_SetRequest, ASN_Stream),  // Icb_OpInternetGateway_SetRequest_Encode
    U32 (*encodedSizeFunc)(Icb_OpInternetGateway_SetRequest),  // Icb_OpInternetGateway_SetRequest_EncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestCurrentInternetGateway(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpSubscriptionActivation_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpSubscriptionActivation_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpSubscriptionActivation_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpSubscriptionActivation(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpTelematicSettings_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpTelematicSettings_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpTelematicSettings_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpTelematicSettings(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpEcallSettings_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpEcallSettings_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpEcallSettings_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpECallSettings(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpCallHandling_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpCallHandling_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpCallHandling_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpCallHandling(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpServiceActivation_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpServiceActivation_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpServiceActivation_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpServiceActivation(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpPSIMResetDataCounter_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpPSIMResetDataCounter_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpPSIMResetDataCounter_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpPSIMResetDataCounter(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpPSIMEnterCode_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpPSIMEnterCode_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpPSIMEnterCode_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpPSIMEnterCode(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpPSIMNetworkConfiguration_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpPSIMNetworkConfiguration_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpPSIMNetworkConfiguration_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpPSIMPSIMNetworkConfiguration(
        pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpPSIMNetworkOperatorCommand_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpPSIMNetworkOperatorCommand_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpPSIMNetworkOperatorCommand_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpPSIMNetworkSelection(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpPSIMRoamingConfiguration_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpPSIMRoamingConfiguration_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpPSIMRoamingConfiguration_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpPSIMRoamingConfiguration(
        pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpPSIMSecurity_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpPSIMSecurity_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpPSIMSecurity_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpPSIMSecurity(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequest(Icb_OpPSIMUssd_SetRequest pMsg,
                                   ASN_Result (*encodeFunc)(Icb_OpPSIMUssd_SetRequest, ASN_Stream),
                                   U32 (*encodedSizeFunc)(Icb_OpPSIMUssd_SetRequest),
                                   const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestOpPSIMUssd(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

template <>
void MockIpService::SendSetRequestNoReturn(Icb_OpSoHPacketSend_SetRequestNoReturn pMsg,
                                           ASN_Result (*encodeFunc)(Icb_OpSoHPacketSend_SetRequestNoReturn, ASN_Stream),
                                           U32 (*encodedSizeFunc)(Icb_OpSoHPacketSend_SetRequestNoReturn),
                                           const VccIpCmd::ServiceId serviceId,
                                           const VccIpCmd::OperationId operationId,
                                           const Message::Ecu ecu,
                                           std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    SendSetRequestNoReturnOpSoHPacket(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, pCallerData);
}

// =========================================================================================
// SendResponse() specializations

template <>
void MockIpService::SendResponse(
    Icb_OpDeviceInstall_Response pMsg,
    ASN_Result (*encodeFunc)(Icb_OpDeviceInstall_Response, ASN_Stream),  // Icb_OpDeviceInstall_Response_Encode
    U32 (*encodedSizeFunc)(Icb_OpDeviceInstall_Response),                // Icb_OpDeviceInstall_Response_EncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    const VccIpCmd::SenderHandleId senderHandleId)
{
    SendResponseDeviceInstall(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, senderHandleId);
}

template <>
void MockIpService::SendResponse(
    Icb_OpLocalStorage_Response pMsg,
    ASN_Result (*encodeFunc)(Icb_OpLocalStorage_Response, ASN_Stream),  // Icb_OpLocalStorage_ResponseEncode
    U32 (*encodedSizeFunc)(Icb_OpLocalStorage_Response),                // Icb_OpLocalStorage_ResponseEncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    const VccIpCmd::SenderHandleId senderHandleId)
{
    SendResponseLocalStorage(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, senderHandleId);
}

template <>
void MockIpService::SendResponse(
    Icb_OpInternetSourceCtrlBT_Response pMsg,
    ASN_Result (*encodeFunc)(Icb_OpInternetSourceCtrlBT_Response,
                             ASN_Stream),                         // Icb_OpInternetSourceCtrlBT_ResponseEncode
    U32 (*encodedSizeFunc)(Icb_OpInternetSourceCtrlBT_Response),  // Icb_OpInternetSourceCtrlBT_ResponseEncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    const VccIpCmd::SenderHandleId senderHandleId)
{
    SendResponseInternetSourceCtrlBT(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, senderHandleId);
}

template <>
void MockIpService::SendResponse(
    Icb_OpInternetSourceCtrlUSB_Response pMsg,
    ASN_Result (*encodeFunc)(Icb_OpInternetSourceCtrlUSB_Response,
                             ASN_Stream),                          // Icb_OpInternetSourceCtrlUSB_ResponseEncode
    U32 (*encodedSizeFunc)(Icb_OpInternetSourceCtrlUSB_Response),  // Icb_OpInternetSourceCtrlUSB_ResponseEncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    const VccIpCmd::SenderHandleId senderHandleId)
{
    SendResponseInternetSourceCtrlUSB(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, senderHandleId);
}

template <>
void MockIpService::SendResponse(
    Icb_OpInternetGateway_Response pMsg,
    ASN_Result (*encodeFunc)(Icb_OpInternetGateway_Response, ASN_Stream),  // Icb_OpInternetGateway_ResponseEncode
    U32 (*encodedSizeFunc)(Icb_OpInternetGateway_Response),                // Icb_OpInternetGateway_ResponseEncodedSize
    const VccIpCmd::ServiceId serviceId,
    const VccIpCmd::OperationId operationId,
    const Message::Ecu ecu,
    const VccIpCmd::SenderHandleId senderHandleId)
{
    SendResponseInternetGateway(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, senderHandleId);
}

template <>
void MockIpService::SendResponse(Icb_OpPremiumAudio_Response pMsg,
                                 ASN_Result (*encodeFunc)(Icb_OpPremiumAudio_Response, ASN_Stream),
                                 U32 (*encodedSizeFunc)(Icb_OpPremiumAudio_Response),
                                 const VccIpCmd::ServiceId serviceId,
                                 const VccIpCmd::OperationId operationId,
                                 const Message::Ecu ecu,
                                 const VccIpCmd::SenderHandleId senderHandleId)
{
    SendPremiumAudioResponse(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, senderHandleId);
}

template <>
void MockIpService::SendResponse(Icb_OpGenericSettingSynch_Response pMsg,
                                 ASN_Result (*encodeFunc)(Icb_OpGenericSettingSynch_Response, ASN_Stream),
                                 U32 (*encodedSizeFunc)(Icb_OpGenericSettingSynch_Response),
                                 const VccIpCmd::ServiceId serviceId,
                                 const VccIpCmd::OperationId operationId,
                                 const Message::Ecu ecu,
                                 const VccIpCmd::SenderHandleId senderHandleId)
{
    SendResponseGenericSetting(pMsg, encodeFunc, encodedSizeFunc, serviceId, operationId, ecu, senderHandleId);
}

// =========================================================================================
// DecodeResponse() specializations
template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpWLANSTADisconnect_Response &pDecodedMsg,
    Icb_OpWLANSTADisconnect_Response (*createFunc)(ASN_Session session),  // Icb_OpWLANSTADisconnect_Response
    ASN_Result (*decodeFunc)(Icb_OpWLANSTADisconnect_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpWLANSTADisconnect_Response_Decode
    )
{
    return DecodeResponseWlanStaDisconnect(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpWLANSTAConnect_Response &pDecodedMsg,
    Icb_OpWLANSTAConnect_Response (*createFunc)(ASN_Session session),  // Icb_OpWLANSTAConnect_Response
    ASN_Result (*decodeFunc)(Icb_OpWLANSTAConnect_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpWLANSTAConnect_Response_Decode
    )
{
    return DecodeResponseWlanStaConnect(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpWLANSTAStatus_Response &pDecodedMsg,
    Icb_OpWLANSTAStatus_Response (*createFunc)(ASN_Session session),  // Icb_OpWLANSTAStatus_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpWLANSTAStatus_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpWLANSTAStatus_Response_Decode
    )
{
    return DecodeResponseWlanStaStatus(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpWLANMode_Response &pDecodedMsg,
    Icb_OpWLANMode_Response (*createFunc)(ASN_Session session),                 // Icb_OpWLANMode_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpWLANMode_Response, ASN_Session, ASN_Stream)  // Icb_OpWLANMode_Response_Decode
    )
{
    return DecodeResponseWlanMode(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpWLANSTARememberedNetwks_Response &pDecodedMsg,
    Icb_OpWLANSTARememberedNetwks_Response (*createFunc)(ASN_Session session),  // Icb_OpWLANMode_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpWLANSTARememberedNetwks_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpWLANMode_Response_Decode
    )
{
    return DecodeResponseWlanStaRememberedNetwks(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpWLANSTAForgetRememberedNetwk_Response &pDecodedMsg,
    Icb_OpWLANSTAForgetRememberedNetwk_Response (*createFunc)(
        ASN_Session session),  // Icb_OpWLANSTAForgetRememberedNetwk_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpWLANSTAForgetRememberedNetwk_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpWLANSTAForgetRememberedNetwk_Response_Decode
    )
{
    return DecodeResponseWlanStaForgetRememberedNetwkResponse(
        encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpDownloadStatus_Response &pDecodedMsg,
    Icb_OpDownloadStatus_Response (*createFunc)(ASN_Session session),  // Icb_OpDownloadStatus_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpDownloadStatus_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpDownloadStatus_Response_Decode
    )
{
    return DecodeResponseDownloadStatus(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpCancelDownload_Response &pDecodedMsg,
    Icb_OpCancelDownload_Response (*createFunc)(ASN_Session session),  // Icb_OpCancelDownload_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpCancelDownload_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpCancelDownload_Response_Decode
    )
{
    return DecodeResponseCancelDownload(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpCurrentInternetSource_Response &pDecodedMsg,
                                   Icb_OpCurrentInternetSource_Response (*createFunc)(
                                       ASN_Session session),  // Icb_OpCurrentInternetSource_Response_Create
                                   ASN_Result (*decodeFunc)(Icb_OpCurrentInternetSource_Response,
                                                            ASN_Session,
                                                            ASN_Stream)  // Icb_OpCurrentInternetSource_Response_Decode
                                   )
{
    return DecodeResponseCurrentInternetSource(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpInternetGateway_Response &pDecodedMsg,
    Icb_OpInternetGateway_Response (*createFunc)(ASN_Session session),  // Icb_OpInternetGateway_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpInternetGateway_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpInternetGateway_Response_Decode
    )
{
    return DecodeResponseCurrentInternetGateway(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpConnectivityStatus_Response &pDecodedMsg,
    Icb_OpConnectivityStatus_Response (*createFunc)(ASN_Session session),  // Icb_OpConnectivityStatus_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpConnectivityStatus_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpConnectivityStatus_Response_Decode
    )
{
    return DecodeResponseConnectivityStatus(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpSubscriptionActivation_Response &pDecodedMsg,
                                   Icb_OpSubscriptionActivation_Response (*createFunc)(
                                       ASN_Session session),  // Icb_OpSubscriptionActivation_Response_Create
                                   ASN_Result (*decodeFunc)(Icb_OpSubscriptionActivation_Response,
                                                            ASN_Session,
                                                            ASN_Stream)  // Icb_OpSubscriptionActivation_Response_Decode
                                   )
{
    return DecodeResponseSubscriptionActivation(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpPositionData_Response &pDecodedMsg,
    Icb_OpPositionData_Response (*createFunc)(ASN_Session session),  // Icb_OpPositionData_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpPositionData_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpPositionData_Response_Decode
    )
{
    return DecodeResponsePositionData(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMConnectionStatus_Response &pDecodedMsg,
                                   Icb_OpPSIMConnectionStatus_Response (*createFunc)(
                                       ASN_Session session),  // Icb_OpPSIMConnectionStatus_Response_Create
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMConnectionStatus_Response,
                                                            ASN_Session,
                                                            ASN_Stream)  // Icb_OpPSIMConnectionStatus_Response_Decode
                                   )
{
    return DecodeResponsePSIMConnectionStatus(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(
    Message &encodedMsg,
    std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
    Icb_OpPSIMState_Response &pDecodedMsg,
    Icb_OpPSIMState_Response (*createFunc)(ASN_Session session),                 // Icb_OpPSIMState_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpPSIMState_Response, ASN_Session, ASN_Stream)  // Icb_OpPSIMState_Response_Decode
    )
{
    return DecodeResponsePSIMState(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMRegisterState_Response &pDecodedMsg,
                                   Icb_OpPSIMRegisterState_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMRegisterState_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponsePSIMRegisterState(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMDataCounter_Response &pDecodedMsg,
                                   Icb_OpPSIMDataCounter_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMDataCounter_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponsePSIMDataCounter(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMResetDataCounter_Response &pDecodedMsg,
                                   Icb_OpPSIMResetDataCounter_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMResetDataCounter_Response,
                                                            ASN_Session,
                                                            ASN_Stream))
{
    return DecodeResponsePSIMResetDataCounter(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMUssd_Response &pDecodedMsg,
                                   Icb_OpPSIMUssd_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMUssd_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponsePSIMUssd(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpServiceActivation_Response &pDecodedMsg,
                                   Icb_OpServiceActivation_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpServiceActivation_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponseServiceActivation(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpSVTStatus_Response &pDecodedMsg,
                                   Icb_OpSVTStatus_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpSVTStatus_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponseSVTStatus(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpTNStatus_Response &pDecodedMsg,
                                   Icb_OpTNStatus_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpTNStatus_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponseTNStatus(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMNetworkConfiguration_Response &pDecodedMsg,
                                   Icb_OpPSIMNetworkConfiguration_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkConfiguration_Response,
                                                            ASN_Session,
                                                            ASN_Stream))
{
    return DecodeResponsePSIMNetworkConfiguration(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMRoamingConfiguration_Response &pDecodedMsg,
                                   Icb_OpPSIMRoamingConfiguration_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMRoamingConfiguration_Response,
                                                            ASN_Session,
                                                            ASN_Stream))
{
    return DecodeResponsePSIMRoamingConfiguration(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMNetworkOperators_Response &pDecodedMsg,
                                   Icb_OpPSIMNetworkOperators_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkOperators_Response,
                                                            ASN_Session,
                                                            ASN_Stream))
{
    return DecodeResponsePSIMNetworkScan(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMNetworkOperatorCommand_Response &pDecodedMsg,
                                   Icb_OpPSIMNetworkOperatorCommand_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkOperatorCommand_Response,
                                                            ASN_Session,
                                                            ASN_Stream))
{
    return DecodeResponsePSIMNetworkSelection(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpPSIMSecurity_Response &pDecodedMsg,
                                   Icb_OpPSIMSecurity_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpPSIMSecurity_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponsePSIMSecurity(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpAccessRightPinCode_Response &pDecodedMsg,
                                   Icb_OpAccessRightPinCode_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpAccessRightPinCode_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponseAccessRightPinCode(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpTelematicSettings_Response &pDecodedMsg,
                                   Icb_OpTelematicSettings_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpTelematicSettings_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponseTelematicSetting(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpEcallSettings_Response &pDecodedMsg,
                                   Icb_OpEcallSettings_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpEcallSettings_Response, ASN_Session, ASN_Stream))
{
    return DecodeResponseECallSettings(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeResponse(Message &encodedMsg,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                                   Icb_OpFactoryDefaultRestore_Response &pDecodedMsg,
                                   Icb_OpFactoryDefaultRestore_Response (*createFunc)(ASN_Session session),
                                   ASN_Result (*decodeFunc)(Icb_OpFactoryDefaultRestore_Response,
                                                            ASN_Session,
                                                            ASN_Stream))
{
    return DecodeResponseFactoryDefaultRestore(encodedMsg, pCallerData, pDecodedMsg, createFunc, decodeFunc);
}

/// =========================================================================================
// DecodeMessage() specializations
template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpGNSSPositionData_Response &pDecodedMsg,
    Icb_OpGNSSPositionData_Response (*createFunc)(ASN_Session),  // Icb_OpGNSSPositionData_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpGNSSPositionData_Response,
                             ASN_Session,
                             ASN_Stream)  //  Icb_OpGNSSPositionData_Response_Decode
    )
{
    return DecodeMessageGnssPositionDataResponse(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpWLANSTAConnect_Response &pDecodedMsg,
    Icb_OpWLANSTAConnect_Response (*createFunc)(ASN_Session),  // Icb_OpWLANSTAConnect_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpWLANSTAConnect_Response,
                             ASN_Session,
                             ASN_Stream)  //  Icb_OpWLANSTAConnect_Response_Decode
    )
{
    return DecodeMessageWlanStaConnect(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpWLANSTAStatus_Notification &pDecodedMsg,
    Icb_OpWLANSTAStatus_Notification (*createFunc)(ASN_Session),  // Icb_OpWLANSTAStatus_Notification_Create
    ASN_Result (*decodeFunc)(Icb_OpWLANSTAStatus_Notification,
                             ASN_Session,
                             ASN_Stream)  //  Icb_OpWLANSTAStatus_Notification_Decode
    )
{
    return DecodeMessageWlanStaStatus(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpWLANMode_Notification &pDecodedMsg,
    Icb_OpWLANMode_Notification (*createFunc)(ASN_Session),  // Icb_OpWLANMode_Notification_Create
    ASN_Result (*decodeFunc)(Icb_OpWLANMode_Notification,
                             ASN_Session,
                             ASN_Stream)  //  Icb_OpWLANMode_Notification_Decode
    )
{
    return DecodeMessageWlanMode(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpWLANSTAForgetRememberedNetwk_Response &pDecodedMsg,
    Icb_OpWLANSTAForgetRememberedNetwk_Response (*createFunc)(
        ASN_Session),  // Icb_OpWLANSTAForgetRememberedNetwk_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpWLANSTAForgetRememberedNetwk_Response,
                             ASN_Session,
                             ASN_Stream)  //  Icb_OpWLANSTAForgetRememberedNetwk_Response_Decode
    )
{
    return DecodeMessageWlanStaForgetRememberedNetwkResponse(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpDownloadStatus_Response &pDecodedMsg,
    Icb_OpDownloadStatus_Response (*createFunc)(ASN_Session session),  // Icb_OpDownloadStatus_Response_Create
    ASN_Result (*decodeFunc)(Icb_OpDownloadStatus_Response,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpDownloadStatus_Response_Decode
    )
{
    return DecodeMessageDownloadStatus(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpCurrentInternetSource_Notification &pDecodedMsg,
    Icb_OpCurrentInternetSource_Notification (*createFunc)(
        ASN_Session session),  // Icb_OpCurrentInternetSource_Notification_Create
    ASN_Result (*decodeFunc)(Icb_OpCurrentInternetSource_Notification,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpCurrentInternetSource_Notification_Decode
    )
{
    return DecodeMessageCurrentInternetSource(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpConnectivityStatus_Notification &pDecodedMsg,
                                  Icb_OpConnectivityStatus_Notification (*createFunc)(
                                      ASN_Session session),  // Icb_OpConnectivityStatus_Notification_Create
                                  ASN_Result (*decodeFunc)(Icb_OpConnectivityStatus_Notification,
                                                           ASN_Session,
                                                           ASN_Stream)  // Icb_OpConnectivityStatus_Notification_Decode
                                  )
{
    return DecodeMessageConnectivityStatus(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPSIMNetworkOperatorCommand_Notification &pDecodedMsg,
                                  Icb_OpPSIMNetworkOperatorCommand_Notification (*createFunc)(
                                      ASN_Session session),  // Icb_OpPSIMNetworkOperatorCommand_Notification
                                  ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkOperatorCommand_Notification,
                                                           ASN_Session,
                                                           ASN_Stream)  // Icb_OpPSIMNetworkOperatorCommand_Notification
                                  )
{
    return DecodeMessagePSIMNetworkSelection(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpInternetSourceCtrlBT_SetRequest &pDecodedMsg,
                                  Icb_OpInternetSourceCtrlBT_SetRequest (*createFunc)(
                                      ASN_Session session),  // Icb_OpInternetSourceCtrlBT_SetRequest_Create
                                  ASN_Result (*decodeFunc)(Icb_OpInternetSourceCtrlBT_SetRequest,
                                                           ASN_Session,
                                                           ASN_Stream)  // Icb_OpInternetSourceCtrlBT_SetRequest_Decode
                                  )
{
    return DecodeMessageInternetSourceCtrlBT(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpInternetSourceCtrlUSB_SetRequest &pDecodedMsg,
                                  Icb_OpInternetSourceCtrlUSB_SetRequest (*createFunc)(
                                      ASN_Session session),  // Icb_OpInternetSourceCtrlUSB_SetRequest_Create
                                  ASN_Result (*decodeFunc)(Icb_OpInternetSourceCtrlUSB_SetRequest,
                                                           ASN_Session,
                                                           ASN_Stream)  // Icb_OpInternetSourceCtrlUSB_SetRequest_Decode
                                  )
{
    return DecodeMessageInternetSourceCtrlUSB(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpInternetGateway_SetRequest &pDecodedMsg,
    Icb_OpInternetGateway_SetRequest (*createFunc)(ASN_Session session),  // Icb_OpInternetGateway_SetRequest_Create
    ASN_Result (*decodeFunc)(Icb_OpInternetGateway_SetRequest,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpInternetGateway_SetRequest_Decode
    )
{
    return DecodeMessageInternetGateway(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpSoHPackageUploaded_Notification &pDecodedMsg,
                                  Icb_OpSoHPackageUploaded_Notification (*createFunc)(
                                      ASN_Session session),  // Icb_OpSoHPacketSend_SetRequestNoReturn_Create
                                  ASN_Result (*decodeFunc)(Icb_OpSoHPackageUploaded_Notification,
                                                           ASN_Session,
                                                           ASN_Stream)  // Icb_OpSoHPacketSend_SetRequestNoReturn_Decode
                                  )
{
    return DecodeMessageOpSoHPacketSend(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpSubscriptionActivation_Notification &pDecodedMsg,
    Icb_OpSubscriptionActivation_Notification (*createFunc)(
        ASN_Session session),  // Icb_OpSubscriptionActivation_Notification_Create
    ASN_Result (*decodeFunc)(Icb_OpSubscriptionActivation_Notification,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpSubscriptionActivation_Notification_Decode
    )
{
    return DecodeMessageOpSubscriptionActivationNotification(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpRescueStatus_Notification &pDecodedMsg,
    Icb_OpRescueStatus_Notification (*createFunc)(ASN_Session session),  // Icb_OpRescueStatus_Notification_Create
    ASN_Result (*decodeFunc)(Icb_OpRescueStatus_Notification,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpRescueStatus_Notification_Decode
    )
{
    return DecodeMessageOpRescueStatusNotification(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpSVTStatus_Notification &pDecodedMsg,
    Icb_OpSVTStatus_Notification (*createFunc)(ASN_Session session),  // Icb_OpSVTStatus_Notification_Create
    ASN_Result (*decodeFunc)(Icb_OpSVTStatus_Notification,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpSVTStatus_Notification_Decode
    )
{
    return DecodeMessageOpSVTStatusNotification(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(
    Message &encodedMsg,
    Icb_OpTNStatus_Notification &pDecodedMsg,
    Icb_OpTNStatus_Notification (*createFunc)(ASN_Session session),  // Icb_OpSVTStatus_Notification_Create
    ASN_Result (*decodeFunc)(Icb_OpTNStatus_Notification,
                             ASN_Session,
                             ASN_Stream)  // Icb_OpSVTStatus_Notification_Decode
    )
{
    return DecodeMessageOpTNStatusNotification(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpConnectWorkshopWlan_Notification &pDecodedMsg,
                                  Icb_OpConnectWorkshopWlan_Notification (*createFunc)(ASN_Session session),
                                  ASN_Result (*decodeFunc)(Icb_OpConnectWorkshopWlan_Notification,
                                                           ASN_Session,
                                                           ASN_Stream))
{
    return DecodeMessageConnectWorkshopWlanNotification(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpWorkshopWlanDetected_Notification &pDecodedMsg,
                                  Icb_OpWorkshopWlanDetected_Notification (*createFunc)(ASN_Session session),
                                  ASN_Result (*decodeFunc)(Icb_OpWorkshopWlanDetected_Notification,
                                                           ASN_Session,
                                                           ASN_Stream))
{
    return DecodeMessageWorkshopWlanDetectedNotification(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

void IpService::PreparePduHeader(Pdu &pdu,
                                 const VccIpCmd::ServiceId serviceId,
                                 const VccIpCmd::OperationId operationId,
                                 const VccIpCmd::OperationType operationType,
                                 const VccIpCmd::DataType dataType,
                                 const VccIpCmd::SenderHandleId senderHandleId)

{
    static_cast<MockIpService *>(this)->PreparePduHeader(
        pdu, serviceId, operationId, operationType, dataType, senderHandleId);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpDeviceInstall_SetRequest &pDecodedMsg,
                                  Icb_OpDeviceInstall_SetRequest (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpDeviceInstall_SetRequest, ASN_Session, ASN_Stream))
{
    return DecodeMessageDeviceInstallSetRequest(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpGenericSettingSynch_Request &pDecodedMsg,
                                  Icb_OpGenericSettingSynch_Request (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpGenericSettingSynch_Request, ASN_Session, ASN_Stream))
{
    return DecodeMessageGenericSettingRequest(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpGenericSettingSynch_SetRequest &pDecodedMsg,
                                  Icb_OpGenericSettingSynch_SetRequest (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpGenericSettingSynch_SetRequest,
                                                           ASN_Session,
                                                           ASN_Stream))
{
    return DecodeMessageGenericSettingSetRequest(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPSIMConnectionStatus_Notification &pDecodedMsg,
                                  Icb_OpPSIMConnectionStatus_Notification (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpPSIMConnectionStatus_Notification,
                                                           ASN_Session,
                                                           ASN_Stream))
{
    return DecodeMessagePSIMConnectionStatusNotification(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPSIMRegisterState_Response &pDecodedMsg,
                                  Icb_OpPSIMRegisterState_Response (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpPSIMRegisterState_Response, ASN_Session, ASN_Stream))
{
    return DecodeMessagePSIMRegisterState(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPSIMState_Response &pDecodedMsg,
                                  Icb_OpPSIMState_Response (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpPSIMState_Response, ASN_Session, ASN_Stream))
{
    return DecodeMessagePSIMState(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPSIMSignalStrength_Notification &pDecodedMsg,
                                  Icb_OpPSIMSignalStrength_Notification (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpPSIMSignalStrength_Notification,
                                                           ASN_Session,
                                                           ASN_Stream))
{
    return DecodeMessagePSIMSignalStrengthNotification(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPSIMNetworkConfiguration_Response &pDecodedMsg,
                                  Icb_OpPSIMNetworkConfiguration_Response (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkConfiguration_Response,
                                                           ASN_Session,
                                                           ASN_Stream))
{
    return DecodeMessagePSIMNetworkConfigurationResponse(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpTextMessage_SetRequest &pDecodedMsg,
                                  Icb_OpTextMessage_SetRequest (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpTextMessage_SetRequest, ASN_Session, ASN_Stream))
{
    return DecodeMessageTextMessageSetRequest(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPremiumAudio_SetRequest &pDecodedMsg,
                                  Icb_OpPremiumAudio_SetRequest (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpPremiumAudio_SetRequest, ASN_Session, ASN_Stream))
{
    return DecodeMessagePremiumAudioSetRequest(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPSIMMessage_SetRequest &pDecodedMsg,
                                  Icb_OpPSIMMessage_SetRequest (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpPSIMMessage_SetRequest, ASN_Session, ASN_Stream))
{
    return DecodeMessagePSIMMessageSetRequest(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpPSIMUssd_Response &pDecodedMsg,
                                  Icb_OpPSIMUssd_Response (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpPSIMUssd_Response, ASN_Session, ASN_Stream))
{
    return DecodeMessagePSIMUssd(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpWLANChannelFreqInfo_Notification &pDecodedMsg,
                                  Icb_OpWLANChannelFreqInfo_Notification (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpWLANChannelFreqInfo_Notification,
                                                           ASN_Session,
                                                           ASN_Stream))
{
    return DecodeMessageWlanChannelFreqInfo(encodedMsg, pDecodedMsg, createFunc, decodeFunc);
}

IpService::NotificationSubscriber::NotificationSubscriber(const Message &msg)
    : serviceId(msg.pdu.header.service_id), operationId(msg.pdu.header.operation_id), ecu(msg.ecu)
{
}

IpService::NotificationSubscriber::NotificationSubscriber(VccIpCmd::ServiceId serviceId,
                                                          VccIpCmd::OperationId operationId,
                                                          Message::Ecu ecu)
    : serviceId(serviceId), operationId(operationId), ecu(ecu)
{
}

IpService::NotificationSubscriber::NotificationSubscriber()
    : serviceId(VccIpCmd::ServiceId::Undefined),
      operationId(VccIpCmd::OperationId::Undefined),
      ecu(Message::Ecu::UNKNOWN)
{
}

}  // namespace
