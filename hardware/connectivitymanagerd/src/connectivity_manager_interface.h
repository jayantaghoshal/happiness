/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <vendor/volvocars/hardware/connectivitymanager/1.0/types.h>

namespace vcc {
namespace conman {

namespace conman_hal_v1_0 = ::vendor::volvocars::hardware::connectivitymanager::V1_0;

/*
 *
 */
class IConnectivityManager {
  public:
    virtual ~IConnectivityManager() = default;

    virtual bool RequestWifiStationMode() = 0;

    virtual bool RequestWifiStationModeChange(conman_hal_v1_0::WifiStationMode mode) = 0;

    virtual void NotifyWifiStationMode(conman_hal_v1_0::WifiStationMode mode) = 0;
};

}  // namespace conman
}  // namespace vcc