/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/gnss/1.0/IGnssGeofencing.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::IGnssGeofenceCallback;
using ::android::hardware::gnss::V1_0::IGnssGeofencing;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssGeofencing : public IGnssGeofencing {
    // Methods from ::android::hardware::gnss::V1_0::IGnssGeofencing follow.
    Return<void> setCallback(const sp<IGnssGeofenceCallback>& callback) override;
    Return<void> addGeofence(int32_t geofenceId,
                             double latitudeDegrees,
                             double longitudeDegrees,
                             double radiusMeters,
                             IGnssGeofenceCallback::GeofenceTransition lastTransition,
                             int32_t monitorTransitions,
                             uint32_t notificationResponsivenessMs,
                             uint32_t unknownTimerMs) override;
    Return<void> pauseGeofence(int32_t geofenceId) override;
    Return<void> resumeGeofence(int32_t geofenceId, int32_t monitorTransitions) override;
    Return<void> removeGeofence(int32_t geofenceId) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
};

extern "C" IGnssGeofencing* HIDL_FETCH_IGnssGeofencing(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
