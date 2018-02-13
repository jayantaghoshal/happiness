/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>

#include <algorithm>
#include <istream>
#include <sstream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "rule_handler.h"

#define LOG_TAG "Netboyd"
#include <cutils/log.h>

namespace vcc {
namespace netman {

bool RuleHandler::parseAttribute(const std::string& attr, NetboyRule& rule) {
    std::string attr_copy(attr);

    // strip whitespace characters if any
    attr_copy.erase(std::remove_if(attr_copy.begin(), attr_copy.end(), isspace), attr_copy.end());

    std::stringstream attr_stream(attr_copy);
    std::string key, value;

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

bool RuleHandler::parseRules(const std::vector<std::string>& rules) {
    for (auto& it : rules) {
        std::stringstream rule_str(it);
        std::string attribute;
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
        }

        if (Intent::RENAME == rule.ACTION->intent_ &&
            (std::static_pointer_cast<RuleActionRename>(rule.ACTION)->NEW_NAME.empty())) {
            ALOGE("RENAME rule must contain NEW_NAME");
            return false;
        }

        if (Intent::MOVE_AND_RENAME == rule.ACTION->intent_ &&
            ((std::static_pointer_cast<RuleActionMoveAndRename>(rule.ACTION)->NEW_NS.empty()) ||
             (std::static_pointer_cast<RuleActionMoveAndRename>(rule.ACTION)->NEW_NAME.empty()))) {
            ALOGE("MOVE_AND_RENAME rule must contain NEW_NAME and NEW_NS");
            return false;
        }

        rules_.push_back(rule);
    }

    return true;
}

bool RuleHandler::loadRules(const std::string& rules_file_path) {
    boost::property_tree::ptree root;
    boost::property_tree::read_json(rules_file_path, root);

    std::vector<std::string> rules;

    for (const auto& rule : root.get_child("NetboyRules")) {
        rules.emplace_back(rule.second.data());
    }

    if (!parseRules(rules)) {
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
    for (const auto& it : rules_) {
        if (compareRule(attributes_to_match, it)) {
            return_action = it.ACTION;
            return true;
        }
    }
    return false;
}

bool RuleHandler::compareRule(const NetboyRule& attributes_to_match, const NetboyRule& rule) {
    // An empty (or nonexistent) attribute in a rule
    if (!(rule.DEVPATH.empty() || attributes_to_match.DEVPATH == rule.DEVPATH)) {
        return false;
    }

    if (!(rule.SUBSYSTEM.empty() || attributes_to_match.SUBSYSTEM == rule.SUBSYSTEM)) {
        return false;
    }

    if (!(rule.DEVTYPE.empty() || attributes_to_match.DEVTYPE == rule.DEVTYPE)) {
        return false;
    }

    if (!(rule.INTERFACE_NAME.empty() || attributes_to_match.INTERFACE_NAME == rule.INTERFACE_NAME)) {
        return false;
    }

    if (!(rule.DRIVER.empty() || attributes_to_match.DRIVER == rule.DRIVER)) {
        return false;
    }

    return true;
}

}  // namespace netman
}  // namespace vcc
