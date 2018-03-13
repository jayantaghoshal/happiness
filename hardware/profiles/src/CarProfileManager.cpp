/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "CarProfileManager.h"
// #include <algorithm>
#include "internalsignals.h"
#include "libsettings/settingidentifiers.h"
#include "vcc/localconfig.h"
#include "vfc_helper.h"

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
using namespace ApplicationDataElement::Helper;
using namespace autosar;
using namespace tarmac;
using namespace timeprovider;
namespace andrHw = ::android::hardware;

static constexpr uint64_t IPROFILE_HAL_DEATH_COOKIE = 0xDEAD;

CarProfileManager::CarProfileManager(std::shared_ptr<TimerManagerInterface> time_provider,
                                     const android::sp<SettingsManager>& manager)
    : UNUSED_ANDROID_USER("0"),
      time_provider_(std::move(time_provider)),
      prof_chg_timer_handle_(nullptr),
      // TODO (TS) what is the default android user? Now set to "0"
      s_android_user_(SettingId::AndroidUserId, UNUSED_ANDROID_USER, manager),
      s_current_profile_(SettingId::CurrentProfileId, IdPen::Prof13, manager),
      s_profile_info_(SettingId::PairedKeyId, KeyId::None, manager),
      current_profile_(ProfileIdentifier::None),
      state_(State::IDLE) {
    // Keep mapping between android user and profile in sync with paired users
    s_android_user_.setCallback([this](const auto&) {
        ALOGV("s_android_user callback triggered");
        const std::vector<IdPen> all_id{IdPen::Prof1,
                                        IdPen::Prof2,
                                        IdPen::Prof3,
                                        IdPen::Prof4,
                                        IdPen::Prof5,
                                        IdPen::Prof6,
                                        IdPen::Prof7,
                                        IdPen::Prof8,
                                        IdPen::Prof9,
                                        IdPen::Prof10,
                                        IdPen::Prof11,
                                        IdPen::Prof12};  // TODO (TS) IdPen::Prof13 == Guest. How will that be used??
                                                         // android_user_to_profile_.clear();
        decltype(android_user_to_profile_) android_user_to_profile_temp;
        decltype(unused_profiles_) unused_profiles_temp;
        for (const auto id : all_id) {
            auto android_user = s_android_user_.getForProfile(static_cast<SettingsFramework::ProfileIdentifier>(id));
            if (android_user != UNUSED_ANDROID_USER) {
                android_user_to_profile_temp.emplace(android_user, id);
            } else {
                unused_profiles_temp.push(id);
            }
        }
        {  // lock scope
            std::lock_guard<std::mutex> lock(profile_storage_lock_);
            android_user_to_profile_ = android_user_to_profile_temp;
            unused_profiles_ = unused_profiles_temp;
        }
    });

    // Handle the notifications caused by the profile change acknowledged by CEM
    s_current_profile_.setCallback([this](const auto& current_profile) {
        ALOGD("Setting current_profile was updated");  // TODO (TS) make verbose
        current_profile_ = static_cast<ProfileIdentifier>(current_profile.value);
        std::list<sp<IProfileChangedHandler>> listeners_to_notify;
        {
            // Copy list to avoid recursive lock cb->profileChanged() -> listener.???() -> subscribeUserChange()
            std::lock_guard<std::mutex> lock(callback_lock_);
            listeners_to_notify = profile_changed_listeners_;
        }

        auto new_android_user =
                s_android_user_.getForProfile(static_cast<SettingsFramework::ProfileIdentifier>(current_profile_));
        for (auto& cb : listeners_to_notify) {
            auto status = cb->profileChanged(new_android_user, current_profile_);
            if (!status.isOk()) {
                ALOGW("ProfileChangedHandler.profileChanged callback failed: %s", status.description().c_str());
            }
        }
    });

    // Update the current profile setting
    prof_pen_sts1_receiver_.subscribe([this]() {
        if (prof_pen_sts1_receiver_.get().isOk()) {
            auto current_profile = prof_pen_sts1_receiver_.get().value();
            ALOGI("ActiveUserProfile changed to: %d", current_profile);
            s_current_profile_.set(current_profile);
        } else {
            ALOGI("Signal ProfPenSts1 is in error.");
        }
    });
}

