/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_IP_SERVICE_H
#define MOCK_IP_SERVICE_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <functional>

#include "IpService.h"
#include "ipcommandbus/Message.h"
#include "ipcommandbus/vcc_pdu_header.h"

#ifdef __cplusplus
extern "C" {
#include "infotainmentIpBus.h"
#include "pl/asn_base/asn_base.h"
}
#endif

namespace Connectivity
{
/**
 *
 */
class MockIpService : public Connectivity::IpService
{
public:
    MOCK_METHOD1(setDispatcher, void(Connectivity::MessageDispatcher *dispatcher));

    /// Register a set of client operations according to the given \c table.
    ///
    /// @param[in] table Table with operations to register
    MOCK_METHOD1(RegisterClient, bool(const std::vector<ClientIpCommandBusService> &table));

    /// Register a set of local operations this service supports
    ///
    /// @param[in] table Table with operations to register
    MOCK_METHOD1(RegisterServer, bool(const std::vector<ServerIpCommandBusService> &table));

    MOCK_METHOD2(sendMessage, void(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD1(broadcastMessage, void(const Message &msg));

    MOCK_METHOD4(SendGenericRequest,
                 void(const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD4(SendGenericSetRequestNoReturn,
                 void(const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD5(SendGenericResponse,
                 void(const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      const VccIpCmd::SenderHandleId senderHandleId,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    // Google mock does (currently) not support mocking of template methods.
    // The only (known) workaround is to mock method separately pe- template-type instantiation...

    // TODO: Try with one send function and separate the actual work into a template sub class...
    //       (It is possible to mock template classes but not template methods.)

    MOCK_METHOD7(SendSetRequestWlanMode,
                 void(Icb_OpWLANMode_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpWLANMode_SetRequest,
                                               ASN_Stream),               // Icb_OpWLANMode_SetRequest_Encode
                      U32 (*encodedSizeFunc)(Icb_OpWLANMode_SetRequest),  // Icb_OpWLANMode_SetRequest_EncodedSize
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(
        SendSetRequestWlanStaConnect,
        void(Icb_OpWLANSTAConnect_SetRequest pMsg,
             ASN_Result (*encodeFunc)(Icb_OpWLANSTAConnect_SetRequest,
                                      ASN_Stream),                     // Icb_OpWLANSTAConnect_SetRequest_Encode
             U32 (*encodedSizeFunc)(Icb_OpWLANSTAConnect_SetRequest),  // Icb_OpWLANSTAConnect_SetRequest_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(
        SendSetRequestCancelDownload,
        void(Icb_OpCancelDownload_SetRequest pMsg,
             ASN_Result (*encodeFunc)(Icb_OpCancelDownload_SetRequest,
                                      ASN_Stream),                     // Icb_OpCancelDownload_SetRequest_Encode
             U32 (*encodedSizeFunc)(Icb_OpCancelDownload_SetRequest),  // Icb_OpCancelDownload_SetRequest_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(
        SendSetRequestDeletedSoftware,
        void(Icb_OpDeletedSoftware_SetRequest pMsg,
             ASN_Result (*encodeFunc)(Icb_OpDeletedSoftware_SetRequest,
                                      ASN_Stream),                      // Icb_OpDeletedSoftware_SetRequest_Encode
             U32 (*encodedSizeFunc)(Icb_OpDeletedSoftware_SetRequest),  // Icb_OpDeletedSoftware_SetRequest_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestDownloadSoftware,
                 void(Icb_OpDownloadSoftware_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpDownloadSoftware_SetRequest,
                                               ASN_Stream),  // Icb_OpDownloadSoftware_SetRequest_Encode
                      U32 (*encodedSizeFunc)(
                          Icb_OpDownloadSoftware_SetRequest),  // Icb_OpDownloadSoftware_SetRequest_EncodedSize
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(
        SendSetRequestCurrentInternetSource,
        void(Icb_OpCurrentInternetSource_SetRequest pMsg,
             ASN_Result (*encodeFunc)(Icb_OpCurrentInternetSource_SetRequest,
                                      ASN_Stream),  // Icb_OpCurrentInternetSource_SetRequest_Encode
             U32 (*encodedSizeFunc)(
                 Icb_OpCurrentInternetSource_SetRequest),  // Icb_OpCurrentInternetSource_SetRequest_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(
        SendSetRequestCurrentInternetGateway,
        void(Icb_OpInternetGateway_SetRequest pMsg,
             ASN_Result (*encodeFunc)(Icb_OpInternetGateway_SetRequest,
                                      ASN_Stream),                      // Icb_OpInternetGateway_SetRequest_Encode
             U32 (*encodedSizeFunc)(Icb_OpInternetGateway_SetRequest),  // Icb_OpInternetGateway_SetRequest_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestNoReturnOpSoHPacket,
                 void(Icb_OpSoHPacketSend_SetRequestNoReturn pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpSoHPacketSend_SetRequestNoReturn,
                                               ASN_Stream),  // Icb_OpInternetGateway_SetRequest_Encode
                      U32 (*encodedSizeFunc)(Icb_OpSoHPacketSend_SetRequestNoReturn),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpSubscriptionActivation,
                 void(Icb_OpSubscriptionActivation_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpSubscriptionActivation_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpSubscriptionActivation_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpECallSettings,
                 void(Icb_OpEcallSettings_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpEcallSettings_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpEcallSettings_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpTelematicSettings,
                 void(Icb_OpTelematicSettings_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpTelematicSettings_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpTelematicSettings_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpCallHandling,
                 void(Icb_OpCallHandling_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpCallHandling_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpCallHandling_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpServiceActivation,
                 void(Icb_OpServiceActivation_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpServiceActivation_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpServiceActivation_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpPSIMResetDataCounter,
                 void(Icb_OpPSIMResetDataCounter_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpPSIMResetDataCounter_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpPSIMResetDataCounter_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpPSIMEnterCode,
                 void(Icb_OpPSIMEnterCode_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpPSIMEnterCode_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpPSIMEnterCode_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpPSIMPSIMNetworkConfiguration,
                 void(Icb_OpPSIMNetworkConfiguration_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpPSIMNetworkConfiguration_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpPSIMNetworkConfiguration_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpPSIMRoamingConfiguration,
                 void(Icb_OpPSIMRoamingConfiguration_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpPSIMRoamingConfiguration_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpPSIMRoamingConfiguration_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpPSIMSecurity,
                 void(Icb_OpPSIMSecurity_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpPSIMSecurity_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpPSIMSecurity_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpPSIMUssd,
                 void(Icb_OpPSIMUssd_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpPSIMUssd_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpPSIMUssd_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    MOCK_METHOD7(SendSetRequestOpPSIMNetworkSelection,
                 void(Icb_OpPSIMNetworkOperatorCommand_SetRequest pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpPSIMNetworkOperatorCommand_SetRequest, ASN_Stream),
                      U32 (*encodedSizeFunc)(Icb_OpPSIMNetworkOperatorCommand_SetRequest),
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData));

    // Wrapper calling the mock method.
    // Per-template-type-set instantiations follow after the class declaration.
    template <class MsgTypePtr, typename EncodeFunc, typename EncodedSizeFunc>
    void SendSetRequest(MsgTypePtr pMsg,
                        EncodeFunc encodeFunc,
                        EncodedSizeFunc encodedSizeFunc,
                        const VccIpCmd::ServiceId serviceId,
                        const VccIpCmd::OperationId operationId,
                        const Message::Ecu ecu,
                        std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    // Wrapper calling the mock method.
    // Per-template-type-set instantiations follow after the class declaration.
    template <class MsgTypePtr, typename EncodeFunc, typename EncodedSizeFunc>
    void SendSetRequestNoReturn(MsgTypePtr pMsg,
                                EncodeFunc encodeFunc,
                                EncodedSizeFunc encodedSizeFunc,
                                const VccIpCmd::ServiceId serviceId,
                                const VccIpCmd::OperationId operationId,
                                const Message::Ecu ecu,
                                std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    // Per-template-type-set instantiations follow after the class declaration.
    template <class MsgTypePtr, typename EncodeFunc, typename EncodedSizeFunc>
    void SendResponse(MsgTypePtr pMsg,
                      EncodeFunc encodeFunc,
                      EncodedSizeFunc encodedSizeFunc,
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      const VccIpCmd::SenderHandleId senderHandleId);

    MOCK_METHOD7(
        SendResponseDeviceInstall,
        void(Icb_OpDeviceInstall_Response pMsg,
             ASN_Result (*encodeFunc)(Icb_OpDeviceInstall_Response,
                                      ASN_Stream),                  // Icb_OpDownloadSoftware_SetRequest_Encode
             U32 (*encodedSizeFunc)(Icb_OpDeviceInstall_Response),  // Icb_OpDownloadSoftware_SetRequest_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             const VccIpCmd::SenderHandleId senderHandleId));

    MOCK_METHOD7(SendResponseLocalStorage,
                 void(Icb_OpLocalStorage_Response pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpLocalStorage_Response,
                                               ASN_Stream),                 // Icb_OpLocalStorage_Response_Encode
                      U32 (*encodedSizeFunc)(Icb_OpLocalStorage_Response),  // Icb_OpLocalStorage_Response_EncodedSize
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      const VccIpCmd::SenderHandleId senderHandleId));

    MOCK_METHOD7(SendResponseInternetSourceCtrlUSB,
                 void(Icb_OpInternetSourceCtrlUSB_Response pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpInternetSourceCtrlUSB_Response,
                                               ASN_Stream),  // Icb_OpInternetSourceCtrlUSB_Response_Encode
                      U32 (*encodedSizeFunc)(
                          Icb_OpInternetSourceCtrlUSB_Response),  // Icb_OpInternetSourceCtrlUSB_Response_EncodedSize
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      const VccIpCmd::SenderHandleId senderHandleId));

    MOCK_METHOD7(SendResponseInternetSourceCtrlBT,
                 void(Icb_OpInternetSourceCtrlBT_Response pMsg,
                      ASN_Result (*encodeFunc)(Icb_OpInternetSourceCtrlBT_Response,
                                               ASN_Stream),  // Icb_OpInternetSourceCtrlBT_Response_Encode
                      U32 (*encodedSizeFunc)(
                          Icb_OpInternetSourceCtrlBT_Response),  // Icb_OpInternetSourceCtrlBT_Response_EncodedSize
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      const VccIpCmd::SenderHandleId senderHandleId));

    MOCK_METHOD7(
        SendResponseInternetGateway,
        void(Icb_OpInternetGateway_Response pMsg,
             ASN_Result (*encodeFunc)(Icb_OpInternetGateway_Response,
                                      ASN_Stream),                    // Icb_OpInternetGateway_Response_Encode
             U32 (*encodedSizeFunc)(Icb_OpInternetGateway_Response),  // Icb_OpInternetGateway_Response_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             const VccIpCmd::SenderHandleId senderHandleId));

    MOCK_METHOD7(
        SendPremiumAudioResponse,
        void(Icb_OpPremiumAudio_Response pMsg,
             ASN_Result (*encodeFunc)(Icb_OpPremiumAudio_Response,
                                      ASN_Stream),                 // Icb_OpInternetGateway_Response_Encode
             U32 (*encodedSizeFunc)(Icb_OpPremiumAudio_Response),  // Icb_OpInternetGateway_Response_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             const VccIpCmd::SenderHandleId senderHandleId));

    MOCK_METHOD5(SendGenericError,
                 void(Icb_OpGeneric_Error pMsg,
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const Message::Ecu ecu,
                      const VccIpCmd::SenderHandleId senderHandleId));

    MOCK_METHOD7(
        SendResponseGenericSetting,
        void(Icb_OpGenericSettingSynch_Response pMsg,
             ASN_Result (*encodeFunc)(Icb_OpGenericSettingSynch_Response,
                                      ASN_Stream),                        // Icb_OpInternetGateway_Response_Encode
             U32 (*encodedSizeFunc)(Icb_OpGenericSettingSynch_Response),  // Icb_OpInternetGateway_Response_EncodedSize
             const VccIpCmd::ServiceId serviceId,
             const VccIpCmd::OperationId operationId,
             const Message::Ecu ecu,
             const VccIpCmd::SenderHandleId senderHandleId));

    // Wrapper calling the mock method.
    // Per-template-type-set instantiations follow after the class declaration.
    template <class MsgTypePtr, typename CreateFunc, typename DecodeFunc>
    bool DecodeResponse(Message &encodedMsg,
                        std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                        MsgTypePtr &pDecodedMsg,
                        CreateFunc createFunc,
                        DecodeFunc decodeFunc);

    MOCK_METHOD5(
        DecodeResponseWlanStaDisconnect,
        bool(Message &encodedMsg,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
             Icb_OpWLANSTADisconnect_Response &pDecodedMsg,
             Icb_OpWLANSTADisconnect_Response (*createFunc)(ASN_Session session),  // Icb_OpWLANSTADisconnect_Response
             ASN_Result (*decodeFunc)(Icb_OpWLANSTADisconnect_Response,
                                      ASN_Session,
                                      ASN_Stream)  // Icb_OpWLANSTADisconnect_Response_Decode
             ));

    MOCK_METHOD5(
        DecodeResponseWlanStaConnect,
        bool(Message &encodedMsg,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
             Icb_OpWLANSTAConnect_Response &pDecodedMsg,
             Icb_OpWLANSTAConnect_Response (*createFunc)(ASN_Session session),  // Icb_OpWLANSTAConnect_Response
             ASN_Result (*decodeFunc)(Icb_OpWLANSTAConnect_Response,
                                      ASN_Session,
                                      ASN_Stream)  // Icb_OpWLANSTAConnect_Response_Decode
             ));

    MOCK_METHOD5(
        DecodeResponseWlanStaStatus,
        bool(Message &encodedMsg,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
             Icb_OpWLANSTAStatus_Response &pDecodedMsg,
             Icb_OpWLANSTAStatus_Response (*createFunc)(ASN_Session session),  // Icb_OpWLANSTAStatus_Response_Create
             ASN_Result (*decodeFunc)(Icb_OpWLANSTAStatus_Response,
                                      ASN_Session,
                                      ASN_Stream)  // Icb_OpWLANSTAStatus_Response_Decode
             ));

    MOCK_METHOD5(DecodeResponseWlanMode,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpWLANMode_Response &pDecodedMsg,
                      Icb_OpWLANMode_Response (*createFunc)(ASN_Session session),  // Icb_OpWLANMode_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpWLANMode_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpWLANMode_Response_Decode
                      ));

    MOCK_METHOD5(DecodeResponseWlanStaRememberedNetwks,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpWLANSTARememberedNetwks_Response &pDecodedMsg,
                      Icb_OpWLANSTARememberedNetwks_Response (*createFunc)(
                          ASN_Session session),  // Icb_OpWLANSTARememberedNetwks_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpWLANSTARememberedNetwks_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpWLANSTARememberedNetwks_Response_Decode
                      ));

    MOCK_METHOD5(DecodeResponseWlanStaForgetRememberedNetwkResponse,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpWLANSTAForgetRememberedNetwk_Response &pDecodedMsg,
                      Icb_OpWLANSTAForgetRememberedNetwk_Response (*createFunc)(
                          ASN_Session session),  // Icb_OpWLANSTAForgetRememberedNetwk_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpWLANSTAForgetRememberedNetwk_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpWLANSTAForgetRememberedNetwk_Response_Decode
                      ));

    MOCK_METHOD5(
        DecodeResponseDownloadStatus,
        bool(Message &encodedMsg,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
             Icb_OpDownloadStatus_Response &pDecodedMsg,
             Icb_OpDownloadStatus_Response (*createFunc)(ASN_Session session),  // Icb_OpDownloadStatus_Response_Create
             ASN_Result (*decodeFunc)(Icb_OpDownloadStatus_Response,
                                      ASN_Session,
                                      ASN_Stream)  // Icb_OpDownloadStatus_Response_Decode
             ));

    MOCK_METHOD5(
        DecodeResponseCancelDownload,
        bool(Message &encodedMsg,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
             Icb_OpCancelDownload_Response &pDecodedMsg,
             Icb_OpCancelDownload_Response (*createFunc)(ASN_Session session),  // Icb_OpCancelDownload_Response_Create
             ASN_Result (*decodeFunc)(Icb_OpCancelDownload_Response,
                                      ASN_Session,
                                      ASN_Stream)  // Icb_OpCancelDownload_Response_Decode
             ));

    MOCK_METHOD5(DecodeResponseCurrentInternetSource,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpCurrentInternetSource_Response &pDecodedMsg,
                      Icb_OpCurrentInternetSource_Response (*createFunc)(
                          ASN_Session session),  // Icb_OpCurrentInternetSource_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpCurrentInternetSource_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpCurrentInternetSource_Response_Decode
                      ));

    MOCK_METHOD5(DecodeResponseCurrentInternetGateway,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpInternetGateway_Response &pDecodedMsg,
                      Icb_OpInternetGateway_Response (*createFunc)(
                          ASN_Session session),  // Icb_OpInternetGateway_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpInternetGateway_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpInternetGateway_Response_Decode
                      ));

    MOCK_METHOD5(DecodeResponseConnectivityStatus,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpConnectivityStatus_Response &pDecodedMsg,
                      Icb_OpConnectivityStatus_Response (*createFunc)(
                          ASN_Session session),  // Icb_OpConnectivityStatus_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpConnectivityStatus_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpConnectivityStatus_Response_Decode
                      ));

    MOCK_METHOD5(DecodeResponseSubscriptionActivation,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpSubscriptionActivation_Response &pDecodedMsg,
                      Icb_OpSubscriptionActivation_Response (*createFunc)(
                          ASN_Session session),  // Icb_OpSubscriptionActivation_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpSubscriptionActivation_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpSubscriptionActivation_Response_Decode
                      ));

    MOCK_METHOD5(
        DecodeResponsePositionData,
        bool(Message &encodedMsg,
             std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
             Icb_OpPositionData_Response &pDecodedMsg,
             Icb_OpPositionData_Response (*createFunc)(ASN_Session session),  // Icb_OpPositionData_Response_Create
             ASN_Result (*decodeFunc)(Icb_OpPositionData_Response,
                                      ASN_Session,
                                      ASN_Stream)  // Icb_OpPositionData_Response_Decode
             ));

    MOCK_METHOD5(DecodeResponsePSIMConnectionStatus,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMConnectionStatus_Response &pDecodedMsg,
                      Icb_OpPSIMConnectionStatus_Response (*createFunc)(
                          ASN_Session session),  // Icb_OpPSIMConnectionStatus_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpPSIMConnectionStatus_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpPSIMConnectionStatus_Response_Decode
                      ));

    MOCK_METHOD5(DecodeResponsePSIMState,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMState_Response &pDecodedMsg,
                      Icb_OpPSIMState_Response (*createFunc)(ASN_Session session),  // Icb_OpPSIMState_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpPSIMState_Response,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpPSIMState_Response_Decode
                      ));

    MOCK_METHOD5(DecodeResponsePSIMRegisterState,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMRegisterState_Response &pDecodedMsg,
                      Icb_OpPSIMRegisterState_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMRegisterState_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponsePSIMDataCounter,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMDataCounter_Response &pDecodedMsg,
                      Icb_OpPSIMDataCounter_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMDataCounter_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponsePSIMResetDataCounter,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMResetDataCounter_Response &pDecodedMsg,
                      Icb_OpPSIMResetDataCounter_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMResetDataCounter_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponsePSIMUssd,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMUssd_Response &pDecodedMsg,
                      Icb_OpPSIMUssd_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMUssd_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponseServiceActivation,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpServiceActivation_Response &pDecodedMsg,
                      Icb_OpServiceActivation_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpServiceActivation_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponseSVTStatus,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpSVTStatus_Response &pDecodedMsg,
                      Icb_OpSVTStatus_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpSVTStatus_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponseTNStatus,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpTNStatus_Response &pDecodedMsg,
                      Icb_OpTNStatus_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpTNStatus_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponsePSIMNetworkConfiguration,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMNetworkConfiguration_Response &pDecodedMsg,
                      Icb_OpPSIMNetworkConfiguration_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkConfiguration_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponsePSIMRoamingConfiguration,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMRoamingConfiguration_Response &pDecodedMsg,
                      Icb_OpPSIMRoamingConfiguration_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMRoamingConfiguration_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponsePSIMSecurity,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMSecurity_Response &pDecodedMsg,
                      Icb_OpPSIMSecurity_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMSecurity_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponseAccessRightPinCode,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpAccessRightPinCode_Response &pDecodedMsg,
                      Icb_OpAccessRightPinCode_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpAccessRightPinCode_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponsePSIMNetworkScan,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMNetworkOperators_Response &pDecodedMsg,
                      Icb_OpPSIMNetworkOperators_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkOperators_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponsePSIMNetworkSelection,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpPSIMNetworkOperatorCommand_Response &pDecodedMsg,
                      Icb_OpPSIMNetworkOperatorCommand_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkOperatorCommand_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponseTelematicSetting,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpTelematicSettings_Response &pDecodedMsg,
                      Icb_OpTelematicSettings_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpTelematicSettings_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponseECallSettings,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpEcallSettings_Response &pDecodedMsg,
                      Icb_OpEcallSettings_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpEcallSettings_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD5(DecodeResponseFactoryDefaultRestore,
                 bool(Message &encodedMsg,
                      std::shared_ptr<MessageDispatcher::CallerData> pCallerData,
                      Icb_OpFactoryDefaultRestore_Response &pDecodedMsg,
                      Icb_OpFactoryDefaultRestore_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpFactoryDefaultRestore_Response, ASN_Session, ASN_Stream)));

    // Wrapper calling the mock method.
    // Per-template-type-set instantiations follow after the class declaration.
    template <class MsgTypePtr, typename CreateFunc, typename DecodeFunc>
    bool DecodeMessage(Message &encodedMsg, MsgTypePtr &pDecodedMsg, CreateFunc createFunc, DecodeFunc decodeFunc);

    MOCK_METHOD4(
        DecodeMessageGnssPositionDataResponse,
        bool(Message &encodedMsg,
             Icb_OpGNSSPositionData_Response &pDecodedMsg,
             Icb_OpGNSSPositionData_Response (*createFunc)(ASN_Session),  // Icb_OpGNSSPositionData_Response_Create
             ASN_Result (*decodeFunc)(Icb_OpGNSSPositionData_Response,
                                      ASN_Session,
                                      ASN_Stream)  //  Icb_OpGNSSPositionData_Response_Decode
             ));

    MOCK_METHOD4(DecodeMessageWlanStaConnect,
                 bool(Message &encodedMsg,
                      Icb_OpWLANSTAConnect_Response &pDecodedMsg,
                      Icb_OpWLANSTAConnect_Response (*createFunc)(ASN_Session),  // Icb_OpWLANSTAConnect_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpWLANSTAConnect_Response,
                                               ASN_Session,
                                               ASN_Stream)  //  Icb_OpWLANSTAConnect_Response_Decode
                      ));

    MOCK_METHOD4(
        DecodeMessageWlanStaStatus,
        bool(Message &encodedMsg,
             Icb_OpWLANSTAStatus_Notification &pDecodedMsg,
             Icb_OpWLANSTAStatus_Notification (*createFunc)(ASN_Session),  // Icb_OpWLANSTAStatus_Notification_Create
             ASN_Result (*decodeFunc)(Icb_OpWLANSTAStatus_Notification,
                                      ASN_Session,
                                      ASN_Stream)  //  Icb_OpWLANSTAStatus_Notification_Decode
             ));

    MOCK_METHOD4(DecodeMessageWlanMode,
                 bool(Message &encodedMsg,
                      Icb_OpWLANMode_Notification &pDecodedMsg,
                      Icb_OpWLANMode_Notification (*createFunc)(ASN_Session),  // Icb_OpWLANMode_Notification_Create
                      ASN_Result (*decodeFunc)(Icb_OpWLANMode_Notification,
                                               ASN_Session,
                                               ASN_Stream)  //  Icb_OpWLANMode_Notification_Decode
                      ));

    MOCK_METHOD4(DecodeMessageWlanStaForgetRememberedNetwkResponse,
                 bool(Message &encodedMsg,
                      Icb_OpWLANSTAForgetRememberedNetwk_Response &pDecodedMsg,
                      Icb_OpWLANSTAForgetRememberedNetwk_Response (*createFunc)(
                          ASN_Session),  // Icb_OpWLANSTAForgetRememberedNetwk_Response_Create
                      ASN_Result (*decodeFunc)(Icb_OpWLANSTAForgetRememberedNetwk_Response,
                                               ASN_Session,
                                               ASN_Stream)  //  Icb_OpWLANSTAForgetRememberedNetwk_Response_Decode
                      ));

    MOCK_METHOD4(DecodeMessageWlanChannelFreqInfo,
                 bool(Message &encodedMsg,
                      Icb_OpWLANChannelFreqInfo_Notification &pDecodedMsg,
                      Icb_OpWLANChannelFreqInfo_Notification (*createFunc)(
                          ASN_Session),  // Icb_OpWLANChannelFreqInfo_Notification_Create
                      ASN_Result (*decodeFunc)(Icb_OpWLANChannelFreqInfo_Notification,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpWLANChannelFreqInfo_Notification_Decode
                      ));

    MOCK_METHOD4(
        DecodeMessageDownloadStatus,
        bool(Message &encodedMsg,
             Icb_OpDownloadStatus_Response &pDecodedMsg,
             Icb_OpDownloadStatus_Response (*createFunc)(ASN_Session session),  // Icb_OpDownloadStatus_Response_Create
             ASN_Result (*decodeFunc)(Icb_OpDownloadStatus_Response,
                                      ASN_Session,
                                      ASN_Stream)  // Icb_OpDownloadStatus_Response_Decode
             ));

    MOCK_METHOD4(DecodeMessageCurrentInternetSource,
                 bool(Message &encodedMsg,
                      Icb_OpCurrentInternetSource_Notification &pDecodedMsg,
                      Icb_OpCurrentInternetSource_Notification (*createFunc)(
                          ASN_Session session),  // Icb_OpCurrentInternetSource_Notification_Create
                      ASN_Result (*decodeFunc)(Icb_OpCurrentInternetSource_Notification,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpCurrentInternetSource_Notification_Decode
                      ));

    MOCK_METHOD4(DecodeMessageConnectivityStatus,
                 bool(Message &encodedMsg,
                      Icb_OpConnectivityStatus_Notification &pDecodedMsg,
                      Icb_OpConnectivityStatus_Notification (*createFunc)(
                          ASN_Session session),  // Icb_OpConnectivityStatus_Notification_Create
                      ASN_Result (*decodeFunc)(Icb_OpConnectivityStatus_Notification,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpConnectivityStatus_Notification_Decode
                      ));

    MOCK_METHOD4(
        DecodeMessagePSIMNetworkSelection,
        bool(Message &encodedMsg,
             Icb_OpPSIMNetworkOperatorCommand_Notification &pDecodedMsg,
             Icb_OpPSIMNetworkOperatorCommand_Notification (*createFunc)(ASN_Session session),
             ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkOperatorCommand_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageInternetSourceCtrlBT,
                 bool(Message &encodedMsg,
                      Icb_OpInternetSourceCtrlBT_SetRequest &pDecodedMsg,
                      Icb_OpInternetSourceCtrlBT_SetRequest (*createFunc)(
                          ASN_Session session),  // Icb_OpInternetSourceCtrlBT_SetRequest_Create
                      ASN_Result (*decodeFunc)(Icb_OpInternetSourceCtrlBT_SetRequest,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpInternetSourceCtrlBT_SetRequest_Decode
                      ));

    MOCK_METHOD4(DecodeMessageInternetSourceCtrlUSB,
                 bool(Message &encodedMsg,
                      Icb_OpInternetSourceCtrlUSB_SetRequest &pDecodedMsg,
                      Icb_OpInternetSourceCtrlUSB_SetRequest (*createFunc)(
                          ASN_Session session),  // Icb_OpInternetSourceCtrlUSB_SetRequest_Create
                      ASN_Result (*decodeFunc)(Icb_OpInternetSourceCtrlUSB_SetRequest,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpInternetSourceCtrlUSB_SetRequest_Decode
                      ));

    MOCK_METHOD4(DecodeMessageInternetGateway,
                 bool(Message &encodedMsg,
                      Icb_OpInternetGateway_SetRequest &pDecodedMsg,
                      Icb_OpInternetGateway_SetRequest (*createFunc)(
                          ASN_Session session),  // Icb_OpInternetGateway_SetRequest_Create
                      ASN_Result (*decodeFunc)(Icb_OpInternetGateway_SetRequest,
                                               ASN_Session,
                                               ASN_Stream)  // Icb_OpInternetGateway_SetRequest_Decode
                      ));

    MOCK_METHOD4(DecodeMessageOpSoHPacketSend,
                 bool(Message &encodedMsg,
                      Icb_OpSoHPackageUploaded_Notification &pDecodedMsg,
                      Icb_OpSoHPackageUploaded_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpSoHPackageUploaded_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageOpSubscriptionActivationNotification,
                 bool(Message &encodedMsg,
                      Icb_OpSubscriptionActivation_Notification &pDecodedMsg,
                      Icb_OpSubscriptionActivation_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpSubscriptionActivation_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageOpRescueStatusNotification,
                 bool(Message &encodedMsg,
                      Icb_OpRescueStatus_Notification &pDecodedMsg,
                      Icb_OpRescueStatus_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpRescueStatus_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageOpSVTStatusNotification,
                 bool(Message &encodedMsg,
                      Icb_OpSVTStatus_Notification &pDecodedMsg,
                      Icb_OpSVTStatus_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpSVTStatus_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageOpTNStatusNotification,
                 bool(Message &encodedMsg,
                      Icb_OpTNStatus_Notification &pDecodedMsg,
                      Icb_OpTNStatus_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpTNStatus_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessagePSIMConnectionStatusNotification,
                 bool(Message &encodedMsg,
                      Icb_OpPSIMConnectionStatus_Notification &pDecodedMsg,
                      Icb_OpPSIMConnectionStatus_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMConnectionStatus_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessagePSIMRegisterState,
                 bool(Message &encodedMsg,
                      Icb_OpPSIMRegisterState_Response &pDecodedMsg,
                      Icb_OpPSIMRegisterState_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMRegisterState_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessagePSIMState,
                 bool(Message &encodedMsg,
                      Icb_OpPSIMState_Response &pDecodedMsg,
                      Icb_OpPSIMState_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMState_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessagePSIMSignalStrengthNotification,
                 bool(Message &encodedMsg,
                      Icb_OpPSIMSignalStrength_Notification &pDecodedMsg,
                      Icb_OpPSIMSignalStrength_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMSignalStrength_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessagePSIMNetworkConfigurationResponse,
                 bool(Message &encodedMsg,
                      Icb_OpPSIMNetworkConfiguration_Response &pDecodedMsg,
                      Icb_OpPSIMNetworkConfiguration_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMNetworkConfiguration_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessagePSIMUssd,
                 bool(Message &encodedMsg,
                      Icb_OpPSIMUssd_Response &pDecodedMsg,
                      Icb_OpPSIMUssd_Response (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMUssd_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageTextMessageSetRequest,
                 bool(Message &encodedMsg,
                      Icb_OpTextMessage_SetRequest &pDecodedMsg,
                      Icb_OpTextMessage_SetRequest (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpTextMessage_SetRequest, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessagePremiumAudioSetRequest,
                 bool(Message &encodedMsg,
                      Icb_OpPremiumAudio_SetRequest &pDecodedMsg,
                      Icb_OpPremiumAudio_SetRequest (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpPremiumAudio_SetRequest, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageConnectWorkshopWlanNotification,
                 bool(Message &encodedMsg,
                      Icb_OpConnectWorkshopWlan_Notification &pDecodedMsg,
                      Icb_OpConnectWorkshopWlan_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpConnectWorkshopWlan_Notification, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageWorkshopWlanDetectedNotification,
                 bool(Message &encodedMsg,
                      Icb_OpWorkshopWlanDetected_Notification &pDecodedMsg,
                      Icb_OpWorkshopWlanDetected_Notification (*createFunc)(ASN_Session session),
                      ASN_Result (*decodeFunc)(Icb_OpWorkshopWlanDetected_Notification, ASN_Session, ASN_Stream)));

    // Note: There is by design no 'registerErrorCallback'.
    //       Errors associated with earlier sent requests are returned trough response callback.
    //       (Errors associated with earlier sent responses are currently not handled above TransportServices level...)

    /*
    // Needs to be mocked separately per template type instantiation...
    template <class MsgTypePtr, typename EncodeFunc, typename EncodedSizeFunc>
    void SendError(MsgTypePtr pMsg,
                   EncodeFunc encodeFunc,
                   EncodedSizeFunc encodedSizeFunc,
                   const VccIpCmd::ServiceId serviceId,
                   const VccIpCmd::OperationId operationId,
                   const Message::Ecu ecu,
                   const uint32_t senderHandleId);
    */

    MOCK_METHOD0(getNewSequenceId, uint8_t());

    MOCK_METHOD6(PreparePduHeader,
                 void(Pdu &pdu,
                      const VccIpCmd::ServiceId serviceId,
                      const VccIpCmd::OperationId operationId,
                      const VccIpCmd::OperationType operationType,
                      const VccIpCmd::DataType dataType,
                      const VccIpCmd::SenderHandleId senderHandleId));

    MOCK_METHOD4(DecodeMessageDeviceInstallSetRequest,
                 bool(Message &encodedMsg,
                      Icb_OpDeviceInstall_SetRequest &pDecodedMsg,
                      Icb_OpDeviceInstall_SetRequest (*createFunc)(ASN_Session),
                      ASN_Result (*decodeFunc)(Icb_OpDeviceInstall_SetRequest, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageLocalStorageRequest,
                 bool(Message &encodedMsg,
                      Icb_OpLocalStorage_Response &pDecodedMsg,
                      Icb_OpLocalStorage_Response (*createFunc)(ASN_Session),
                      ASN_Result (*decodeFunc)(Icb_OpLocalStorage_Response, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageGenericSettingRequest,
                 bool(Message &encodedMsg,
                      Icb_OpGenericSettingSynch_Request &pDecodedMsg,
                      Icb_OpGenericSettingSynch_Request (*createFunc)(ASN_Session),
                      ASN_Result (*decodeFunc)(Icb_OpGenericSettingSynch_Request, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessageGenericSettingSetRequest,
                 bool(Message &encodedMsg,
                      Icb_OpGenericSettingSynch_SetRequest &pDecodedMsg,
                      Icb_OpGenericSettingSynch_SetRequest (*createFunc)(ASN_Session),
                      ASN_Result (*decodeFunc)(Icb_OpGenericSettingSynch_SetRequest, ASN_Session, ASN_Stream)));

    MOCK_METHOD4(DecodeMessagePSIMMessageSetRequest,
                 bool(Message &encodedMsg,
                      Icb_OpPSIMMessage_SetRequest &pDecodedMsg,
                      Icb_OpPSIMMessage_SetRequest (*createFunc)(ASN_Session),
                      ASN_Result (*decodeFunc)(Icb_OpPSIMMessage_SetRequest, ASN_Session, ASN_Stream)));
};

}  // Connectivity

#endif  // MOCK_IP_SERVICE_H
