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

#include <istream>
#include <sstream>

#include "rule_handler.h"
#define LOG_TAG "Netboyd"

namespace vcc {
namespace netman {

using std::string;
using std::vector;

bool RuleHandler::parseAttribute(const string& attr, NetboyRule& rule) {
    std::stringstream attr_stream(attr);
    string key, value;

    std::getline(attr_stream, key, '=');
    std::getline(attr_stream, value);

    if (attr_stream.fail()) {
        return false;
    }

    if (key == "DEVPATH") {
        rule.DEVPATH = value;
    } else if (key == "SUBSYSTEM") {
        rule.SUBSYSTEM = value;
    } else if (key == "DEVTYPE") {
        rule.DEVTYPE = value;
    } else if (key == "INTERFACE_NAME") {
        rule.INTERFACE_NAME = value;
    } else if (key == "DRIVER") {
        rule.DRIVER = value;
    } else if (key == "ACTION") {
        if (value == "MOVE") {
            rule.ACTION = std::make_shared<RuleActionMove>();
        } else if (value == "RENAME") {
            rule.ACTION = std::make_shared<RuleActionRename>();
        } else if (value == "MOVE_AND_RENAME") {
            rule.ACTION = std::make_shared<RuleActionMoveAndRename>();
        } else {
            ALOGE("Invalid action %s", value.c_str());
            return false;
        }
    } else if (key == "NEW_NAME" && rule.ACTION) {
        if ((rule.ACTION)->intent_ == Intent::RENAME) {
            std::static_pointer_cast<RuleActionRename>(rule.ACTION)->NEW_NAME = value;
        } else if ((rule.ACTION)->intent_ == Intent::MOVE_AND_RENAME) {
            std::static_pointer_cast<RuleActionMoveAndRename>(rule.ACTION)->NEW_NAME = value;
        } else {
            ALOGE("Invalid action attribute NEW_NAME");
            return false;
        }
    } else if (key == "NEW_NS" && rule.ACTION) {
        if ((rule.ACTION)->intent_ == Intent::MOVE) {
            std::static_pointer_cast<RuleActionMove>(rule.ACTION)->NEW_NS = value;
        } else if ((rule.ACTION)->intent_ == Intent::MOVE_AND_RENAME) {
            std::static_pointer_cast<RuleActionMoveAndRename>(rule.ACTION)->NEW_NS = value;
        } else {
            ALOGE("Invalid action attribute NEW_NS");
            return false;
        }
    } else {
        ALOGE("Unknown attribute %s", key.c_str());
        return false;
    }

    return true;
}

bool RuleHandler::parseRules(const vector<string>& rules) {
    for (auto it = rules.cbegin(); it != rules.cend(); ++it) {
        std::stringstream rule_str(it->c_str());
        string attribute;
        NetboyRule rule;

        while (std::getline(rule_str, attribute, ',')) {
            if (!parseAttribute(attribute, rule)) {
                return false;
            }
        }

        if (!rule.ACTION || Intent::NONE == (rule.ACTION)->intent_) {
            ALOGE("Rule must contain an action");
            return false;
        }

        if (Intent::MOVE == rule.ACTION->intent_ &&
            (std::static_pointer_cast<RuleActionMove>(rule.ACTION)->NEW_NS.empty())) {
            ALOGE("MOVE rule must contain NEW_NS");
            return false;
        } else if (Intent::RENAME == rule.ACTION->intent_ &&
                   (std::static_pointer_cast<RuleActionRename>(rule.ACTION)->NEW_NAME.empty())) {
            ALOGE("RENAME rule must contain NEW_NAME");
            return false;
        } else if (Intent::MOVE_AND_RENAME == rule.ACTION->intent_ &&
                   ((std::static_pointer_cast<RuleActionMoveAndRename>(rule.ACTION)->NEW_NS.empty()) ||
                    (std::static_pointer_cast<RuleActionMoveAndRename>(rule.ACTION)->NEW_NAME.empty()))) {
            ALOGE("MOVE_AND_RENAME rule must contain NEW_NAME and NEW_NS");
            return false;
        }

        rules_.push_back(rule);
    }

    return true;
}

bool RuleHandler::loadRules(const vcc::LocalConfigReaderInterface* lcfg) {
    vector<string> unparsed_rules = lcfg->GetStringArray("NetboyRules");

    if (!parseRules(unparsed_rules)) {
        rules_.clear();
        return false;
    }

    return true;
}

RuleHandler& RuleHandler::getInstance() {
    static RuleHandler handler;
    return handler;
}

bool RuleHandler::getMatchingRule(const NetboyRule& attributes_to_match, std::shared_ptr<RuleAction>& return_action) {
    for (auto it = rules_.cbegin(); it != rules_.cend(); ++it) {
        if (compareRule(attributes_to_match, *it)) {
            return_action = it->ACTION;
            return true;
        }
    }
    return false;
}

bool RuleHandler::compareRule(const NetboyRule& attributes_to_match, const NetboyRule& rule) {
    // An empty (or nonexistent) attribute in a rule
    if (!(rule.DEVPATH.empty() || attributes_to_match.DEVPATH == rule.DEVPATH)) {
        return false;
    } else if (!(rule.SUBSYSTEM.empty() || attributes_to_match.SUBSYSTEM == rule.SUBSYSTEM)) {
        return false;
    } else if (!(rule.DEVTYPE.empty() || attributes_to_match.DEVTYPE == rule.DEVTYPE)) {
        return false;
    } else if (!(rule.INTERFACE_NAME.empty() || attributes_to_match.INTERFACE_NAME == rule.INTERFACE_NAME)) {
        return false;
    } else if (!(rule.DRIVER.empty() || attributes_to_match.DRIVER == rule.DRIVER)) {
        return false;
    }

    return true;
}

}  // namespace netman
}  // namespace vcc
