/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "AGnssRil.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IAGnssRil follow.
Return<void> AGnssRil::setCallback(const sp<IAGnssRilCallback>& callback) {
    // TODO implement
    return Void();
}

Return<void> AGnssRil::setRefLocation(const IAGnssRil::AGnssRefLocation& agnssReflocation) {
    // TODO implement
    return Void();
}

Return<bool> AGnssRil::setSetId(IAGnssRil::SetIDType type, const hidl_string& setid) {
    // TODO implement
    return bool{};
}

Return<bool> AGnssRil::updateNetworkState(bool connected, IAGnssRil::NetworkType type, bool roaming) {
    // TODO implement
    return bool{};
}

Return<bool> AGnssRil::updateNetworkAvailability(bool available, const hidl_string& apn) {
    // TODO implement
    return bool{};
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IAGnssRil* HIDL_FETCH_IAGnssRil(const char* /* name */) {
    return new AGnssRil();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
