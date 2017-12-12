/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssMeasurement.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssMeasurement follow.
Return<IGnssMeasurement::GnssMeasurementStatus> GnssMeasurement::setCallback(
        const sp<IGnssMeasurementCallback>& callback) {
    // TODO implement
    return ::android::hardware::gnss::V1_0::IGnssMeasurement::GnssMeasurementStatus{};
}

Return<void> GnssMeasurement::close() {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssMeasurement* HIDL_FETCH_IGnssMeasurement(const char* /* name */) { return new GnssMeasurement(); }

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
