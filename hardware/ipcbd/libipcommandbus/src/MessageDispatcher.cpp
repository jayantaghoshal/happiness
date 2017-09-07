/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "lipcb.MessageDispatcher"

#include "ipcommandbus/MessageDispatcher.h"

#include <cassert>
#include <cstdint>
#include <cutils/log.h>

#include "ipcommandbus/net_serializer.h"

using namespace tarmac::eventloop;

namespace Connectivity
{

MessageDispatcher::MessageDispatcher(ITransportServices *transport, IDispatcher& dispatcher) : wakeUpApplicationThread(dispatcher)
{
    assert(transport);

    m_transport = transport;
    m_transport->registerIncomingRequestCallback(
        std::bind(&MessageDispatcher::IPCBThread_cbIncomingRequest, this, std::placeholders::_1));
    m_transport->registerIncomingNotificationCallback(
        std::bind(&MessageDispatcher::IPCBThread_cbIncomingNotification, this, std::placeholders::_1));
    m_transport->registerIncomingResponseCallback(
        std::bind(&MessageDispatcher::IPCBThread_cbIncomingResponse, this, std::placeholders::_1));
    m_transport->registerErrorOnRequestCallback(
                std::bind(&MessageDispatcher::IPCBThread_cbIncomingError, this, std::placeholders::_1, std::placeholders::_2));
}


void MessageDispatcher::setDiagnostics(IDiagnosticsClient *diagnostics)
{
    assert(diagnostics);
    m_diagnostics = diagnostics;
    m_transport->setDiagnostics(diagnostics, &wakeUpApplicationThread);
}

void MessageDispatcher::registerResponseCallback(VccIpCmd::ServiceId serviceId,
                                                 VccIpCmd::OperationId operationId,
                                                 ResponseMessageCallback messageCb)
{
    ALOGD("Register response callback for %s / %04X.%04X",
                               VccIpCmd::toString(VccIpCmd::CombinedId(serviceId, operationId)).c_str(),
                               (unsigned int)serviceId,
                               (unsigned int)operationId);

    RegInfo ri(serviceId, operationId, messageCb);
    {
        std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);
        m_registeredReceivers.push_back(ri);
    }
}

void MessageDispatcher::registerRequestCallback(VccIpCmd::ServiceId serviceId,
                                                VccIpCmd::OperationId operationId,
                                                MessageCallback messageCb)
{
    return registerMessageCallback(serviceId, operationId, VccIpCmd::OperationType::REQUEST, messageCb);
}

void MessageDispatcher::registerSetRequestCallback(VccIpCmd::ServiceId serviceId,
                                                   VccIpCmd::OperationId operationId,
                                                   MessageCallback messageCb)
{
    return registerMessageCallback(serviceId, operationId, VccIpCmd::OperationType::SETREQUEST, messageCb);
}

void MessageDispatcher::registerNotificationCallback(VccIpCmd::ServiceId serviceId,
                                                     VccIpCmd::OperationId operationId,
                                                     MessageCallback messageCb)
{
    return registerMessageCallback(serviceId, operationId, VccIpCmd::OperationType::NOTIFICATION, messageCb);
}

void MessageDispatcher::registerNotificationCyclicCallback(VccIpCmd::ServiceId serviceId,
                                                           VccIpCmd::OperationId operationId,
                                                           MessageCallback messageCb)
{
    return registerMessageCallback(serviceId, operationId, VccIpCmd::OperationType::NOTIFICATION_CYCLIC, messageCb);
}

void MessageDispatcher::registerNotificationRequestCallback(VccIpCmd::ServiceId serviceId,
                                                            VccIpCmd::OperationId operationId,
                                                            MessageCallback messageCb)
{
    return registerMessageCallback(serviceId, operationId, VccIpCmd::OperationType::NOTIFICATION_REQUEST, messageCb);
}

void MessageDispatcher::registerMessageCallback(VccIpCmd::ServiceId serviceId,
                                                VccIpCmd::OperationId operationId,
                                                VccIpCmd::OperationType operationType,
                                                MessageCallback messageCb)
{
    ALOGD("Register message callback for (0x%04X, 0x%04X, 0x%02X) '%s.%s'",
                               (unsigned int)serviceId,
                               (unsigned int)operationId,
                               (unsigned int)operationType,
                               VccIpCmd::toString(VccIpCmd::CombinedId(serviceId, operationId)).c_str(),
                               VccIpCmd::toString(operationType));
    RegInfo ri(serviceId, operationId, operationType, messageCb);
    {
        std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);
        m_registeredReceivers.push_back(ri);
    }
}

