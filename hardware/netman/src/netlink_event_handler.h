#ifndef _NETLINK_EVENT_HANDLER_H
#define _NETLINK_EVENT_HANDLER_H

#include "interface_configuration.h"
#include "netlink_event_listener.h"

namespace vcc {
namespace netman {

class NetlinkEventHandler
{
    const std::vector<InterfaceConfiguration> &interface_configurations_;

public:
    NetlinkEventHandler(const std::vector<InterfaceConfiguration> &interface_configurations);
    ~NetlinkEventHandler();

    void HandleEvent(struct nlmsghdr *nl_message_header);

protected:
    void HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg);
    void HandleNewAddressEvent(struct nlmsghdr *nl_message_header, struct ifaddrmsg *if_addr_msg);
};

} // using netman
} // using vcc

#endif
