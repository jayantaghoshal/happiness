#include <errno.h>

#include <cutils/log.h>
#include <cutils/properties.h>

#include "firewall_config.h"
#include "netlink_event_listener.h"
#include "netman_netlink_event_handler.h"
#include "netutils.h"
#include "vcc/localconfig.h"

#define LOG_TAG "Netmand"

using namespace vcc::netman;

int main() {
  try {
    ALOGI("Netmand 0.1 starting");

    ALOGV("Setting up firewall configuration");
    auto *lcfg = vcc::LocalConfigDefault();

    FirewallConfig fw_conf = FirewallConfig(lcfg);
    if (!fw_conf.ParseAndSave(FirewallConfig::kDefaultIptablesRulesPath)) {
      ALOGE("Error parsing and saving iptables.rules");
      exit(1);
    }

    if (!fw_conf.ApplyRules(FirewallConfig::IP::IPv4_)) {
      ALOGE("Error applying iptables v4 rules");
      exit(1);
    }

    if (!fw_conf.ApplyRules(FirewallConfig::IP::IPv6_)) {
      ALOGE("Error applying iptables v6 rules");
      exit(1);
    }

    ALOGV("Firewall configured");

    std::vector<InterfaceConfiguration> interface_configurations;

    LoadInterfaceConfiguration(&interface_configurations, lcfg);

    ALOGV("Setting initial configuration on network interfaces");

    SetupInterface(interface_configurations);

    ALOGV("Initial configurations set");

    NetmanNetlinkEventHandler nl_event_handler(interface_configurations);

    NetlinkSocketListener &nl_socket_listener = NetlinkSocketListener::Instance();
    nl_socket_listener.SetNetlinkEventHandler(nl_event_handler);

    property_set("netmand.startup_completed", "1");

    if (nl_socket_listener.StartListening()) {
      ALOGE("Unable to start NetlinkSocketListener (%s)", strerror(errno));
      exit(1);
    }
  } catch (const std::runtime_error &e) {
    ALOGE("ABORTING: Exception thrown: %s", e.what());
  }

  exit(0);
}
