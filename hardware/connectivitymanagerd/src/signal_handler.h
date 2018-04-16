/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "signal_handler_interface.h"

namespace vcc {
namespace conman {

namespace conman_hal_v1_0 = ::vendor::volvocars::hardware::connectivitymanager::V1_0;

class SignalHandler : public ISignalHandler {
  public:
    SignalHandler(std::weak_ptr<IConnectivityManager> connectivity_manager);
    virtual ~SignalHandler() = default;

    bool RequestWifiStationMode() override;
    bool RequestSetWifiStationMode(conman_hal_v1_0::WifiStationMode mode) override;

  private:
    std::weak_ptr<IConnectivityManager> connectivity_manager_;
};

}  // namespace conman
}  // namespace vcc