/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/gnss/1.0/IGnss.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IAGnss;
using ::android::hardware::gnss::V1_0::IAGnssRil;
using ::android::hardware::gnss::V1_0::IGnss;
using ::android::hardware::gnss::V1_0::IGnssBatching;
using ::android::hardware::gnss::V1_0::IGnssCallback;
using ::android::hardware::gnss::V1_0::IGnssConfiguration;
using ::android::hardware::gnss::V1_0::IGnssDebug;
using ::android::hardware::gnss::V1_0::IGnssGeofencing;
using ::android::hardware::gnss::V1_0::IGnssMeasurement;
using ::android::hardware::gnss::V1_0::IGnssNavigationMessage;
using ::android::hardware::gnss::V1_0::IGnssNi;
using ::android::hardware::gnss::V1_0::IGnssXtra;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Gnss : public IGnss {
    Gnss();

    void updateLocation(const android::hardware::gnss::V1_0::GnssLocation& location);

    // Methods from ::android::hardware::gnss::V1_0::IGnss follow.
    Return<bool> setCallback(const sp<IGnssCallback>& callback) override;
    Return<bool> start() override;
    Return<bool> stop() override;
    Return<void> cleanup() override;
    Return<bool> injectTime(int64_t timeMs, int64_t timeReferenceMs, int32_t uncertaintyMs) override;
    Return<bool> injectLocation(double latitudeDegrees, double longitudeDegrees, float accuracyMeters) override;
    Return<void> deleteAidingData(IGnss::GnssAidingData aidingDataFlags) override;
    Return<bool> setPositionMode(IGnss::GnssPositionMode mode, IGnss::GnssPositionRecurrence recurrence,
                                 uint32_t minIntervalMs, uint32_t preferredAccuracyMeters,
                                 uint32_t preferredTimeMs) override;
    Return<sp<IAGnssRil>> getExtensionAGnssRil() override;
    Return<sp<IGnssGeofencing>> getExtensionGnssGeofencing() override;
    Return<sp<IAGnss>> getExtensionAGnss() override;
    Return<sp<IGnssNi>> getExtensionGnssNi() override;
    Return<sp<IGnssMeasurement>> getExtensionGnssMeasurement() override;
    Return<sp<IGnssNavigationMessage>> getExtensionGnssNavigationMessage() override;
    Return<sp<IGnssXtra>> getExtensionXtra() override;
    Return<sp<IGnssConfiguration>> getExtensionGnssConfiguration() override;
    Return<sp<IGnssDebug>> getExtensionGnssDebug() override;
    Return<sp<IGnssBatching>> getExtensionGnssBatching() override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

    bool started_;
    sp<IGnssCallback> callback_;
};

extern "C" IGnss* HIDL_FETCH_IGnss(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
