#include "GnssDebug.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssDebug follow.
Return<void> GnssDebug::getDebugData(getDebugData_cb _hidl_cb) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssDebug* HIDL_FETCH_IGnssDebug(const char* /* name */) {
    return new GnssDebug();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
