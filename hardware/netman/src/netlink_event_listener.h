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

#ifndef _NETLINK_SOCKET_LISTENER_H_
#define _NETLINK_SOCKET_LISTENER_H_

#include "netutils.h"

namespace vcc {
namespace netman {
class NetlinkEventHandler;

class NetlinkSocketListener final {
 public:
  enum class SocketType { NLSOC_TYPE_UNKNOWN, NLSOC_TYPE_UEVENT, NLSOC_TYPE_ROUTE };

  NetlinkSocketListener() = delete;
  ~NetlinkSocketListener();

  NetlinkSocketListener(NetlinkSocketListener &other) = delete;
  NetlinkSocketListener(NetlinkSocketListener &&other) = delete;

  NetlinkSocketListener &operator=(NetlinkSocketListener &other) = delete;
  NetlinkSocketListener &operator=(NetlinkSocketListener &&other) = delete;

  int StartListening();
  void StopListening();
  void SetNetlinkEventHandler(NetlinkEventHandler &nl_event_handler);

  static NetlinkSocketListener &Instance(const SocketType type);

 private:
  NetlinkSocketListener(SocketType type) : sock_type_(type) {}
  int SetupSocket();
  int RecvMessage();

  NetlinkEventHandler *netlink_event_handler_ = nullptr;
  int netlink_socket_ = -1;
  SocketType sock_type_;
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETLINK_SOCKET_LISTENER_H_
