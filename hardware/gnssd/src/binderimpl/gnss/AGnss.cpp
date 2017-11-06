#include "AGnss.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IAGnss follow.
Return<void> AGnss::setCallback(const sp<IAGnssCallback>& callback) {
    // TODO implement
    return Void();
}

Return<bool> AGnss::dataConnClosed() {
    // TODO implement
    return bool{};
}

Return<bool> AGnss::dataConnFailed() {
    // TODO implement
    return bool{};
}

Return<bool> AGnss::setServer(IAGnssCallback::AGnssType type, const hidl_string& hostname, int32_t port) {
    // TODO implement
    return bool{};
}

Return<bool> AGnss::dataConnOpen(const hidl_string& apn, IAGnss::ApnIpType apnIpType) {
    // TODO implement
    return bool{};
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IAGnss* HIDL_FETCH_IAGnss(const char* /* name */) { return new AGnss(); }

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
