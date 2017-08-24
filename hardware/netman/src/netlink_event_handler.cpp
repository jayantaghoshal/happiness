#define LOG_TAG "Netmand"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <linux/rtnetlink.h>
#include <linux/socket.h>

#include <net/if.h>

#include <cutils/log.h>

#include <vector>
#include <unistd.h>

#include "netlink_event_handler.h"
#include "netlink_event_listener.h"
#include "netman.h"

namespace vcc {
namespace netman {

NetlinkEventHandler::NetlinkEventHandler(const InterfaceConfiguration &eth1_configuration)
    : eth1_configuration_(eth1_configuration)
{
    PrintInterfaceConfiguration("NetlinkEventHandler", eth1_configuration);
}

NetlinkEventHandler::~NetlinkEventHandler()
{
}

void NetlinkEventHandler::HandleEvent(struct nlmsghdr *nl_message_header)
{
    struct ifinfomsg *if_info_msg = (struct ifinfomsg *)NLMSG_DATA(nl_message_header);
    struct ifaddrmsg *if_addr_msg = (struct ifaddrmsg *)NLMSG_DATA(nl_message_header);
    struct rtmsg *rt_msg = (struct rtmsg *)NLMSG_DATA(nl_message_header);

    switch (nl_message_header->nlmsg_type) {
    case NLMSG_DONE:
        ALOGI("NLMSG_DONE handler.");
        break;
    case NLMSG_ERROR:
        ALOGI("NLMSG_ERROR handler.");
        break; // TODO: Handle error
    case RTM_NEWLINK: // Interface up/down
        HandleNewLinkEvent(nl_message_header, if_info_msg);
        break;
    case RTM_NEWADDR: // Name, Ip address, Broadcast address, Netmask, Mtu, MAC
        HandleNewAddressEvent(nl_message_header, if_addr_msg);
        break;
    default:
        ALOGI("Unknown event arrived: (%i)", nl_message_header->nlmsg_type);
        break;
    }
}

void NetlinkEventHandler::HandleNewLinkEvent(struct nlmsghdr *nl_message_header,
                                             struct ifinfomsg *if_info_msg)
{
    ALOGI("Message received: RTM_NEWLINK");

    const char *interface_name = eth1_configuration_.name.c_str();
    const char *ip_addr = eth1_configuration_.ip_address.c_str();
    const char *netmask = eth1_configuration_.netmask.c_str();
    const char *broadcast_addr = eth1_configuration_.broadcast_address.c_str();
    const int mtu = eth1_configuration_.mtu;

    char name[IF_NAMESIZE];

    // Comments:
    // IFF_UP = ifconfig eth0 up
    // IFF_RUNNING = cable plugged in
    // Should if statement around IFF_RUNNING be ! ??? /Philip Werner
    if ((if_info_msg != NULL) &&
        (if_indextoname(if_info_msg->ifi_index, name) != NULL) &&
        (if_info_msg->ifi_flags & IFF_UP) &&
        !(if_info_msg->ifi_flags & IFF_RUNNING)) {
        if (0 == strncmp(name, interface_name, strlen(interface_name))) {
            ALOGI("Detected invalid interface configuration. Resetting configuration.");
            SetupInterface(interface_name,
                           eth1_configuration_.mac_address_bytes,
                           ip_addr,
                           netmask,
                           broadcast_addr,
                           mtu);
        }
    }
}

void NetlinkEventHandler::HandleNewAddressEvent(struct nlmsghdr *nl_message_header,
                                                struct ifaddrmsg *if_addr_msg)
{
    ALOGI("RTM_NEWADDR handler.");
}

}
}
