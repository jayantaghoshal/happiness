/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "installation_master_daemon.h"

#define LOG_TAG "InstallationMasterD"
#include <cutils/log.h>
namespace com {
namespace volvocars {
namespace installationmaster {

bool InstallationMasterDaemon::Initialize() {
    // Subscribe to ipcbd
    android::status_t status = IInstallationMaster::registerAsService();
    if (status != android::OK) {
        ALOGW("Failed to register installation master binder service: %d", status);
        return false;
    } else {
        ALOGV("installation master binder service register ok");
    }
    return true;
}

/**
 * Assigns an installation order for installation. Provided that both the Installation Master and SoftwareUpdateService
 * uses the same working directory (e.g., /vendor/data/OTA), Installation Master will find the installation order that
 * is assigned for installation in that directory. The progress of the installation will be broadcasted to all
 * registered IInstallationMasterEventListeners.
 * @param installationOrder The UUID of the installation order to install.
 */
Return<void> InstallationMasterDaemon::assignInstallation(const hidl_string& installationOrder) {
    ALOGI("assignInstallation for installationOrder: %s", installationOrder.c_str());

    // Used for testing! Todo: Replace with real implementation
    for (auto& listener : listeners_) {
        listener->installNotification(installationOrder, InstallNotification::INSTALLATION_STARTED);
    }
    return Void();
}

/**
 * Ask installation master to verify downloaded files against an installation order. If verification failes,
 * an installation report will be created and returned, (else null ?)
 * @param installationOrder The UUID of the the Installation order to verify.
 * @return datafiles        List of data files and their status.
 */
Return<void> InstallationMasterDaemon::verifyDownload(const hidl_string& installationOrder,
                                                      verifyDownload_cb _hidl_cb) {
    ALOGI("verifyDownload for installationOrder: %s", installationOrder.c_str());

    // Used for testing! Todo: Replace with real implementation
    std::vector<DataFile> data_files = {};
    _hidl_cb(data_files);

    InstallationSummary summary;
    for (auto& listener : listeners_) {
        listener->installationReport(installationOrder, summary);
    }
    return Void();
}

/**
 * Register a handle to the client for the installation master to call.
 * @param eventListener An implemetination of Iimel
 */
Return<void> InstallationMasterDaemon::registerInstallationStatusListener(
        const android::sp<IInstallationMasterEventListener>& eventListener) {
    listeners_.push_back(eventListener);
    return Void();
}

}  // namespace installationmaster
}  // namespace volvocars
}  // namespace com