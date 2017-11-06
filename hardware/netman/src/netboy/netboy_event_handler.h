#ifndef _NETBOY_EVENT_HANDLER_H_
#define _NETBOY_EVENT_HANDLER_H_

#include "uevent_handler.h"

namespace vcc {
namespace netman {

class NetboyEventHandler final : public UeventHandler {
  protected:
    void HandleEvent(const char *uevent, const int message_length) override;

  private:
    void RuleMatchAndRun(const UeventHandler::NetDeviceAttr &device);
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETBOY_EVENT_HANDLER_H_
