#include "GnssBatchingCallback.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssBatchingCallback follow.
Return<void> GnssBatchingCallback::gnssLocationBatchCb(const hidl_vec<GnssLocation>& locations) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssBatchingCallback* HIDL_FETCH_IGnssBatchingCallback(const char* /* name */) {
    return new GnssBatchingCallback();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
