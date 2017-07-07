#ifndef ANDROID_HARDWARE_GNSS_V1_0_GNSSCONFIGURATION_H
#define ANDROID_HARDWARE_GNSS_V1_0_GNSSCONFIGURATION_H

#include <android/hardware/gnss/1.0/IGnssConfiguration.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssConfiguration;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssConfiguration : public IGnssConfiguration {
    // Methods from ::android::hardware::gnss::V1_0::IGnssConfiguration follow.
    Return<bool> setSuplEs(bool enabled) override;
    Return<bool> setSuplVersion(uint32_t version) override;
    Return<bool> setSuplMode(uint8_t mode) override;
    Return<bool> setGpsLock(uint8_t lock) override;
    Return<bool> setLppProfile(uint8_t lppProfile) override;
    Return<bool> setGlonassPositioningProtocol(uint8_t protocol) override;
    Return<bool> setEmergencySuplPdn(bool enable) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" IGnssConfiguration* HIDL_FETCH_IGnssConfiguration(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_GNSSCONFIGURATION_H
