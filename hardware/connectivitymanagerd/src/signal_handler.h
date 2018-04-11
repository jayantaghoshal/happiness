/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "signal_handler_interface.h"

namespace vcc {
namespace conman {

class SignalHandler : public ISignalHandler {
  public:
    SignalHandler(std::weak_ptr<IConnectivityManager> connectivity_manager);
    virtual ~SignalHandler() = default;

    bool RequestWifiStationMode() override;
    bool RequestSetWifiStationMode(/* WifiStationMode mode */) override;

  private:
    std::weak_ptr<IConnectivityManager> connectivity_manager_;
};

}  // namespace conman
}  // namespace vcc