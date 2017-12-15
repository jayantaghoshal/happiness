/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>

#include <cutils/properties.h>
#include <cerrno>

#include "netboy_event_handler.h"
#include "rule_handler.h"
#include "uevent_listener.h"
#include "vcc/localconfig.h"

#define LOG_TAG "Netboyd"

using namespace vcc::netman;

int main() {
    try {
        ALOGI("Netboyd 0.1 starting");

        auto *lcfg = vcc::LocalConfigDefault();

        RuleHandler &rule_handler = RuleHandler::getInstance();
        if (!rule_handler.loadRules(lcfg)) {
            ALOGE("Unable to read rules from local config");
            return EXIT_FAILURE;
        }

        NetboyEventHandler event_handler;

        UEventListener &nl_socket_listener = UEventListener::Instance();
        nl_socket_listener.SetNetlinkEventHandler(event_handler);

        // Need to set property before Blocking on netlink socket
        property_set("netboyd.startup_completed", "1");

        if (nl_socket_listener.StartListening() < 0) {
            ALOGE("Unable to recv on Netlink socket");
        }
    } catch (const std::runtime_error &e) {
        ALOGE("ABORTING: Exception thrown: %s", e.what());
    }

    // Netboy is never expected to quit listening for events. So if control reaches here; it's a failure
    return EXIT_FAILURE;
}
