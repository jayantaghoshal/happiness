/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef ANDROID_HARDWARE_GNSS_V1_0_AGNSSRIL_H
#define ANDROID_HARDWARE_GNSS_V1_0_AGNSSRIL_H

#include <android/hardware/gnss/1.0/IAGnssRil.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IAGnssRil;
using ::android::hardware::gnss::V1_0::IAGnssRilCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct AGnssRil : public IAGnssRil {
    // Methods from ::android::hardware::gnss::V1_0::IAGnssRil follow.
    Return<void> setCallback(const sp<IAGnssRilCallback>& callback) override;
    Return<void> setRefLocation(const IAGnssRil::AGnssRefLocation& agnssReflocation) override;
    Return<bool> setSetId(IAGnssRil::SetIDType type, const hidl_string& setid) override;
    Return<bool> updateNetworkState(bool connected, IAGnssRil::NetworkType type, bool roaming) override;
    Return<bool> updateNetworkAvailability(bool available, const hidl_string& apn) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
};

extern "C" IAGnssRil* HIDL_FETCH_IAGnssRil(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_AGNSSRIL_H