void MessageDispatcher::sendMessage(Message &&msg, std::shared_ptr<CallerData> pCallerData)
{
    ALOGD("Send message %s to %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    {
        std::lock_guard<std::mutex> guard(m_msgQueueMutex);
        m_msgQueue.push(std::move(msg));
        const unsigned long size = m_msgQueue.size();
        if ((size > 300) && (size % 20 == 0))
        {
            ALOGW("Message queue exceedingly large, size: %lu ",size);
        }
    }

    if (msg.pdu.header.operation_type == VccIpCmd::OperationType::REQUEST ||
        msg.pdu.header.operation_type == VccIpCmd::OperationType::SETREQUEST ||
        msg.pdu.header.operation_type == VccIpCmd::OperationType::NOTIFICATION_REQUEST)
    {
        // TODO: Uncomment assert after having stricted up public send api (IPService.h)...
        //       assert(pCallerData);
        if (pCallerData)
        {
            const VccIpCmd::SenderHandleId senderHandleId = msg.pdu.header.sender_handle_id;

            if (m_requestsMap.find(senderHandleId) != m_requestsMap.end())
            {
                // ...Can't completely rule out that this may happen some time
                //    due to using 8 bit 'global' sequence number...
                ALOGW("There already is an outstanding request with sender handle id 0x%08X!",
                                          senderHandleId);
            }

            m_requestsMap[senderHandleId] = pCallerData;
        }
        else
        {
            ALOGW("Missing caller data for request for %s", Pdu::toString(msg.pdu).c_str());
        }
    }

    if (m_transport)
    {
        m_transport->getThreadDispatcher().Enqueue([this] { IPCBThread_sendPendingMessages(); });
    }
}

void MessageDispatcher::IPCBThread_sendPendingMessages(void)
{
    ALOGV("MessageDispatcher::IPCBThread_sendPendingMessages");

    std::queue<Message> queueReadInMutex;
    {
        //TODO: Do we need a max limit here? Right now we always dispatch the whole queue in one batch
        std::lock_guard<std::mutex> guard(m_msgQueueMutex);
        queueReadInMutex.swap(m_msgQueue);
    }
    while (!queueReadInMutex.empty())
    {
        Message msg = std::move(queueReadInMutex.front());
        queueReadInMutex.pop();
        m_transport->sendMessage(std::move(msg));
    }
}

bool MessageDispatcher::IPCBThread_cbIncomingRequest(Message &msg)
{
    ALOGD("Incoming request %s from %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    // Proper callback not found. Create error message and call diagnostics.
    auto service_id_and_op_id_predicate = [&msg](const RegInfo &ri) {
        return msg.pdu.header.service_id == ri.serviceId && msg.pdu.header.operation_id == ri.operationId;
    };
    auto service_id_predicate = [&msg](const RegInfo &ri) { return msg.pdu.header.service_id == ri.serviceId; };


    MessageCallback cb;
    bool serviceAndOpExists = true;
    bool serviceExists = true;
    {
        std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);
        auto it = FindReceiver(msg);
        if (m_registeredReceivers.end() != it)
        {
            ALOGV("MessageDispatcher::cbIncomingRequest, Request callback for service found");
            assert(m_diagnostics);
            Message::Ecu ecu = msg.ecu;
            wakeUpApplicationThread.Enqueue([this, ecu] () {
                m_diagnostics->SetInvalidData(ecu, false);
            });
            cb = it->messageCb;
        }
        else
        {
            serviceAndOpExists = ReceiverExists(service_id_and_op_id_predicate);
            serviceExists = ReceiverExists(service_id_predicate);
        }
    }
    wakeUpApplicationThread.Enqueue([this, cb, m2=msg]() mutable {
        AppThread_cbIncomingRequest(cb, m2);
    });

    if (cb)
    {
        return true;
    }
    else
    {
        if (serviceAndOpExists)
        {
            // The operation id is supported but we do not support requests.
            ALOGW("No callback registered for service %s'. Responding 'invalid op. type'.",
                                         Pdu::toString(msg.pdu).c_str());

            m_transport->sendError(msg.ecu,
                                   msg.pdu,
                                   ITransportServices::ErrorCode::INVALID_OPERATION_TYPE,
                                   static_cast<std::uint16_t>(msg.pdu.header.operation_type));
        }
        else if (serviceExists)
        {
            // The operation id is not supported at all (for any operation type).
            ALOGW("No callback registered for service %s. Responding 'invalid op. id'.",
                                         Pdu::toString(msg.pdu).c_str());

            m_transport->sendError(msg.ecu,
                                   msg.pdu,
                                   ITransportServices::ErrorCode::INVALID_OPERATION_ID,
                                   static_cast<std::uint16_t>(msg.pdu.header.operation_id));
        }
        else
        {
            ALOGW("No callback registered for service %s. Responding 'invalid service id'.",
                                         Pdu::toString(msg.pdu).c_str());

            m_transport->sendError(msg.ecu,
                                   msg.pdu,
                                   ITransportServices::ErrorCode::INVALID_SERVICE_ID,
                                   static_cast<std::uint16_t>(msg.pdu.header.service_id));
        }
        return false;
    }

}

