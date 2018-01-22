/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/gnss/1.0/IGnssGeofenceCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

using ::android::hardware::gnss::V1_0::GnssLocation;
using ::android::hardware::gnss::V1_0::IGnssGeofenceCallback;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct GnssGeofenceCallback : public IGnssGeofenceCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssGeofenceCallback follow.
    Return<void> gnssGeofenceTransitionCb(int32_t geofenceId, const GnssLocation& location,
                                          IGnssGeofenceCallback::GeofenceTransition transition,
                                          int64_t timestamp) override;
    Return<void> gnssGeofenceStatusCb(IGnssGeofenceCallback::GeofenceAvailability status,
                                      const GnssLocation& lastLocation) override;
    Return<void> gnssGeofenceAddCb(int32_t geofenceId, IGnssGeofenceCallback::GeofenceStatus status) override;
    Return<void> gnssGeofenceRemoveCb(int32_t geofenceId, IGnssGeofenceCallback::GeofenceStatus status) override;
    Return<void> gnssGeofencePauseCb(int32_t geofenceId, IGnssGeofenceCallback::GeofenceStatus status) override;
    Return<void> gnssGeofenceResumeCb(int32_t geofenceId, IGnssGeofenceCallback::GeofenceStatus status) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
};

extern "C" IGnssGeofenceCallback* HIDL_FETCH_IGnssGeofenceCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
