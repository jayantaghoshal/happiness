/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "CarProfileManager.h"

#undef LOG_TAG
#define LOG_TAG "CAR_PROFILE_MANAGER"
#include <cutils/log.h>

namespace vendor {
namespace volvocars {
namespace hardware {
namespace profiles {
namespace V1_0 {
namespace implementation {

using namespace ApplicationDataElement;
using namespace autosar;
using namespace tarmac;
using namespace timeprovider;

CarProfileManager::CarProfileManager(std::shared_ptr<ITimeProvider> time_provider)
    : time_provider_(time_provider), prof_chg_timer_handle_(nullptr), profile_changed_cb_(nullptr) {
    prof_pen_sts1_receiver_.subscribe([this]() {
        if (prof_pen_sts1_receiver_.get().isOk()) {
            auto new_profile = prof_pen_sts1_receiver_.get().value();
            {  // lock scope
                std::lock_guard<std::mutex> lock(callback_lock_);
                if (profile_changed_cb_ != nullptr) {
                    profile_changed_cb_->profileChanged("DUMMY_USER_ID", static_cast<ProfileIdentifier>(new_profile));
                }
            }
            ALOGI("ActiveUserProfile changed to: %d", new_profile);  // TODO remove this log
        } else {
            ALOGI("%s signal ProfPenSts1 is in error.", __FUNCTION__);
        }
    });
}

Return<void> CarProfileManager::subscribeUserChange(const sp<IProfileChangedHandler>& cb) {
    {  // lock scope
        std::lock_guard<std::mutex> lock(callback_lock_);
        profile_changed_cb_ = cb;
        return Void();
    }
}

Return<void> CarProfileManager::requestSwitchUser(const hidl_string& androidUserId) {
    ALOGI("%s to change android user to %s", __FUNCTION__, androidUserId.c_str());
    // TODO (ts) Hard cast from string to int, Get ProfileId from settings instead
    auto string_as_int = std::stoi(androidUserId);
    prof_chg_sender_.send(static_cast<IdPen>(string_as_int));
    prof_chg_timer_handle_ = time_provider_->add_single_shot_timer(
            std::chrono::milliseconds{1000}, [this]() { prof_chg_sender_.send(IdPen::ProfUkwn); });
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
    // TODO implement. Store profile and android user in setting
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
    // Subscribe to the two relevant input signals during the pairing sequence
    key_read_sts_receiver_.subscribe([this, onKeySearchCompleted]() {
        if (key_read_sts_receiver_.get().isOk()) {
            auto key_paired = key_read_sts_receiver_.get().value();
            // Remove subscriptions since we expect only one answer to each pair request
            // TODO (TS) Should we unsubscribe only to expected values or not?
            key_read_sts_receiver_.subscribe(nullptr);
            nr_of_key_receiver_.subscribe(nullptr);
            if (key_paired.Boolean) {
                ALOGI("Key successfully paired active profile to key %d", static_cast<KeyId1>(key_paired.KeyId));
                onKeySearchCompleted->keySearchCompleted(KeyPairStatus::Success);
            } else {
                onKeySearchCompleted->keySearchCompleted(KeyPairStatus::NoKeysFound);
            }
        }
    });

    nr_of_key_receiver_.subscribe([this, onKeySearchCompleted]() {
        if (nr_of_key_receiver_.get().isOk()) {
            auto more_than_one_key_found = nr_of_key_receiver_.get().value();
            if (more_than_one_key_found) {
                // Report failure and clear subscriptions related to pairing
                key_read_sts_receiver_.subscribe(nullptr);
                nr_of_key_receiver_.subscribe(nullptr);
                onKeySearchCompleted->keySearchCompleted(KeyPairStatus::TooManyKeysFound);
            }
        }
    });

    // Send search key search request for one second and then back to idle
    switch (searchLocation) {
        case KeyLocation::AllZones:
            key_read_req_sender_.send(KeyLocn1::KeyLocnAllInt);
            break;

        case KeyLocation::BackupReader:
            key_read_req_sender_.send(KeyLocn1::KeyLocnResvInt);
            break;
    }
    time_provider_->add_single_shot_timer(std::chrono::milliseconds{1000},
                                          [this]() { key_read_req_sender_.send(KeyLocn1::KeyLocnIdle); });

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
