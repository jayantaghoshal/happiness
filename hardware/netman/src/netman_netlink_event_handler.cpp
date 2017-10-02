#include <linux/rtnetlink.h>
#include <linux/socket.h>

#include <net/if.h>

#define LOG_TAG "Netmand"

#include <cutils/log.h>

#include "netlink_event_handler.h"
#include "netlink_event_listener.h"
#include "netman.h"
#include "netman_netlink_event_handler.h"

namespace vcc
{
namespace netman
{
NetmanNetlinkEventHandler::NetmanNetlinkEventHandler(
    const std::vector<InterfaceConfiguration> &interface_configurations)
    : interface_configurations_(interface_configurations)
{
}

void NetmanNetlinkEventHandler::HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg)
{
  ALOGI("Message received: RTM_NEWLINK");

  char name[IF_NAMESIZE];

  // IFF_UP = ifconfig eth0 up
  // IFF_RUNNING = cable plugged in
  // TODO Should if statement around IFF_RUNNING be ! ??? /Philip Werner
  if ((if_info_msg != NULL) && (if_indextoname(if_info_msg->ifi_index, name) != NULL) &&
      (if_info_msg->ifi_flags & IFF_UP) && !(if_info_msg->ifi_flags & IFF_RUNNING))
  {
    for (const auto &ic : interface_configurations_)
    {
      if (name == ic.name)
      {
        SetupInterface(ic.name.c_str(), ic.mac_address_bytes, ic.ip_address.c_str(), ic.netmask.c_str(),
                       ic.broadcast_address.c_str(), ic.mtu);
      }
    }
  }
}

void NetmanNetlinkEventHandler::HandleNewAddressEvent(struct nlmsghdr *nl_message_header, struct ifaddrmsg *if_addr_msg)
{
  ALOGI("Message received: RTM_NEWADDR");
  char name[IF_NAMESIZE];

  // TODO: Refactor common parts in HandleNewLinkEvent and HandleNewAddressEvent
  if ((if_addr_msg != NULL) && (if_indextoname(if_addr_msg->ifa_index, name) != NULL))
  {
    for (const auto &ic : interface_configurations_)
    {
      if (name == ic.name)
      {
        SetupInterface(ic.name.c_str(), ic.mac_address_bytes, ic.ip_address.c_str(), ic.netmask.c_str(),
                       ic.broadcast_address.c_str(), ic.mtu);
      }
    }
  }
}

}  // namespace netman
}  // namespace vcc
