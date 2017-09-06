/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "Infotainment_IP_Service.conn"

#include <assert.h>
#include <memory>
#include <cutils/log.h>

#include "Connectivity.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/net_serializer.h"

// TODO!! Ensure that correct intervals are used for response timeouts in both directions...
//        Example: CurrentInternetSource:
//        'This signal must implement the following specific WFR timerout. WFR timeout 5800 ms.
//         Note: The signal specific WFR timeout value must be adjustable in Local Config.'
//        Would guess that there are default intervals specified for all operations in both directions. Check spec...

namespace Connectivity
{

void ConnectivityX::cbCurrentInetSourceResponse(Message &msg,
                                                std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    ALOGD("cbCurrentInetSourceResponse %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());
    assert(pCallerData);  // Should not get called when originating request is unknown.

    Icb_OpCurrentInternetSource_Response p;

    Icb_InternetSource_t currentInternetSource = e_Icb_InternetSource_none;

    if (DecodeResponse(msg,
                       pCallerData,
                       p,
                       Icb_OpCurrentInternetSource_Response_Create,
                       Icb_OpCurrentInternetSource_Response_Decode))
    {
        // All ok. Fill in the service specific data to return.
        currentInternetSource = static_cast<Icb_InternetSource_t>(p->currentinternetsource);
        ALOGD(
            "Current internet source: %s (%d)", InetSourceToString(currentInternetSource), currentInternetSource);
    }

    // TODO(johan, 160412) Propagate pCallerData->errorType in callback so upper layers
    // Tell listener that we have a new response from VCM (or error/timeout).

    //Disabled to be able to build (abengt34)
    std::shared_ptr<CurrentInetSourceRespCallback> pCallback = nullptr;
        //std::dynamic_pointer_cast<CurrentInetSourceRespCallback>(pCallerData);
    if (pCallback && pCallback->callback)
    {
        pCallback->callback(currentInternetSource, pCallback);
    }
    else
    {
        ALOGW("Late response on %04X.%04X.%02d 0x%08X",
                                (int)msg.pdu.header.service_id,
                                (int)msg.pdu.header.operation_id,
                                (int)msg.pdu.header.operation_type,
                                (int)msg.pdu.header.sender_handle_id);
    }
}

void ConnectivityX::cbCurrentInetSourceNotification(Message &msg)
{
    Icb_OpCurrentInternetSource_Notification p = nullptr;

    ALOGD("cbCurrentInetSourceNotification %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    if (DecodeMessage(
            msg, p, Icb_OpCurrentInternetSource_Notification_Create, Icb_OpCurrentInternetSource_Notification_Decode))
    {
        // All ok. Fill in the service specific data.
        const Icb_InternetSource_t currentInternetSource = static_cast<Icb_InternetSource_t>(p->currentinternetsource);

        ALOGD(
            "Current internet source: %s (%d)", InetSourceToString(currentInternetSource), currentInternetSource);

        // Notify listener (if one exists) that we have a new internet source event from VCM.
        m_pListener->NotifyCurrentInternetSource(currentInternetSource);
    }
}

void ConnectivityX::SetInternetSource(Icb_InternetSource_t src,
                                      std::shared_ptr<CurrentInetSourceRespCallback> pCallback)
{
    // Note: This function is called from CommonAPI thread.
    //       Currently not needing lock since data is not shared with functions outside thread
    //       but take care if updating code...

    ALOGD("Setting current internet source to: %s (%d)", InetSourceToString(src), src);

    ASN_Session_Reset(m_session_common_api);
    Icb_OpCurrentInternetSource_SetRequest pMsg = Icb_OpCurrentInternetSource_SetRequest_Create(m_session_common_api);

    pMsg->currentinternetsource = src;

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData =
        std::static_pointer_cast<MessageDispatcher::CallerData>(pCallback);
    assert(pCallerData);

    SendSetRequest(pMsg,
                   Icb_OpCurrentInternetSource_SetRequest_Encode,
                   Icb_OpCurrentInternetSource_SetRequest_EncodedSize,
                   VccIpCmd::ServiceId::Connectivity,
                   VccIpCmd::OperationId::CurrentInternetSource,
                   Message::Ecu::VCM,
                   pCallerData);
}

void ConnectivityX::RequestInternetSource(std::shared_ptr<CurrentInetSourceRespCallback> pCallback)
{
    // Note: This function is called from CommonAPI thread.
    //       Currently not needing lock since data is not shared with functions outside thread
    //       but take care if updating code...

    ALOGD("Sending request for current internet source.");

    SendGenericRequest(
        VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::CurrentInternetSource, Message::Ecu::VCM, pCallback);
}

void ConnectivityX::cbInetGatewayAsClientResponse(Message &msg,
                                                  std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    ALOGD("cbCurrentInetGatewayResponse %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());
    assert(pCallerData);  // Should not get called when originating request is unknown.

    Icb_OpInternetGateway_Response p;

    Icb_Ecu_t gateway = e_Icb_Ecu_none;

    if (DecodeResponse(
            msg, pCallerData, p, Icb_OpInternetGateway_Response_Create, Icb_OpInternetGateway_Response_Decode))
    {
        // All ok. Fill in the service specific data to return.
        gateway = static_cast<Icb_Ecu_t>(p->internetGateway);
        ALOGD("Internet gateway: %s (%d)", EcuToString(gateway), gateway);
        m_inetGatewayAsClient = gateway;
    }

    // TODO(johan, 160412) Propagate pCallerData->errorType in callback so upper layers
    // Tell listener that we have a new response from VCM (or error/timeout).
    //Disabled to be able to build (abengt34)
    //std::shared_ptr<InetGatewayAsClientRespCallback> pCallback =
        //std::dynamic_pointer_cast<InetGatewayAsClientRespCallback>(pCallerData);
    //assert(pCallback);
    //assert(pCallback->callback);
    //pCallback->callback(gateway, pCallback);
    ALOGI("TODO: Fix dynamic cast and re-enable code...");
}

void ConnectivityX::SetInetGatewayAsClient(Icb_Ecu_t ecu, std::shared_ptr<InetGatewayAsClientRespCallback> pCallback)
{
    // Note: This function is called from CommonAPI thread.
    //       Currently not needing lock since data is not shared with functions outside thread
    //       but take care if updating code...

    ALOGD("Setting internet gateway to: %s (%d)", EcuToString(ecu), ecu);

    ASN_Session_Reset(m_session_common_api);
    Icb_OpInternetGateway_SetRequest pMsg = Icb_OpInternetGateway_SetRequest_Create(m_session_common_api);

    pMsg->internetGateway = ecu;

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData =
        std::static_pointer_cast<MessageDispatcher::CallerData>(pCallback);
    assert(pCallerData);

    SendSetRequest(pMsg,
                   Icb_OpInternetGateway_SetRequest_Encode,
                   Icb_OpInternetGateway_SetRequest_EncodedSize,
                   VccIpCmd::ServiceId::Connectivity,
                   VccIpCmd::OperationId::InternetGateway,
                   Message::Ecu::TEM,
                   pCallerData);
}

void ConnectivityX::RequestInetGatewayAsClient(std::shared_ptr<InetGatewayAsClientRespCallback> pCallback)
{
    // Note: This function is called from CommonAPI thread.
    //       Currently not needing lock since data is not shared with functions outside thread
    //       but take care if updating code...

    ALOGD("Sending request for internet gateway.");

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData =
        std::static_pointer_cast<MessageDispatcher::CallerData>(pCallback);
    assert(pCallerData);

    SendGenericRequest(
        VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetGateway, Message::Ecu::TEM, pCallerData);
}

Icb_Ecu_t ConnectivityX::GetInetGatewayAsClient()
{
    const unsigned int i = m_inetGatewayAsClient;
    return static_cast<Icb_Ecu_t>(i);
}

void ConnectivityX::cbConnectivityStatusResponse(Message &msg,
                                                 std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    Icb_OpConnectivityStatus_Response p = nullptr;

    ALOGD("cbConnectivityStatusResponse %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());
    assert(pCallerData);  // Should not get called when originating request is unknown.

    BOOL generalConnectivityAvailable = 0;  // FALSE

    if (DecodeResponse(
            msg, pCallerData, p, Icb_OpConnectivityStatus_Response_Create, Icb_OpConnectivityStatus_Response_Decode))
    {
        // All ok. Fill in the service specific data to return.
        ALOGD("General connectivity available: %u", p->generalConnectivityAvailable);
        generalConnectivityAvailable = p->generalConnectivityAvailable;
        m_generalConnectivityAvailable = p->generalConnectivityAvailable;
    }

    // TODO(johan, 160412) Propagate pCallerData->errorType in callback so upper layers
    // Tell listener that we have a new response from VCM (or error/timeout).
    //Disabled to be able to build (abengt34)
    //std::shared_ptr<ConnectivityStatusRespCallback> pCallback =
      //  std::dynamic_pointer_cast<ConnectivityStatusRespCallback>(pCallerData);
    //assert(pCallback);
    //assert(pCallback->callback);
    //pCallback->callback(static_cast<bool>(generalConnectivityAvailable), pCallback);
    ALOGI("TODO: Fix dynamic cast and re-enable code...");
}

void ConnectivityX::cbConnectivityStatusNotification(Message &msg)
{
    Icb_OpConnectivityStatus_Notification p = nullptr;

    ALOGD("cbConnectivityStatusNotification %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    if (DecodeMessage(
            msg, p, Icb_OpConnectivityStatus_Notification_Create, Icb_OpConnectivityStatus_Notification_Decode))
    {
        // All ok. Fill in the service specific data.
        ALOGD("General connectivity available: %u", p->generalConnectivityAvailable);
        m_generalConnectivityAvailable = p->generalConnectivityAvailable;

        // Notify listener (if one exists) that we have a new internet source event from VCM.
        m_pListener->NotifyConnectivityStatus(static_cast<bool>(p->generalConnectivityAvailable));
    }
}

void ConnectivityX::RequestConnectivityStatus(std::shared_ptr<ConnectivityStatusRespCallback> pCallback)
{
    // Note: This function is called from CommonAPI thread.
    //       Currently not needing lock since data is not shared with functions outside thread
    //       but take care if updating code...

    ALOGD("Sending request for connectivity status.");

    std::shared_ptr<MessageDispatcher::CallerData> pCallerData =
        std::static_pointer_cast<MessageDispatcher::CallerData>(pCallback);
    assert(pCallerData);

    SendGenericRequest(
        VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::ConnectivityStatus, Message::Ecu::VCM, pCallerData);
}

bool ConnectivityX::GetConnectivityStatus()
{
    const bool b = m_generalConnectivityAvailable;
    return static_cast<bool>(b);
}

void ConnectivityX::cbInetSourceCtrlBTReq(Message &msg)
{
    ALOGD("cbInetSourceCtrlBTReq %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    // Note: No point in decoding the payload-less generic request data (and no generated code for it).

    // Notify listener (if one exists) that we have a request.
    // (Listener is supposed to retrieve the requested value and then call our Respond/RespondError function.)
    m_pListener->RequestInternetSourceCtrlBT(msg.ecu, msg.pdu.header.sender_handle_id);
}

void ConnectivityX::cbInetSourceCtrlBTNotificationReq(Message &msg)
{
    ALOGD("cbInetSourceCtrlBTNotificationReq %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    // Add node to subscriber list.
    {
        std::lock_guard<std::mutex> guard(m_btSubscriberMutex);
        m_inetSourceCtrlBtSubscribers.push_back(NotificationSubscriber(msg));
    }

    // A notification with latest status must be sent at once!
    // ...TODO!! Discuss how to ensure that we have a correct value if executing soon after startup.
    //           Put requirements on CommonAPI stub implementation?
    //           Remove direct send from here and use 'TriggerNotification' callback instead?
    //           Other alternatives?...

    const Icb_OnOffSetting onOff = m_bluetoothOnOff;
    ALOGD("Reporting back bluetooth on off status: %u", onOff);

    ASN_Session_Reset(m_session_msgd);
    Icb_OpInternetSourceCtrlBT_Notification pMsg = Icb_OpInternetSourceCtrlBT_Notification_Create(m_session_msgd);

    pMsg->onOff = onOff;

    SendEncodedMsg(pMsg,
                   Icb_OpInternetSourceCtrlBT_Notification_Encode,
                   Icb_OpInternetSourceCtrlBT_Notification_EncodedSize,
                   VccIpCmd::ServiceId::Connectivity,
                   VccIpCmd::OperationId::InternetSourceCtrlBT,
                   VccIpCmd::OperationType::NOTIFICATION,
                   msg.ecu,
                   getNewSequenceId());
}

void ConnectivityX::NotifyInternetSourceBT(Icb_OnOffSetting onOff)
{
    ALOGD("Bluetooth on/off status updated to: %u", onOff);

    // NOTE! We are called from CommonAPI's thread here so must
    //       ensure thread safe access to forwarded data!
    //       (In this case only needing one parameter which is atomic.)
    {
        // std::lock_guard<std::mutex> guard(m_btDataMutex);
        m_bluetoothOnOff = onOff;
    }

    // Send notifications to subscribing nodes on ip command bus.
    ASN_Session_Reset(m_session_common_api);
    Icb_OpInternetSourceCtrlBT_Notification pMsg = Icb_OpInternetSourceCtrlBT_Notification_Create(m_session_common_api);
    pMsg->onOff = onOff;

    std::lock_guard<std::mutex> guard(m_btSubscriberMutex);
    for (NotificationSubscriber &subscriber : m_inetSourceCtrlBtSubscribers)
    {
        SendEncodedMsg(pMsg,
                       Icb_OpInternetSourceCtrlBT_Notification_Encode,
                       Icb_OpInternetSourceCtrlBT_Notification_EncodedSize,
                       VccIpCmd::ServiceId::Connectivity,
                       VccIpCmd::OperationId::InternetSourceCtrlBT,
                       VccIpCmd::OperationType::NOTIFICATION,
                       subscriber.ecu,
                       getNewSequenceId());
    }
}

void ConnectivityX::RespondInternetSourceBT(const Icb_OnOffSetting onOff,
                                            const Message::Ecu ecu,
                                            const VccIpCmd::SenderHandleId senderHandleId)
{
    ALOGD("Responding on/off status: %u", onOff);

    // NOTE! We are called from CommonAPI's thread here so must
    //       ensure thread safe access to forwarded data!
    //       (In this case only needing one parameter which is atomic.)
    {
        // std::lock_guard<std::mutex> guard(m_btDataMutex);
        m_bluetoothOnOff = onOff;
    }

    // Send notifications to subscribing nodes on ip command bus.
    ASN_Session_Reset(m_session_common_api);
    Icb_OpInternetSourceCtrlBT_Response pMsg = Icb_OpInternetSourceCtrlBT_Response_Create(m_session_common_api);
    pMsg->onOff = onOff;

    SendResponse(pMsg,
                 Icb_OpInternetSourceCtrlBT_Response_Encode,
                 Icb_OpInternetSourceCtrlBT_Response_EncodedSize,
                 VccIpCmd::ServiceId::Connectivity,
                 VccIpCmd::OperationId::InternetSourceCtrlBT,
                 ecu,
                 senderHandleId);
}

void ConnectivityX::RespondErrorInternetSourceBT(const Icb_OpGeneric_Error_t error,
                                                 const Message::Ecu ecu,
                                                 const VccIpCmd::SenderHandleId senderHandleId)
{
    ALOGD("Responding error code: %u", error.errorCode);

    // Send notifications to subscribing nodes on ip command bus.
    ASN_Session_Reset(m_session_common_api);
    Icb_OpGeneric_Error pMsg = Icb_OpGeneric_Error_Create(m_session_common_api);

    *pMsg = error;

    SendGenericError(
        pMsg, VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetSourceCtrlBT, ecu, senderHandleId);
}

void ConnectivityX::cbInetSourceCtrlBTSetReq(Message &msg)
{
    Icb_OpInternetSourceCtrlBT_SetRequest p = nullptr;

    ALOGD("cbInetSourceCtrlBTSetReq %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    if (DecodeMessage(
            msg, p, Icb_OpInternetSourceCtrlBT_SetRequest_Create, Icb_OpInternetSourceCtrlBT_SetRequest_Decode))
    {
        // All ok. Get the service specific data and call listener.
        ALOGD("Requested change of bluetooth on/off to: %u", p->onOff);
        Icb_OnOffSetting_t convertedOnOff = static_cast<Icb_OnOffSetting_t>(p->onOff);
        m_pListener->SetInternetSourceCtrlBT(convertedOnOff, msg.ecu, msg.pdu.header.sender_handle_id);
    }
}

void ConnectivityX::cbInetSourceCtrlUSBReq(Message &msg)
{
    ALOGD("cbInetSourceCtrlUSBReq %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    // Note: No point in decoding the payload-less generic request data (and no generated code for it).

    // Notify listener (if one exists) that we have a request.
    // (Listener is supposed to retrieve the requested value and then call our Respond/RespondError function.)
    m_pListener->RequestInternetSourceCtrlUSB(msg.ecu, msg.pdu.header.sender_handle_id);
}

void ConnectivityX::cbInetSourceCtrlUSBNotificationReq(Message &msg)
{
    ALOGD("cbInetSourceCtrlUSBNotificationReq %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    // Add node to subscriber list.
    {
        std::lock_guard<std::mutex> guard(m_usbSubscriberMutex);
        m_inetSourceCtrlUsbSubscribers.push_back(NotificationSubscriber(msg));
    }

    // A notification with latest status must be sent at once!
    // ...TODO!! Discuss how to ensure that we have a correct value if executing soon after startup.
    //           Put requirements on CommonAPI stub implementation?
    //           Remove direct send from here and use 'TriggerNotification' callback instead?
    //           Other alternatives?...

    const Icb_OnOffSetting onOff = m_usbOnOff;
    ALOGD("Reporting back usb on off status: %u", onOff);

    ASN_Session_Reset(m_session_msgd);
    Icb_OpInternetSourceCtrlUSB_Notification pMsg = Icb_OpInternetSourceCtrlUSB_Notification_Create(m_session_msgd);

    pMsg->onOff = onOff;

    SendEncodedMsg(pMsg,
                   Icb_OpInternetSourceCtrlUSB_Notification_Encode,
                   Icb_OpInternetSourceCtrlUSB_Notification_EncodedSize,
                   VccIpCmd::ServiceId::Connectivity,
                   VccIpCmd::OperationId::InternetSourceCtrlUSB,
                   VccIpCmd::OperationType::NOTIFICATION,
                   msg.ecu,
                   getNewSequenceId());
}

void ConnectivityX::NotifyInternetSourceUSB(Icb_OnOffSetting onOff)
{
    ALOGD("usb on/off status updated to: %u", onOff);

    // NOTE! We are called from CommonAPI's thread here so must
    //       ensure thread safe access to forwarded data!
    //       (In this case only needing one parameter which is atomic.)
    {
        // std::lock_guard<std::mutex> guard(m_usbDataMutex);
        m_usbOnOff = onOff;
    }

    // Send notifications to subscribing nodes on ip command bus.
    ASN_Session_Reset(m_session_common_api);
    Icb_OpInternetSourceCtrlUSB_Notification pMsg =
        Icb_OpInternetSourceCtrlUSB_Notification_Create(m_session_common_api);
    pMsg->onOff = onOff;

    std::lock_guard<std::mutex> guard(m_usbSubscriberMutex);
    for (NotificationSubscriber &subscriber : m_inetSourceCtrlUsbSubscribers)
    {
        SendEncodedMsg(pMsg,
                       Icb_OpInternetSourceCtrlUSB_Notification_Encode,
                       Icb_OpInternetSourceCtrlUSB_Notification_EncodedSize,
                       VccIpCmd::ServiceId::Connectivity,
                       VccIpCmd::OperationId::InternetSourceCtrlUSB,
                       VccIpCmd::OperationType::NOTIFICATION,
                       subscriber.ecu,
                       getNewSequenceId());
    }
}

void ConnectivityX::RespondInternetSourceUSB(const Icb_OnOffSetting onOff,
                                             const Message::Ecu ecu,
                                             const VccIpCmd::SenderHandleId senderHandleId)
{
    ALOGD("Responding on/off status: %u", onOff);

    // NOTE! We are called from CommonAPI's thread here so must
    //       ensure thread safe access to forwarded data!
    //       (In this case only needing one parameter which is atomic.)
    {
        // std::lock_guard<std::mutex> guard(m_usbDataMutex);
        m_usbOnOff = onOff;
    }

    // Send notifications to subscribing nodes on ip command bus.
    ASN_Session_Reset(m_session_common_api);
    Icb_OpInternetSourceCtrlUSB_Response pMsg = Icb_OpInternetSourceCtrlUSB_Response_Create(m_session_common_api);
    pMsg->onOff = onOff;

    SendResponse(pMsg,
                 Icb_OpInternetSourceCtrlUSB_Response_Encode,
                 Icb_OpInternetSourceCtrlUSB_Response_EncodedSize,
                 VccIpCmd::ServiceId::Connectivity,
                 VccIpCmd::OperationId::InternetSourceCtrlUSB,
                 ecu,
                 senderHandleId);
}

void ConnectivityX::RespondErrorInternetSourceUSB(const Icb_OpGeneric_Error_t error,
                                                  const Message::Ecu ecu,
                                                  const VccIpCmd::SenderHandleId senderHandleId)
{
    ALOGD("Responding error code: %u", error.errorCode);

    // Send notifications to subscribing nodes on ip command bus.
    ASN_Session_Reset(m_session_common_api);
    Icb_OpGeneric_Error pMsg = Icb_OpGeneric_Error_Create(m_session_common_api);

    *pMsg = error;

    SendGenericError(
        pMsg, VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetSourceCtrlUSB, ecu, senderHandleId);
}

void ConnectivityX::cbInetSourceCtrlUSBSetReq(Message &msg)
{
    Icb_OpInternetSourceCtrlUSB_SetRequest p = nullptr;

    ALOGD("cbInetSourceCtrlUSBSetReq %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    if (DecodeMessage(
            msg, p, Icb_OpInternetSourceCtrlUSB_SetRequest_Create, Icb_OpInternetSourceCtrlUSB_SetRequest_Decode))
    {
        // All ok. Get the service specific data and call listener.
        ALOGD("Requested change of usb on/off to: %u", p->onOff);
        Icb_OnOffSetting_t convertedOnOff = static_cast<Icb_OnOffSetting_t>(p->onOff);
        m_pListener->SetInternetSourceCtrlUSB(convertedOnOff, msg.ecu, msg.pdu.header.sender_handle_id);
    }
}

void ConnectivityX::cbInetGatewayAsServerReq(Message &msg)
{
    ALOGD("cbInetGatewayReq %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    // Note: No point in decoding the payload-less generic request data (and no generated code for it).

    // Notify listener (if one exists) that we have a request.
    // (Listener is supposed to retrieve the requested value and then call our Respond/RespondError function.)
    m_pListener->RequestInternetGateway(msg.ecu, msg.pdu.header.sender_handle_id);
}

void ConnectivityX::RespondInetGatewayAsServer(const Icb_Ecu_t gateway,
                                               const Message::Ecu ecu,
                                               const VccIpCmd::SenderHandleId senderHandleId)
{
    ALOGD("Responding internet gateway ecu: %s (%u)", EcuToString(gateway), gateway);

    // NOTE! We are called from CommonAPI's thread here so must
    //       ensure thread safe access to forwarded data!
    //       (In this case only needing one parameter which is atomic.)
    {
        // std::lock_guard<std::mutex> guard(m_inetGatewayDataMutex);
        m_inetGatewayAsServer = gateway;
    }

    // Send notifications to subscribing nodes on ip command bus.
    ASN_Session_Reset(m_session_common_api);
    Icb_OpInternetGateway_Response pMsg = Icb_OpInternetGateway_Response_Create(m_session_common_api);
    pMsg->internetGateway = static_cast<Icb_Ecu>(gateway);

    SendResponse(pMsg,
                 Icb_OpInternetGateway_Response_Encode,
                 Icb_OpInternetGateway_Response_EncodedSize,
                 VccIpCmd::ServiceId::Connectivity,
                 VccIpCmd::OperationId::InternetGateway,
                 ecu,
                 senderHandleId);
}

void ConnectivityX::RespondErrorInternetGatewayAsServer(const Icb_OpGeneric_Error_t error,
                                                        const Message::Ecu ecu,
                                                        const VccIpCmd::SenderHandleId senderHandleId)
{
    ALOGD("Responding error code: %u", error.errorCode);

    // Send notifications to subscribing nodes on ip command bus.
    ASN_Session_Reset(m_session_common_api);
    Icb_OpGeneric_Error pMsg = Icb_OpGeneric_Error_Create(m_session_common_api);

    *pMsg = error;

    SendGenericError(
        pMsg, VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetGateway, ecu, senderHandleId);
}

void ConnectivityX::cbInetGatewayAsServerSetReq(Message &msg)
{
    Icb_OpInternetGateway_SetRequest p = nullptr;

    ALOGD("cbInetGatewaySetReq %04X.%04X.%02d 0x%08X(size: %d)",
                          (int)msg.pdu.header.service_id,
                          (int)msg.pdu.header.operation_id,
                          (int)msg.pdu.header.operation_type,
                          (int)msg.pdu.header.sender_handle_id,
                          (int)msg.pdu.payload.size());

    if (DecodeMessage(msg, p, Icb_OpInternetGateway_SetRequest_Create, Icb_OpInternetGateway_SetRequest_Decode))
    {
        // All ok. Get the service specific data and call listener.
        const Icb_Ecu_t gateway = static_cast<Icb_Ecu_t>(p->internetGateway);
        ALOGD(
            "Requested change of internet gateway ecu to: %s (%u)", EcuToString(gateway), gateway);
        m_pListener->SetInternetGateway(gateway, msg.ecu, msg.pdu.header.sender_handle_id);
    }
}

void ConnectivityX::init(IConnectivityServiceProvider *pListener)
{
    bool ret = true;

    m_pListener = pListener;

    m_inetGatewayAsClient = e_Icb_Ecu_none;
    m_generalConnectivityAvailable = false;
    m_bluetoothOnOff = e_Icb_OnOffStatus_off;
    m_inetGatewayAsServer = e_Icb_Ecu_none;

    // Init notification subscriber lists.
    // (I.e. lists of subscribing nodes for notifications from us.
    //  Shall initially only contain 'static' subscribers, then more may be added in runtime
    //  upon receival of notification requests.)
    m_inetSourceCtrlBtSubscribers = m_inetSourceCtrlBtStaticSubscribers;
    m_inetSourceCtrlUsbSubscribers = m_inetSourceCtrlUsbStaticSubscribers;

    // Register our service, both client- and server part.
    ret = IpService::RegisterClient(ipBusClientSubscriptions);
    if (!ret)
    {
        ALOGE("Can not register as client");
    }
    ret = IpService::RegisterServer(ipBusServerSubscriptions);
    if (!ret)
    {
        ALOGE("Can not register as server");
    }
}

const char *ConnectivityX::InetSourceToString(Icb_InternetSource_t src)
{
    switch (src)
    {
        case e_Icb_InternetSource_none:
        {
            return "none";
        }
        break;
        case e_Icb_InternetSource_psim:
        {
            return "psim";
        }
        break;
        case e_Icb_InternetSource_bluetooth:
        {
            return "bluetooth";
        }
        break;
        case e_Icb_InternetSource_usb:
        {
            return "usb";
        }
        break;
        default:
        {
            return "undefined internet source enum";
        }
        break;
    }
}

const char *ConnectivityX::EcuToString(Icb_Ecu_t ecu)
{
    switch (ecu)
    {
        case e_Icb_Ecu_none:
        {
            return "none";
        }
        break;
        case e_Icb_Ecu_ihu:
        {
            return "ihu";
        }
        break;
        case e_Icb_Ecu_vcm:
        {
            return "vcm";
        }
        break;
        case e_Icb_Ecu_tem:
        {
            return "tem";
        }
        break;
        default:
        {
            return "undefined ecu enum";
        }
        break;
    }
}

}  // Connectivity
