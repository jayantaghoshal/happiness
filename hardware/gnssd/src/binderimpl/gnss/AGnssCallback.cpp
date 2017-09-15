#include "AGnssCallback.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IAGnssCallback follow.
Return<void> AGnssCallback::agnssStatusIpV4Cb(const IAGnssCallback::AGnssStatusIpV4& status) {
    // TODO implement
    return Void();
}

Return<void> AGnssCallback::agnssStatusIpV6Cb(const IAGnssCallback::AGnssStatusIpV6& status) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IAGnssCallback* HIDL_FETCH_IAGnssCallback(const char* /* name */) {
    return new AGnssCallback();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
