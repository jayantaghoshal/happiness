/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <mock_ip_service.h>

namespace Connectivity
{
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
    (void)pMsg;
    (void)encodeFunc;
    (void)encodedSizeFunc;
    (void)serviceId;
    (void)operationId;
    (void)ecu;
    (void)pCallerData;
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
    (void)pMsg;
    (void)encodeFunc;
    (void)encodedSizeFunc;
    (void)serviceId;
    (void)operationId;
    (void)ecu;
    (void)pCallerData;
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
    (void)pMsg;
    (void)encodeFunc;
    (void)encodedSizeFunc;
    (void)serviceId;
    (void)operationId;
    (void)ecu;
    (void)pCallerData;
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
    (void)pMsg;
    (void)encodeFunc;
    (void)encodedSizeFunc;
    (void)serviceId;
    (void)operationId;
    (void)ecu;
    (void)pCallerData;
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
    (void)pMsg;
    (void)encodeFunc;
    (void)encodedSizeFunc;
    (void)serviceId;
    (void)operationId;
    (void)ecu;
    (void)pCallerData;
}

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
    (void)pMsg;
    (void)encodeFunc;
    (void)encodedSizeFunc;
    (void)serviceId;
    (void)operationId;
    (void)ecu;
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
    (void)pMsg;
    (void)encodeFunc;
    (void)encodedSizeFunc;
    (void)serviceId;
    (void)operationId;
    (void)ecu;
}

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
    (void)encodedMsg;
    (void)pCallerData;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pCallerData;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pCallerData;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pCallerData;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pCallerData;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pCallerData;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pCallerData;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pCallerData;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
}

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
    (void)encodedMsg;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
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
    (void)encodedMsg;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpDeviceInstall_SetRequest &pDecodedMsg,
                                  Icb_OpDeviceInstall_SetRequest (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpDeviceInstall_SetRequest, ASN_Session, ASN_Stream))
{
    (void)encodedMsg;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
}

template <>
bool MockIpService::DecodeMessage(Message &encodedMsg,
                                  Icb_OpLocalStorage_Response &pDecodedMsg,
                                  Icb_OpLocalStorage_Response (*createFunc)(ASN_Session),
                                  ASN_Result (*decodeFunc)(Icb_OpLocalStorage_Response, ASN_Session, ASN_Stream))
{
    (void)encodedMsg;
    (void)pDecodedMsg;
    (void)createFunc;
    (void)decodeFunc;
    return true;
}

// void IpService::PreparePduHeader(Pdu &pdu,
//                                 const VccIpCmd::ServiceId serviceId,
//                                 const VccIpCmd::OperationId operationId,
//                                 const VccIpCmd::OperationType operationType,
//                                 const VccIpCmd::DataType dataType,
//                                 const VccIpCmd::SenderHandleId senderHandleId)
//
//{
//}
}
