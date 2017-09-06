#ifndef ANDROID_HARDWARE_GNSS_V1_0_GNSSBATCHINGCALLBACK_H
#define ANDROID_HARDWARE_GNSS_V1_0_GNSSBATCHINGCALLBACK_H

#include <android/hardware/gnss/1.0/IGnssBatchingCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::GnssLocation;
using ::android::hardware::gnss::V1_0::IGnssBatchingCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssBatchingCallback : public IGnssBatchingCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssBatchingCallback follow.
    Return<void> gnssLocationBatchCb(const hidl_vec<GnssLocation>& locations) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" IGnssBatchingCallback* HIDL_FETCH_IGnssBatchingCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_GNSSBATCHINGCALLBACK_H
