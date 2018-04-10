/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <vendor/volvocars/hardware/installationmaster/1.0/IInstallationMaster.h>
#include <vendor/volvocars/hardware/installationmaster/1.0/IInstallationMasterEventListener.h>
#include <list>

using ::vendor::volvocars::hardware::installationmaster::V1_0::DataFile;
using ::vendor::volvocars::hardware::installationmaster::V1_0::IInstallationMaster;
using ::vendor::volvocars::hardware::installationmaster::V1_0::IInstallationMasterEventListener;
using ::vendor::volvocars::hardware::installationmaster::V1_0::InstallationStatus;
using ::vendor::volvocars::hardware::installationmaster::V1_0::Summary;

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;

namespace com {
namespace volvocars {
namespace installationmaster {

class InstallationMasterDaemon : public IInstallationMaster {
  private:
    std::list<android::sp<IInstallationMasterEventListener>> listeners_;

  public:
    InstallationMasterDaemon() = default;
    ~InstallationMasterDaemon() = default;

    bool Initialize();

  private:
    // Methods from IInstallationMaster follow.
    /**
     * Assigns an installation order for installation. Provided that both the Installation Master and
     * SoftwareUpdateService
     * uses the same working directory (e.g., /vendor/data/OTA), Installation Master will find the installation order
     * that
     * is assigned for installation in that directory. The progress of the installation will be broadcasted to all
     * registered
     * IInstallationMasterEventListeners.
     * @param installationOrder The UUID of the installation order to install.
     */
    Return<void> assignInstallation(const hidl_string& installationOrder);

    /**
     * Ask installation master to verify downloaded files against an installation order. If verification failes,
     * an installation report will be created and returned, (else null ?)
     * @param installationOrder The UUID of the the Installation order to verify.
     * @return datafiles        List of data files and their status.
     */
    Return<void> verifyDownload(const hidl_string& installationOrder, verifyDownload_cb _hidl_cb);

    /**
     * Register a handle to the client for the installation master to call.
     * @param eventListener An implemetination of Iimel
     */
    Return<void> registerInstallationStatusListener(const android::sp<IInstallationMasterEventListener>& eventListener);

    void TryNotifyListener(
            const std::function<Return<void>(std::list<android::sp<IInstallationMasterEventListener>>::iterator)>& f);
};

}  // namespace installationmaster
}  // namespace volvocars
}  // namespace com