#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <linux/rtnetlink.h>
#include <linux/socket.h>

#include <net/if.h>

#define  LOG_TAG    "Netmand"

#include <cutils/log.h>

#include <sstream>
#include <vector>
#include <unistd.h>

#include "netlink_event_handler.h"
#include "netlink_event_listener.h"
#include "netman.h"

namespace vcc {
namespace netman {

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

}
}
