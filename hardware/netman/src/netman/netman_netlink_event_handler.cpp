#include <cutils/log.h>
#include <linux/rtnetlink.h>
#include <linux/socket.h>
#include <net/if.h>

#include "netman_netlink_event_handler.h"
#include "netutils.h"

#define LOG_TAG "Netmand"

namespace vcc {
namespace netman {

NetmanNetlinkEventHandler::NetmanNetlinkEventHandler(
    const std::vector<InterfaceConfiguration> &interface_configurations)
    : interface_configurations_(interface_configurations) {}

void NetmanNetlinkEventHandler::HandleEvent(NetlinkEventData *eventData) {
  switch (eventData->eventType) {
    case NetlinkEventType::NETLINK_NEW_LINK: {
      NetlinkNewLinkEvent *pData = static_cast<NetlinkNewLinkEvent *>(eventData);
      if (pData) HandleNewLinkEvent(pData->info_msg);
      break;
    }
    case NetlinkEventType::NETLINK_NEW_ADDRESS: {
      NetlinkNewAddrEvent *pData = static_cast<NetlinkNewAddrEvent *>(eventData);
      if (pData) HandleNewAddressEvent(pData->addr_msg);
      break;
    }
    default:
      break;
  }
}

void NetmanNetlinkEventHandler::HandleNewLinkEvent(const struct ifinfomsg *if_info_msg) {
  char name[IF_NAMESIZE];

  ALOGD("Message received: RTM_NEWLINK");

  // IFF_UP = ifconfig eth0 up
  // IFF_RUNNING = cable plugged in
  // TODO Should if statement around IFF_RUNNING be ! ??? /Philip Werner
  if ((if_info_msg != NULL) && (if_indextoname(if_info_msg->ifi_index, name) != NULL) &&
      (if_info_msg->ifi_flags & IFF_UP) && !(if_info_msg->ifi_flags & IFF_RUNNING)) {
    for (const auto &ic : interface_configurations_) {
      if (name == ic.name) {
        SetupInterface(ic.name.c_str(), ic.mac_address_bytes, ic.ip_address.c_str(), ic.netmask.c_str(),
                       ic.broadcast_address.c_str(), ic.mtu);
      }
    }
  }
}

void NetmanNetlinkEventHandler::HandleNewAddressEvent(const struct ifaddrmsg *if_addr_msg) {
  char name[IF_NAMESIZE];

  ALOGD("Message received: RTM_NEWADDR");

  // TODO: Refactor common parts in HandleNewLinkEvent and HandleNewAddressEvent
  if ((if_addr_msg != NULL) && (if_indextoname(if_addr_msg->ifa_index, name) != NULL)) {
    for (const auto &ic : interface_configurations_) {
      if (name == ic.name) {
        SetupInterface(ic.name.c_str(), ic.mac_address_bytes, ic.ip_address.c_str(), ic.netmask.c_str(),
                       ic.broadcast_address.c_str(), ic.mtu);
      }
    }
  }
}

}  // namespace netman
}  // namespace vcc