Return<void> CarProfileManager::subscribeUserChange(const sp<IProfileChangedHandler>& cb) {
    {  // lock scope
        std::lock_guard<std::mutex> lock(callback_lock_);
        profile_changed_listeners_.push_back(cb);
    }

    // TODO (TS) is this if correct? Could a setting be None for current_profile?
    if (current_profile_ != ProfileIdentifier::None) {
        try {
            auto current_user =
                    s_android_user_.getForProfile(static_cast<SettingsFramework::ProfileIdentifier>(current_profile_));
            ALOGD("%s current user=%s", __FUNCTION__, current_user.c_str());  // TODO (TS) remove log
            auto status = cb->profileChanged(current_user, current_profile_);
            if (!status.isOk()) {
                ALOGW("ProfileChangedHandler.subscribeUserChange callback failed: %s", status.description().c_str());
            }
        } catch (const std::runtime_error& e) {
            // If current_profile != None and settings proxy is dead I guess proxy must has connected and then died
            // again
            ALOGI("Settings proxy was dead while trying to add callback");
        }
    }
    cb->linkToDeath(this, IPROFILE_HAL_DEATH_COOKIE);
    return Void();
}

void CarProfileManager::serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) {
    if (cookie != IPROFILE_HAL_DEATH_COOKIE) {
        return;
    }

    android::sp<::android::hidl::base::V1_0::IBase> lockedWho = who.promote();
    if (lockedWho == nullptr) {
        return;  // If wp can't be promoted it means sp is already removed from profile_changed_listeners_
    }
    std::lock_guard<std::mutex> lock(callback_lock_);
    profile_changed_listeners_.remove_if([&lockedWho](const auto& x) { return (x == lockedWho); });
}

Return<void> CarProfileManager::requestSwitchUser(const hidl_string& androidUserId) {
    // TODO (TS)(TS) This should probably be alowed only when standing still
    ALOGI("%s to change android user to %s", __FUNCTION__, androidUserId.c_str());
    autosar::IdPen profile_id;
    {  // lock scope
        std::lock_guard<std::mutex> lock(profile_storage_lock_);
        auto user_it = android_user_to_profile_.find(androidUserId);
        if (user_it == android_user_to_profile_.end()) {
            ALOGE("Tried to switch to android user %s that has not been created yet", androidUserId.c_str());
            return Void();
        }
        profile_id = user_it->second;
    }
    request_vfc(Vfc::UserInputSettings);
    prof_chg_sender_.send(profile_id);
    prof_chg_timer_handle_ = time_provider_->AddSingleShotTimer(std::chrono::milliseconds{1000},
                                                                [this]() { prof_chg_sender_.send(IdPen::ProfUkwn); });
    return Void();
}

Return<KeyId> CarProfileManager::getUserProfileInformation(const hidl_string& androidUserId) {
    ALOGV("%s android_user=%s", __FUNCTION__, androidUserId.c_str());

    SettingsFramework::ProfileIdentifier profile_id;
    {  // lock scope
        std::lock_guard<std::mutex> lock(profile_storage_lock_);
        auto user_it = android_user_to_profile_.find(androidUserId);
        if (user_it == android_user_to_profile_.end()) {
            ALOGE("Tried to get info for android user %s that has not been created yet", androidUserId.c_str());
            return Return<KeyId>(andrHw::Status::fromExceptionCode(andrHw::Status::Exception::EX_ILLEGAL_ARGUMENT));
        }

        profile_id = static_cast<SettingsFramework::ProfileIdentifier>(user_it->second);
    }

    try {
        // This profile could be other user than than the active one
        return s_profile_info_.getForProfile(profile_id);
    } catch (const std::runtime_error& e) {
        // Hidl connection to settingsstorage was not established
        return Return<KeyId>(andrHw::Status::fromExceptionCode(andrHw::Status::Exception::EX_ILLEGAL_STATE));
    }
}

Return<int16_t> CarProfileManager::getNrOfUnusedProfiles() { return static_cast<int16_t>(unused_profiles_.size()); }

