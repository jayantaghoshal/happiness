/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "CarProfileManager.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace profiles {
namespace V1_0 {
namespace implementation {

CarProfileManager::CarProfileManager() {}

Return<void> CarProfileManager::subscribeUserChange(const sp<IProfileChangedHandler>& cb) {
    // TODO implement
    (void)cb;
    return Void();
}

Return<void> CarProfileManager::requestSwitchUser(const hidl_string& androidUserId) {
    // TODO implement
    (void)androidUserId;
    return Void();
}

Return<void> CarProfileManager::getUserProfileInformation(const hidl_string& androidUserId,
                                                          getUserProfileInformation_cb _hidl_cb) {
    // TODO implement
    (void)androidUserId;
    (void)_hidl_cb;
    return Void();
}

Return<int16_t> CarProfileManager::getNrOfUnusedProfiles() {
    // TODO implement
    return int16_t{};
}

Return<bool> CarProfileManager::pairAndroidUserToUnusedVehicleProfile(const hidl_string& androidUserId) {
    // TODO implement
    (void)androidUserId;
    return bool{};
}

Return<void> CarProfileManager::deleteUser(const hidl_string& androidUserId) {
    // TODO implement
    (void)androidUserId;
    return Void();
}

Return<void> CarProfileManager::pairCurrentProfileToKey(KeyLocation searchLocation,
                                                        const sp<IKeyPairResponseHandler>& onKeySearchCompleted) {
    // TODO implement
    (void)searchLocation;
    (void)onKeySearchCompleted;
    return Void();
}

Return<bool> CarProfileManager::disconnectCurrentProfileFromKey() {
    // TODO implement
    return bool{};
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
