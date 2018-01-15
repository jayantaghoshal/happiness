/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VENDOR_VOLVOCARS_HARDWARE_PROFILES_V1_0_CARPROFILEMANAGER_H
#define VENDOR_VOLVOCARS_HARDWARE_PROFILES_V1_0_CARPROFILEMANAGER_H

#include <mutex>

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/volvocars/hardware/profiles/1.0/ICarProfileManager.h>
#include <string>
#include "itime_provider.h"

#include "Application_dataelement.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace profiles {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using namespace tarmac::timeprovider;

class CarProfileManager : public ICarProfileManager {
  public:
    CarProfileManager(std::shared_ptr<ITimeProvider> time_provider);
    Return<void> subscribeUserChange(const sp<IProfileChangedHandler>& cb) override;
    Return<void> requestSwitchUser(const hidl_string& androidUserId) override;
    Return<void> getUserProfileInformation(const hidl_string& androidUserId,
                                           getUserProfileInformation_cb _hidl_cb) override;
    Return<int16_t> getNrOfUnusedProfiles() override;
    Return<bool> pairAndroidUserToUnusedVehicleProfile(const hidl_string& androidUserId) override;
    Return<void> deleteUser(const hidl_string& androidUserId) override;
    Return<void> pairCurrentProfileToKey(KeyLocation searchLocation,
                                         const sp<IKeyPairResponseHandler>& onKeySearchCompleted) override;
    Return<bool> disconnectCurrentProfileFromKey() override;

  private:
    ApplicationDataElement::DESender<autosar::ProfChg_info> prof_chg_sender_;
    ApplicationDataElement::DEReceiver<autosar::ProfPenSts1_info> prof_pen_sts1_receiver_;
    ApplicationDataElement::DESender<autosar::KeyReadReqFromSetgMgr_info> key_read_req_sender_;
    ApplicationDataElement::DEReceiver<autosar::KeyReadStsToProfCtrl_info> key_read_sts_receiver_;
    ApplicationDataElement::DEReceiver<autosar::NrOfKeyAvl_info> nr_of_key_receiver_;

    std::shared_ptr<ITimeProvider> time_provider_;
    std::unique_ptr<TimerSubscriptionHandle> prof_chg_timer_handle_;

    sp<IProfileChangedHandler> profile_changed_cb_;
    std::mutex callback_lock_;  // Guard for registering and using callback
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor

#endif  // VENDOR_VOLVOCARS_HARDWARE_PROFILES_V1_0_CARPROFILEMANAGER_H
