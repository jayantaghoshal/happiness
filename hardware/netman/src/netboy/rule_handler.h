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

#ifndef _RULE_HANDLER_H_
#define _RULE_HANDLER_H_

#include "vcc/localconfig.h"

#include <memory>
#include <string>
#include <vector>

namespace vcc {
namespace netman {

enum struct Intent {
    NONE,  // Only used as a default error value
    MOVE,
    RENAME,
    MOVE_AND_RENAME
};

struct RuleAction {
    RuleAction(Intent intent) : intent_(intent) {}
    virtual ~RuleAction() = default;
    Intent intent_;
};

struct RuleActionMove : public RuleAction {
    RuleActionMove() : RuleAction(Intent::MOVE){};
    std::string NEW_NS;
};

struct RuleActionRename : public RuleAction {
    RuleActionRename() : RuleAction(Intent::RENAME){};
    std::string NEW_NAME;
};

struct RuleActionMoveAndRename : public RuleAction {
    RuleActionMoveAndRename() : RuleAction(Intent::MOVE_AND_RENAME){};
    std::string NEW_NAME;
    std::string NEW_NS;
};

struct NetboyRule {
    std::string DEVPATH;
    std::string SUBSYSTEM;
    std::string DEVTYPE;
    std::string INTERFACE_NAME;
    std::string DRIVER;
    std::shared_ptr<RuleAction> ACTION;
};

class RuleHandler {
  public:
    RuleHandler(const RuleHandler&) = delete;
    RuleHandler& operator=(RuleHandler&) = delete;

    RuleHandler(RuleHandler&&) = delete;
    RuleHandler& operator=(RuleHandler&&) = delete;

    bool loadRules(const vcc::LocalConfigReaderInterface* lcfg);
    static RuleHandler& getInstance();

    bool getMatchingRule(const NetboyRule& attributes_to_match, std::shared_ptr<RuleAction>& return_action);

  private:
    RuleHandler() = default;

    bool parseAttribute(const std::string& attr, NetboyRule& rule);
    bool parseRules(const std::vector<std::string>& rules);
    bool compareRule(const NetboyRule& attributes_to_match, const NetboyRule& rule);

    std::vector<NetboyRule> rules_;
};

}  // namespace netman
}  // namespace vcc

#endif  // _RULE_HANDLER_H_
