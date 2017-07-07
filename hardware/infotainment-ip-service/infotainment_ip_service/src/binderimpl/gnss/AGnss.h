#ifndef ANDROID_HARDWARE_GNSS_V1_0_AGNSS_H
#define ANDROID_HARDWARE_GNSS_V1_0_AGNSS_H

#include <android/hardware/gnss/1.0/IAGnss.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IAGnss;
using ::android::hardware::gnss::V1_0::IAGnssCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct AGnss : public IAGnss {
    // Methods from ::android::hardware::gnss::V1_0::IAGnss follow.
    Return<void> setCallback(const sp<IAGnssCallback>& callback) override;
    Return<bool> dataConnClosed() override;
    Return<bool> dataConnFailed() override;
    Return<bool> setServer(IAGnssCallback::AGnssType type, const hidl_string& hostname, int32_t port) override;
    Return<bool> dataConnOpen(const hidl_string& apn, IAGnss::ApnIpType apnIpType) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" IAGnss* HIDL_FETCH_IAGnss(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_AGNSS_H
