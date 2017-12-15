/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef ANDROID_HARDWARE_GNSS_V1_0_GNSSNAVIGATIONMESSAGECALLBACK_H
#define ANDROID_HARDWARE_GNSS_V1_0_GNSSNAVIGATIONMESSAGECALLBACK_H

#include <android/hardware/gnss/1.0/IGnssNavigationMessageCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssNavigationMessageCallback : public IGnssNavigationMessageCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback follow.
    Return<void> gnssNavigationMessageCb(const IGnssNavigationMessageCallback::GnssNavigationMessage& message) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
};

extern "C" IGnssNavigationMessageCallback* HIDL_FETCH_IGnssNavigationMessageCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_GNSSNAVIGATIONMESSAGECALLBACK_H
