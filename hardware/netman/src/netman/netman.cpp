/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>

#include <cutils/properties.h>

#include <vector>
#include "firewall_config.h"
#include "netman_event_handler.h"
#include "netutils.h"
#include "uevent_listener.h"
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

        std::vector<InterfaceConfiguration> interface_configurations;

        LoadInterfaceConfiguration(&interface_configurations, lcfg);

        ALOGV("Setting initial configuration on network interfaces");

        NetmanEventHandler event_handler(interface_configurations);

        UEventListener &nl_socket_listener = UEventListener::Instance();
        nl_socket_listener.SetNetlinkEventHandler(event_handler);

        property_set("netmand.startup_completed", "1");

        if (nl_socket_listener.StartListening() < 0) {
            ALOGE("Unable to recv on  Netlink socket");
        }

    } catch (const std::runtime_error &e) {
        ALOGE("ABORTING: Exception thrown: %s", e.what());
    }

    // Netman is never expected to quit listening for events. So if control reaches here; it's a failure
    return EXIT_FAILURE;
}
