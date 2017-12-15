/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "netboy_event_handler.h"

#include <fstream>
#include <string>
#include "netutils.h"

#include "rule_handler.h"

#define LOG_TAG "Netboyd"
#include <cutils/log.h>

namespace vcc {
namespace netman {

void NetboyEventHandler::RuleMatchAndRun(const UeventHandler::NetDeviceAttr &device) {
    // Create and fill structure for rule matching
    NetboyRule rule{device.devpath, device.subsystem, device.devtype, device.interface_name, device.driver, nullptr};
    std::shared_ptr<RuleAction> action;

    if (RuleHandler::getInstance().getMatchingRule(rule, action)) {
        if (!action) {
            ALOGE("Found a matching rule but ACTION is NULL");
            return;
        }

        // quit further processing if intent is unknown
        if (!(action->intent_ == Intent::MOVE || action->intent_ == Intent::MOVE_AND_RENAME ||
              action->intent_ == Intent::RENAME)) {
            return;
        }

        TakeInterfaceDown(device.interface_name.c_str());

        std::string ns, name;

        if (action->intent_ == Intent::MOVE) {
            ns = std::static_pointer_cast<RuleActionMove>(action)->NEW_NS;
        } else if (action->intent_ == Intent::RENAME) {
            name = std::static_pointer_cast<RuleActionRename>(action)->NEW_NAME;
        } else if (action->intent_ == Intent::MOVE_AND_RENAME) {
            ns = std::static_pointer_cast<RuleActionMoveAndRename>(action)->NEW_NS;
            name = std::static_pointer_cast<RuleActionMoveAndRename>(action)->NEW_NAME;
        }

        MoveNetworkInterfaceToNamespace(device.interface_name, ns, name);
    }
}

// handle new message
void NetboyEventHandler::HandleEvent(const char *uevent, const int message_length) {
    if (!uevent) return;

    const char *const ADD_ACTION = "add@";

    // Currently only add@ event are handled. Can be extended as per need
    if (message_length < static_cast<int>(std::string(ADD_ACTION).length()) ||
        std::string(uevent).find(ADD_ACTION) == std::string::npos)
        return;

    // advance buffer past first segment
    int index = strlen(uevent) + 1;
    bool read_parent = false;
    NetDeviceAttr device;

    while (index < message_length) {
        std::string line(uevent + index);

        // see if it is a networking event
        if (!read_parent && (line.find(ATTR_IFINDEX) != std::string::npos)) {
            read_parent = true;
        }

        ReadDeviceAttr(line, device);
        index += line.length() + 1;
    }

    if (read_parent) {
        ReadParentDeviceAttr(device);
        RuleMatchAndRun(device);
    }
}

}  // namespace netman
}  // namespace vcc
