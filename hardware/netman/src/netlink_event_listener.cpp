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

int NetlinkSocketListener::RecvMessage() {
  const int kRecvBufferSize = 4096;
  char buf[kRecvBufferSize] = {0};
  struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};
  struct sockaddr_nl sa;
  struct msghdr msg = {.msg_name = (void *)&sa,
                       .msg_namelen = sizeof(sa),
                       .msg_iov = &iov,
                       .msg_iovlen = 1,
                       .msg_control = NULL,
                       .msg_controllen = 0,
                       .msg_flags = 0};

  struct nlmsghdr *nl_message_header;
  int message_length;
  message_length = TEMP_FAILURE_RETRY(recvmsg(netlink_socket_, &msg, 0));

  if (message_length <= 0) {
    ALOGE("Error receiving message from Netlink socket: %s", strerror(errno));
    return -1;
  }

  // Parse message
  if (sock_type_ == SocketType::NLSOC_TYPE_ROUTE) {
    for (nl_message_header = (struct nlmsghdr *)buf; NLMSG_OK(nl_message_header, (unsigned int)message_length);
         nl_message_header = NLMSG_NEXT(nl_message_header, message_length)) {
      netlink_event_handler_->HandleEvent(nl_message_header);
    }
  } else if (sock_type_ == SocketType::NLSOC_TYPE_UEVENT) {
    netlink_event_handler_->HandleEvent(buf, message_length);
  }

  return 0;
}

}  // namespace netman
}  // namespace vcc
