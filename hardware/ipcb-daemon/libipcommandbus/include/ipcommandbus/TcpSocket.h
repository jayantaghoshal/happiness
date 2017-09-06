/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once
#include <cstdint>
#include <limits>
#include <queue>
#include <deque>
#include "ipcommandbus/socket.h"

namespace Connectivity
{
class TcpSocket final : public Socket
{
public:
    TcpSocket(tarmac::eventloop::IDispatcher & dispatcher, EcuIpMap ecu_ip_map = Socket::defaultEcuMap());
    ~TcpSocket();

    void registerReadReadyCb(std::function<void(void)> readReadyCb);
    void setup(const Message::Ecu& ecu) override;
    void read(std::vector<uint8_t>& buffer, Message::Ecu& ecu) override;
    void writeTo(const std::vector<uint8_t>& buffer, const Message::Ecu& ecu) override;
    void endConnection() override;

private:
    static int reconnectionRetryHandler(std::uint64_t usec, void* userdata);

    void readEventHandler();
    void packetizer();
    void resetConnection();
    void reconnect();

    std::deque<std::uint8_t> input_byte_stream_;
    std::queue<std::vector<std::uint8_t>> read_frame_buffer_;
    std::function<void(void)> ready_cb_ = nullptr;

    //SdEventSourceReference time_event_source_;
    Message::Ecu peer_ecu_;
};

}  // Connectivity
