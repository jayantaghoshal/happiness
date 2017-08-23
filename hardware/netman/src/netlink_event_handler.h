#ifndef _NETLINK_EVENT_HANDLER_H
#define _NETLINK_EVENT_HANDLER_H

#include "interface_configuration.h"
#include "netlink_event_listener.h"

namespace vcc {
namespace netman {

class NetlinkEventHandler
{
    InterfaceConfiguration eth1_configuration_;

public:
    NetlinkEventHandler(const InterfaceConfiguration &eth1_configuration_);
    ~NetlinkEventHandler();

    void HandleEvent(struct nlmsghdr *nl_message_header);

protected:
    void HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg);
    void HandleNewAddressEvent(struct nlmsghdr *nl_message_header, struct ifaddrmsg *if_addr_msg);
};

} // using netman
} // using vcc

#endif
