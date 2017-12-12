/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssNavigationMessageCallback.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssNavigationMessageCallback follow.
Return<void> GnssNavigationMessageCallback::gnssNavigationMessageCb(
        const IGnssNavigationMessageCallback::GnssNavigationMessage& message) {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssNavigationMessageCallback* HIDL_FETCH_IGnssNavigationMessageCallback(const char* /* name */) {
    return new GnssNavigationMessageCallback();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