Return<bool> CarProfileManager::pairAndroidUserToUnusedVehicleProfile(const hidl_string& androidUserId) {
    ALOGD("%s android_user_id=%s", __FUNCTION__, androidUserId.c_str());  // TODO (TS) remove log

    autosar::IdPen profile_to_map;
    {  // lock scope
        std::lock_guard<std::mutex> lock(profile_storage_lock_);
        const auto& user_it = android_user_to_profile_.find(androidUserId);
        if (user_it != android_user_to_profile_.end()) {
            ALOGI("Android user %s is already mapped to profile %d",
                  androidUserId.c_str(),
                  static_cast<int>(user_it->second));
            return true;
        }

        if (unused_profiles_.empty()) {
            ALOGI("No unused profiles available");
            return false;
        }

        profile_to_map = unused_profiles_.front();
        unused_profiles_.pop();
        // Set forForProfile will not trigger setting callback if current profile != profile_to_map. Hence we need to
        // update
        // the android_user_to_profile_ manually here
        android_user_to_profile_.emplace(androidUserId, profile_to_map);
    }

    s_android_user_.setForProfile(androidUserId, static_cast<SettingsFramework::ProfileIdentifier>(profile_to_map));
    ALOGI("Map %s to profile %d", androidUserId.c_str(), static_cast<int>(profile_to_map));
    return true;
}

Return<void> CarProfileManager::deleteUser(const hidl_string& androidUserId) {
    autosar::IdPen profile_id;
    {  // lock scope
        std::lock_guard<std::mutex> lock(profile_storage_lock_);
        auto user_it = android_user_to_profile_.find(androidUserId);
        if (user_it == android_user_to_profile_.end()) {
            ALOGE("Tried unpair for android user %s that has not been created yet", androidUserId.c_str());
            return Void();
        }
        profile_id = user_it->second;
    }

    s_android_user_.setForProfile(UNUSED_ANDROID_USER, static_cast<SettingsFramework::ProfileIdentifier>(profile_id));

    disconnectProfileFromKey(static_cast<ProfileIdentifier>(profile_id));
    return Void();
}

