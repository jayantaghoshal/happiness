/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VENDOR_VOLVOCARS_HARDWARE_PROFILES_V1_0_CARPROFILEMANAGER_H
#define VENDOR_VOLVOCARS_HARDWARE_PROFILES_V1_0_CARPROFILEMANAGER_H

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/volvocars/hardware/profiles/1.0/ICarProfileManager.h>
#include <string>

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

struct CarProfileManager : public ICarProfileManager {
    CarProfileManager();
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
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor

#endif  // VENDOR_VOLVOCARS_HARDWARE_PROFILES_V1_0_CARPROFILEMANAGER_H
