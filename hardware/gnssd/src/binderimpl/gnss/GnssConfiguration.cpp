#include "GnssConfiguration.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssConfiguration follow.
Return<bool> GnssConfiguration::setSuplEs(bool enabled) {
    // TODO implement
    return bool{};
}

Return<bool> GnssConfiguration::setSuplVersion(uint32_t version) {
    // TODO implement
    return bool{};
}

Return<bool> GnssConfiguration::setSuplMode(uint8_t mode) {
    // TODO implement
    return bool{};
}

Return<bool> GnssConfiguration::setGpsLock(uint8_t lock) {
    // TODO implement
    return bool{};
}

Return<bool> GnssConfiguration::setLppProfile(uint8_t lppProfile) {
    // TODO implement
    return bool{};
}

Return<bool> GnssConfiguration::setGlonassPositioningProtocol(uint8_t protocol) {
    // TODO implement
    return bool{};
}

Return<bool> GnssConfiguration::setEmergencySuplPdn(bool enable) {
    // TODO implement
    return bool{};
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssConfiguration* HIDL_FETCH_IGnssConfiguration(const char* /* name */) { return new GnssConfiguration(); }

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
