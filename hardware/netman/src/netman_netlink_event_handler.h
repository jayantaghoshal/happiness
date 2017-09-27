#ifndef _NETMAN_NETLINK_EVENT_HANDLER_H
#define _NETMAN_NETLINK_EVENT_HANDLER_H

#include "netlink_event_handler.h"

namespace vcc {
namespace netman {

class NetmanNetlinkEventHandler : public NetlinkEventHandler
{
    const std::vector<InterfaceConfiguration> &interface_configurations_;

public:
    NetmanNetlinkEventHandler(const std::vector<InterfaceConfiguration> &interface_configurations);

protected:
    virtual void HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg);
    virtual void HandleNewAddressEvent(struct nlmsghdr *nl_message_header, struct ifaddrmsg *if_addr_msg);
};

} // using netman
} // using vcc

#endif
