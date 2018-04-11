/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>

#include "connectivity_manager_interface.h"

namespace vcc {
namespace conman {

class ISignalHandler {
  public:
    virtual ~ISignalHandler() = default;

    virtual bool RequestWifiStationMode() = 0;
    virtual bool RequestSetWifiStationMode(/* WifiStationMode mode */) = 0;
};

}  // namespace conman
}  // namespace vcc