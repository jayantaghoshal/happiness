/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssMeasurementCallback.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssMeasurementCallback follow.
Return<void> GnssMeasurementCallback::GnssMeasurementCb(const IGnssMeasurementCallback::GnssData& data) {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssMeasurementCallback* HIDL_FETCH_IGnssMeasurementCallback(const char* /* name */) {
    return new GnssMeasurementCallback();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
