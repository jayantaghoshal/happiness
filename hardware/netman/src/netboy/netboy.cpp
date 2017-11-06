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

#include <cutils/log.h>
#include <cutils/properties.h>
#include <errno.h>

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

        NetlinkSocketListener &nl_socket_listener = NetlinkSocketListener::Instance();
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
