/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssGeofencing.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssGeofencing follow.
Return<void> GnssGeofencing::setCallback(const sp<IGnssGeofenceCallback>& callback) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofencing::addGeofence(int32_t geofenceId,
                                         double latitudeDegrees,
                                         double longitudeDegrees,
                                         double radiusMeters,
                                         IGnssGeofenceCallback::GeofenceTransition lastTransition,
                                         int32_t monitorTransitions,
                                         uint32_t notificationResponsivenessMs,
                                         uint32_t unknownTimerMs) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofencing::pauseGeofence(int32_t geofenceId) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofencing::resumeGeofence(int32_t geofenceId, int32_t monitorTransitions) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofencing::removeGeofence(int32_t geofenceId) {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssGeofencing* HIDL_FETCH_IGnssGeofencing(const char* /* name */) { return new GnssGeofencing(); }

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
