/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssXtra.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssXtra follow.
Return<bool> GnssXtra::setCallback(const sp<IGnssXtraCallback>& callback) {
    // TODO implement
    return bool{};
}

Return<bool> GnssXtra::injectXtraData(const hidl_string& xtraData) {
    // TODO implement
    return bool{};
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssXtra* HIDL_FETCH_IGnssXtra(const char* /* name */) {
    return new GnssXtra();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
