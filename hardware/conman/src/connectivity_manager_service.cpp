/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "connectivity_manager_service.h"

#undef LOG_TAG
#define LOG_TAG "ConMan.Main"
#include <cutils/log.h>

namespace vcc {
namespace conman {

/*
 *
 */
ConnectivityManagerService::ConnectivityManagerService() { ALOGD("Non-trivial constructor. Aspiring to be at least."); }

void ConnectivityManagerService::StartService() {
    ALOGD("%s", __FUNCTION__);
    android::status_t status = INativeConnectivityManager::registerAsService();
    if (status != android::OK) {
        ALOGW("Failed to register binder service: %d", status);
    } else {
        ALOGV("Binder service register ok");
    }
}

::android::hardware::Return<void> ConnectivityManagerService::registerConnectivityManagerHandler(
        const ::android::sp<ISystemConnectivityManager>& handler) {
    ALOGD("%s", __FUNCTION__);

    // TODO (Patrik): validate sender is a system java service
    ALOGD("TODO (Patrik): validate sender is a system java service");
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (nullptr == system_gw_handler_.get()) {
            system_gw_handler_ = handler;
        } else {
            ALOGE("Re-registration for binder ConnectivityManager denied..");
        }
    }

    return Void();
}

::android::hardware::Return<void> ConnectivityManagerService::unregisterConnectivityManagerHandler(
        const ::android::sp<ISystemConnectivityManager>& handler) {
    ALOGD("%s", __FUNCTION__);
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (system_gw_handler_ == handler) {
            system_gw_handler_.clear();
        }
    }
    return Void();
}

::android::hardware::Return<bool> ConnectivityManagerService::requestWifiStationMode() {
    ALOGD("%s", __FUNCTION__);

    ALOGE("Not Implemented");

    return false;
}

::android::hardware::Return<bool> ConnectivityManagerService::requestWifiStationModeChange(WifiStationMode mode) {
    ALOGD("%s", __FUNCTION__);

    ALOGE("Not Implemented");

    (void)mode;
    return false;
}

}  // namespace conman
}  // namespace vcc