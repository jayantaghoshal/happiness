/*
 * ===========================================================================
 * [2017]- Volvo Car Corporation
 * All Rights Reserved
 * LEGAL
 * NOTICE:  All information (including intellectual and technical concepts) contained herein is, and remains the
 * property of or licensed to Volvo Car Corporation. This information is proprietary to Volvo Car Corporation and may be
 * covered by patents or patent applications. This information is protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from Volvo Car Corporation.
 */

#include <errno.h>

#include <cutils/log.h>
#include <cutils/properties.h>

#include "firewall_config.h"
#include "netman_netlink_event_handler.h"
#include "netutils.h"
#include "vcc/localconfig.h"

#define LOG_TAG "Netmand"

using namespace vcc::netman;

int main() {
  try {
    ALOGI("Netmand 0.1 starting");

    auto *lcfg = vcc::LocalConfigDefault();

    FirewallConfig fw_conf = FirewallConfig(lcfg);
    if (!fw_conf.ParseAndSave(FirewallConfig::kDefaultIptablesRulesPath)) {
      ALOGE("Error parsing and saving iptables.rules");
      return EXIT_FAILURE;
    }

    if (!fw_conf.ApplyRules(FirewallConfig::IP::IPv4_)) {
      ALOGE("Error applying iptables v4 rules");
      return EXIT_FAILURE;
    }

    if (!fw_conf.ApplyRules(FirewallConfig::IP::IPv6_)) {
      ALOGE("Error applying iptables v6 rules");
      return EXIT_FAILURE;
    }

    ALOGD("Firewall configured");

    std::vector<InterfaceConfiguration> interface_configurations;

    LoadInterfaceConfiguration(&interface_configurations, lcfg);

    ALOGV("Setting initial configuration on network interfaces");

    SetupInterface(interface_configurations);

    ALOGV("Initial configurations set");

    NetmanNetlinkEventHandler nl_event_handler(interface_configurations);

    NetlinkSocketListener &nl_socket_listener =
        NetlinkSocketListener::Instance(NetlinkSocketListener::SocketType::NLSOC_TYPE_ROUTE);
    nl_socket_listener.SetNetlinkEventHandler(nl_event_handler);

    property_set("netmand.startup_completed", "1");

    if (nl_socket_listener.StartListening() < 0) {
      ALOGE("Unable to start NetlinkSocketListener (%s)", strerror(errno));
      return EXIT_FAILURE;
    }
  } catch (const std::runtime_error &e) {
    ALOGE("ABORTING: Exception thrown: %s", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
