#ifndef _NETLINK_EVENT_HANDLER_H
#define _NETLINK_EVENT_HANDLER_H

#include "interface_configuration.h"
#include "netlink_event_listener.h"

namespace vcc
{
namespace netman
{
class NetlinkEventHandler
{
 public:
  void HandleEvent(struct nlmsghdr *nl_message_header);

 protected:
  virtual void HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg) = 0;
  virtual void HandleNewAddressEvent(struct nlmsghdr *nl_message_header, struct ifaddrmsg *if_addr_msg) = 0;
};

}  // using netman
}  // using vcc

#endif
