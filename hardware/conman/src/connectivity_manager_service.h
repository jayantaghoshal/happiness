/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>
#include <mutex>

#include <vendor/volvocars/hardware/connectivitymanager/1.0/INativeConnectivityManager.h>
#include <vendor/volvocars/hardware/connectivitymanager/1.0/ISystemConnectivityManager.h>

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::vendor::volvocars::hardware::connectivitymanager::V1_0::INativeConnectivityManager;
using ::vendor::volvocars::hardware::connectivitymanager::V1_0::ISystemConnectivityManager;
using ::vendor::volvocars::hardware::connectivitymanager::V1_0::WifiStationMode;

namespace vcc {
namespace conman {

/*
 *
 */
class ConnectivityManagerService : public INativeConnectivityManager {
  public:
    explicit ConnectivityManagerService();
    ~ConnectivityManagerService() = default;

    void StartService();

    virtual Return<void> registerConnectivityManagerHandler(
            const ::android::sp<ISystemConnectivityManager>& handler) final;

    virtual Return<void> unregisterConnectivityManagerHandler(
            const ::android::sp<ISystemConnectivityManager>& handler) final;

    virtual Return<bool> requestWifiStationMode() final;

    virtual Return<bool> requestWifiStationModeChange(WifiStationMode mode) final;

  private:
    std::mutex guard_;
    ::android::sp<ISystemConnectivityManager> system_gw_handler_;
};

}  // namespace conman
}  // namespace vcc
