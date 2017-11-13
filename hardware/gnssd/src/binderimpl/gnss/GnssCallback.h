#ifndef ANDROID_HARDWARE_GNSS_V1_0_GNSSCALLBACK_H
#define ANDROID_HARDWARE_GNSS_V1_0_GNSSCALLBACK_H

#include <android/hardware/gnss/1.0/IGnssCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::GnssLocation;
using ::android::hardware::gnss::V1_0::IGnssCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssCallback : public IGnssCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssCallback follow.
    Return<void> gnssLocationCb(const GnssLocation& location) override;
    Return<void> gnssStatusCb(IGnssCallback::GnssStatusValue status) override;
    Return<void> gnssSvStatusCb(const IGnssCallback::GnssSvStatus& svInfo) override;
    Return<void> gnssNmeaCb(int64_t timestamp, const hidl_string& nmea) override;
    Return<void> gnssSetCapabilitesCb(uint32_t capabilities) override;
    Return<void> gnssAcquireWakelockCb() override;
    Return<void> gnssReleaseWakelockCb() override;
    Return<void> gnssRequestTimeCb() override;
    Return<void> gnssSetSystemInfoCb(const IGnssCallback::GnssSystemInfo& info) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
};

extern "C" IGnssCallback* HIDL_FETCH_IGnssCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_GNSSCALLBACK_H
