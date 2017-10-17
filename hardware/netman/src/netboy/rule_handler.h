#include "vcc/localconfig.h"

#include <cutils/log.h>

namespace vcc {
namespace netman {

enum struct intent{
  NONE, // Only used as a default error value
  MOVE,
  RENAME,
  MOVE_AND_RENAME
};

struct RuleAction {
  RuleAction(intent i) { INTENT = i; };
  virtual ~RuleAction() = default;
  intent INTENT = intent::NONE;
};

struct RuleActionMove : public RuleAction {
  RuleActionMove() : RuleAction(intent::MOVE) {};
  std::string NEW_NS;
};

struct RuleActionRename : public RuleAction {
  RuleActionRename() : RuleAction(intent::RENAME) {};
  std::string NEW_NAME;
};

struct RuleActionMoveAndRename : public RuleAction {
  RuleActionMoveAndRename() : RuleAction(intent::MOVE_AND_RENAME) {};
  std::string NEW_NAME;
  std::string NEW_NS;
};

struct NetboyRule {
  ~NetboyRule() { if (ACTION) free(ACTION); ACTION = nullptr; };
  std::string  DEVPATH;
  std::string  SUBSYSTEM;
  std::string  DEVTYPE;
  std::string  DEVNAME;
  std::string  DRIVER;
  std::string  IFINDEX;
  RuleAction* ACTION = nullptr;
};

class RuleHandler {
public:
  bool loadRules(const vcc::LocalConfigReaderInterface* lcfg);

  bool getMatchingRule(const NetboyRule& attributes_to_match, RuleAction &return_action);

private:
  bool parseAttribute(const std::string& attr, NetboyRule &rule);
  bool parseRules(const std::vector<std::string> &rules);
  bool compareRule(const NetboyRule& attributes_to_match, const NetboyRule& rule);

  std::vector<NetboyRule> rules_;
};

} // namespace netman
} // namespace vcc
