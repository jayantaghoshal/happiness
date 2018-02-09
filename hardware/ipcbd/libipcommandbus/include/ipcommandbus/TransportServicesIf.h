/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <IDispatcher.h>
#include "ipcommandbus/TrackMessage.h"
#include "ipcommandbus/idiagnostics_client.h"
#include "ipcommandbus/isocket.h"

namespace Connectivity {

/**
 * Transport layer implementation of the IP command bus.
 */
class ITransportServices {
  public:
    virtual ~ITransportServices() = default;

    /**
     * Possible error codes that can be sent to/received from peer.
     * Note that the codes are ranked and a lower value have precedence over a higher one.
     */
    enum ErrorCode {
        NOT_OK = 0x00,  ///< Generic error
        INVALID_SERVICE_ID = 0x01,
        INVALID_OPERATION_ID = 0x02,
        INVALID_OPERATION_TYPE = 0x03,
        INVALID_PROTOCOL_VERSION = 0x04,
        SEGMENTATION_ERROR = 0x05,
        INVALID_LENGTH = 0x06,
        APPLICATION_ERROR = 0x07,
        TIMEOUT_ERROR = 0x08,
        BUSY_ERROR = 0x09
    };

    enum ErrorType {
        OK,
        REMOTE_ERROR,
        LOCAL_TIMEOUT,
        PAYLOAD_DECODE_ERROR  // (For use by service specific layer.)
    };

    virtual tarmac::eventloop::IDispatcher& getThreadDispatcher() = 0;

    /**
    * Sets the Diagnostics object to be used by TransportServices
    *
    * @param[in] diagnostics                pointer to an IDiagnostics object
    */
    virtual void setDiagnostics(IDiagnosticsClient* diagnostics, tarmac::eventloop::IDispatcher* dispatcher) = 0;

    /**
     * Register a callback that will be called upon well formed incoming
     * request / set request / set request no return / notification request.
     * The return value from the callback determine whether an ACK shall be sent or not.
     * If false is returned, TransportServices is told that a specific ERROR message have been sent and no ACK shall be
     * emitted.
     *
     * @param[in] messageCb                 The callback.
     */
    virtual void registerIncomingRequestCallback(std::function<bool(Message&)> messageCb) = 0;

    /**
     * Register a callback that will be called upon well formed incoming notification / cyclic notification.
     *
     * @param[in] messageCb                 The callback.
     */
    virtual void registerIncomingNotificationCallback(std::function<void(Message&)> messageCb) = 0;

    /**
     * Register a callback that will be called upon well formed incoming response.
     *
     * @param[in] messageCb                 The callback.
     */
    virtual void registerIncomingResponseCallback(std::function<void(Message&)> messageCb) = 0;

    /**
     * Register a callback that will be called upon any error / timeout related to previously sent REQUEST message.
     *
     * @param[in] messageCb                 The callback.
     */
    virtual void registerErrorOnRequestCallback(std::function<void(Message&, ErrorType)> messageCb) = 0;

    /**
     * Sends a message to the default target (set in the assigned socket).
     *
     * @param[in] msg                       Message to send. TransportServices takes ownership of the message.
     */
    virtual void sendMessage(Message&& msg) = 0;

    /**
     * Send an error for the given senderHandleId.
     *
     * @param[in] destination               Where to send the Pdu.
     * @param[in] pdu                       The faulty Pdu that caused the error state.
     * @param[in] errorCode                 The error to transmit.
     * @param[in] errorInfo                 Additional error information
     */
    virtual void sendError(Message::Ecu destination, const Pdu& pdu, ErrorCode errorCode, uint16_t errorInfo = 0) = 0;
};

}  // Connectivity
