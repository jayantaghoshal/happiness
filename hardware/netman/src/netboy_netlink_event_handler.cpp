#include <linux/rtnetlink.h>
#include <linux/socket.h>

#include <net/if.h>

#define LOG_TAG "Netmand"

#include <cutils/log.h>

#include "netboy_netlink_event_handler.h"
#include "netlink_event_handler.h"
#include "netlink_event_listener.h"
#include "netman.h"

namespace vcc
{
namespace netman
{
void NetboyNetlinkEventHandler::HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg)
{
  ALOGI("Message received: RTM_NEWLINK.");

  char network_interface_name[IF_NAMESIZE];
  const std::string ns = "vcc";

  if ((if_info_msg != NULL) && if_indextoname(if_info_msg->ifi_index, network_interface_name) != NULL)
  {
    ALOGI("Moving interface %s to namespace", network_interface_name);

    TakeInterfaceDown(network_interface_name);

    MoveNetworkInterfaceToNamespace(network_interface_name, ns);

    /*
      Comment: This is an ugly way of notifying netman that a new interface has arrived in
      the namespace. If there is an event that can be subscribed to, then it should be used
      instead.
     */
    BringInterfaceUp(network_interface_name, ns);
  }
}

void NetboyNetlinkEventHandler::HandleNewAddressEvent(struct nlmsghdr *nl_message_header, struct ifaddrmsg *if_addr_msg)
{
  ALOGI("Message received: RTM_NEWADDR");
}

}  // namespace netman
}  // namespace vcc
