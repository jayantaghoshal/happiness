#include "GnssNi.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssNi follow.
Return<void> GnssNi::setCallback(const sp<IGnssNiCallback>& callback) {
    // TODO implement
    return Void();
}

Return<void> GnssNi::respond(int32_t notifId, IGnssNiCallback::GnssUserResponseType userResponse) {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssNi* HIDL_FETCH_IGnssNi(const char* /* name */) { return new GnssNi(); }

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
