#ifndef _NETBOY_NETLINK_EVENT_HANDLER_H
#define _NETBOY_NETLINK_EVENT_HANDLER_H

#include "netlink_event_handler.h"

namespace vcc {
namespace netman {
class NetboyNetlinkEventHandler : public NetlinkEventHandler {
 protected:
  virtual void HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg);
  virtual void HandleNewAddressEvent(struct nlmsghdr *nl_message_header, struct ifaddrmsg *if_addr_msg);
};

}  // namespace netman
}  // namespace vcc

#endif
