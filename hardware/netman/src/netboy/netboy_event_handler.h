/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "uevent_handler.h"

namespace vcc {
namespace netman {

class NetboyEventHandler final : public UeventHandler {
  protected:
    void HandleEvent(const char* uevent, int message_length) override;

  private:
    void RuleMatchAndRun(const UeventHandler::NetDeviceAttr& device);
};

}  // namespace netman
}  // namespace vcc
