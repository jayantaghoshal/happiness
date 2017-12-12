/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssNiCallback.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssNiCallback follow.
Return<void> GnssNiCallback::niNotifyCb(const IGnssNiCallback::GnssNiNotification& notification) {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssNiCallback* HIDL_FETCH_IGnssNiCallback(const char* /* name */) { return new GnssNiCallback(); }

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
