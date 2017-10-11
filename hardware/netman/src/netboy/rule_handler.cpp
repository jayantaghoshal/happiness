#include <sstream>
#include <istream>

#include "rule_handler.h"
#define LOG_TAG "Netboyd"

namespace vcc {
namespace netman {

using std::string;
using std::vector;

bool RuleHandler::parseAttribute(const string &attr, NetboyRule &rule) {
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
  } else if (key == "DEVNAME") {
    rule.DEVNAME = value;
  } else if (key == "DRIVER") {
    rule.DRIVER = value;
  } else if (key == "IFINDEX") {
    rule.IFINDEX = value;
  } else if (key == "ACTION") {
    if (value == "MOVE") {
      rule.ACTION = new RuleActionMove;
    } else if (value == "RENAME") {
      rule.ACTION = new RuleActionRename;
    } else if (value == "MOVE_AND_RENAME") {
      rule.ACTION = new RuleActionMoveAndRename;
    } else {
      ALOGE("Invalid action %s", value.c_str());
      return false;
    }
  } else if (key == "NEW_NAME" && rule.ACTION) {
    if ((rule.ACTION)->INTENT == intent::RENAME) {
      static_cast<RuleActionRename*>(rule.ACTION)->NEW_NAME = value;
    } else if ((rule.ACTION)->INTENT == intent::MOVE_AND_RENAME) {
      static_cast<RuleActionMoveAndRename*>(rule.ACTION)->NEW_NAME = value;
    } else {
      ALOGE("Invalid action attribute NEW_NAME");
      return false;
    }
  } else if (key == "NEW_NS" && rule.ACTION) {
    if ((rule.ACTION)->INTENT == intent::MOVE) {
      static_cast<RuleActionMove*>(rule.ACTION)->NEW_NS = value;
    } else if ((rule.ACTION)->INTENT == intent::MOVE_AND_RENAME) {
      static_cast<RuleActionMoveAndRename*>(rule.ACTION)->NEW_NS = value;
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

    if (!rule.ACTION || (rule.ACTION)->INTENT == intent::NONE) {
      ALOGE("Rule must contain an action");
      return false;
    }

    if (rule.ACTION->INTENT == intent::MOVE
        && (static_cast<RuleActionMove*>(rule.ACTION)->NEW_NS.empty())) {
      ALOGE("MOVE rule must contain NEW_NS");
      return false;
    } else if (rule.ACTION->INTENT == intent::RENAME
               && (static_cast<RuleActionRename*>(rule.ACTION)->NEW_NAME.empty())) {
      ALOGE("RENAME rule must contain NEW_NAME");
      return false;
    } else if (rule.ACTION->INTENT == intent::MOVE_AND_RENAME
               && ((static_cast<RuleActionMoveAndRename*>(rule.ACTION)->NEW_NS.empty()) ||
                   (static_cast<RuleActionMoveAndRename*>(rule.ACTION)->NEW_NAME.empty()))) {
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

bool RuleHandler::getMatchingRule(const NetboyRule& attributes_to_match, RuleAction& return_action) {
  for (auto it = rules_.cbegin(); it != rules_.cend(); ++it) {
    if (compareRule(attributes_to_match, *it)) {
      return_action = *(it->ACTION);
      return true;
    }
  }
  return false;
}

bool RuleHandler::compareRule(const NetboyRule& attributes_to_match, const NetboyRule& rule) {
  // An empty (or nonexistent) attribute in a rule
  if (!(attributes_to_match.DEVPATH == rule.DEVPATH || rule.DEVPATH.empty())) {
    return false;
  } else if (!(attributes_to_match.SUBSYSTEM == rule.SUBSYSTEM || rule.SUBSYSTEM.empty())) {
    return false;
  } else if (!(attributes_to_match.DEVTYPE == rule.DEVTYPE || rule.DEVTYPE.empty())) {
    return false;
  } else if (!(attributes_to_match.DEVNAME == rule.DEVNAME || rule.DEVNAME.empty())) {
    return false;
  } else if (!(attributes_to_match.DRIVER == rule.DRIVER || rule.DRIVER.empty())) {
    return false;
  } else if (!(attributes_to_match.IFINDEX == rule.IFINDEX || rule.IFINDEX.empty())) {
    return false;
  }

  return true;
}

} // namespace netman
} // namespace vcc
