/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <cstdint>
#include <limits>
#include <queue>
#include "ipcommandbus/socket.h"

namespace Connectivity {
/**
 * Wraps some socket functions as creation, read and write.
 */
class UdpSocket final : public Socket {
  public:
    UdpSocket(tarmac::eventloop::IDispatcher& dispatcher, EcuIpMap ecu_ip_map = Socket::defaultEcuMap());
    ~UdpSocket();

    void setup(const Message::Ecu& ecu) override;
    void read(std::vector<uint8_t>& buffer, Message::Ecu& ecu) override;
    void writeTo(const std::vector<uint8_t>& buffer, const Message::Ecu& ecu) override;

  private:
    void readEventHandler();
    void registerReadReadyCb(std::function<void(void)> readReadyCb);
    void resetup();

    std::queue<std::pair<std::vector<std::uint8_t>, Message::Ecu> > read_frame_buffer_;
    std::function<void(void)> read_cb_ = nullptr;
    Message::Ecu ecu_;
};

}  // Connectivity
