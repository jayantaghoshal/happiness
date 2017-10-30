/*
 * ===========================================================================
 * [2017]- Volvo Car Corporation
 * All Rights Reserved
 * LEGAL
 * NOTICE:  All information (including intellectual and technical concepts) contained herein is, and remains the
 * property of or licensed to Volvo Car Corporation. This information is proprietary to Volvo Car Corporation and may be
 * covered by patents or patent applications. This information is protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from Volvo Car Corporation.
 */

#include <errno.h>
#include <stdio.h>

#include <asm/types.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>

#include <cutils/log.h>

#include <net/if.h>

#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_compat.h>
#include <linux/netfilter/nfnetlink_log.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <arpa/inet.h>

#include <android-base/macros.h>

#include "netlink_event_handler.h"
#include "netlink_event_listener.h"

#define LOG_TAG "Netmand"

namespace vcc {
namespace netman {

NetlinkSocketListener &NetlinkSocketListener::Instance(const SocketType type) {
  static NetlinkSocketListener instance(type);
  return instance;
}

NetlinkSocketListener::~NetlinkSocketListener() { StopListening(); }

int NetlinkSocketListener::SetupSocket() {
  if (netlink_socket_ != -1) {
    ALOGE("Trying to setup the already opened netlink socket.");
    return 0;
  }

  struct sockaddr_nl nladdr;

  memset(&nladdr, 0, sizeof(nladdr));
  nladdr.nl_family = AF_NETLINK;
  nladdr.nl_pid = getpid();

  int socket_type = SOCK_RAW;
  int socket_protocol = NETLINK_ROUTE;

  if (SocketType::NLSOC_TYPE_UEVENT == sock_type_) {
    nladdr.nl_groups = -1;
    socket_type = SOCK_DGRAM;
    socket_protocol = NETLINK_KOBJECT_UEVENT;
  } else if (SocketType::NLSOC_TYPE_ROUTE == sock_type_) {
    nladdr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;
  }

  if ((netlink_socket_ = socket(AF_NETLINK, socket_type, socket_protocol)) < 0) {
    ALOGE("Unable to create netlink socket: %s", strerror(errno));
    return -1;
  }

  int on = 1;
  if (0 != setsockopt(netlink_socket_, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on))) {
    ALOGE("Failed to set credentials option for socket %s", strerror(errno));
    return -1;
  }

  if (bind(netlink_socket_, (struct sockaddr *)&nladdr, sizeof(nladdr)) < 0) {
    ALOGE("Unable to bind netlink socket: %s", strerror(errno));
    close(netlink_socket_);
    return -1;
  }

  return 0;
}

int NetlinkSocketListener::StartListening() {
  if (netlink_event_handler_ == nullptr) {
    ALOGE("Netlink event handler not set on Netlink socket listener.");
    return -1;
  }

  if (SetupSocket() == -1) {
    ALOGE("Netlink event handler failed to setup socket");
    return -1;
  }

  ALOGV("Waiting for netlink messages to arrive...");

  for (;;) {
    if (RecvMessage() == -1) {
      ALOGE("Netlink event handler failed on RecvMessage");
      return -1;
    }
  }

  return 0;
}

void NetlinkSocketListener::StopListening() {
  if (netlink_socket_ != -1) {
    close(netlink_socket_);
    netlink_socket_ = -1;
  }
}

void NetlinkSocketListener::SetNetlinkEventHandler(NetlinkEventHandler &nl_event_handler) {
  netlink_event_handler_ = &nl_event_handler;
}

// TODO : This function should not return plain int. Need to defind enum for error codes
int NetlinkSocketListener::RecvMessage() {
  const int kRecvBufferSize = 4096;
  char buf[kRecvBufferSize] = {0};
  struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf) - 1};
  char cred_control[CMSG_SPACE(sizeof(struct ucred))];
  struct sockaddr_nl sa;
  struct msghdr msg = {.msg_name = (void *)&sa,
                       .msg_namelen = sizeof(sa),
                       .msg_iov = &iov,
                       .msg_iovlen = 1,
                       .msg_control = cred_control,
                       .msg_controllen = sizeof(cred_control),
                       .msg_flags = 0};

  struct nlmsghdr *nl_message_header;
  int message_length;
  message_length = TEMP_FAILURE_RETRY(recvmsg(netlink_socket_, &msg, 0));

  if (message_length <= 0) {
    ALOGE("Error receiving message from Netlink socket: %s", strerror(errno));
    return -1;
  }

  // NOTE: Code below is to safe guard us from changes in future.
  // Strictly speaking CMSG loop below is not needed for present code where we only have enabled single auxillary
  // message to request credentials. But to avoid the trap in future where we may enable additional auxillary messages,
  // below code is needed.
  struct ucred *cred = nullptr;
  for (struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)) {
    if (cmsg->cmsg_type == SCM_CREDENTIALS) {
      cred = reinterpret_cast<struct ucred *>(CMSG_DATA(cmsg));
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

  // Parse message
  if (sock_type_ == SocketType::NLSOC_TYPE_ROUTE) {
    for (nl_message_header = reinterpret_cast<struct nlmsghdr *>(buf);
         NLMSG_OK(nl_message_header, static_cast<unsigned int>(message_length));
         nl_message_header = NLMSG_NEXT(nl_message_header, message_length)) {
      netlink_event_handler_->HandleEvent(nl_message_header);
    }
  } else if (sock_type_ == SocketType::NLSOC_TYPE_UEVENT) {
    // explicitly null terminate as it is not gurenteed that buffer is null terminated from kernel
    buf[kRecvBufferSize - 1] = '\0';
    netlink_event_handler_->HandleEvent(buf, message_length);
  }

  return 0;
}  // namespace netman

}  // namespace netman
}  // namespace vcc
