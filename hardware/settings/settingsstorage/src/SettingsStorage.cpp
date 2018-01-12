/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "SettingsStorage.h"

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "SettingsStorage"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace settings {
namespace V1_0 {
namespace implementation {

Return<void> SettingsStorage::set(const hidl_string& key,
                                  ::vendor::volvocars::hardware::profiles::V1_0::ProfileIdentifier profileId,
                                  const hidl_string& data) {
    ALOGD("set %s = %s", key.c_str(), data.c_str());
    const auto fullKey = std::make_pair(key, profileId);
    storage[fullKey] = data;
    return Void();
}

Return<void> SettingsStorage::get(const hidl_string& key,
                                  ::vendor::volvocars::hardware::profiles::V1_0::ProfileIdentifier profileId,
                                  get_cb _hidl_cb) {
    ALOGD("get %s", key.c_str());
    const auto fullKey = std::make_pair(key, profileId);
    const auto& value = storage[fullKey];
    _hidl_cb(value);
    return Void();
}

Return<void> SettingsStorage::subscribe(const hidl_string& key, const sp<ISettingsListener>& listener) {
    ALOGD("subscribe %s", key.c_str());

    SettingsChangeReason reason = SettingsChangeReason::ProfileChange;
    profileHidl::ProfileIdentifier activeProfile = profileHidl::ProfileIdentifier::None;
    const auto fullKey = std::make_pair(key, activeProfile);

    const auto it = storage.find(fullKey);
    if (it == storage.end()) {
        reason = SettingsChangeReason::Reset;
    }

    listener->settingsForCurrentUserChanged(key, reason, activeProfile);

    // TODO(ee) implement
    return Void();
}

Return<void> SettingsStorage::unsubscribe(const hidl_string& key, const sp<ISettingsListener>& listener) {
    (void)listener;
    ALOGD("unsubscribe %s", key.c_str());
    // TODO(ee) implement
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace settings
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
