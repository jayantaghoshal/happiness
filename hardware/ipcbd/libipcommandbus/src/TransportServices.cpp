/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ipcommandbus/TransportServices.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <memory>

#include "ipcommandbus/Pdu.h"
#include "ipcommandbus/net_serializer.h"

#define LOG_TAG "libipcb"
#include <cutils/log.h>

using namespace tarmac::eventloop;

namespace Connectivity {

TransportServices::TransportServices(IDispatcher& timeProvider,
                                     IDispatcher& threadDispatcher,
                                     Message::Ecu selfEcu,
                                     bool useWfaTimer)
    : timeProvider{timeProvider}, threadDispatcher{threadDispatcher}, m_useWfaTimer(useWfaTimer), selfEcu{selfEcu} {}

void TransportServices::setSocket(ISocket* pSocket) {
    m_pSocket = pSocket;
    m_pSocket->registerReadReadyCb(std::bind(&TransportServices::handleInUnicastData, this));
}

void TransportServices::setBroadcastSocket(ISocket* pSocket) {
    m_pBroadcastSocket = pSocket;
    m_pBroadcastSocket->registerReadReadyCb(std::bind(&TransportServices::handleInBroadcastData, this));
}

void TransportServices::setDiagnostics(IDiagnosticsClient* diagnostics, IDispatcher* dispatcher) {
    assert(diagnostics);
    assert(dispatcher);
    m_diagnostics = diagnostics;
    m_applicationThreadDispatcher = dispatcher;
}

void TransportServices::registerIncomingRequestCallback(std::function<bool(Message&)> messageCb) {
    m_incomingRequestCb = messageCb;
}

void TransportServices::registerIncomingNotificationCallback(std::function<void(Message&)> messageCb) {
    m_incomingNotificationCb = messageCb;
}

void TransportServices::registerIncomingResponseCallback(std::function<void(Message&)> messageCb) {
    m_incomingResponseCb = messageCb;
}

void TransportServices::registerErrorOnRequestCallback(std::function<void(Message&, ErrorType)> messageCb) {
    m_errorOnRequestCb = messageCb;
}

void TransportServices::sendMessage(Message&& msg) {
    ALOGV("[TransportServices] sendMessage - %s to ECU %s", Pdu::toString(msg.pdu).c_str(), Message::EcuStr(msg.ecu));

    // Need to set the proper protocol version
    msg.pdu.header.protocol_version = TransportServices::PROTOCOL_VERSION;

    switch (msg.pdu.header.operation_type) {
        case IpCmdTypes::OperationType::SETREQUEST_NORETURN:
        case IpCmdTypes::OperationType::NOTIFICATION_REQUEST: {
            ALOGV("[TransportServices] Send request (0x%02x): ", (int)msg.pdu.header.operation_type);
            if (!m_useWfaTimer) {
                // Does not require book keeping
                this->sendPdu(msg.ecu, msg.pdu);
                break;
            }
            // If WFA timer should be used, SETREQUEST_NORETURN and NOTIFICATION_REQUEST should have the same behaviour
            // as REQUEST and SETREQUEST: fall through
        }

        case IpCmdTypes::OperationType::REQUEST:
        case IpCmdTypes::OperationType::SETREQUEST: {
            ALOGV("[TransportServices] Send request (0x%02x): ", (int)msg.pdu.header.operation_type);
            // Set up timers and states
            std::unique_ptr<TrackMessage> pTm(new TrackMessage);
            pTm->wfa = TimeoutInfo();
            if (msg.retry_info.override_default) {
                pTm->wfr = TimeoutInfo((std::chrono::milliseconds)msg.retry_info.retry_timeout_ms,
                                       msg.retry_info.max_retries);
            } else {
                pTm->wfr = TimeoutInfo();
            }
            pTm->msg = std::move(msg);
            auto pTmRef = std::ref(*pTm);
            std::chrono::milliseconds timeoutValue;
            if (m_useWfaTimer) {
                timeoutValue = pTm->wfa.getTimeoutValue();
                pTm->state = msg.pdu.header.operation_type == IpCmdTypes::OperationType::SETREQUEST_NORETURN
                                     ? TrackMessage::WAIT_FOR_SET_REQUEST_NO_RETURN_ACK
                                     : TrackMessage::WAIT_FOR_REQUEST_ACK;
            } else {
                timeoutValue = pTm->wfr.getTimeoutValue();
                pTm->state = TrackMessage::WAIT_FOR_REQUEST_RESPONSE;
            }
            pTm->timer = timeProvider.EnqueueWithDelay(
                    timeoutValue,
                    [ this, pTmRef, id = pTm->msg.pdu.header.sender_handle_id ]() { messageTimeout(pTmRef, id); });
            // Add the message to pending and send it
            m_pendingMessages.push_back(std::move(pTm));
            std::unique_ptr<TrackMessage>& pendTm = m_pendingMessages.back();
            this->sendPdu(pendTm->msg.ecu, pendTm->msg.pdu);
        } break;

        case IpCmdTypes::OperationType::RESPONSE: {
            ALOGV("[TransportServices] Send response: 0x%08X", msg.pdu.header.sender_handle_id);

            if (m_useWfaTimer) {
                // Setup timers and states
                std::unique_ptr<TrackMessage> pTm(new TrackMessage);
                pTm->state = TrackMessage::WAIT_FOR_RESPONSE_ACK;
                pTm->wfa = TimeoutInfo();
                pTm->msg = std::move(msg);
                auto pTmRef = std::ref(*pTm);
                pTm->timer = timeProvider.EnqueueWithDelay(
                        pTm->wfa.getTimeoutValue(),
                        [ this, pTmRef, id = pTm->msg.pdu.header.sender_handle_id ]() { messageTimeout(pTmRef, id); });

                // Add to pending message and send it
                m_pendingMessages.push_back((std::move(pTm)));
                std::unique_ptr<TrackMessage>& pendTm = m_pendingMessages.back();
                this->sendPdu(pendTm->msg.ecu, pendTm->msg.pdu);
            } else {
                // No book keeping needed since ack timer is not used.
                this->sendPdu(msg.ecu, msg.pdu);
            }

        } break;

        case IpCmdTypes::OperationType::NOTIFICATION: {
            ALOGV("[TransportServices] Send notification (0x%02x): ", (int)msg.pdu.header.operation_type);

            if (m_useWfaTimer) {
                // Set up timers and states
                std::unique_ptr<TrackMessage> pTm(new TrackMessage);
                pTm->state = TrackMessage::WAIT_FOR_NOTIFICATION_ACK;
                pTm->wfa = TimeoutInfo();
                if (msg.retry_info.override_default) {
                    pTm->wfr = TimeoutInfo((std::chrono::milliseconds)msg.retry_info.retry_timeout_ms,
                                           msg.retry_info.max_retries);
                } else {
                    pTm->wfr = TimeoutInfo();
                }
                pTm->msg = std::move(msg);
                auto pTmRef = std::ref(*pTm);
                pTm->timer = timeProvider.EnqueueWithDelay(
                        pTm->wfa.getTimeoutValue(),
                        [ this, pTmRef, id = pTm->msg.pdu.header.sender_handle_id ]() { messageTimeout(pTmRef, id); });

                // Add the message to pending and send it
                m_pendingMessages.push_back(std::move(pTm));
                std::unique_ptr<TrackMessage>& pendTm = m_pendingMessages.back();
                this->sendPdu(pendTm->msg.ecu, pendTm->msg.pdu);
            } else {
                // No book keeping needed as we are not using the ack timer
                this->sendPdu(msg.ecu, msg.pdu);
            }
        } break;

        case IpCmdTypes::OperationType::ERROR:
        case IpCmdTypes::OperationType::NOTIFICATION_CYCLIC:
            // Both ERROR and NOTIFICATION_CYCLIC does not require book keeping...
            // I.e. we shall not wait for an ACK after this one...
            this->sendPdu(msg.ecu, msg.pdu);
            break;

        case IpCmdTypes::OperationType::ACK:
        case IpCmdTypes::OperationType::UNDEFINED:
            // Not handled
            break;
    }
}

TrackMessage* TransportServices::getTrackMessage(uint32_t senderHandleId) {
    // TODO: What if same operation (operationid+operation+type+service) happens to be sent by two different nodes with
    // same seq nr...
    //       Can we rule out this ever happens?
    //       If not, we'll need to extend checks, e.g. using ecu.
    // No we can not. This violates  [VCC IP Prot: 0018/;-0]
    //    >> Req: The applications shall use the SenderHandleID and the IP-source address to distinguish between
    //    >> different messages.

    auto it = std::find_if(
            m_pendingMessages.begin(), m_pendingMessages.end(), [&senderHandleId](std::unique_ptr<TrackMessage>& pTm) {
                return (senderHandleId == pTm->msg.pdu.header.sender_handle_id);
            });

    if (m_pendingMessages.end() != it) {
        return (*it).get();
    }

    return nullptr;
}

std::unique_ptr<TrackMessage> TransportServices::removeTrackMessage(uint32_t senderHandleId) {
    auto it = std::find_if(
            m_pendingMessages.begin(), m_pendingMessages.end(), [&senderHandleId](std::unique_ptr<TrackMessage>& pTm) {
                return (senderHandleId == pTm->msg.pdu.header.sender_handle_id);
            });

    if (m_pendingMessages.end() != it) {
        std::unique_ptr<TrackMessage> pTm = std::move(*it);
        m_pendingMessages.erase(it);
        return pTm;
    }
    return std::unique_ptr<TrackMessage>();
}

void TransportServices::sendPdu(Message::Ecu destination, const Pdu& pdu) {
    ALOGD("[TransportServices] Send PDU: %s to %s", Pdu::toString(pdu).c_str(), Message::EcuStr(destination));

    if (destination == Message::Ecu::UNKNOWN || destination == selfEcu) {
        ALOGE("[TransportServices] Sending pdu to self or unknown");
        return;
    }

    std::vector<uint8_t> buffer;
    pdu.toData(buffer);
    try {
        if (buffer.size() > 1400) {
            //[VCC IP Prot: 0006/;-0]
            ALOGW("[TransportServices] Sending PDU larger than 1400 bytes on UDP");
        }

        if (destination == Message::Ecu::ALL) {
            if (m_pBroadcastSocket == nullptr) {
                ALOGV("[TransportServices] ERROR, trying to send PDU on nullptr socket(m_pBroadcastSocket)");
                return;
            }
            m_pBroadcastSocket->writeTo(buffer, destination);
        } else {
            if (m_pSocket == nullptr) {
                ALOGV("[TransportServices] ERROR, trying to send PDU on nullptr socket(m_pSocket)");
                return;
            }
            m_pSocket->writeTo(buffer, destination);
        }
    } catch (const SocketException& e) {
        ALOGE("[TransportServices] %s . Code (%s : %i)", e.what(), e.code().category().name(), e.code().value());
    }
}
void TransportServices::sendAck(Message::Ecu destination, const Pdu& pdu) {
    if (m_useWfaTimer) {
        ALOGV("[TransportServices] Send ACK for: 0x%08X", pdu.header.sender_handle_id);
        Pdu ackPdu;
        ackPdu.header = pdu.header;
        ackPdu.header.length = VCCPDUHeader::BASE_LENGTH;
        ackPdu.header.operation_type = IpCmdTypes::OperationType::ACK;
        this->sendPdu(destination, ackPdu);
    } else {
        ALOGV("[TransportServices] ACK not sent, m_useWfaTimer=false");
    }
}

void TransportServices::generateErrorPdu(Pdu& errorPdu, const Pdu& pdu, ErrorCode errorCode, uint16_t errorInfo) {
    // Payload for error PDUs have the following bit layout (1 character is one bit):
    //
    // ECCC CIII IIII IIII IIII I000
    // E - Bit specifying if error info is available or not.
    // C - Error code bit. 4 in total.
    // I - Error info bit. 16 in total if E bit set to 1. Excluded if E is 0.
    //
    // VCC IP COMMAND PROTOCOL document no. 31843799 rev. 014 - "2.1.4.1.1.2 [VCC IP Prot: 0061/;-0]"
    // only mentions that error code is encoded in 1 byte always followed by 2 bytes error info.
    // INFOTAINMENT IP BUS - SPA document no. 31841985 rev. 017 - "3.5.3.3 Error codes" has ASN that
    // contradicts 31843799 when PER is used.
    //
    // Note that the ASN is probably wrong. ErrorCode is an enumeration with 15 different values
    // The ASN PER encoder packs this in 4 bits. There are comments mentioning reserved ranges but
    // since they are not explicitly listed in the ASN itself they are not accounted for when code
    // is generated from the ASN.
    //
    // VCM seems to follow ASN in 31841985 and use PER (bits laid out as specified above) so we do
    // the same.

    // Concerning the BUSY flag:
    // In [IHU-15163], the conclusion was to move the responsibility the applications/services
    // to handle the BUSY flag. Only the applications/services themselves will know
    // how many simultaneous requests they are able to handle. Probably this should be configurable
    // in some way.

    bool error_info_available = errorInfo != 0;
    unsigned int payload_size = (error_info_available ? 3 : 1);

    errorPdu.header = pdu.header;
    errorPdu.header.length = VCCPDUHeader::BASE_LENGTH + payload_size;
    errorPdu.header.operation_type = IpCmdTypes::OperationType::ERROR;
    errorPdu.header.data_type = IpCmdTypes::DataType::ENCODED;

    errorPdu.payload.resize(payload_size);

    errorPdu.payload[0] = (error_info_available ? 1 : 0) << 7;
    errorPdu.payload[0] |= (static_cast<std::uint8_t>(errorCode) & 0x0F) << 3;
    if (error_info_available) {
        errorPdu.payload[0] |= (errorInfo >> 13) & 0x07;
        errorPdu.payload[1] = (errorInfo >> 5) & 0xFF;
        errorPdu.payload[2] = (errorInfo << 3) & 0xF8;
    }
}

void TransportServices::sendError(Message::Ecu destination, const Pdu& pdu, ErrorCode errorCode, uint16_t errorInfo) {
    // If incoming message is of typ NOTIFICATION_CYCLIC, no response shall be sent on error. (REQPROD 346855)
    if (pdu.header.operation_type != IpCmdTypes::OperationType::NOTIFICATION_CYCLIC) {
        Pdu errorPdu;
        generateErrorPdu(errorPdu, pdu, errorCode, errorInfo);
        this->sendPdu(destination, errorPdu);
    } else {
        ALOGV("[TransportServices]: Will not send error message as reponse to NOTIFICATION_CYCLIC message");
    }
}

void TransportServices::handleInUnicastData(void) {
    if (m_pSocket) {
        handleInData(m_pSocket);
    }
}

void TransportServices::handleInBroadcastData(void) {
    if (m_pBroadcastSocket) {
        handleInData(m_pBroadcastSocket);
    }
}

void TransportServices::handleInData(ISocket* socket) {
    std::vector<uint8_t> buffer;

    // Read one UDP message
    Message::Ecu sourceEcu = Message::Ecu::UNKNOWN;

    try {
        socket->read(buffer, sourceEcu);
    } catch (const SocketException& e) {
        ALOGE("[TransportServices] %s . Code (%s : %i)", e.what(), e.code().category().name(), e.code().value());
        return;
    }

    if (sourceEcu == selfEcu) {
        // Ignore broadcasts from this unit
        return;
    }

    ALOGV("[TransportServices] Received data from %s", Message::EcuStr(sourceEcu));

    while (buffer.size() > 0) {
        // Extract PDUs from the data
        // Do we have enough data to extract a header?
        if (buffer.size() < VCCPDUHeader::DATA_SIZE) {
            ALOGE("[TransportServices] Not enough data in buffer");
            // Since we're not able to trust the sender handle id (it might not even be present)
            // we can't respond with an error here.
            return;
        }

        Pdu pdu;
        const bool pduDataValid = pdu.fromData(buffer);

        // Since the error codes are ranked we shall not test for payload length here, but after the other header tests.
        // TODO: Resolve if this check is valid. According to example in spec a message with no sender handle id shall
        // be ignored and discarded.
        uint32_t sender_handle_mask = ((static_cast<uint16_t>(pdu.header.service_id) & 0xFF) << 24) |
                                      ((static_cast<uint16_t>(pdu.header.operation_id) & 0xFF) << 16) | 0xFFFF;
        if ((sender_handle_mask & pdu.header.sender_handle_id) != pdu.header.sender_handle_id) {
            ALOGE("[TransportServices] Invalid sender handle ID: 0x%08X", pdu.header.sender_handle_id);
            this->sendError(sourceEcu, pdu, NOT_OK);
            return;
        }

        // Check protocol version
        if (pdu.header.protocol_version != PROTOCOL_VERSION) {
            ALOGE("[TransportServices] Invalid protocol version: %d (expected %d)",
                  pdu.header.protocol_version,
                  PROTOCOL_VERSION);
            this->sendError(sourceEcu, pdu, INVALID_PROTOCOL_VERSION, pdu.header.protocol_version);
            assert(m_diagnostics);
            assert(m_applicationThreadDispatcher);
            m_applicationThreadDispatcher->Enqueue(
                    [this, sourceEcu]() { m_diagnostics->SetInvalidData(sourceEcu, true); });
            return;
        }

        // Check header length and payload size
        if (!pduDataValid || pdu.header.length - VCCPDUHeader::BASE_LENGTH != pdu.payload.size()) {
            ALOGE("[TransportServices] Invalid PDU length");
            this->sendError(sourceEcu, pdu, INVALID_LENGTH);
            assert(m_diagnostics);
            assert(m_applicationThreadDispatcher);
            m_applicationThreadDispatcher->Enqueue(
                    [this, sourceEcu]() { m_diagnostics->SetInvalidData(sourceEcu, true); });
            return;
        }

        // Initial sanity check of message OK, process...
        this->processIncomingPdu(std::move(pdu), sourceEcu);
    }
}

void TransportServices::processIncomingPdu(Pdu&& pdu, Message::Ecu sourceEcu) {
    ALOGD("[TransportServices] Recv PDU: %s", Pdu::toString(pdu).c_str());

    // Handle PDU
    switch (pdu.header.operation_type) {
        case IpCmdTypes::OperationType::ERROR:
            // Handle incoming error
            this->handleIncomingError(pdu);
            break;
        case IpCmdTypes::OperationType::ACK:
            // Handle incoming ACK
            this->handleIncomingAck(pdu, sourceEcu);
            break;
        case IpCmdTypes::OperationType::REQUEST:
        case IpCmdTypes::OperationType::SETREQUEST:
        case IpCmdTypes::OperationType::NOTIFICATION_REQUEST: {
            // * An incoming request shall be responded to by an ACK followed by the RESPONSE or an ERROR.
            // * The RESPONSE might be sent by higher layers in the call to m_incomingRequestCb or at a later time.
            // * In either case we need to ensure that the ACK is sent before WFA timeout
            // * Right now we always ACK instantly but this could be optimized to check if RESPONSE is sent
            //   soon enough.
            // * If no RESPONSE is sent, but the m_incomingRequestCb returns true, an ACK must be sent here since
            //   then the RESPONSE will be delivered at a later time.
            // * If an ERROR for this REQUEST is sent during the state, no ACK shall be sent.
            // * BUSY is handled by the application/service. See comment in [IHU-15163] and comment in
            //   TransportServices::generateErrorPdu()

            Message m(std::move(pdu));
            m.ecu = sourceEcu;

            // Temporary remember the TrackMessage object
            // Can we avoid this? Comments please...
            // Add message to pending
            // Check if PDU can be handled
            assert(m_incomingRequestCb);
            if (m_incomingRequestCb(m)) {
                // TODO: Maybe delay the ack a few 50ms in case we send a RESPONSE quickly?
                // [VCC IP Prot: 0028/;-1] + [VCC IP Prot: 0032/;-1]
                //   Kindof says otherwise but it also covers the "reply before ack" case
                //   so we could actually cheat here to save bandwidth.
                ALOGD("[TransportServices] Request validated OK, but no response sent yet. So, send the ACK now...");
                this->sendAck(sourceEcu, m.pdu);
            } else {
                // NOTE! We rely on callback (MessageDispatcher::cbIncomingRequest) to have already sent the mandatory
                // error message...
                ALOGW("[TransportServices] Incoming request 0x%08X can not be handled by upper layers",
                      pdu.header.sender_handle_id);
                return;
            }
        } break;

        case IpCmdTypes::OperationType::SETREQUEST_NORETURN: {
            Message msg(std::move(pdu));
            msg.ecu = sourceEcu;

            // Check if PDU can be handled
            assert(m_incomingRequestCb);
            if (m_incomingRequestCb(msg)) {
                this->sendAck(sourceEcu, msg.pdu);
            } else {
                // NOTE! We rely on callback (MessageDispatcher::cbIncomingRequest) to have already sent the mandatory
                // error message...
                ALOGW("[TransportServices] Incoming set-request-no-return 0x%08X can not be handled by upper layers",
                      pdu.header.sender_handle_id);
                return;
            }
        } break;

        case IpCmdTypes::OperationType::RESPONSE: {
            std::unique_ptr<TrackMessage> pTm = this->removeTrackMessage(pdu.header.sender_handle_id);

            if (pTm) {
                timeProvider.Cancel(pTm->timer);
                this->sendAck(sourceEcu, pdu);

                // Deliver response
                ALOGV("[TransportServices] Deliver response");
                assert(m_incomingResponseCb);
                if (m_incomingResponseCb) {
                    Message msg(std::move(pdu));
                    msg.ecu = sourceEcu;
                    m_incomingResponseCb(msg);
                }
            } else {
                ALOGW("[TransportServices] Unexpected remote response: 0x%08X", pdu.header.sender_handle_id);
                this->sendAck(sourceEcu, pdu);
                return;
            }
        } break;

        case IpCmdTypes::OperationType::NOTIFICATION:
            // Acknowledge the NOTIFICATION
            this->sendAck(sourceEcu, pdu);
        case IpCmdTypes::OperationType::NOTIFICATION_CYCLIC:
            // Incoming notification shall just be forwarded
            // Cyclic notifications shall not be acknowledged...
            if (m_incomingNotificationCb) {
                Message msg(std::move(pdu));
                msg.ecu = sourceEcu;
                assert(m_incomingNotificationCb);
                m_incomingNotificationCb(msg);
            }
            break;
        default: {
            ALOGE("[TransportServices] Unknown operation type: %s", IpCmdTypes::toString(pdu.header.operation_type));
            this->sendError(
                    sourceEcu, pdu, INVALID_OPERATION_TYPE, static_cast<std::uint16_t>(pdu.header.operation_type));
            assert(m_diagnostics);
            assert(m_applicationThreadDispatcher);
            m_applicationThreadDispatcher->Enqueue(
                    [this, sourceEcu]() { m_diagnostics->SetInvalidData(sourceEcu, true); });
        } break;
    }
}

void TransportServices::handleIncomingAck(const Pdu& pdu, Message::Ecu /*sourceEcu*/) {
    TrackMessage* pTm = this->getTrackMessage(pdu.header.sender_handle_id);

    // Did we find something that matches the ACK?
    if (nullptr == pTm) {
        // No match... nothing to do
        ALOGW("[TransportServices] Unknown ACK received: 0x%08X", pdu.header.sender_handle_id);
        return;
    }

    if (TrackMessage::WAIT_FOR_REQUEST_ACK == pTm->state) {
        ALOGV("[TransportServices] ACK received for pending outgoing request message");

        timeProvider.Cancel(pTm->timer);

        // Reset timer
        pTm->wfa.reset();
        auto pTmRef = std::ref(*pTm);
        pTm->timer = timeProvider.EnqueueWithDelay(
                pTm->wfr.getTimeoutValue(),
                [ this, pTmRef, id = pTm->msg.pdu.header.sender_handle_id ]() { messageTimeout(pTmRef, id); });
        // Update state
        pTm->state = TrackMessage::WAIT_FOR_REQUEST_RESPONSE;
        return;
    }

    // Is this an ACK to a sent set-request-no-return?
    if (TrackMessage::WAIT_FOR_SET_REQUEST_NO_RETURN_ACK == pTm->state) {
        ALOGD("[TransportServices] ACK received for pending outgoing set-request-no-return message");
        // Delete timer
        timeProvider.Cancel(pTm->timer);
        // Message is successfully delivered to client, so we can now remove references to it.
        this->removeTrackMessage(pdu.header.sender_handle_id);
        return;
    }

    // Is this an ACK to a sent response?
    if (TrackMessage::WAIT_FOR_RESPONSE_ACK == pTm->state) {
        ALOGV("[TransportServices] ACK received for pending outgoing response message");
        // Delete timer
        timeProvider.Cancel(pTm->timer);
        // Message is successfully delivered to client, so we can now remove references to it.
        this->removeTrackMessage(pdu.header.sender_handle_id);
        return;
    }

    // Is this an ACK to a sent notification?
    if (TrackMessage::WAIT_FOR_NOTIFICATION_ACK == pTm->state) {
        ALOGV("[TransportServices] ACK received for pending outgoing notification message");
        // Delete timer
        timeProvider.Cancel(pTm->timer);
        // Message is successfully delivered to client, so we can now remove references to it.
        this->removeTrackMessage(pdu.header.sender_handle_id);
        return;
    }

    ALOGW("[TransportServices] Unexpected ACK received for managed message: 0x%08X", pdu.header.sender_handle_id);
}

void TransportServices::handleIncomingError(const Pdu& pdu) {
    TrackMessage* pTm = this->getTrackMessage(pdu.header.sender_handle_id);

    // Did we find something that matches the ERROR?
    if (nullptr == pTm) {
        // No match... nothing to do
        ALOGW("[TransportServices] Unknown ERROR received: 0x%08X", pdu.header.sender_handle_id);
        return;
    }

    if (pdu.payload.size() >= 1) {
        // NOTE: Message unpacking, deviation from spec due to VCM compatiblity, see justification in generateErrorPdu()
        const TransportServices::ErrorCode errorCode =
                static_cast<TransportServices::ErrorCode>((pdu.payload[0] & 0b01111000) >> 3);

        // Handle the busy error
        if (errorCode == TransportServices::ErrorCode::BUSY) {
            ALOGI("[TransportServices] Response=BUSY. Let the timeout handling resend the message. "
                  "SenderhandleID=0x%08X",
                  pdu.header.sender_handle_id);
            return;
        }
    }

    std::unique_ptr<TrackMessage> pRemovedTm = this->removeTrackMessage(pdu.header.sender_handle_id);

    // Always delete timer
    timeProvider.Cancel(pRemovedTm->timer);

    switch (pRemovedTm->state) {
        // We expected an ACK for the request operation, but received an ERROR instead.
        case TrackMessage::WAIT_FOR_REQUEST_ACK:
            ALOGW("[TransportServices] ERROR received instead of request ACK: 0x%08X", pdu.header.sender_handle_id);

            // Report the error to higher layers
            ALOGD("[TransportServices] Notify error: REMOTE_ERROR on sent request");
            assert(m_errorOnRequestCb);
            m_errorOnRequestCb(pRemovedTm->msg, REMOTE_ERROR);
            break;

        // We expected an ACK for the set-request-no-return operation, but received an ERROR instead.
        case TrackMessage::WAIT_FOR_SET_REQUEST_NO_RETURN_ACK:
            ALOGD("[TransportServices] ERROR received instead of set-request-no-return ACK: 0x%08X",
                  pdu.header.sender_handle_id);
            // Not doing anything here in accordance with VCC IP COMMAND PROTOCOL specification section "2.1.2.4.3
            // OperationType SetRequestNoReturn":
            // 'The received ERROR message shall be interpreted by the client as a correct ACK message.'
            break;

        // We expected an ACK on the response we send, but got an ERROR back.
        // TODO: Resolve what to do in this case.
        //       Today there is no "finished transmission" notification to upper layers.
        //       If we introduce the possibility to report an error here we also must be
        //       able to send a completed ok notification so higher layers know when to
        //       clean the message book keeping...
        case TrackMessage::WAIT_FOR_RESPONSE_ACK:
            ALOGW("[TransportServices] ERROR received instead of response ACK: 0x%08X", pdu.header.sender_handle_id);

            // Report the error to higher layers?
            // if (m_errorOnResponseCb)
            //{
            // log_debug("Notify error: REMOTE_ERROR on sent response";
            // Can't report the error as of now... shall we?
            // m_errorOnResponseCb(pTm->msg, REMOTE_ERROR);
            //}
            break;

        case TrackMessage::WAIT_FOR_NOTIFICATION_ACK:
            ALOGW("[TransportServices] ERROR received instead of notification ACK: 0x%08X",
                  pdu.header.sender_handle_id);
            // Report the error to higher layers?
            // if (m_errorOnNotificationCb)
            //{
            // log_debug("Notify error: REMOTE_ERROR on sent notification";
            // Can't report the error as of now... shall we?
            // m_errorOnNotificationCb(pTm->msg, REMOTE_ERROR);
            //}
            break;

        case TrackMessage::WAIT_FOR_REQUEST_RESPONSE:
            ALOGW("[TransportServices] ERROR received instead of response: 0x%08X", pdu.header.sender_handle_id);

            // Report the error to higher layers?
            ALOGD("[TransportServices] Notify error: REMOTE_ERROR on sent request");
            assert(m_errorOnRequestCb);
            m_errorOnRequestCb(pTm->msg, REMOTE_ERROR);
            break;

        default:
            ALOGW("[TransportServices] Unexpected ERROR received for managed message: 0x%08X",
                  pdu.header.sender_handle_id);
            break;
    }
}

void TransportServices::messageTimeout(TrackMessage& tm, IpCmdTypes::SenderHandleId id) {
    ALOGI("[TransportServices] MessageTimeout: senderid: %d", id);

    ALOGV("[TransportServices] Timeout on message 0x%08X, state: 0x%02X", tm.msg.pdu.header.sender_handle_id, tm.state);

    switch (tm.state) {
        case TrackMessage::WAIT_FOR_RESPONSE_ACK: {
            ALOGW("[TransportServices] Timed out while waiting for ACK on sent response: %s, state: 0x%02X",
                  Pdu::toString(tm.msg.pdu).c_str(),
                  tm.state);

            // initialized to 1 on purpose as signal is present in queue if ACK has timed out
            int enqueued_duplicates = 1;

            if (tm.msg.pdu.header.operation_type == IpCmdTypes::OperationType::NOTIFICATION) {
                enqueued_duplicates =
                        std::count_if(m_pendingMessages.begin(),
                                      m_pendingMessages.end(),
                                      [& senderHandleId = id, &ecu = tm.msg.ecu ](std::unique_ptr<TrackMessage> & pTm) {
                                          constexpr uint32_t seq_number_mask = 0xFFFFFF00;
                                          return pTm->msg.ecu == ecu &&
                                                 (senderHandleId & seq_number_mask) ==
                                                         (pTm->msg.pdu.header.sender_handle_id & seq_number_mask);
                                      });
            }

            if (enqueued_duplicates > 1) {
                ALOGW("[TransportServices] Multiple NOTIFICATION requests for same serviceId + operationId. "
                      "Message will not be re-transmitted. message %s state : 0x%02X",
                      Pdu::toString(tm.msg.pdu).c_str(),
                      tm.state);

                this->removeTrackMessage(tm.msg.pdu.header.sender_handle_id);
            } else if (!tm.wfa.increaseTimeout()) {
                // We expected an ACK on the response we sent, but got a timeout.
                // Update timeout levels and resend message
                ALOGW("[TransportServices] Max ACK timeouts. No more resends allowed for response: %s, "
                      "state: 0x%02X",
                      Pdu::toString(tm.msg.pdu).c_str(),
                      tm.state);

                this->removeTrackMessage(tm.msg.pdu.header.sender_handle_id);
            } else {
                // Resend message with updated timeout
                ALOGV("[TransportServices] Resending request...");
                tm.timer = timeProvider.EnqueueWithDelay(
                        tm.wfa.getTimeoutValue(),
                        [ this, &tm, id = tm.msg.pdu.header.sender_handle_id ]() { messageTimeout(tm, id); });

                this->sendPdu(tm.msg.ecu, tm.msg.pdu);
            }
        } break;

        case TrackMessage::WAIT_FOR_NOTIFICATION_ACK:
            ALOGW("[TransportServices] Timed out while waiting for ACK on sent notification: %s, state: 0x%02X",
                  Pdu::toString(tm.msg.pdu).c_str(),
                  tm.state);
            break;

        case TrackMessage::WAIT_FOR_REQUEST_ACK:
        case TrackMessage::WAIT_FOR_SET_REQUEST_NO_RETURN_ACK:
            ALOGW("[TransportServices] Timed out while waiting for ACK on sent request: %s, state: 0x%02X",
                  Pdu::toString(tm.msg.pdu).c_str(),
                  tm.state);

            // Update timeout levels and resend message
            if (!tm.wfa.increaseTimeout()) {
                ALOGW("[TransportServices] Max ACK timeouts. No more resends allowed for request: %s', state: 0x%02X",
                      Pdu::toString(tm.msg.pdu).c_str(),
                      tm.state);

                //  Remove message from pending list
                std::unique_ptr<TrackMessage> pTm = this->removeTrackMessage(tm.msg.pdu.header.sender_handle_id);

                // Report local timeout of the message
                ALOGD("[TransportServices] Notify error: LOCAL_TIMEOUT on sent request");

                // Create error pdu
                Pdu errorPdu;
                generateErrorPdu(errorPdu, pTm->msg.pdu, TransportServices::ErrorCode::TIMEOUT, 0);

                // Create message and set pdu.
                Message m(std::move(errorPdu));
                m.ecu = Message::IHU;  // Set to IHU to indicate locally generated error message
                assert(m_errorOnRequestCb);
                m_errorOnRequestCb(m, LOCAL_TIMEOUT);

                // pTm will go out of scope and die here
            } else {
                // Resend message with updated timeout
                ALOGV("[TransportServices] Resending request...");
                tm.timer = timeProvider.EnqueueWithDelay(tm.wfa.getTimeoutValue(),
                                                         [ this, &tm, id = tm.msg.pdu.header.sender_handle_id ]() {
                                                             messageTimeout(tm, id);
                                                         });  // TODO OK to capture tm like this???
                this->sendPdu(tm.msg.ecu, tm.msg.pdu);
            }

            break;

        case TrackMessage::WAIT_FOR_REQUEST_RESPONSE:
            ALOGW("[TransportServices] Timed out while waiting for response: %s, state: 0x%02X",
                  Pdu::toString(tm.msg.pdu).c_str(),
                  tm.state);
            // Update timeout levels and resend message
            if (!tm.wfr.increaseTimeout()) {
                ALOGW("[TransportServices] Max response timeouts. No more resends allowed for request: %s , state: "
                      "0x%02X",
                      Pdu::toString(tm.msg.pdu).c_str(),
                      tm.state);

                //  Remove message from pending list
                std::unique_ptr<TrackMessage> pTm = this->removeTrackMessage(tm.msg.pdu.header.sender_handle_id);

                // Report local timeout of the message
                ALOGD("[TransportServices] Notify error: LOCAL_TIMEOUT on sent request");

                // Create error pdu
                Pdu errorPdu;
                generateErrorPdu(errorPdu, pTm->msg.pdu, TransportServices::ErrorCode::TIMEOUT, 0);

                // Create message and set pdu.
                Message m(std::move(errorPdu));
                m.ecu = Message::IHU;  // Set to IHU to indicate locally generated error message
                assert(m_errorOnRequestCb);
                m_errorOnRequestCb(m, LOCAL_TIMEOUT);

                // pTm will go out of scope and die here
            } else {
                std::chrono::milliseconds timeoutValue;
                if (m_useWfaTimer) {
                    // Since the response timed out we restart the send cycle. I.e. send message again and wait for ACK
                    timeoutValue = tm.wfa.getTimeoutValue();
                    tm.state = TrackMessage::WAIT_FOR_REQUEST_ACK;
                } else {
                    // Since the response timed out we restart the send cycle. I.e. send message again and wait for
                    // RESPONSE
                    timeoutValue = tm.wfr.getTimeoutValue();
                    tm.state = TrackMessage::WAIT_FOR_REQUEST_RESPONSE;
                }
                // Resend message with updated timeout
                tm.timer = timeProvider.EnqueueWithDelay(
                        timeoutValue,
                        [ this, &tm, id = tm.msg.pdu.header.sender_handle_id ]() { messageTimeout(tm, id); });

                this->sendPdu(tm.msg.ecu, tm.msg.pdu);
            }

            break;

        default:
            // Do nothing
            ALOGW("[TransportServices] Unknown time out reason for: %s, state: 0x%02X",
                  Pdu::toString(tm.msg.pdu).c_str(),
                  tm.state);
            break;
    }
}

const char* TransportServices::ErrorTypeToCString(ErrorType error_type) {
    switch (error_type) {
        case ErrorType::OK:
            return "OK";
        case ErrorType::REMOTE_ERROR:
            return "REMOTE_ERROR";
        case ErrorType::LOCAL_TIMEOUT:
            return "LOCAL_TIMEOUT";
        case ErrorType::PAYLOAD_DECODE_ERROR:
            return "PAYLOAD_DECODE_ERROR";
    }
    return "Unknown error type";
}

IDispatcher& TransportServices::getThreadDispatcher() { return threadDispatcher; }

}  // namespace Connectivity
