/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cerrno>

#include <IDispatcher.h>
#include <android-base/macros.h>
#include <linux/netlink.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <fstream>
#include "uevent_handler.h"
#include "uevent_listener.h"

#define LOG_TAG "Netmand"
#include <cutils/log.h>

namespace vcc {
namespace netman {

UEventListener& UEventListener::Instance() {
    static UEventListener instance;
    return instance;
}

UEventListener::~UEventListener() { StopListening(); }

bool UEventListener::StartListening() {
    if (nullptr == event_handler_) {
        ALOGE("Netlink event handler not set on Netlink socket listener.");
        return false;
    }

    if (SetupSocket() == -1) {
        ALOGE("Netlink event handler failed to setup socket");
        return false;
    }

    tarmac::eventloop::IDispatcher::GetDefaultDispatcher().AddFd(netlink_socket_, [this] { RecvMessage(); });
    UeventHandler::SysfsNetSubsystemWalker();

    return true;
}

void UEventListener::StopListening() {
    if (netlink_socket_ != -1) {
        close(netlink_socket_);
        netlink_socket_ = -1;
    }
}

void UEventListener::SetNetlinkEventHandler(UeventHandler& event_handler) { event_handler_ = &event_handler; }

int UEventListener::SetupSocket() {
    if (netlink_socket_ != -1) {
        ALOGE("Trying to setup the already opened netlink socket.");
        return 0;
    }

    struct sockaddr_nl nladdr;

    memset(&nladdr, 0, sizeof(nladdr));
    nladdr.nl_family = AF_NETLINK;
    nladdr.nl_pid = getpid();
    nladdr.nl_groups = -1;

    if ((netlink_socket_ = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT)) < 0) {
        ALOGE("Unable to create netlink socket: %s", strerror(errno));
        return -1;
    }

    int on = 1;
    if (setsockopt(netlink_socket_, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on)) != 0) {
        ALOGE("Failed to set credentials option for socket %s", strerror(errno));
        return -1;
    }

    if (bind(netlink_socket_, reinterpret_cast<struct sockaddr*>(&nladdr), sizeof(nladdr)) < 0) {
        ALOGE("Unable to bind netlink socket: %s", strerror(errno));
        close(netlink_socket_);
        return -1;
    }

    return 0;
}

// TODO (Abhijeet Shirolikar): This function should not return plain int. Need to define enum for error codes
int UEventListener::RecvMessage() {
    const int kRecvBufferSize = 4096;
    char buf[kRecvBufferSize] = {0};
    struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf) - 1};
    char cred_control[CMSG_SPACE(sizeof(struct ucred))];
    struct sockaddr_nl sa;
    struct msghdr msg = {.msg_name = static_cast<void*>(&sa),
                         .msg_namelen = sizeof(sa),
                         .msg_iov = &iov,
                         .msg_iovlen = 1,
                         .msg_control = cred_control,
                         .msg_controllen = sizeof(cred_control),
                         .msg_flags = 0};

    int message_length;
    message_length = TEMP_FAILURE_RETRY(recvmsg(netlink_socket_, &msg, 0));

    if (message_length <= 0) {
        ALOGE("Error receiving message from Netlink socket: %s", strerror(errno));
        return -1;
    }

    // NOTE: Code below is to safe guard us from changes in future.
    // Strictly speaking CMSG loop below is not needed for present code where we only have enabled single auxillary
    // message to request credentials. But to avoid the trap in future where we may enable additional auxillary
    // messages, below code is needed.
    struct ucred* cred = nullptr;
    for (struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg); cmsg != nullptr; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
        if (cmsg->cmsg_type == SCM_CREDENTIALS) {
            cred = reinterpret_cast<struct ucred*>(CMSG_DATA(cmsg));
            break;
        }
    }

    if (cred == nullptr) {
        ALOGV("Message received without sender's credentials. Ignored");
        return 0;  // DO NOT return error. just ignore and let RecvMessage loop continue
    }

    if (cred->uid != 0) {
        ALOGV("Ignoring message from non-root user");
        return 0;  // DO NOT return error. just ignore and let RecvMessage loop continue
    }

    if (sa.nl_pid != 0) {
        ALOGV("Ignoring message as not received from kernel");
        return 0;  // DO NOT return error. just ignore and let RecvMessage loop continue
    }

    // explicitly null terminate as it is not gurenteed that buffer is null terminated from kernel
    buf[kRecvBufferSize - 1] = '\0';
    event_handler_->HandleEvent(buf, message_length);

    return 0;
}

}  // namespace netman
}  // namespace vcc
