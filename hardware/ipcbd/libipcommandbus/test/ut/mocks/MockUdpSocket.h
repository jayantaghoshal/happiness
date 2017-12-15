/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_UDPSOCKET_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_UDPSOCKET_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ipcommandbus/isocket.h"

class MockUdpSocket : public Connectivity::ISocket {
  public:
    MOCK_METHOD1(setup, void(const Message::Ecu& ecu));
    MOCK_METHOD1(registerReadReadyCb, void(std::function<void(void)> readReadyCb));
    MOCK_METHOD2(read, void(std::vector<uint8_t>& buffer, Message::Ecu& ecu));
    MOCK_METHOD2(writeTo, void(const std::vector<uint8_t>& buffer, const Message::Ecu& ecu));
    MOCK_METHOD0(endConnection, void());
};

#endif  // _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_UDPSOCKET_H_
