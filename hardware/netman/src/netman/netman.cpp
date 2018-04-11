/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <IDispatcher.h>
#include <cutils/properties.h>
#include <hidl/HidlTransportSupport.h>
#include <vector>
#include "diagnostics_reporter.h"
#include "firewall_config.h"
#include "netman_event_handler.h"
#include "netutils.h"
#include "uevent_listener.h"
#include "vcc/localconfig.h"

#undef LOG_TAG  // <hidl/HidlTransportSupport.h>
#define LOG_TAG "Netmand"
#include <cutils/log.h>

using namespace vcc::netman;

int main() {
    try {
        ALOGI("Netmand 0.1 starting");

        auto* lcfg = vcc::LocalConfigDefault();

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

        android::hardware::configureRpcThreadpool(1, false);
        DiagnosticsReporter diag_rep;

        auto veth_configs = GetNetmanConfig();
        SetupVeth(veth_configs);

        std::vector<InterfaceConfiguration> interface_configurations;

        LoadInterfaceConfiguration(&interface_configurations, lcfg);

        ALOGV("Setting initial configuration on network interfaces");

        NetmanEventHandler event_handler(interface_configurations);

        UEventListener& nl_socket_listener = UEventListener::Instance();
        nl_socket_listener.SetNetlinkEventHandler(event_handler);

        property_set("netmand.startup_completed", "1");

        if (!nl_socket_listener.StartListening()) {
            ALOGE("Unable to recv on Netlink socket");
        }

        tarmac::eventloop::IDispatcher::GetDefaultDispatcher().Join();

    } catch (const std::runtime_error& e) {
        ALOGE("ABORTING: Exception thrown: %s", e.what());
    }

    // Netman is never expected to quit listening for events. So if control reaches here; it's a failure
    return EXIT_FAILURE;
}
