#include <linux/ethtool.h>
#include <linux/rtnetlink.h>
#include <linux/socket.h>
#include <net/if.h>
#include <cstdlib>

#define LOG_TAG "Netmand"

#include <cutils/log.h>

#include "netboy_netlink_event_handler.h"
#include "netlink_event_handler.h"
#include "netlink_event_listener.h"
#include "netutils.h"

namespace vcc {
namespace netman {

static bool HasDriverR8152(const char *network_interface_name) {
  int err;
  int fd;
  struct ifreq ifr;
  struct ethtool_drvinfo drvinfo;

  std::memset(&ifr, 0, sizeof(ifr));
  std::strcpy(ifr.ifr_name, network_interface_name);

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    ALOGE("No control socket for HasDriverR8152.");
    return false;
  }

  drvinfo.cmd = ETHTOOL_GDRVINFO;
  ifr.ifr_data = (caddr_t)&drvinfo;
  err = ioctl(fd, SIOCETHTOOL, &ifr);
  if (err < 0) {
    ALOGE("Cannot get driver information");
    return false;
  }

  return std::strcmp("r8152", drvinfo.driver) == 0;
}

void NetboyNetlinkEventHandler::HandleNewLinkEvent(struct nlmsghdr *nl_message_header, struct ifinfomsg *if_info_msg) {
  ALOGI("Message received: RTM_NEWLINK.");

  char network_interface_name[IF_NAMESIZE];
  const std::string ns = "vcc";

  if ((if_info_msg != NULL) && if_indextoname(if_info_msg->ifi_index, network_interface_name) != NULL) {
    // TODO(Patrik Moberg): Temporary Friday hack for NAVI POC. Remove "next" week. :)
    if (HasDriverR8152(network_interface_name)) {
      std::string dhcpcd("/system/bin/netcfg64 ");
      dhcpcd += network_interface_name;
      dhcpcd += " dhcp";
      if (system(dhcpcd.c_str())) ALOGE("Unable to perform dhcp client for %s", network_interface_name);
      return;
    }

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

void NetboyNetlinkEventHandler::HandleNewAddressEvent(struct nlmsghdr *nl_message_header,
                                                      struct ifaddrmsg *if_addr_msg) {
  ALOGI("Message received: RTM_NEWADDR");
}

}  // namespace netman
}  // namespace vcc
