/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/gnss/1.0/IAGnssCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IAGnssCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct AGnssCallback : public IAGnssCallback {
    // Methods from ::android::hardware::gnss::V1_0::IAGnssCallback follow.
    Return<void> agnssStatusIpV4Cb(const IAGnssCallback::AGnssStatusIpV4& status) override;
    Return<void> agnssStatusIpV6Cb(const IAGnssCallback::AGnssStatusIpV6& status) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
};

extern "C" IAGnssCallback* HIDL_FETCH_IAGnssCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
