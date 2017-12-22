/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ipcommandbus/UdpSocket.h"

#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <cassert>
#include <string>

#define LOG_TAG "UDP_Socket"
#include <cutils/log.h>

using namespace tarmac::eventloop;

namespace Connectivity {

UdpSocket::UdpSocket(IDispatcher &dispatcher, EcuIpMap ecu_ip_map)
    : Socket(dispatcher, AF_INET, SOCK_DGRAM, IPPROTO_UDP, ecu_ip_map) {
    setHandler([this] { readEventHandler(); });
}

UdpSocket::~UdpSocket() {
    // flush buffers as well
}

void UdpSocket::setup(const Message::Ecu &ecu) {
    // NOTE! Unlike TCP, the ecu provided here is used to setup the local port.
    // Therefore it is only allowed to use ALL (for broadcast socket) and IHU (for
    // normal socket)

    if ((Message::Ecu::ALL == ecu) || (Message::Ecu::IHU == ecu)) {
        // Look up IHU info from ECU map
        auto it = std::find_if(
                ecu_ip_map_.begin(), ecu_ip_map_.end(),
                [](const std::pair<Message::Ecu, EcuAddress> &pair) { return pair.first == Message::Ecu::IHU; });

        if (it == ecu_ip_map_.end()) {
            ALOGW("IHU not found in ECU map!");
            return;
        }

        std::string local_ip = it->second.ip;
        uint16_t local_port = it->second.port;

        // If we are setting up a broadcast socket
        if (Message::Ecu::ALL == ecu) {
            // Look up ALL (broadcast) info from ECU map
            it = std::find_if(
                    ecu_ip_map_.begin(), ecu_ip_map_.end(),
                    [](const std::pair<Message::Ecu, EcuAddress> &pair) { return pair.first == Message::Ecu::ALL; });

            if (it == ecu_ip_map_.end()) {
                ALOGW("\"ALL\" (broadcast) not found in ECU map!");
                return;
            }

            // Use broadcast address for broadcast socket
            local_ip = it->second.ip;

            set_option(SOL_SOCKET, SO_BROADCAST, true);
        }

        struct sockaddr_in sa;
        sa.sin_family = AF_INET;
        sa.sin_port = htons(local_port);

        ALOGD("UdpSocket setup %s:%d - ecu %d", local_ip.c_str(), local_port, ecu);

        if (1 != inet_pton(AF_INET, local_ip.c_str(), &sa.sin_addr)) {
            throw SocketException(errno, "Address not in correct format");
        }

        const int bindStatus =
                bind(socket_fd_, reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)));
        if (0 != bindStatus) {
            throw SocketException(errno, "Failed to bind to socket");
        }

        ecu_ = ecu;
    } else {
        ALOGE("Only allowed to setup UDP socket with ecu = ALL or IHU! (rejected : "
              "%d)",
              ecu);
    }
}

void UdpSocket::registerReadReadyCb(std::function<void(void)> readReadyCb) { read_cb_ = std::move(readReadyCb); }

void UdpSocket::read(std::vector<uint8_t> &buffer, Message::Ecu &ecu) {
    buffer.clear();
    ecu = Message::Ecu::UNKNOWN;
    if (!read_frame_buffer_.empty()) {
        auto item = read_frame_buffer_.front();
        read_frame_buffer_.pop();
        buffer = item.first;
        ecu = item.second;
    }
}

