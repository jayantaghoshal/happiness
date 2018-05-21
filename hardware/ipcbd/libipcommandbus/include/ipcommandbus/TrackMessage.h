/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>
#include <memory>

#include "ipcommandbus/Message.h"
#include "ipcommandbus/TimeoutInfo.h"

namespace Connectivity {
/**
 * Container for a Message and some extra meta data (I.e. timers and states...).
 * Used to keep track of the Pdu throughout the transfer cycle.
 */
class TrackMessage {
  public:
    enum State {
        UNKNOWN,                             ///< Message created, but no state yet
        WAIT_FOR_REQUEST_ACK,                ///< Waiting for server to ACK the request
        WAIT_FOR_SET_REQUEST_NO_RETURN_ACK,  ///< Waiting for server to ACK the set-request-no-return
        WAIT_FOR_REQUEST_RESPONSE,           ///< Waiting for server to send the response
        WAIT_FOR_LOCAL_REQUEST_VALIDATION,   ///< Waiting for local higher layers to validate the incoming request
        WAIT_FOR_LOCAL_RESPONSE,             ///< Waiting for local higher layers to respond to the received request
        WAIT_FOR_RESPONSE_ACK,               ///< Waiting for client side to ACK the response
        WAIT_FOR_NOTIFICATION_ACK,           ///< Waiting for client side to ACK the notification
        DONE                                 ///< Message transfer is complete.
    };

    Message msg;  ///< The Message to track

    State state = UNKNOWN;  ///< Current transfer state of the message
    tarmac::eventloop::IDispatcher::JobId timer;

    // Timeout data
    TimeoutInfo wfa;  ///< Wait for ack info
    TimeoutInfo wfr;  ///< Wait for response info
};

}  // namespace Connectivity
