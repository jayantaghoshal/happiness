/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <arpa/inet.h>
#include <cutils/log.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>  //exit(0);
#include <string.h>  //memset
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>

#include "include/ipcb_simulator.h"

#define LOG_TAG "IpcbSimulator"

using ::tarmac::eventloop::IDispatcher;

IpcbSimulator::IpcbSimulator(std::string local_ip, uint32_t local_port, uint32_t remote_port, int bcast_enable)
    : local_ip_(local_ip),
      local_port_(local_port),
      remote_port_(remote_port),
      broadcastEnable_(bcast_enable),
      timer_{IDispatcher::GetDefaultDispatcher()} {
    ALOGD("Packet Injector started");
    IpcbSimulator::Setup();
}

IpcbSimulator::~IpcbSimulator() {
    if (local_socket_ != 0) {
        close(local_socket_);
    }
}

void IpcbSimulator::Setup() {
    addrlen_ = static_cast<socklen_t>(sizeof(struct sockaddr_in));

    // Setup local socket socket
    if ((local_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        ALOGE("Failed to create local socket, terminating!");
        exit(1);
    }

    int ret = setsockopt(local_socket_, SOL_SOCKET, SO_BROADCAST, &broadcastEnable_, sizeof(broadcastEnable_));

    memset(&remote_addr_, 0, sizeof(remote_addr_));
    remote_addr_.sin_family = AF_INET;
    remote_addr_.sin_port = htons(remote_port_);
    if (inet_aton(local_ip_.c_str(), &remote_addr_.sin_addr) == 0) {
        ALOGE("inet_aton() failed, terminating!");
        exit(1);
    }

    // Bind local socket to local port
    memset(&local_addr_, 0, sizeof(local_addr_));
    local_addr_.sin_family = AF_INET;
    local_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr_.sin_port = htons(local_port_);

    if (bind(local_socket_, (struct sockaddr*)&local_addr_, sizeof(local_addr_)) < 0) {
        ALOGE("Bind error, terminating!");
        exit(1);
    }
}

void IpcbSimulator::CreateAndSendIpActivityMessage() {
    Pdu pdu;
    ALOGD("Send Activity Message");
    pdu.createHeader(0xFFFF, 0xFF01, IpCmdTypes::OperationType::NOTIFICATION_CYCLIC, IpCmdTypes::DataType::NOT_ENCODED,
                     sequenceId_);

    pdu.header.protocol_version = 2;
    sequenceId_++;

    pdu.setPayload(std::vector<uint8_t>({0x01, (uint8_t)0x00, 0, 0}));

    buffer_.clear();
    pdu.toData(buffer_);
    if (sendto(local_socket_, reinterpret_cast<const void*>(&buffer_[0]), buffer_.size(), 0,
               reinterpret_cast<struct sockaddr*>(&remote_addr_), static_cast<socklen_t>(sizeof(remote_addr_))) == -1) {
        ALOGD("Failed to send UDP packet!");
    }

    StartActivityMessageTimer();
}

void IpcbSimulator::StartActivityMessageTimer() {
    activityPacketInjectorId_ =
            timer_.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { CreateAndSendIpActivityMessage(); });
}

void IpcbSimulator::StopActivityMessageTimer() { timer_.Cancel(activityPacketInjectorId_); }

bool IpcbSimulator::SendPdu(Pdu pdu) {
    ALOGD("Send pdu");

    buffer_.clear();
    pdu.toData(buffer_);

    if (sendto(local_socket_, reinterpret_cast<const void*>(&buffer_[0]), buffer_.size(), 0,
               reinterpret_cast<struct sockaddr*>(&remote_addr_), static_cast<socklen_t>(sizeof(remote_addr_))) == -1) {
        ALOGE("sendto failed with error: %s", strerror(errno));
        return false;
    }
    return true;
}

bool IpcbSimulator::ReceivePdu(Pdu& pdu, const uint32_t timeout_sec) {
    ALOGD("Rcv pdu");

    if (timeout_sec > 0) {
        struct timeval tv;
        tv.tv_sec = timeout_sec;
        tv.tv_usec = 0;
        if (setsockopt(local_socket_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
            ALOGE("Error setting timeout on socket");
        }
    }

    std::vector<uint8_t> rd_buffer;

    ssize_t packet_length = recv(local_socket_, nullptr, 0, MSG_PEEK | MSG_TRUNC);
    if (packet_length <= 0) {
        ALOGE("recv failed with error: %s", strerror(errno));
        return false;
    }

    rd_buffer.resize(packet_length);

    if (0 > recvfrom(local_socket_, static_cast<void*>(rd_buffer.data()), packet_length, 0,
                     reinterpret_cast<struct sockaddr*>(&local_addr_), &addrlen_)) {
        ALOGE("recvfrom failed with error: %s", strerror(errno));
        return false;
    }

    pdu.fromData(rd_buffer);
    return true;
}

void IpcbSimulator::CloseSocket() {
    int result = shutdown(local_socket_, SHUT_RDWR);
    ALOGD("IpcbSimulator::CloseSocket: %d", result);
}
