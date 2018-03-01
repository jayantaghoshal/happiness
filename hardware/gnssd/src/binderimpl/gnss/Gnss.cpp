/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "Gnss.h"

#include <IDispatcher.h>

#define LOG_TAG "GnssD.binder.gnss"
#include <cutils/log.h>

using namespace tarmac::eventloop;

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

Gnss::Gnss() {
    ALOGV("Gnss::Gnss");
    started_ = false;
}

// The assumption is that this method is called on our dispatcher mainloop
void Gnss::updateLocation(const GnssLocation& location) {
    if (started_) {
        if (callback_ != nullptr) {
            auto result = callback_->gnssLocationCb(location);
            // isOk() ALWAYS has to be called
            // see https://source.android.com/devices/architecture/hidl-cpp/interfaces (Death recipients)
            result.isOk();
            if (result.isDeadObject()) {
                callback_ = nullptr;
                ALOGV("callback=null due to dead client");
            }
        }
    } else {
        ALOGV("newLocationDelivered but not started -> ignored");
    }
}

// Methods from ::android::hardware::gnss::V1_0::IGnss follow.
Return<bool> Gnss::setCallback(const sp<IGnssCallback>& callback) {
    ALOGV("setCallback %d", callback != nullptr);

    IDispatcher::EnqueueTask([this, callback]() {
        callback_ = callback;
        if (callback_ != nullptr) {
            ALOGV("Cap+SysInfo callback");
            auto result1 = callback_->gnssSetCapabilitesCb((uint32_t)IGnssCallback::Capabilities::SCHEDULING);
            // We always have to call isOk() even though we don't care about the result. We use care about
            // isDeadObject()
            // https://source.android.com/devices/architecture/hidl-cpp/functions
            result1.isOk();
            IGnssCallback::GnssSystemInfo systeminfo;
            systeminfo.yearOfHw = 2016;
            auto result2 = callback_->gnssSetSystemInfoCb(systeminfo);
            result2.isOk();
            if (result1.isDeadObject() || result2.isDeadObject()) {
                callback_ = nullptr;
                ALOGV("callback=null due to dead client");
            }
        }
    });

    return true;
}

Return<bool> Gnss::start() {
    if (started_) {
        ALOGW("starting BUT already started!!");
    } else {
        ALOGV("start");
    }
    started_ = true;
    return true;
}

Return<bool> Gnss::stop() {
    ALOGV("stop");
    started_ = false;
    return true;
}

Return<void> Gnss::cleanup() {
    ALOGV("cleanup");
    if (started_) {
        ALOGV("... and also stopping");
        stop();
    }
    return Void();
}

Return<bool> Gnss::injectTime(int64_t /*timeMs*/, int64_t /*timeReferenceMs*/, int32_t /*uncertaintyMs*/) {
    // TODO implement
    ALOGV("injectTime");
    return bool{};
}

Return<bool> Gnss::injectLocation(double /*latitudeDegrees*/, double /*longitudeDegrees*/, float /*accuracyMeters*/) {
    // TODO implement
    ALOGV("injectLocation");
    return bool{};
}

Return<void> Gnss::deleteAidingData(IGnss::GnssAidingData /*aidingDataFlags*/) {
    // TODO implement
    return Void();
}

Return<bool> Gnss::setPositionMode(IGnss::GnssPositionMode mode,
                                   IGnss::GnssPositionRecurrence recurrence,
                                   uint32_t minIntervalMs,
                                   uint32_t preferredAccuracyMeters,
                                   uint32_t preferredTimeMs) {
    ALOGV("setPositionMode %hhu %u %u %u %u",
          mode,
          recurrence,
          minIntervalMs,
          preferredAccuracyMeters,
          preferredTimeMs);
    return true;
}

Return<sp<IAGnssRil>> Gnss::getExtensionAGnssRil() {
    // TODO implement
    ALOGV("getExtensionAGnssRil");
    return ::android::sp<::android::hardware::gnss::V1_0::IAGnssRil>{};
}

Return<sp<IGnssGeofencing>> Gnss::getExtensionGnssGeofencing() {
    // TODO implement
    ALOGV("getExtensionGnssGeofencing");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssGeofencing>{};
}

Return<sp<IAGnss>> Gnss::getExtensionAGnss() {
    // TODO implement
    ALOGV("getExtensionAGnss");
    return ::android::sp<::android::hardware::gnss::V1_0::IAGnss>{};
}

Return<sp<IGnssNi>> Gnss::getExtensionGnssNi() {
    // TODO implement
    ALOGV("getExtensionGnssNi");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssNi>{};
}

Return<sp<IGnssMeasurement>> Gnss::getExtensionGnssMeasurement() {
    // TODO implement
    ALOGV("getExtensionGnssMeasurement");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssMeasurement>{};
}

Return<sp<IGnssNavigationMessage>> Gnss::getExtensionGnssNavigationMessage() {
    // TODO implement
    ALOGV("getExtensionGnssNavigationMessage");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssNavigationMessage>{};
}

Return<sp<IGnssXtra>> Gnss::getExtensionXtra() {
    // TODO implement
    ALOGV("getExtensionXtra");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssXtra>{};
}

Return<sp<IGnssConfiguration>> Gnss::getExtensionGnssConfiguration() {
    // TODO implement
    ALOGV("getExtensionGnssConfiguration");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssConfiguration>{};
}

Return<sp<IGnssDebug>> Gnss::getExtensionGnssDebug() {
    // TODO implement
    ALOGV("getExtensionGnssDebug");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssDebug>{};
}

Return<sp<IGnssBatching>> Gnss::getExtensionGnssBatching() {
    // TODO implement
    ALOGV("getExtensionGnssBatching");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssBatching>{};
}

// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnss* HIDL_FETCH_IGnss(const char* name) {
    ALOGV("HIDL_FETCH_IGnss %s", name);
    return new Gnss();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
