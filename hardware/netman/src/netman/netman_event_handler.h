/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

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
