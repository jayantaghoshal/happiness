/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "ipcommandbus/socket.h"
#include <cutils/log.h>

#define LOG_TAG "ipcb_socket"

using namespace tarmac::eventloop;

namespace Connectivity {

std::vector<std::pair<Message::Ecu, ISocket::EcuAddress>> Socket::defaultEcuMap() {
    LocalconfigParameters& local_config = LocalconfigParameters::getInstance();
    const std::string local_ip = local_config.getLocalIpAddress();
    const std::string local_broadcast = local_config.getBroadcastIpAddress();
    const std::string vcm_ip = local_config.getVCMIpAddress();
    const std::string tem_ip = local_config.getTEMIpAddress();
    const std::string dim_ip = local_config.getDIMIpAddress();
    const std::string tcam_ip = local_config.getTCAMIpAddress();
    const std::string vgm_ip = local_config.getVGMIpAddress();

    const uint16_t local_port = local_config.getLocalPort();
    const uint16_t local_broadcast_port = local_config.getLocalBroadcastPort();
    const uint16_t vcm_port = local_config.getVCMPort();
    const uint16_t tem_port = local_config.getTEMPort();
    const uint16_t dim_port = local_config.getDIMPort();
    const uint16_t tcam_port = local_config.getTCAMPort();
    const uint16_t vgm_port = local_config.getVGMPort();

    return std::vector<std::pair<Message::Ecu, ISocket::EcuAddress>>{
            std::make_pair(Message::Ecu::IHU, ISocket::EcuAddress{local_ip, local_port}),
            std::make_pair(Message::Ecu::ALL, ISocket::EcuAddress{local_broadcast, local_broadcast_port}),
            std::make_pair(Message::Ecu::VCM, ISocket::EcuAddress{vcm_ip, vcm_port}),
            std::make_pair(Message::Ecu::TEM, ISocket::EcuAddress{tem_ip, tem_port}),
            std::make_pair(Message::Ecu::DIM, ISocket::EcuAddress{dim_ip, dim_port}),
            std::make_pair(Message::Ecu::TCAM, ISocket::EcuAddress{tcam_ip, tcam_port}),
            std::make_pair(Message::Ecu::VGM, ISocket::EcuAddress{vgm_ip, vgm_port})};
}

Socket::Socket(IDispatcher& dispatcher, int domain, int type, int protocol, EcuIpMap ecu_ip_map)
    : dispatcher_(dispatcher), ecu_ip_map_{ecu_ip_map} {
    setup(domain, type, protocol);
}

Socket::~Socket() {
    if (socket_fd_ > 0) {
        // Not advised to handle EINTR error for close
        close(socket_fd_);
        socket_fd_ = -1;
    }
}

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

    set_option(IPPROTO_IP, IP_TOS,
               getIpPrecedenceValue(LocalconfigParameters::getInstance().getNetworkControlPriority()));

    /*sd_event_source *source;
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket_fd_;
    int sdstatus = sd_event_add_io(&sd_event_, &source, socket_fd_, event.events, wakeup_cb, this);

    assert_sd_throw(sdstatus >= 0, "Failed to add socket fd to event loop");
    io_event_source_.reset(source);*/
    dispatcher_.AddFd(socket_fd_, [this]() {
        if (this->read_ready_cb_) this->read_ready_cb_();
    });
    ALOGD("Socket created %d/%d/%d", domain, type, protocol);
}

void Socket::setHandler(std::function<void(void)> readEventHandler) { read_ready_cb_ = std::move(readEventHandler); }

uint32_t Socket::getTestSimPort() {
    uint32_t test_port = local_config.getTestPort();
    return test_port;
}
}
