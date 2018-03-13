/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ipcommandbus/socket.h"

#define LOG_TAG "ipcb_socket"
#include <cutils/log.h>

using namespace tarmac::eventloop;

namespace Connectivity {

std::vector<std::pair<Message::Ecu, ISocket::EcuAddress>> Socket::defaultEcuMap() {
    LocalconfigParameters& local_config = LocalconfigParameters::getInstance();
    const std::string local_ip = local_config.getLocalIpAddress();
    const std::string broadcast_ip = local_config.getBroadcastIpAddress();
    const std::string vcm_ip = local_config.getVCMIpAddress();
    const std::string tem_ip = local_config.getTEMIpAddress();
    const std::string dim_ip = local_config.getDIMIpAddress();
    const std::string tcam_ip = local_config.getTCAMIpAddress();
    const std::string vgm_ip = local_config.getVGMIpAddress();

    const uint16_t local_port = local_config.getLocalPort();
    const uint16_t broadcast_port = local_config.getBroadcastPort();
    const uint16_t vcm_port = local_config.getVCMPort();
    const uint16_t tem_port = local_config.getTEMPort();
    const uint16_t dim_port = local_config.getDIMPort();
    const uint16_t tcam_port = local_config.getTCAMPort();
    const uint16_t vgm_port = local_config.getVGMPort();

    return std::vector<std::pair<Message::Ecu, ISocket::EcuAddress>>{
            std::make_pair(Message::Ecu::IHU, ISocket::EcuAddress{local_ip, local_port}),
            std::make_pair(Message::Ecu::ALL, ISocket::EcuAddress{broadcast_ip, broadcast_port}),
            std::make_pair(Message::Ecu::VCM, ISocket::EcuAddress{vcm_ip, vcm_port}),
            std::make_pair(Message::Ecu::TEM, ISocket::EcuAddress{tem_ip, tem_port}),
            std::make_pair(Message::Ecu::DIM, ISocket::EcuAddress{dim_ip, dim_port}),
            std::make_pair(Message::Ecu::TCAM, ISocket::EcuAddress{tcam_ip, tcam_port}),
            std::make_pair(Message::Ecu::VGM, ISocket::EcuAddress{vgm_ip, vgm_port})};
}

Socket::Socket(IDispatcher& dispatcher, int domain, int type, int protocol, EcuIpMap ecu_ip_map)
    : dispatcher_(dispatcher), ecu_ip_map_{ecu_ip_map} {
    backoffReset();
    setup(domain, type, protocol);
}

Socket::~Socket() { teardown(); }

void Socket::set_option(int level, int option, int value) {
    socklen_t length = sizeof value;

    if (setsockopt(socket_fd_, level, option, &value, length) < 0) {
        throw SocketException(errno, "setsockopt() failed");
    }
}

int Socket::get_option(int level, int option) {
    int value;
    socklen_t length = sizeof value;

    if (getsockopt(socket_fd_, level, option, &value, &length) < 0) {
        throw SocketException(errno, "getsockopt() failed");
    }

    return value;
}

int Socket::getIpPrecedenceValue(IpPrecedence ipPrecedence) {
    int ipPrecedence_value = 0;
    switch (ipPrecedence) {
        case IpPrecedence::kRoutine:
            ipPrecedence_value = 0;
            break;
        case IpPrecedence::kPriority:
            ipPrecedence_value = 32;
            break;
        case IpPrecedence::kImmediate:
            ipPrecedence_value = 64;
            break;
        case IpPrecedence::kFlash:
            ipPrecedence_value = 96;
            break;
        case IpPrecedence::kFlashOverride:
            ipPrecedence_value = 128;
            break;
        case IpPrecedence::kCriticEcp:
            ipPrecedence_value = 160;
            break;
        case IpPrecedence::kInternetworkControl:
            ipPrecedence_value = 192;
            break;
        case IpPrecedence::kNetworkControl:
            ipPrecedence_value = 224;
            break;
        default:
            assert(false);
            break;
    }
    return ipPrecedence_value;
}

void Socket::setup(int domain, int type, int protocol) {
    socket_fd_ = socket(domain, type, protocol);
    if (socket_fd_ < 0) {
        throw SocketException(errno, "Socket creation failed ...");
    }

    set_option(
            IPPROTO_IP, IP_TOS, getIpPrecedenceValue(LocalconfigParameters::getInstance().getNetworkControlPriority()));

    dispatcher_.AddFd(socket_fd_, [this] {
        if (read_ready_cb_) read_ready_cb_();
    });
    ALOGD("Socket created %d/%d/%d", domain, type, protocol);
}

void Socket::teardown() {
    if (socket_fd_ > 0) {
        dispatcher_.RemoveFd(socket_fd_);
        close(socket_fd_);
        socket_fd_ = -1;
    }
}

void Socket::setHandler(std::function<void(void)> readEventHandler) { read_ready_cb_ = std::move(readEventHandler); }

uint32_t Socket::getTestSimPort() {
    uint32_t test_port = local_config.getTestPort();
    return test_port;
}

void Socket::backoffReset() { backoff_timeout_ = std::chrono::milliseconds(100); }

// get the current backoff timeout and then backoff with a factor 2 upto a max value
std::chrono::milliseconds Socket::backoffGet() {
    const auto result = backoff_timeout_;
    backoff_timeout_ = std::min(2 * backoff_timeout_, std::chrono::milliseconds(2000));
    return result;
}
}
