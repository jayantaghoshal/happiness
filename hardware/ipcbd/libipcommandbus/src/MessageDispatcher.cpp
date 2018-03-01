/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ipcommandbus/MessageDispatcher.h"

#include <cassert>
#include <cstdint>

#include "ipcommandbus/net_serializer.h"

#define LOG_TAG "lipcb.MessageDispatcher"
#include <cutils/log.h>

using namespace tarmac::eventloop;

namespace Connectivity {

MessageDispatcher::MessageDispatcher(ITransportServices* transport, IDispatcher& dispatcher)
    : wakeUpApplicationThread(dispatcher) {
    assert(transport);

    m_transport = transport;
    m_transport->registerIncomingRequestCallback(
            std::bind(&MessageDispatcher::IPCBThread_cbIncomingRequest, this, std::placeholders::_1));
    m_transport->registerIncomingNotificationCallback(
            std::bind(&MessageDispatcher::IPCBThread_cbIncomingNotification, this, std::placeholders::_1));
    m_transport->registerIncomingResponseCallback(
            std::bind(&MessageDispatcher::IPCBThread_cbIncomingResponse, this, std::placeholders::_1));
    m_transport->registerErrorOnRequestCallback(std::bind(
            &MessageDispatcher::IPCBThread_cbIncomingError, this, std::placeholders::_1, std::placeholders::_2));
}

void MessageDispatcher::setDiagnostics(IDiagnosticsClient* diagnostics) {
    assert(diagnostics);
    m_diagnostics = diagnostics;
    m_transport->setDiagnostics(diagnostics, &wakeUpApplicationThread);
}

uint64_t MessageDispatcher::registerMessageCallback(IpCmdTypes::ServiceId serviceId,
                                                    IpCmdTypes::OperationId operationId,
                                                    IpCmdTypes::OperationType operationType,
                                                    MessageCallback messageCb) {
    ALOGD("Register message callback for (0x%04X, 0x%04X, 0x%02X) '%s'",
          (unsigned int)serviceId,
          (unsigned int)operationId,
          (unsigned int)operationType,
          IpCmdTypes::toString(operationType));

    std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);

    // If operation type is REQUEST, SETREQUEST or NOTIFICATION_REQUEST,
    // only one subscriber is allowed. Check if that is the case and fail if so.
    if (IpCmdTypes::OperationType::REQUEST == operationType || IpCmdTypes::OperationType::SETREQUEST == operationType ||
        IpCmdTypes::OperationType::NOTIFICATION_REQUEST == operationType) {
        auto it = FindReceiver([&serviceId, &operationId, &operationType](const RegInfo& ri) {
            return (serviceId == ri.serviceId && operationId == ri.operationId && operationType == ri.operationType);
        });
        if (it != m_registeredReceivers.end()) {
            return 0;
        }
    }

    // Create receiver and store it in list
    RegInfo ri(serviceId, operationId, operationType, m_registeredReceiverIds++, messageCb);
    { m_registeredReceivers.push_back(ri); }

    return ri.registeredReceiverId;
}

bool MessageDispatcher::unregisterCallback(uint64_t registeredReceiverId) {
    std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);

    // Find id and remove from list!
    auto it = FindReceiver(
            [&registeredReceiverId](const RegInfo& ri) { return registeredReceiverId == ri.registeredReceiverId; });

    if (m_registeredReceivers.end() != it) {
        m_registeredReceivers.erase(it);
    } else {
        return false;
    }

    return true;
}