void MessageDispatcher::AppThread_cbIncomingRequest(MessageCallback cb, Message &msg)
{
    if (cb)
    {
        cb(msg);
    }
    else
    {
        assert(m_diagnostics);
        m_diagnostics->SetInvalidData(msg.ecu, true);
    }
}


void MessageDispatcher::IPCBThread_cbIncomingNotification(Message &msg) {
    //TODO: Inefficient copy of msg, should modify TransportServices to pass msg as unique_ptr
    wakeUpApplicationThread.Enqueue([this, m2=msg]() mutable {
        Appthread_cbIncomingNotification(m2);
    });
}

void MessageDispatcher::Appthread_cbIncomingNotification(Message &msg)
{
    ALOGD("Incoming notification %s from %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    MessageCallback cb;
    {
        std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);
        auto it = FindReceiver(msg);
        if (m_registeredReceivers.end() != it)
        {
            cb = it->messageCb;
        }
    }

    if (cb)
    {
        ALOGV("MessageDispatcher::cbIncomingNotification, Notification callback for service found");
        assert(m_diagnostics);
        m_diagnostics->SetInvalidData(msg.ecu, false);
        cb(msg);
    }
    else
    {
        ALOGW("No notification callback registered for service %s",Pdu::toString(msg.pdu).c_str());

        // NOTE! We shall NOT send error message on ip bus here!
        //       Ip Command Protocol specification says [VCC IP Prot: 0037/;-1]
        //       'Req : Notification messages (no matter which type) shall never be followed up with an ERROR message.'

        assert(m_diagnostics);
        Message::Ecu ecu = msg.ecu;
        m_diagnostics->SetInvalidData(ecu, true);
    }
}

void MessageDispatcher::IPCBThread_cbIncomingResponse(Message &msg)
{
    ALOGD("Incoming response %s from %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    ResponseMessageCallback cb;
    {
        std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);
        auto it = FindReceiver(msg);

        if (m_registeredReceivers.end() == it)
        {
            ALOGE("No response callback registered for service %s", Pdu::toString(msg.pdu).c_str());

            // Note: Ip-bus error message shall NOT be sent for responses.

            assert(m_diagnostics);
            Message::Ecu ecu = msg.ecu;
            wakeUpApplicationThread.Enqueue([this, ecu] () {
                m_diagnostics->SetInvalidData(ecu, true);
            });
            return;
        }
        cb = it->messageCbResp;
    }

    ALOGV("MessageDispatcher::cbIncomingResponse, Response callback for service found");

    wakeUpApplicationThread.Enqueue([this, cb, m2=msg]() mutable {
        AppThread_cbIncomingResponse(m2, cb);
    });
}

void MessageDispatcher::AppThread_cbIncomingResponse(Message &msg, ResponseMessageCallback cb)
{
    std::shared_ptr<CallerData> pCallerData = nullptr;

    auto itc = m_requestsMap.find(msg.pdu.header.sender_handle_id);
    if (itc != m_requestsMap.end())
    {
        pCallerData = itc->second;
        m_requestsMap.erase(itc);
        pCallerData->errorType = ITransportServices::ErrorType::OK;
    }
    else
    {
        ALOGW("Received response with sender handle id 0x%08X without finding caller data from request (timed out?)",
              msg.pdu.header.sender_handle_id);
        // There should be no point in service specific handling in case we don't (any longer) know which request
        // the response maps to.
        return;
    }


    // All ok. Forward the response to service layer callback.
    assert(m_diagnostics);
    Message::Ecu ecu = msg.ecu;
    wakeUpApplicationThread.Enqueue([this, ecu] () {
        m_diagnostics->SetInvalidData(ecu, false);
    });
    cb(msg, pCallerData);
}


