/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "connectivity_manager_service.h"

#undef LOG_TAG
#define LOG_TAG "ConMan.ConManService"
#include <cutils/log.h>

namespace vcc {
namespace conman {

using ::android::hardware::Return;

/*
 *
 */
ConnectivityManagerService::ConnectivityManagerService(std::weak_ptr<IConnectivityManager> connectivity_manager) {
    if (auto spt = connectivity_manager.lock()) {
        std::swap(connectivity_manager_, connectivity_manager);
    } else {
        throw std::runtime_error("Connectivity Manager was expired");
    }
}

void ConnectivityManagerService::StartService() {
    ALOGV("%s", __FUNCTION__);
    android::status_t status = conman_hal_v1_0::INativeConnectivityManager::registerAsService();

    ALOGW_IF(status != android::OK, "Failed to register binder service: %d", status);
}

void ConnectivityManagerService::StopService() {
    ALOGV("%s", __FUNCTION__);
    ALOGE("wat");
}

Return<void> ConnectivityManagerService::registerConnectivityManagerHandler(
        const ::android::sp<conman_hal_v1_0::ISystemConnectivityManager>& handler) {
    ALOGV("%s", __FUNCTION__);

    // TODO (Patrik): validate sender is a system java service
    ALOGW("TODO (Patrik): validate sender is a system java service");

    if (nullptr == system_gw_handler_.get()) {
        system_gw_handler_ = handler;
    } else {
        ALOGE("Re-registration for binder ConnectivityManager denied..");
    }

    return Void();
}

Return<void> ConnectivityManagerService::unregisterConnectivityManagerHandler(
        const ::android::sp<conman_hal_v1_0::ISystemConnectivityManager>& handler) {
    ALOGV("%s", __FUNCTION__);

    if (system_gw_handler_ == handler) {
        system_gw_handler_.clear();
    }

    return Void();
}

Return<bool> ConnectivityManagerService::requestWifiStationMode() {
    ALOGV("%s", __FUNCTION__);

    if (auto spt = connectivity_manager_.lock()) {
        return spt->RequestWifiStationMode();
    }

    throw std::runtime_error("Connectivity Manager has expired");
}

Return<bool> ConnectivityManagerService::requestWifiStationModeChange(conman_hal_v1_0::WifiStationMode mode) {
    ALOGV("%s", __FUNCTION__);

    if (auto spt = connectivity_manager_.lock()) {
        return spt->RequestWifiStationModeChange(mode);
    }

    throw std::runtime_error("Connectivity Manager has expired");
}

}  // namespace conman
}  // namespace vcc