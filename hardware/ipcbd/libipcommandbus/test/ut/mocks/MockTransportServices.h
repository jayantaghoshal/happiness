/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_TRANSPORTSERVICES_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_TRANSPORTSERVICES_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "connectivity-sd/thread_dispatcher.h"
#include "ipcommandbus/Message.h"
#include "ipcommandbus/TransportServicesIf.h"

class MockTransportServices : public Connectivity::ITransportServices {
  public:
    MOCK_METHOD1(registerIncomingRequestCallback, void(std::function<bool(Connectivity::Message &)> messageCb));
    MOCK_METHOD1(registerIncomingNotificationCallback, void(std::function<void(Connectivity::Message &)> messageCb));
    MOCK_METHOD1(registerIncomingResponseCallback, void(std::function<void(Connectivity::Message &)> messageCb));
    MOCK_METHOD1(
            registerErrorCallback,
            void(std::function<void(Connectivity::Message &, Connectivity::ITransportServices::ErrorType)> messageCb));

    MOCK_METHOD1(
            registerErrorOnRequestCallback,
            void(std::function<void(Connectivity::Message &, Connectivity::ITransportServices::ErrorType)> messageCb));

    MOCK_METHOD2(setDiagnostics, void(Connectivity::IDiagnosticsClient *diagnostics, ThreadDispatcher *dispatcher));

    MOCK_METHOD0(getThreadDispatcher, Connectivity::ThreadDispatcher &());

    MOCK_METHOD1(sendMessage_mocked, void(Connectivity::Message &msg));
    void sendMessage(Connectivity::Message &&msg) override { sendMessage_mocked(msg); }

    MOCK_METHOD4(sendError, void(Connectivity::Message::Ecu, const Connectivity::Pdu &, ErrorCode, uint16_t));
};

#endif  // _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_TRANSPORTSERVICES_H_
