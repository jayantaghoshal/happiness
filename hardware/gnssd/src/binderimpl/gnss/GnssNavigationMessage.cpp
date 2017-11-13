#include "GnssNavigationMessage.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssNavigationMessage follow.
Return<IGnssNavigationMessage::GnssNavigationMessageStatus> GnssNavigationMessage::setCallback(
        const sp<IGnssNavigationMessageCallback>& callback) {
    // TODO implement
    return ::android::hardware::gnss::V1_0::IGnssNavigationMessage::GnssNavigationMessageStatus{};
}

Return<void> GnssNavigationMessage::close() {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssNavigationMessage* HIDL_FETCH_IGnssNavigationMessage(const char* /* name */) {
    return new GnssNavigationMessage();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
