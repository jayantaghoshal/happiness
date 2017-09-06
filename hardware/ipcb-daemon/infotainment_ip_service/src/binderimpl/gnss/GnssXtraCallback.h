#ifndef ANDROID_HARDWARE_GNSS_V1_0_GNSSXTRACALLBACK_H
#define ANDROID_HARDWARE_GNSS_V1_0_GNSSXTRACALLBACK_H

#include <android/hardware/gnss/1.0/IGnssXtraCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssXtraCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssXtraCallback : public IGnssXtraCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssXtraCallback follow.
    Return<void> downloadRequestCb() override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" IGnssXtraCallback* HIDL_FETCH_IGnssXtraCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_GNSSXTRACALLBACK_H
