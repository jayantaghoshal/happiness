/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>

#include "connectivity_manager_interface.h"
#include "connectivity_manager_service.h"
#include "signal_handler_interface.h"

namespace vcc {
namespace conman {

namespace conman_hal_v1_0 = ::vendor::volvocars::hardware::connectivitymanager::V1_0;

using UniqueConnectivityManagerService =
        std::unique_ptr<ConnectivityManagerService, void (*)(ConnectivityManagerService*)>;

/*
 *
 */
class ConnectivityManager : public std::enable_shared_from_this<ConnectivityManager>, public IConnectivityManager {
  public:
    ConnectivityManager();
    virtual ~ConnectivityManager() = default;

    void Initialize(std::weak_ptr<ISignalHandler> signal_handler /*, std::weak_ptr<INetmanClient> netman_client*/);

    bool RequestWifiStationMode() override;

    bool RequestWifiStationModeChange(conman_hal_v1_0::WifiStationMode mode) override;

    void NotifyWifiStationMode(conman_hal_v1_0::WifiStationMode mode) override;

  private:
    std::weak_ptr<ISignalHandler> signal_handler_;
    // std::weak_ptr<INetmanClient> netman_client_;

    UniqueConnectivityManagerService connectivity_manager_service_;
};

}  // namespace conman
}  // namespace vcc