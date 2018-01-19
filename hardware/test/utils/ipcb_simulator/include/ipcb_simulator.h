/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ipcommandbus/Pdu.h"

using namespace Connectivity;

class IpcbSimulator {
  public:
    IpcbSimulator(std::string local_ip, uint32_t local_port, uint32_t remote_port, int bcast_enable);
    ~IpcbSimulator();

    bool SendPdu(Pdu pdu);
    bool ReceivePdu(Pdu& pdu);
    void CreateAndSendIpActivityMessage();  // TODO: Consider removing, this is per implementation specific code
    void CloseSocket();

  private:
    void Setup();
    void Read();
    void StartActivityMessageTimer();  // TODO: Consider removing, this is per implementation specific code
    void StopActivityMessageTimer();   // TODO: Consider removing, this is per implementation specific code

    std::string local_ip_;
    uint32_t local_port_;   // The default port from which to send data
    uint32_t remote_port_;  // Port to which to send data
    int broadcastEnable_;
    int local_socket_;
    std::uint8_t sequenceId_ = 0;
    std::vector<uint8_t> buffer_;

    struct sockaddr_in remote_addr_;
    struct sockaddr_in local_addr_;
    socklen_t addrlen_;

    tarmac::eventloop::IDispatcher& timer_;
    tarmac::eventloop::IDispatcher::JobId activityPacketInjectorId_;
};
