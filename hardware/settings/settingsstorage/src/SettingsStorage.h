/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VENDOR_VOLVOCARS_HARDWARE_SETTINGS_V1_0_SETTINGSSTORAGE_H
#define VENDOR_VOLVOCARS_HARDWARE_SETTINGS_V1_0_SETTINGSSTORAGE_H

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <sqlite3.h>
#include <vendor/volvocars/hardware/profiles/1.0/ICarProfileManager.h>
#include <vendor/volvocars/hardware/profiles/1.0/IProfileChangedHandler.h>
#include <vendor/volvocars/hardware/profiles/1.0/types.h>
#include <vendor/volvocars/hardware/settings/1.0/ISettingsStorage.h>
#include <list>

namespace vendor {
namespace volvocars {
namespace hardware {
namespace settings {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

namespace andrHw = ::android::hardware;
namespace profileHidl = vendor::volvocars::hardware::profiles::V1_0;

class ProfileListener : public profileHidl::IProfileChangedHandler,
                        public ::android::hidl::manager::V1_0::IServiceNotification,
                        public andrHw::hidl_death_recipient {
  public:
    ProfileListener(std::function<void(profileHidl::ProfileIdentifier)> onProfileChange);
    Return<void> profileChanged(const andrHw::hidl_string& android_user_id,
                                profileHidl::ProfileIdentifier new_profile_id) override;
    andrHw::Return<void> onRegistration(const andrHw::hidl_string& fqName, const andrHw::hidl_string& name,
                                        bool preexisting) override;
    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override;

  private:
    sp<profileHidl::ICarProfileManager> car_profile_manager_;
    std::function<void(profileHidl::ProfileIdentifier)> on_profile_change_;
};

struct SettingsListener {
    SettingsListener(const std::string key, const UserScope user_scope, const sp<ISettingsListener>& listener)
        : key_{key}, user_scope_{user_scope}, listener_{listener} {}
    const std::string key_;
    const UserScope user_scope_;
    const sp<ISettingsListener> listener_;
};

class SettingsStorage : public ISettingsStorage, public andrHw::hidl_death_recipient {
  public:
    SettingsStorage();
    ~SettingsStorage();

    Return<void> set(const hidl_string& key, profileHidl::ProfileIdentifier profileId,
                     const hidl_string& data) override;
    Return<void> get(const hidl_string& key, profileHidl::ProfileIdentifier profileId, get_cb _hidl_cb) override;
    Return<void> subscribe(const hidl_string& key, UserScope userScope, const sp<ISettingsListener>& listener) override;
    Return<void> unsubscribe(const hidl_string& key, const sp<ISettingsListener>& listener) override;
    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override;

  private:
    // Returned pointer is valid until next time getData is called, or until this is destroyed
    const unsigned char* getData(const hidl_string& key, profileHidl::ProfileIdentifier profileId);
    void onProfileChange(profileHidl::ProfileIdentifier profileId);
    profileHidl::ProfileIdentifier activeProfileId = profileHidl::ProfileIdentifier::None;

    std::list<SettingsListener> settings_listeners_;
    android::sp<ProfileListener> profile_listener_;

    sqlite3* db_ = nullptr;
    sqlite3_stmt* insert_stmt_ = nullptr;
    sqlite3_stmt* select_setting_stmt_ = nullptr;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace settings
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor

#endif  // VENDOR_VOLVOCARS_HARDWARE_SETTINGS_V1_0_SETTINGSSTORAGE_H
