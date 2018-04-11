/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>
#include <mutex>

#include <vendor/volvocars/hardware/connectivitymanager/1.0/INativeConnectivityManager.h>
#include <vendor/volvocars/hardware/connectivitymanager/1.0/ISystemConnectivityManager.h>

#include "connectivity_manager_interface.h"

namespace vcc {
namespace conman {

using ::android::hardware::Return;
using ::android::hardware::Void;

namespace conman_hal_v1_0 = ::vendor::volvocars::hardware::connectivitymanager::V1_0;

/*
 *
 */
class ConnectivityManagerService : public conman_hal_v1_0::INativeConnectivityManager {
  public:
    ConnectivityManagerService(std::weak_ptr<IConnectivityManager> connectivity_manager);
    ~ConnectivityManagerService() = default;

    void StartService();
    void StopService();

    Return<void> registerConnectivityManagerHandler(
            const ::android::sp<conman_hal_v1_0::ISystemConnectivityManager>& handler) override;

    Return<void> unregisterConnectivityManagerHandler(
            const ::android::sp<conman_hal_v1_0::ISystemConnectivityManager>& handler) override;

    Return<bool> requestWifiStationMode() override;

    Return<bool> requestWifiStationModeChange(conman_hal_v1_0::WifiStationMode mode) override;

  private:
    ::android::sp<conman_hal_v1_0::ISystemConnectivityManager> system_gw_handler_;

    std::weak_ptr<IConnectivityManager> connectivity_manager_;
};

}  // namespace conman
}  // namespace vcc