void MessageDispatcher::IPCBThread_cbIncomingError(Message &msg, ITransportServices::ErrorType eType) {
    wakeUpApplicationThread.Enqueue([this, m2=msg, eType]() mutable{
        AppThread_cbIncomingError(m2, eType);
    });
}

void MessageDispatcher::AppThread_cbIncomingError(Message &msg, ITransportServices::ErrorType eType)
{
    ALOGD("Incoming error %u,  %s from %s",
                               static_cast<uint32_t>(eType),
                               Pdu::toString(msg.pdu).c_str(),
                               Message::EcuStr(msg.ecu));

    std::shared_ptr<CallerData> pCallerData = nullptr;


    RequestsMap::iterator itc = m_requestsMap.find(msg.pdu.header.sender_handle_id);
    if (itc != m_requestsMap.end())
    {
        pCallerData = itc->second;
        m_requestsMap.erase(itc);
        pCallerData->errorType = eType;

        if (eType == ITransportServices::ErrorType::REMOTE_ERROR)
        {
            DecodeGenericError(msg, pCallerData->errorInfo);
        }
        else
        {
            pCallerData->errorInfo.exists__optional__errorInfo = false;
            pCallerData->errorInfo.errorCode = 0;
            pCallerData->errorInfo.errorInfo = 0;
        }
    }
    else
    {
        ALOGW(
            "Received error / timeout with sender handle id 0x%08X without finding caller data from request (timed "
            "out?)",
            msg.pdu.header.sender_handle_id);
        // There should be no point in service specific handling in case we don't (any longer) know which request
        // the error maps to.
        return;
    }

    ResponseMessageCallback cb;
    {
        std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);
        auto it = FindReceiver([&msg](const RegInfo &ri) {
            // Note: We shall call the regular response callback at the service layer.
            //       (Thus the last condition looking as it does.)
            return msg.pdu.header.service_id == ri.serviceId && msg.pdu.header.operation_id == ri.operationId &&
                   VccIpCmd::OperationType::RESPONSE == ri.operationType;
        });

        if (m_registeredReceivers.end() != it)
        {
            ALOGV("MessageDispatcher::cbIncomingError, Reponse callback for service found");
            cb = it->messageCbResp;

        }
        else
        {
            ALOGW("No error/response callback registered for service %s",
                                         Pdu::toString(msg.pdu).c_str());
            return;
        }
    }
    cb(msg, pCallerData);
}

void MessageDispatcher::DecodeGenericError(Message &msg, Icb_OpGeneric_Error_t &errorReturn)
{
    ALOGD("DecodeGenericError %s (size: %zu)", Pdu::toString(msg.pdu).c_str(), msg.pdu.payload.size());

    // NOTE! Not explicitly mentioned in specification, but since errorInfo is an optional element,
    //       the ASN encoding does in fact use an initial boolean byte to indicate whether errorInfo is included or not.
    //       (See corresponding generated functions in InfotainmentIpBus.c.)

    // NOTE2: See TransportServices::generateErrorPdu, for description of the bit-packing
    //        The implementation in VCM deviates from the spec

    errorReturn.exists__optional__errorInfo = false;
    errorReturn.errorCode = 0;
    errorReturn.errorInfo = 0;

    if (msg.pdu.payload.size() < 1)
    {
        ALOGI("Failed to decode error details, payload is too short.");
        return;
    }

    errorReturn.exists__optional__errorInfo = (msg.pdu.payload[0] & 0b10000000) != 0;
    errorReturn.errorCode = static_cast<Icb_ErrorCode>((msg.pdu.payload[0] & 0b01111000) >> 3);
    if (errorReturn.exists__optional__errorInfo)
    {
        if (msg.pdu.payload.size() >= 3)
        {
            uint16_t errInfoHi = msg.pdu.payload[0] & 0b00000111;
            uint16_t errInfoMi = msg.pdu.payload[1];
            uint16_t errInfoLo = (msg.pdu.payload[2] & 0b11111000) >> 3;
            uint16_t errInfo = static_cast<uint16_t>((errInfoHi << 13u) | (errInfoMi << 5u) | errInfoLo);
            errorReturn.errorInfo = errInfo;
        }
        else
        {
            ALOGI("Failed to decode service specific info from error message, payload is too short.");
        }
    }

    ALOGW("DecodeGenericError: error: 0x%X-%d:0x%X for %s, byte0 = 0x%02x'",
                                 errorReturn.errorCode,
                                 errorReturn.exists__optional__errorInfo,
                                 errorReturn.errorInfo,
                                 Pdu::toString(msg.pdu).c_str(),
                                 msg.pdu.payload[0]);
}

}  // Connectivity
