/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "MockMessageDispatcher.h"
#include "ipcommandbus/TransportServices.h"

namespace Connectivity
{
MockMessageDispatcher::MockMessageDispatcher() : MessageDispatcher(nullptr, nullptr)
{
}
MockMessageDispatcher::MockMessageDispatcher(ITransportServices *transport, ThreadDispatcher *dispatcher)
    : MessageDispatcher(transport, dispatcher)
{
}

void MockMessageDispatcher::simulateMessageReceived(Message &m, std::shared_ptr<CallerData> callerData)
{
    const auto it = std::find_if(m_registeredReceivers.begin(), m_registeredReceivers.end(), [&](const RegInfo &r) {
        return (r.serviceId == m.pdu.header.service_id) && (r.operationId == m.pdu.header.operation_id) &&
               (r.operationType == m.pdu.header.operation_type);
    });
    if (it != m_registeredReceivers.end())
    {
        if (it->operationType == VccIpCmd::OperationType::RESPONSE)
        {
            // TODO: Maybe it's a bit too much magic to allow both passing callerData by arg AND lookup in the map?
            if (callerData == nullptr)
            {
                auto cdIt = userCallerData.find(m.pdu.header.sender_handle_id);
                if (cdIt != userCallerData.end())
                {
                    userCallerData.erase(cdIt);
                    callerData = cdIt->second;
                    callerData->errorType = ITransportServices::ErrorType::OK;
                }
            }
            it->messageCbResp(m, callerData);
        }
        else
        {
            it->messageCb(m);
        }
    }
}

MessageDispatcher::MessageDispatcher(ITransportServices *transport, ThreadDispatcher *dispatcher)
    : wakeUpApplicationThread{dispatcher}, m_transport{transport}
{
}
void MessageDispatcher::setDiagnostics(IDiagnosticsClient *diagnostics)
{
    (void)diagnostics;
}

void MessageDispatcher::registerResponseCallback(VccIpCmd::ServiceId serviceId,
                                                 VccIpCmd::OperationId operationId,
                                                 ResponseMessageCallback responseMessageCb)
{
    static_cast<MockMessageDispatcher *>(this)->m_registeredReceivers.emplace_back(
        serviceId, operationId, responseMessageCb);
    static_cast<MockMessageDispatcher *>(this)->registerResponseCallback_mocked(
        serviceId, operationId, responseMessageCb);
}

void MessageDispatcher::registerRequestCallback(VccIpCmd::ServiceId serviceId,
                                                VccIpCmd::OperationId operationId,
                                                MessageCallback messageCb)
{
    static_cast<MockMessageDispatcher *>(this)->m_registeredReceivers.emplace_back(
        serviceId, operationId, VccIpCmd::OperationType::REQUEST, messageCb);
    static_cast<MockMessageDispatcher *>(this)->registerRequestCallback_mocked(serviceId, operationId, messageCb);
}

void MessageDispatcher::registerSetRequestCallback(VccIpCmd::ServiceId serviceId,
                                                   VccIpCmd::OperationId operationId,
                                                   MessageCallback messageCb)
{
    static_cast<MockMessageDispatcher *>(this)->m_registeredReceivers.emplace_back(
        serviceId, operationId, VccIpCmd::OperationType::SETREQUEST, messageCb);
    static_cast<MockMessageDispatcher *>(this)->registerSetRequestCallback_mocked(serviceId, operationId, messageCb);
}

void MessageDispatcher::registerNotificationCallback(VccIpCmd::ServiceId serviceId,
                                                     VccIpCmd::OperationId operationId,
                                                     MessageCallback messageCb)
{
    static_cast<MockMessageDispatcher *>(this)->m_registeredReceivers.emplace_back(
        serviceId, operationId, VccIpCmd::OperationType::NOTIFICATION, messageCb);
    static_cast<MockMessageDispatcher *>(this)->registerNotificationCallback_mocked(serviceId, operationId, messageCb);
}

void MessageDispatcher::registerNotificationCyclicCallback(VccIpCmd::ServiceId serviceId,
                                                           VccIpCmd::OperationId operationId,
                                                           MessageCallback messageCb)
{
    static_cast<MockMessageDispatcher *>(this)->m_registeredReceivers.emplace_back(
        serviceId, operationId, VccIpCmd::OperationType::NOTIFICATION_CYCLIC, messageCb);
    static_cast<MockMessageDispatcher *>(this)->registerNotificationCyclicCallback_mocked(
        serviceId, operationId, messageCb);
}

void MessageDispatcher::registerNotificationRequestCallback(VccIpCmd::ServiceId serviceId,
                                                            VccIpCmd::OperationId operationId,
                                                            MessageCallback messageCb)
{
    static_cast<MockMessageDispatcher *>(this)->m_registeredReceivers.emplace_back(
        serviceId, operationId, VccIpCmd::OperationType::NOTIFICATION_REQUEST, messageCb);
    static_cast<MockMessageDispatcher *>(this)->registerNotificationRequestCallback_mocked(
        serviceId, operationId, messageCb);
}

void MessageDispatcher::sendMessage(Connectivity::Message &&msg, std::shared_ptr<CallerData> pCallerData)
{
    static_cast<MockMessageDispatcher *>(this)->userCallerData.emplace(msg.pdu.header.sender_handle_id, pCallerData);
    static_cast<MockMessageDispatcher *>(this)->sendMessage(std::ref(msg), pCallerData);
}
}
