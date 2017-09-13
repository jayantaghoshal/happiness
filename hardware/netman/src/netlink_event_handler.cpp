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

NetlinkEventHandler::NetlinkEventHandler(const std::vector<InterfaceConfiguration> &interface_configurations)
    : interface_configurations_(interface_configurations)
{
}

NetlinkEventHandler::~NetlinkEventHandler()
{
}

void NetlinkEventHandler::HandleEvent(struct nlmsghdr *nl_message_header)
{
    struct rtmsg *rt_msg = (struct rtmsg *)NLMSG_DATA(nl_message_header);

    switch (nl_message_header->nlmsg_type) {
    case NLMSG_DONE:
        ALOGI("NLMSG_DONE handler.");
        break;
    case NLMSG_ERROR:
        ALOGI("NLMSG_ERROR handler.");
        break; // TODO: Handle error
    case RTM_NEWLINK: // Interface up/down
        {
            struct ifinfomsg *if_info_msg = (struct ifinfomsg *)NLMSG_DATA(nl_message_header);
            HandleNewLinkEvent(nl_message_header, if_info_msg);
            break;
        }
    case RTM_NEWADDR: // Name, Ip address, Broadcast address, Netmask, Mtu, MAC
        {
            struct ifaddrmsg *if_addr_msg = (struct ifaddrmsg *)NLMSG_DATA(nl_message_header);
            HandleNewAddressEvent(nl_message_header, if_addr_msg);
            break;
        }
    default:
        //ALOGI("Unknown event arrived: (%i)", nl_message_header->nlmsg_type);
        break;
    }
}

void NetlinkEventHandler::HandleNewLinkEvent(struct nlmsghdr *nl_message_header,
                                             struct ifinfomsg *if_info_msg)
{
    ALOGI("Message received: RTM_NEWLINK");

    char name[IF_NAMESIZE];

    // IFF_UP = ifconfig eth0 up
    // IFF_RUNNING = cable plugged in
    // TODO Should if statement around IFF_RUNNING be ! ??? /Philip Werner
    if ((if_info_msg != NULL) &&
        (if_indextoname(if_info_msg->ifi_index, name) != NULL) &&
        (if_info_msg->ifi_flags & IFF_UP) &&
        !(if_info_msg->ifi_flags & IFF_RUNNING)) {

        for (const auto &ic : interface_configurations_) {
            if (name == ic.name) {
                SetupInterface(ic.name.c_str(),
                               ic.mac_address_bytes,
                               ic.ip_address.c_str(),
                               ic.netmask.c_str(),
                               ic.broadcast_address.c_str(),
                               ic.mtu);
            }
        }
    }
}

void NetlinkEventHandler::HandleNewAddressEvent(struct nlmsghdr *nl_message_header,
                                                struct ifaddrmsg *if_addr_msg)
{
    ALOGI("Message received: RTM_NEWADDR");
    char name[IF_NAMESIZE];

    // TODO: Refactor common parts in HandleNewLinkEvent and HandleNewAddressEvent
    if ((if_addr_msg != NULL) &&
        (if_indextoname(if_addr_msg->ifa_index, name) != NULL)) {

        for (const auto &ic : interface_configurations_) {
            if (name == ic.name) {
                SetupInterface(ic.name.c_str(),
                               ic.mac_address_bytes,
                               ic.ip_address.c_str(),
                               ic.netmask.c_str(),
                               ic.broadcast_address.c_str(),
                               ic.mtu);
            }
        }
    }
}

}
}
