/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VENDOR_VOLVOCARS_HARDWARE_SETTINGS_V1_0_SETTINGSSTORAGE_H
#define VENDOR_VOLVOCARS_HARDWARE_SETTINGS_V1_0_SETTINGSSTORAGE_H

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/volvocars/hardware/profiles/1.0/types.h>
#include <vendor/volvocars/hardware/settings/1.0/ISettingsStorage.h>

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

namespace profileHidl = vendor::volvocars::hardware::profiles::V1_0;
using settingskey = std::pair<std::string, profileHidl::ProfileIdentifier>;

struct SettingsStorage : public ISettingsStorage {
    Return<void> set(const hidl_string& key, profileHidl::ProfileIdentifier profileId,
                     const hidl_string& data) override;
    Return<void> get(const hidl_string& key, profileHidl::ProfileIdentifier profileId, get_cb _hidl_cb) override;
    Return<void> subscribe(const hidl_string& key, const sp<ISettingsListener>& listener) override;
    Return<void> unsubscribe(const hidl_string& key, const sp<ISettingsListener>& listener) override;

    // TODO: Temp, should be replaced with something stored persistently to disk
    std::map<settingskey, std::string> storage;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace settings
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor

#endif  // VENDOR_VOLVOCARS_HARDWARE_SETTINGS_V1_0_SETTINGSSTORAGE_H
