/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssCallback.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssCallback follow.
Return<void> GnssCallback::gnssLocationCb(const GnssLocation& location) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssStatusCb(IGnssCallback::GnssStatusValue status) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssSvStatusCb(const IGnssCallback::GnssSvStatus& svInfo) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssNmeaCb(int64_t timestamp, const hidl_string& nmea) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssSetCapabilitesCb(uint32_t capabilities) {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssAcquireWakelockCb() {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssReleaseWakelockCb() {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssRequestTimeCb() {
    // TODO implement
    return Void();
}

Return<void> GnssCallback::gnssSetSystemInfoCb(const IGnssCallback::GnssSystemInfo& info) {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssCallback* HIDL_FETCH_IGnssCallback(const char* /* name */) { return new GnssCallback(); }

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