void UdpSocket::writeTo(const std::vector<uint8_t> &buffer, const Message::Ecu &ecu) {
    // Protect from trying to send to a bad ECU
    if (Message::Ecu::UNKNOWN == ecu) {
        ALOGE("Can not send to UNKNOWN socket!");
        return;
    }

    // Protect from trying to send to self
    if (Message::Ecu::IHU == ecu) {
        ALOGE("Can not send to self!");
        return;
    }

    // Protect from trying to send broadcast from normal socket
    if (Message::Ecu::ALL == ecu) {
        if (Message::Ecu::ALL != ecu_) {
            ALOGE("Can not send broadcast on ordinary socket! (socket set up for %d "
                  "and trying to send to %d",
                  ecu_, ecu);

            return;
        }
    }

    // Protect from trying to send normal message from broadcast socket
    if (Message::Ecu::ALL != ecu) {
        if (Message::Ecu::ALL == ecu_) {
            ALOGE("Can not send normal packet on broadcast socket! (socket set up "
                  "for %d and trying to send to %d",
                  ecu_, ecu);
            return;
        }
    }

    auto it = std::find_if(ecu_ip_map_.begin(), ecu_ip_map_.end(),
                           [ecu](const std::pair<Message::Ecu, EcuAddress> &pair) { return pair.first == ecu; });

    if (it == ecu_ip_map_.end()) {
        return;
    }

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(it->second.port);

    if (1 != inet_pton(AF_INET, it->second.ip.c_str(), &sa.sin_addr)) {
        throw SocketException(errno, "Address not in correct format");
    }

    auto remaining_bytes = buffer.size();
    int sent_bytes = 0;

    while (remaining_bytes > 0) {
        int ret = -1;
        ret = sendto(socket_fd_, reinterpret_cast<const void *>(&buffer[sent_bytes]), remaining_bytes, 0,
                     reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)));

        if (-1 == ret) {
            if (errno == EINTR) {
                // try to continue
                continue;
            }
            if (errno == EPIPE || errno == ECONNRESET || errno == ETIMEDOUT) {
                // connection is broken. reconnection will be triggered on read. What
                // should happen with reqested
                // buffer? throw away? as WFR will be triggered and packet will be
                // resent
                break;
            }
            throw SocketException(errno, "Failed to send packet");
        }
        sent_bytes += ret;
        remaining_bytes -= ret;
    }
}

void UdpSocket::readEventHandler() {
    ssize_t packet_length = recv(socket_fd_, nullptr, 0, MSG_PEEK | MSG_TRUNC);
    if (packet_length <= 0) {
        // socket suddenly failed. Could be a wakeup from suspend.
        dispatcher_.EnqueueWithDelay(std::chrono::milliseconds(200), [this] { resetup(); });
        return;
    }

    std::vector<uint8_t> buffer(packet_length);

    struct sockaddr_in sa_out;

    socklen_t addrlen = static_cast<socklen_t>(sizeof(sa_out));
    memset(static_cast<void *>(&sa_out), 0, addrlen);
    sa_out.sin_family = AF_INET;

    if (0 > recvfrom(socket_fd_, static_cast<void *>(buffer.data()), packet_length, 0,
                     reinterpret_cast<struct sockaddr *>(&sa_out), &addrlen)) {
        // socket suddenly failed. Could be a wakeup from suspend.
        dispatcher_.EnqueueWithDelay(std::chrono::milliseconds(200), [this] { resetup(); });
        return;
    }

    char buf[INET_ADDRSTRLEN];
    const char *const bufConst = inet_ntop(AF_INET, &sa_out.sin_addr, buf, INET_ADDRSTRLEN);
    if (bufConst == nullptr || bufConst != buf) {
        ALOGW("Address not in correct format");
        return;
    }

    const std::string fromIp = std::string(buf);
    const auto fromPort = ntohs(sa_out.sin_port);

    auto it = std::find_if(ecu_ip_map_.begin(), ecu_ip_map_.end(),
                           [&fromIp, fromPort](const std::pair<Message::Ecu, EcuAddress> &pair) {
                               return (pair.second.ip == fromIp && pair.second.port == fromPort);
                           });

    auto ecu = (it != ecu_ip_map_.end()) ? it->first : Message::Ecu::UNKNOWN;
    read_frame_buffer_.emplace(std::make_pair(buffer, ecu));

    if (read_cb_ && !read_frame_buffer_.empty()) {
        read_cb_();
    }
}

void UdpSocket::resetup() {
    try {
        teardown();
        Socket::setup(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        setup(ecu_);
        backoffReset();
    } catch (const SocketException &e) {
        dispatcher_.EnqueueWithDelay(backoffGet(), [this] { resetup(); });
    }
}
}
