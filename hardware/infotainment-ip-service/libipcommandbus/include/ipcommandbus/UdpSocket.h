/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once
#include <cstdint>
#include <limits>
#include "ipcommandbus/socket.h"

namespace Connectivity
{
/**
 * Wraps some socket functions as creation, read and write.
 */
class UdpSocket final : public Socket
{
public:
    UdpSocket(tarmac::eventloop::IDispatcher & dispatcher, EcuIpMap ecu_ip_map = Socket::defaultEcuMap());
    ~UdpSocket();

    void setup(const Message::Ecu& ecu) override;
    void read(std::vector<uint8_t>& buffer, Message::Ecu& ecu) override;
    void writeTo(const std::vector<uint8_t>& buffer, const Message::Ecu& ecu) override;

private:
    void registerReadReadyCb(std::function<void(void)> readReadyCb);

    std::function<void(void)> read_cb_ = nullptr;
};

}  // Connectivity
