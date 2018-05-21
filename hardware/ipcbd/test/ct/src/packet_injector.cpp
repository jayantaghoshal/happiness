/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <arpa/inet.h>

#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>  //exit(0);
#include <cstring>  //memset
#include <iostream>
#include <stdexcept>

#include "packet_injector.h"

#undef LOG_TAG
#define LOG_TAG "PacketInjector"
#include <cutils/log.h>

namespace vcc {
namespace ipcb {
namespace testing {

PacketInjector::PacketInjector(const std::string& local_ip,
                               const uint32_t& local_port,
                               const std::string& remote_ip,
                               const uint32_t& remote_port,
                               const int& enable_broadcast)
    : local_ip_(local_ip),
      local_port_(local_port),
      remote_ip_(remote_ip),
      remote_port_(remote_port),
      enable_broadcast_(enable_broadcast),
      timer_{tarmac::eventloop::IDispatcher::GetDefaultDispatcher()} {
    ALOGD("+ %s", __func__);
    addrlen_ = static_cast<socklen_t>(sizeof(struct sockaddr_in));

    // Setup local socket socket
    if (-1 == (local_socket_ = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP))) {
        ALOGE("Failed to create socket: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (-1 == setsockopt(local_socket_, SOL_SOCKET, SO_BROADCAST, &enable_broadcast_, sizeof(enable_broadcast_))) {
        ALOGE("Failed to set up socket: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(&remote_addr_, 0, sizeof(remote_addr_));
    remote_addr_.sin_family = AF_INET;
    remote_addr_.sin_port = htons(remote_port_);
    if (0 == inet_pton(AF_INET, remote_ip_.c_str(), &remote_addr_.sin_addr)) {
        ALOGE("Address is not valid.");
        exit(EXIT_FAILURE);
    }

    // Bind local socket to local port
    memset(&local_addr_, 0, sizeof(local_addr_));
    local_addr_.sin_family = AF_INET;
    local_addr_.sin_port = htons(local_port_);

    if (1 != inet_pton(AF_INET, local_ip_.c_str(), &local_addr_.sin_addr)) {
        ALOGE("Address is not valid.");
        exit(EXIT_FAILURE);
    }

    if (-1 == bind(local_socket_, reinterpret_cast<struct sockaddr*>(&local_addr_), sizeof(local_addr_))) {
        ALOGE("Failed to bind socket: %s", std::strerror(errno));
        exit(EXIT_FAILURE);
    }
    ALOGD("- %s", __func__);
}

PacketInjector::~PacketInjector() {
    if (local_socket_ >= 0) {
        if (-1 == shutdown(local_socket_, SHUT_RDWR)) {
            ALOGE("Failed to shutdown socket: %s", std::strerror(errno));
        }
        if (-1 == close(local_socket_)) {
            ALOGE("Failed to close socket: %s", std::strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

bool PacketInjector::SendPdu(const Connectivity::Pdu& pdu) {
    ALOGD("+ %s", __func__);

    buffer_.clear();
    pdu.toData(buffer_);

    if (sendto(local_socket_,
               reinterpret_cast<const void*>(&buffer_[0]),
               buffer_.size(),
               0,
               reinterpret_cast<struct sockaddr*>(&remote_addr_),
               static_cast<socklen_t>(sizeof(remote_addr_))) == -1) {
        ALOGE("sendto failed with error: %s", strerror(errno));
        return false;
    }
    ALOGD("- %s", __func__);
    return true;
}

bool PacketInjector::ReceivePdu(Connectivity::Pdu& pdu, const unsigned int& timeout_sec) {
    ALOGD("+ %s", __func__);

    if (timeout_sec > 0) {
        struct timeval tv;
        tv.tv_sec = timeout_sec;
        tv.tv_usec = 0;
        if (-1 == setsockopt(local_socket_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))) {
            ALOGE("Failed to set up socket with timeout: %s", std::strerror(errno));
            return false;
        }
    }

    std::vector<uint8_t> rd_buffer;

    ssize_t packet_length = recv(local_socket_, nullptr, 0, MSG_PEEK | MSG_TRUNC);
    if (packet_length <= 0) {
        ALOGE("recv failed with error: %s", strerror(errno));
        return false;
    }

    rd_buffer.resize(packet_length);

    if (0 > recvfrom(local_socket_,
                     static_cast<void*>(rd_buffer.data()),
                     packet_length,
                     0,
                     reinterpret_cast<struct sockaddr*>(&local_addr_),
                     &addrlen_)) {
        ALOGE("recvfrom failed with error: %s", strerror(errno));
        return false;
    }

    pdu.fromData(rd_buffer);
    ALOGD("- %s", __func__);
    return true;
}

}  // namespace testing
}  // namespace ipcb
}  // namespace vcc