void MessageDispatcher::sendMessage(Message&& msg, std::shared_ptr<CallerData> pCallerData) {
    ALOGD("Send message %s to %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    {
        std::lock_guard<std::mutex> guard(m_msgQueueMutex);
        m_msgQueue.push(std::move(msg));
        const unsigned long size = m_msgQueue.size();
        if ((size > 300) && (size % 20 == 0)) {
            ALOGW("Message queue exceedingly large, size: %lu ", size);
        }
    }

    if (msg.pdu.header.operation_type == IpCmdTypes::OperationType::REQUEST ||
        msg.pdu.header.operation_type == IpCmdTypes::OperationType::SETREQUEST ||
        msg.pdu.header.operation_type == IpCmdTypes::OperationType::NOTIFICATION_REQUEST) {
        // TODO: Uncomment assert after having stricted up public send api (IPService.h)...
        //       assert(pCallerData);
        if (pCallerData) {
            const IpCmdTypes::SenderHandleId senderHandleId = msg.pdu.header.sender_handle_id;

            if (m_requestsMap.find(senderHandleId) != m_requestsMap.end()) {
                // ...Can't completely rule out that this may happen some time
                //    due to using 8 bit 'global' sequence number...
                ALOGW("There already is an outstanding request with sender handle id 0x%08X!", senderHandleId);
            }

            m_requestsMap[senderHandleId] = pCallerData;
        } else {
            ALOGW("Missing caller data for request for %s", Pdu::toString(msg.pdu).c_str());
        }
    }

    if (m_transport) {
        m_transport->getThreadDispatcher().Enqueue([this] { IPCBThread_sendPendingMessages(); });
    }
}

void MessageDispatcher::IPCBThread_sendPendingMessages(void) {
    ALOGV("MessageDispatcher::IPCBThread_sendPendingMessages");

    std::queue<Message> queueReadInMutex;
    {
        // TODO: Do we need a max limit here? Right now we always dispatch the whole queue in one batch
        std::lock_guard<std::mutex> guard(m_msgQueueMutex);
        queueReadInMutex.swap(m_msgQueue);
    }
    while (!queueReadInMutex.empty()) {
        Message msg = std::move(queueReadInMutex.front());
        queueReadInMutex.pop();
        m_transport->sendMessage(std::move(msg));
    }
}

