#ifndef _NETMAN_EVENT_HANDLER_H_
#define _NETMAN_EVENT_HANDLER_H_

#include <vector>
#include "uevent_handler.h"

namespace vcc {
namespace netman {

class NetmanEventHandler final : public UeventHandler {
  public:
    explicit NetmanEventHandler(const std::vector<InterfaceConfiguration>& interface_configurations);

  protected:
    void HandleEvent(const char* uevent, const int message_length) override;

  private:
    const std::vector<InterfaceConfiguration>& interface_configurations_;
};

}  // namespace netman
}  // namespace vcc

#endif  // _NETMAN_EVENT_HANDLER_H_