Return<void> CarProfileManager::pairCurrentProfileToKey(KeyLocation searchLocation,
                                                        const sp<IKeyPairResponseHandler>& onKeySearchCompleted) {
    ALOGV("%s. searchLocation=%d", __FUNCTION__, static_cast<int>(searchLocation));

    if (state_ != State::IDLE) {
        ALOGI("Pair current profile to key already in progress, ignoring new request");
        return Void();
    }

    // This signal indicates key found and paired or no key found
    key_read_sts_receiver_.subscribe([this, onKeySearchCompleted]() {
        ALOGD("KeyReadStsToProfCtrl_info callback triggered");  // TODO (TS) remove log

        if (state_ != State::KEY_SEARCH_ACTIVE) {
            return;
        }

        if (key_read_sts_receiver_.get().isOk()) {
            auto key_paired = key_read_sts_receiver_.get().value();
            ALOGD("KeyReadStsToProfCtrl_info with value: %d", static_cast<int>(key_paired.Boolean));
            // Remove subscriptions since we expect only one answer on flexray to each pair request
            key_read_sts_receiver_.subscribe(nullptr);
            nr_of_key_receiver_.subscribe(nullptr);
            // Clear the timeout timer
            pair_key_timeout_handle_ = nullptr;
            if (key_paired.Boolean) {
                ALOGI("Key successfully paired active profile to key %d", static_cast<KeyId1>(key_paired.KeyId));
                s_profile_info_.set(static_cast<KeyId>(key_paired.KeyId));
                auto status = onKeySearchCompleted->keySearchCompleted(KeyPairStatus::Success);
                if (!status.isOk()) {
                    ALOGW("onKeySearchCompleted hidl call returned an error: %s", status.description().c_str());
                }
            } else {
                // TODO (TS) not sure if this is a valid case. Will there be a keypaired.Boolean==false or should we
                // only
                // send NoKeysFound in the timeout handling?
                ALOGI("No key found - Key not successfully paired");
                auto status = onKeySearchCompleted->keySearchCompleted(KeyPairStatus::NoKeysFound);
                if (!status.isOk()) {
                    ALOGW("onKeySearchCompleted hidl call returned an error: %s", status.description().c_str());
                }
            }
            state_ = State::IDLE;
        }
    });

    // This signal indicates if more than one key was found
    nr_of_key_receiver_.subscribe([this, onKeySearchCompleted]() {
        if (state_ != State::KEY_SEARCH_ACTIVE) {
            return;
        };
        if (nr_of_key_receiver_.get().isOk()) {
            auto more_than_one_key_found = nr_of_key_receiver_.get().value();
            if (more_than_one_key_found) {
                ALOGI("Key search failed - more than one key found during key search.");
                // Report failure and clear subscriptions related to pairing
                key_read_sts_receiver_.subscribe(nullptr);
                nr_of_key_receiver_.subscribe(nullptr);
                state_ = State::IDLE;
                onKeySearchCompleted->keySearchCompleted(KeyPairStatus::TooManyKeysFound);
            }
        }
    });

    // Send search key search request for one second...
    state_ = State::KEY_SEARCH_ACTIVE;
    request_vfc(Vfc::UserInputSettings);
    std::chrono::milliseconds timeout{};
    switch (searchLocation) {
        case KeyLocation::AllZones:
            ALOGD("Send fr to search all zones");  // TODO (TS) remove log
            timeout = std::chrono::milliseconds{
                    vcc::LocalConfigDefault()->GetInt("vehiclefunctions.profiles.T_PassiveKeysearch_Timeout_ms")};
            key_read_req_sender_.send(KeyLocn1::KeyLocnAllInt);
            break;

        case KeyLocation::BackupReader:
            ALOGD("Send fr to search backup reader");  // TODO (TS) remove log
            timeout = std::chrono::milliseconds{
                    vcc::LocalConfigDefault()->GetInt("vehiclefunctions.profiles.T_Keysearch_Timeout_ms")};
            key_read_req_sender_.send(KeyLocn1::KeyLocnResvInt);
            break;
        default:
            ALOGE("Incorrect key search location %d", static_cast<int>(searchLocation));
            state_ = State::IDLE;
            key_read_sts_receiver_.subscribe(nullptr);
            nr_of_key_receiver_.subscribe(nullptr);
            return Void();
    }

    // ... and then back to idle
    pair_key_timer_handle_ = time_provider_->AddSingleShotTimer(
            std::chrono::milliseconds{1000}, [this]() { key_read_req_sender_.send(KeyLocn1::KeyLocnIdle); });

    // Timeout handling in case we get no response from key search/pairing.
    pair_key_timeout_handle_ = time_provider_->AddSingleShotTimer(timeout, [this, onKeySearchCompleted]() {
        ALOGI("Key pairing failed because of timeout while waiting for response from CEM");
        key_read_sts_receiver_.subscribe(nullptr);
        nr_of_key_receiver_.subscribe(nullptr);
        onKeySearchCompleted->keySearchCompleted(KeyPairStatus::NoKeysFound);
        state_ = State::IDLE;
    });

    return Void();
}

Return<bool> CarProfileManager::disconnectCurrentProfileFromKey() {
    disconnectProfileFromKey(current_profile_);
    return true;
}

void CarProfileManager::disconnectProfileFromKey(ProfileIdentifier profile) {
    KeyProfMpgUpd1 key_and_user_to_unpair{};
    key_and_user_to_unpair.KeyProfMpgUpdOff = true;
    key_and_user_to_unpair.KeyProfMpgUpdForIdPen = static_cast<autosar::IdPen>(profile);
    key_prof_mgr_upd_sender_.send(key_and_user_to_unpair);

    unpair_key_timer_handle_ = time_provider_->AddSingleShotTimer(
            std::chrono::milliseconds{1000}, [this, key_and_user_to_unpair]() mutable {
                key_and_user_to_unpair.KeyProfMpgUpdOff = false;
                key_prof_mgr_upd_sender_.send(key_and_user_to_unpair);
            });
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
