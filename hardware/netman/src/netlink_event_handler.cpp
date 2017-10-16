#include <linux/rtnetlink.h>
#include <linux/socket.h>
#include <net/if.h>

#include <cutils/log.h>

#include <unistd.h>
#include <sstream>
#include <vector>

#include "netlink_event_handler.h"

#define LOG_TAG "Netmand"

namespace vcc {
namespace netman {

void NetlinkEventHandler::HandleEvent(struct nlmsghdr *nl_message_header) {
  switch (nl_message_header->nlmsg_type) {
    case NLMSG_DONE:
      ALOGV("NLMSG_DONE handler.");
      break;
    case NLMSG_ERROR:
      ALOGE("NLMSG_ERROR handler.");
      break;           // TODO: Handle error
    case RTM_NEWLINK:  // Interface up/down
    {
      NetlinkNewLinkEvent eventData((struct ifinfomsg *)NLMSG_DATA(nl_message_header));
      HandleEvent(&eventData);
      break;
    }
    case RTM_NEWADDR:  // Name, Ip address, Broadcast address, Netmask, Mtu, MAC
    {
      NetlinkNewAddrEvent eventData((struct ifaddrmsg *)NLMSG_DATA(nl_message_header));
      HandleEvent(&eventData);
      break;
    }
    default:
      // ALOGI("Unknown event arrived: (%i)", nl_message_header->nlmsg_type);
      break;
  }
}

void NetlinkEventHandler::HandleEvent(char *message, const int length) {
  NetlinkUevent eventData(message, length);
  HandleEvent(&eventData);
}

}  // namespace netman
}  // namespace vcc
