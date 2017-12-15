/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "GnssGeofenceCallback.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::gnss::V1_0::IGnssGeofenceCallback follow.
Return<void> GnssGeofenceCallback::gnssGeofenceTransitionCb(int32_t geofenceId, const GnssLocation& location,
                                                            IGnssGeofenceCallback::GeofenceTransition transition,
                                                            int64_t timestamp) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofenceStatusCb(IGnssGeofenceCallback::GeofenceAvailability status,
                                                        const GnssLocation& lastLocation) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofenceAddCb(int32_t geofenceId, IGnssGeofenceCallback::GeofenceStatus status) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofenceRemoveCb(int32_t geofenceId,
                                                        IGnssGeofenceCallback::GeofenceStatus status) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofencePauseCb(int32_t geofenceId,
                                                       IGnssGeofenceCallback::GeofenceStatus status) {
    // TODO implement
    return Void();
}

Return<void> GnssGeofenceCallback::gnssGeofenceResumeCb(int32_t geofenceId,
                                                        IGnssGeofenceCallback::GeofenceStatus status) {
    // TODO implement
    return Void();
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnssGeofenceCallback* HIDL_FETCH_IGnssGeofenceCallback(const char* /* name */) { return new GnssGeofenceCallback(); }

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
