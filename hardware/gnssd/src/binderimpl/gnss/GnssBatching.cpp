/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssBatching.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssBatching follow.
Return<bool> GnssBatching::init(const sp<IGnssBatchingCallback>& callback) {
    // TODO implement
    return bool{};
}

Return<uint16_t> GnssBatching::getBatchSize() {
    // TODO implement
    return uint16_t{};
}

Return<bool> GnssBatching::start(const IGnssBatching::Options& options) {
    // TODO implement
    return bool{};
}

Return<void> GnssBatching::flush() {
    // TODO implement
    return Void();
}

Return<bool> GnssBatching::stop() {
    // TODO implement
    return bool{};
}

Return<void> GnssBatching::cleanup() {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssBatching* HIDL_FETCH_IGnssBatching(const char* /* name */) {
    return new GnssBatching();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
