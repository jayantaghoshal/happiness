/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "IpService.h"

namespace Connectivity
{

bool IpService::RegisterClient(const std::vector<ClientIpCommandBusService> &table)
{
    bool ret = true;
    ALOGI("[%s] Registering client...", service_name_.c_str());

    // Register a callback function for every notification which we are
    // interested in and register us for notifications.
    for (auto elem : table)
    {
        // Register the callback function to be called when the responses/errors/timeouts are received.
        if (elem.messageCbResp != nullptr)
        {
            registerResponseCallback(elem.service_id, elem.operation_id, elem.messageCbResp);
        }

        // Register the callback function to be called when the notification(s)
        // are received.
        if (elem.messageCbNotification != nullptr)
        {
            registerNotificationCyclicCallback(elem.service_id, elem.operation_id, elem.messageCbNotification);

            registerNotificationCallback(elem.service_id, elem.operation_id, elem.messageCbNotification);
        }

        // Send the request (when specified)
        if (elem.bSendNotificationRequest)
        {
            // NOTE! According to VCC, Notification.Request messages are not
            // sent at all in current system, neither from VCM or IHU. This
            // may explain why we don't get ACK on these requests.
            // TODO(johan, 160413) Perhaps we shall disable this completely
            // independent on value of bSendNotificationRequest

            // Prepare header
            Pdu pdu;
            pdu.createHeader(elem.service_id,
                             elem.operation_id,
                             VccIpCmd::OperationType::NOTIFICATION_REQUEST,
                             VccIpCmd::DataType::NOT_ENCODED,
                             getNewSequenceId());

            // Prepare payload
            std::vector<uint8_t> payload;
            payload.reserve(4);
            payload.push_back(static_cast<uint8_t>(VccIpCmd::OperationType::NOTIFICATION));
            payload.push_back(static_cast<uint8_t>((static_cast<std::uint16_t>(elem.operation_id)) >> 8));
            payload.push_back(static_cast<uint8_t>(elem.operation_id));

            // .. and move it to pdu
            pdu.setPayload(std::move(payload));

            // Create message and set pdu.
            Message msg(std::move(pdu));
            msg.ecu = elem.ecu;

            // Send the request
            std::shared_ptr<MessageDispatcher::CallerData> caller_data;
            caller_data = std::make_shared<MessageDispatcher::CallerData>();

            ALOGI("[%s]   Subscribe on notifications for '%s' (%04X.%04X) from %s",
                                      service_name_.c_str(),
                                      toString(elem.service_id, elem.operation_id).c_str(),
                                      (int)elem.service_id,
                                      (int)elem.operation_id,
                                      Message::EcuStr(elem.ecu));

            sendMessage(std::move(msg), caller_data);

            // TODO: Handling of unsuccessful notification requests?...
        }
        else
        {
            ALOGI("[%s]   _not_ subscribing on notifications for '%s' (%04X.%04X) from %s",
                                      service_name_.c_str(),
                                      toString(elem.service_id, elem.operation_id).c_str(),
                                      (int)elem.service_id,
                                      (int)elem.operation_id,
                                      Message::EcuStr(elem.ecu));
        }
    }  // for

    return ret;
}

bool IpService::RegisterServer(const std::vector<ServerIpCommandBusService> &table)
{
    bool ret = true;
    ALOGI("[%s] Registering server...", service_name_.c_str());

    // Register a callback function for every operation type which we support
    for (auto elem : table)
    {
        if (elem.messageCbReq != nullptr)
        {
            registerRequestCallback(elem.service_id, elem.operation_id, elem.messageCbReq);
        }

        if (elem.messageCbSetReq != nullptr)
        {
            registerSetRequestCallback(elem.service_id, elem.operation_id, elem.messageCbSetReq);
        }

        if (elem.messageCbNotificationReq != nullptr)
        {
            registerNotificationRequestCallback(elem.service_id, elem.operation_id, elem.messageCbNotificationReq);
        }
    }  // for

    return ret;
}

void IpService::setDispatcher(MessageDispatcher *dispatcher)
{
    m_dispatcher = dispatcher;
}

void IpService::sendMessage(Message &&msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    if (m_dispatcher)
    {
        ALOGV("[%s] Send message %s to %s",
                                     service_name_.c_str(),
                                     Pdu::toString(msg.pdu).c_str(),
                                     Message::EcuStr(msg.ecu));

        m_dispatcher->sendMessage(std::move(msg), pCallerData);
    }
}

void IpService::broadcastMessage(Message &&msg)
{
    msg.ecu = Message::ALL;

    // TODO: Broadcast is not supported. //TODO: WHY not ?
    if (m_dispatcher)
    {
        ALOGV(
            "[%s] Broadcasting message %s", service_name_.c_str(), Pdu::toString(msg.pdu).c_str());
        m_dispatcher->sendMessage(std::move(msg));
    }
}

void IpService::sendMessage(Message::Ecu ecu, Message &&msg)
{
    msg.ecu = ecu;

    if (m_dispatcher)
    {
        ALOGV("[%s] Send message %s to %s",
                                     service_name_.c_str(),
                                     Pdu::toString(msg.pdu).c_str(),
                                     Message::EcuStr(msg.ecu));

        m_dispatcher->sendMessage(std::move(msg));
    }
}

void IpService::SendGenericRequest(const VccIpCmd::ServiceId serviceId,
                                   const VccIpCmd::OperationId operationId,
                                   const Message::Ecu ecu,
                                   std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    Pdu pdu;

    // Prepare header
    // In case GenericRequest is to be sent to TEM; DataType for the request is set to ENCODED due to a bug TEM node
    pdu.createHeader(serviceId,
                     operationId,
                     VccIpCmd::OperationType::REQUEST,
                     (Message::Ecu::TEM == ecu) ? VccIpCmd::DataType::ENCODED : VccIpCmd::DataType::NOT_ENCODED,
                     getNewSequenceId());

    // No payload used in generic request!

    Message msg(std::move(pdu));
    msg.ecu = ecu;
    sendMessage(std::move(msg), pCallerData);
}

void IpService::SendGenericSetRequestNoReturn(const VccIpCmd::ServiceId serviceId,
                                              const VccIpCmd::OperationId operationId,
                                              const Message::Ecu ecu,
                                              std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    Pdu pdu;

    // Prepare header
    // In case GenericRequest is to be sent to TEM; DataType for the request is set to ENCODED due to a bug TEM node
    pdu.createHeader(serviceId,
                     operationId,
                     VccIpCmd::OperationType::SETREQUEST_NORETURN,
                     (Message::Ecu::TEM == ecu) ? VccIpCmd::DataType::ENCODED : VccIpCmd::DataType::NOT_ENCODED,
                     getNewSequenceId());

    // No payload used in generic request!

    Message msg(std::move(pdu));
    msg.ecu = ecu;
    sendMessage(std::move(msg), pCallerData);
}

void IpService::SendGenericResponse(const VccIpCmd::ServiceId serviceId,
                                    const VccIpCmd::OperationId operationId,
                                    const Message::Ecu ecu,
                                    const VccIpCmd::SenderHandleId senderHandleId,
                                    std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    Pdu pdu;

    // Prepare header
    // In case GenericResponse is to be sent to TEM; DataType for the request is set to ENCODED due to a bug TEM node
    PreparePduHeader(pdu,
                     serviceId,
                     operationId,
                     VccIpCmd::OperationType::RESPONSE,
                     (Message::Ecu::TEM == ecu) ? VccIpCmd::DataType::ENCODED : VccIpCmd::DataType::NOT_ENCODED,
                     senderHandleId);

    // No payload used in generic response!

    Message msg(std::move(pdu));
    msg.ecu = ecu;
    sendMessage(std::move(msg), pCallerData);
}

void IpService::SendGenericError(Icb_OpGeneric_Error pMsg,
                                 const VccIpCmd::ServiceId serviceId,
                                 const VccIpCmd::OperationId operationId,
                                 const Message::Ecu ecu,
                                 const uint32_t senderHandleId)
{
    SendError(
        pMsg, Icb_OpGeneric_Error_Encode, Icb_OpGeneric_Error_EncodedSize, serviceId, operationId, ecu, senderHandleId);
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

uint8_t IpService::getNewSequenceId()
{
    return ++m_sequenceId;
}

void IpService::PreparePduHeader(Pdu &pdu,
                                 const VccIpCmd::ServiceId serviceId,
                                 const VccIpCmd::OperationId operationId,
                                 const VccIpCmd::OperationType operationType,
                                 const VccIpCmd::DataType dataType,
                                 const VccIpCmd::SenderHandleId senderHandleId)

{
    pdu.createHeader(serviceId, operationId, operationType, dataType, senderHandleId & 0xFF);

    if ((VccIpCmd::OperationType::RESPONSE == operationType) || (VccIpCmd::OperationType::ERROR == operationType))
    {
        // We must use the sender handle id of the original request!
        // (Assembling a new one with other parameters would be WRONG since not equal to that of original request.)
        // (Only the operation type should actually differ, but regardless, better do it according to spec.)
        pdu.header.sender_handle_id = senderHandleId;
    }
}

void IpService::CreateAndSendPduMessage(const Message::Ecu ecu,
                                        Pdu &pdu,
                                        std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    Message message(std::move(pdu));
    message.ecu = ecu;
    sendMessage(std::move(message), pCallerData);
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
}