bool MessageDispatcher::IPCBThread_cbIncomingRequest(Message& msg) {
    ALOGD("Incoming request %s from %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    bool receiverExists = false;
    bool serviceAndOpExists = true;
    bool serviceExists = true;
    {  // Mutex scope
        std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);

        // Find all receivers for the message
        std::vector<RegInfo> receivers = FindReceivers(msg);
        ALOGE_IF(receivers.size() > 1,
                 "More than one receiver registered for request! This should not be possible, calling only first "
                 "receiver!");

        if (1 == receivers.size()) {
            ALOGV("MessageDispatcher::cbIncomingRequest, Request callback for service found");

            // Indicate that we found at least one subscriber to this message
            receiverExists = true;

            RegInfo ri = receivers[0];
            wakeUpApplicationThread.Enqueue([ this, ri, m2 = msg ]() mutable { AppThread_cbIncomingRequest(ri, m2); });
        } else {
            // Called here inside scope of m_registeredReceiversMutex

            // Check if we have a subscriber for the Service ID and Operation ID
            serviceAndOpExists = ReceiverExists([&msg](const RegInfo& ri) {
                return msg.pdu.header.service_id == ri.serviceId && msg.pdu.header.operation_id == ri.operationId;
            });

            // Check if we have a subscriber for the Service ID
            serviceExists =
                    ReceiverExists([&msg](const RegInfo& ri) { return msg.pdu.header.service_id == ri.serviceId; });
        }
    }

    if (receiverExists) {
        assert(m_diagnostics);
        Message::Ecu ecu = msg.ecu;
        wakeUpApplicationThread.Enqueue([this, ecu]() { m_diagnostics->SetInvalidData(ecu, false); });

        return true;
    } else {
        if (serviceAndOpExists) {
            // The operation id is supported but we do not support requests.
            ALOGW("No callback registered for service %s'. Responding 'invalid op. type'.",
                  Pdu::toString(msg.pdu).c_str());

            m_transport->sendError(msg.ecu,
                                   msg.pdu,
                                   ITransportServices::ErrorCode::INVALID_OPERATION_TYPE,
                                   static_cast<std::uint16_t>(msg.pdu.header.operation_type));
        } else if (serviceExists) {
            // The operation id is not supported at all (for any operation type).
            ALOGW("No callback registered for service %s. Responding 'invalid op. id'.",
                  Pdu::toString(msg.pdu).c_str());

            m_transport->sendError(msg.ecu,
                                   msg.pdu,
                                   ITransportServices::ErrorCode::INVALID_OPERATION_ID,
                                   static_cast<std::uint16_t>(msg.pdu.header.operation_id));
        } else {
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

void MessageDispatcher::AppThread_cbIncomingRequest(RegInfo ri, Message& msg) {
    if (ri.messageCb) {
        ALOGD("MessageDispatcher::AppThread_cbIncomingRequest, Call messageCb()");
        if (!ri.messageCb(msg, ri.registeredReceiverId)) {
            // The operation id is supported but we do not support requests.
            ALOGW("Application registered to handle incoming request died (%s). Responding 'invalid op. type'.",
                  Pdu::toString(msg.pdu).c_str());

            m_transport->sendError(msg.ecu,
                                   msg.pdu,
                                   ITransportServices::ErrorCode::INVALID_OPERATION_TYPE,
                                   static_cast<std::uint16_t>(msg.pdu.header.operation_type));
        }
    } else {
        assert(m_diagnostics);
        m_diagnostics->SetInvalidData(msg.ecu, true);
    }
}

void MessageDispatcher::IPCBThread_cbIncomingNotification(Message& msg) {
    // TODO: Inefficient copy of msg, should modify TransportServices to pass msg as unique_ptr
    wakeUpApplicationThread.Enqueue([ this, m2 = msg ]() mutable { Appthread_cbIncomingNotification(m2); });
}

void MessageDispatcher::Appthread_cbIncomingNotification(Message& msg) {
    ALOGD("Incoming notification %s from %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    {  // Mutex scope
        std::lock_guard<std::mutex> lock(m_registeredReceiversMutex);

        // Find all receivers for the message
        std::vector<RegInfo> receivers = FindReceivers(msg);

        // Loop over all receivers and call them
        for (std::vector<RegInfo>::iterator it = receivers.begin(); it != receivers.end(); it++) {
            it->messageCb(msg, it->registeredReceiverId);
        }

        // If there were at least one receiver, report "No invalid data" to diagnostics
        if (receivers.size() != 0) {
            ALOGV("MessageDispatcher::cbIncomingNotification, Notification callback for service found");
            assert(m_diagnostics);
            m_diagnostics->SetInvalidData(msg.ecu, false);
        } else  // If there were no receivers, report "Invalid data" to diagnostics
        {
            ALOGW("No notification callback registered for service %s", Pdu::toString(msg.pdu).c_str());

            // NOTE! We shall NOT send error message on ip bus here!
            //       Ip Command Protocol specification says [VCC IP Prot: 0037/;-1]
            //       'Req : Notification messages (no matter which type) shall never be followed up with an ERROR
            //       message.'

            assert(m_diagnostics);
            Message::Ecu ecu = msg.ecu;
            m_diagnostics->SetInvalidData(ecu, true);
        }
    }
}

void MessageDispatcher::IPCBThread_cbIncomingResponse(Message& msg) {
    ALOGD("Incoming response %s from %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    wakeUpApplicationThread.Enqueue([ this, m2 = msg ]() mutable { AppThread_cbIncomingResponse(m2); });
}

void MessageDispatcher::AppThread_cbIncomingResponse(Message& msg) {
    auto itc = m_requestsMap.find(msg.pdu.header.sender_handle_id);
    if (itc != m_requestsMap.end()) {
        std::shared_ptr<CallerData> pCallerData = itc->second;
        m_requestsMap.erase(itc);
        pCallerData->errorType = ITransportServices::ErrorType::OK;

        /* TODO: Check locig here since it changed a bit when refactoring (ABENGT34)
          Looking at the old code it seems like:
            1. If we can't find receiver in list, report error
            2. If we have a receiver but no caller data, do nothing
            3. If we have receiver and caller data, report OK and call handler.

            In the new code i do the same, but i also check if we actually can call the
            receiver successfully. If not, report an error...
        */
        if (pCallerData) {
            if (pCallerData->responseCallback(msg)) {
                assert(m_diagnostics);
                Message::Ecu ecu = msg.ecu;
                wakeUpApplicationThread.Enqueue([this, ecu]() { m_diagnostics->SetInvalidData(ecu, false); });

                return;
            }
        }

        assert(m_diagnostics);
        Message::Ecu ecu = msg.ecu;
        wakeUpApplicationThread.Enqueue([this, ecu]() { m_diagnostics->SetInvalidData(ecu, true); });
    } else {
        ALOGW("Received response with sender handle id 0x%08X without finding caller data from request (timed out?)",
              msg.pdu.header.sender_handle_id);
        // There should be no point in service specific handling in case we don't (any longer) know which request
        // the response maps to.
        return;
    }
}

void MessageDispatcher::IPCBThread_cbIncomingError(Message& msg, ITransportServices::ErrorType eType) {
    wakeUpApplicationThread.Enqueue([ this, m2 = msg, eType ]() mutable { AppThread_cbIncomingError(m2, eType); });
}

void MessageDispatcher::AppThread_cbIncomingError(Message& msg, ITransportServices::ErrorType eType) {
    ALOGD("Incoming error %u,  %s from %s",
          static_cast<uint32_t>(eType),
          Pdu::toString(msg.pdu).c_str(),
          Message::EcuStr(msg.ecu));

    RequestsMap::iterator itc = m_requestsMap.find(msg.pdu.header.sender_handle_id);
    if (itc != m_requestsMap.end()) {
        std::shared_ptr<CallerData> pCallerData = itc->second;
        m_requestsMap.erase(itc);
        pCallerData->errorType = eType;

        if (eType == ITransportServices::ErrorType::REMOTE_ERROR) {
            DecodeGenericError(msg, pCallerData->errorInfo);
        } else {
            pCallerData->errorInfo.exists__optional__errorInfo = false;
            pCallerData->errorInfo.errorCode = 0;
            pCallerData->errorInfo.errorInfo = 0;
        }

        if (pCallerData->responseCallback(msg)) {
            ALOGV("MessageDispatcher::cbIncomingError, Reponse callback for service found");
        } else {
            ALOGW("No error/response callback registered for service %s", Pdu::toString(msg.pdu).c_str());
        }
    } else {
        ALOGW("Received error / timeout with sender handle id 0x%08X without finding caller data from request (timed "
              "out?)",
              msg.pdu.header.sender_handle_id);
        // There should be no point in service specific handling in case we don't (any longer) know which request
        // the error maps to.
        return;
    }
}

void MessageDispatcher::DecodeGenericError(Message& msg, Icb_OpGeneric_Error_t& errorReturn) {
    ALOGD("DecodeGenericError %s (size: %zu)", Pdu::toString(msg.pdu).c_str(), msg.pdu.payload.size());

    // NOTE! Not explicitly mentioned in specification, but since errorInfo is an optional element,
    //       the ASN encoding does in fact use an initial boolean byte to indicate whether errorInfo is included or not.
    //       (See corresponding generated functions in InfotainmentIpBus.c.)

    // NOTE2: See TransportServices::generateErrorPdu, for description of the bit-packing
    //        The implementation in VCM deviates from the spec

    errorReturn.exists__optional__errorInfo = false;
    errorReturn.errorCode = 0;
    errorReturn.errorInfo = 0;

    if (msg.pdu.payload.size() < 1) {
        ALOGI("Failed to decode error details, payload is too short.");
        return;
    }

    errorReturn.exists__optional__errorInfo = (msg.pdu.payload[0] & 0b10000000) != 0;
    errorReturn.errorCode = static_cast<Icb_ErrorCode>((msg.pdu.payload[0] & 0b01111000) >> 3);
    if (errorReturn.exists__optional__errorInfo) {
        if (msg.pdu.payload.size() >= 3) {
            uint16_t errInfoHi = msg.pdu.payload[0] & 0b00000111;
            uint16_t errInfoMi = msg.pdu.payload[1];
            uint16_t errInfoLo = (msg.pdu.payload[2] & 0b11111000) >> 3;
            uint16_t errInfo = static_cast<uint16_t>((errInfoHi << 13u) | (errInfoMi << 5u) | errInfoLo);
            errorReturn.errorInfo = errInfo;
        } else {
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
