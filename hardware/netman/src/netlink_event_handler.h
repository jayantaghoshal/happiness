#ifndef _NETLINK_EVENT_HANDLER_H_
#define _NETLINK_EVENT_HANDLER_H_

#include "interface_configuration.h"
#include "netlink_event_listener.h"

namespace vcc {
namespace netman {
class NetlinkEventHandler {
 public:
  void HandleEvent(struct nlmsghdr *nl_message_header);

 protected:
  virtual void HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg) = 0;
  virtual void HandleNewAddressEvent(struct nlmsghdr *nl_message_header, struct ifaddrmsg *if_addr_msg) = 0;
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETLINK_EVENT_HANDLER_H_
