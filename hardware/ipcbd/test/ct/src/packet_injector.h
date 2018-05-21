/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ipcommandbus/Pdu.h"

namespace vcc {
namespace ipcb {
namespace testing {

class PacketInjector {
  public:
    explicit PacketInjector(const std::string& local_ip,
                            const unsigned int& local_port,
                            const std::string& remote_ip,
                            const unsigned int& remote_port,
                            const int& enable_broadcast);
    ~PacketInjector();

    bool SendPdu(const Connectivity::Pdu& pdu);
    bool ReceivePdu(Connectivity::Pdu& pdu, const unsigned int& timeout_sec = 0);

  private:
    void Read();

    std::string local_ip_;
    unsigned int local_port_;  // The default port from which to send data
    std::string remote_ip_;
    unsigned int remote_port_;  // Port to which to send data
    int enable_broadcast_;
    int local_socket_;
    std::vector<uint8_t> buffer_;

    struct sockaddr_in remote_addr_;
    struct sockaddr_in local_addr_;
    socklen_t addrlen_;

    tarmac::eventloop::IDispatcher& timer_;
};
}  // namespace testing
}  // namespace ipcb
}  // namespace vcc