#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  LOG_TAG    "Netmand"

#include <cutils/log.h>
#include <cutils/properties.h>

#include "vcc/localconfig.h"
#include "netlink_event_listener.h"
#include "netman_netlink_event_handler.h"
#include "firewall_config.h"
#include "netman.h"

using namespace vcc::netman;

int main()
{
    try
    {
        ALOGI("Netmand 0.1 starting");

        ALOGI("Setting up firewall configuration");
        auto * lcfg = vcc::LocalConfigDefault();

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

        ALOGI("Firewall configured");

        ALOGI("Loading configuration from local config");

        std::vector<InterfaceConfiguration> interface_configurations;

        LoadInterfaceConfiguration(&interface_configurations, lcfg);

        ALOGI("Setting initial configuration on network interfaces");

        SetupInterface(interface_configurations);

        ALOGI("Initial configurations set");

        NetmanNetlinkEventHandler nl_event_handler(interface_configurations);

        NetlinkSocketListener &nl_socket_listener = NetlinkSocketListener::Instance();
        nl_socket_listener.SetNetlinkEventHandler(nl_event_handler);

        property_set("netmand.startup_completed", "1");

        if (nl_socket_listener.StartListening()) {
            ALOGE("Unable to start NetlinkSocketListener (%s)", strerror(errno));
            exit(1);
        }

        exit(0);
    }
    catch(const std::runtime_error &e)
    {
        ALOGE("ABORTING: Exception thrown: %s", e.what());
    }

    exit(0);
}
