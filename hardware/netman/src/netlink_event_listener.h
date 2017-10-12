#ifndef _NETLINK_SOCKET_LISTENER_H_
#define _NETLINK_SOCKET_LISTENER_H_

#include "netutils.h"

namespace vcc {
namespace netman {
class NetlinkEventHandler;

class NetlinkSocketListener {
 private:
  NetlinkEventHandler *netlink_event_handler_ = nullptr;
  int netlink_socket_ = -1;

 public:
  NetlinkSocketListener() = default;
  NetlinkSocketListener(NetlinkSocketListener &other) = delete;
  NetlinkSocketListener(NetlinkSocketListener &&other) = delete;
  ~NetlinkSocketListener();

  NetlinkSocketListener &operator=(NetlinkSocketListener &other) = delete;
  NetlinkSocketListener &operator=(NetlinkSocketListener &&other) = delete;

  int StartListening();
  void StopListening();
  void SetNetlinkEventHandler(NetlinkEventHandler &nl_event_handler);

  static NetlinkSocketListener &Instance();

 private:
  int SetupSocket();
  int RecvMessage();
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETLINK_SOCKET_LISTENER_H_
