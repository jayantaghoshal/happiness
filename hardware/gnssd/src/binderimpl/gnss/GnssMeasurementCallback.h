/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/gnss/1.0/IGnssMeasurementCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssMeasurementCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssMeasurementCallback : public IGnssMeasurementCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssMeasurementCallback follow.
    Return<void> GnssMeasurementCb(const IGnssMeasurementCallback::GnssData& data) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
};

extern "C" IGnssMeasurementCallback* HIDL_FETCH_IGnssMeasurementCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
