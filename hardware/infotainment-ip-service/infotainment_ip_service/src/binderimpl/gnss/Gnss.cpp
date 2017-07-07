#include "Gnss.h"
#define LOG_TAG "Infotainment_IP_Service.binder.gnss"
#include <cutils/log.h>

#include <IDispatcher.h>

using namespace tarmac::eventloop;

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

Gnss::Gnss() {
    ALOGD("Gnss::Gnss");
}

// Methods from ::android::hardware::gnss::V1_0::IGnss follow.
sp<IGnssCallback> sCallback_;
IDispatcher::JobId sLocationTimer_ = 0xFFFFFFFFFFFFFFFF;

Return<bool> Gnss::setCallback(const sp<IGnssCallback>& callback) {
    // TODO implement
    ALOGD("setCallback %d",callback!=nullptr);
    sCallback_ = callback;

    if (sCallback_!=nullptr)
    {
        IDispatcher::EnqueueTask([](){
            ALOGD("Cap+SysInfo callback");
            sCallback_->gnssSetCapabilitesCb((uint32_t)IGnssCallback::Capabilities::SCHEDULING);

            IGnssCallback::GnssSystemInfo systeminfo;
            systeminfo.yearOfHw = 2016;
            sCallback_->gnssSetSystemInfoCb(systeminfo);
        });
    }

    return true;
}

void locationCallback() {
        ALOGD("Location callback");
        GnssLocation location;
        location.gnssLocationFlags = 
            (uint16_t)GnssLocationFlags::HAS_LAT_LONG |
            (uint16_t)GnssLocationFlags::HAS_ALTITUDE |
            (uint16_t)GnssLocationFlags::HAS_SPEED |
            (uint16_t)GnssLocationFlags::HAS_HORIZONTAL_ACCURACY;
        location.latitudeDegrees = 57.0;
        location.longitudeDegrees = 12.0;
        location.altitudeMeters = 10.0;
        location.speedMetersPerSec = 0.0;
        location.horizontalAccuracyMeters = 10.0;
        location.timestamp = 1.49e12;
        sCallback_->gnssLocationCb(location);
        sLocationTimer_ = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(std::chrono::milliseconds(1000),[]() {
            locationCallback();
        });
}

Return<bool> Gnss::start() {
    // TODO implement
    if (sLocationTimer_!=0xFFFFFFFFFFFFFFFF) {
        ALOGD("starting BUT already started!!, ignored");
    } else {
        ALOGD("start");
        sLocationTimer_ = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(std::chrono::milliseconds(1000),[]() {
            locationCallback();
        });
    }
    return true;
}

Return<bool> Gnss::stop() {
    // TODO implement
    ALOGD("stop");
    IDispatcher::GetDefaultDispatcher().Cancel(sLocationTimer_);
    sLocationTimer_ = 0xFFFFFFFFFFFFFFFF;
    return true;
}

Return<void> Gnss::cleanup() {
    // TODO implement
    ALOGD("cleanup");
    if (sLocationTimer_!=0xFFFFFFFFFFFFFFFF) {
        ALOGD("... and also stopping");
        stop();
    }
    return Void();
}

Return<bool> Gnss::injectTime(int64_t timeMs, int64_t timeReferenceMs, int32_t uncertaintyMs) {
    // TODO implement
    ALOGD("injectTime");
    return bool {};
}

Return<bool> Gnss::injectLocation(double latitudeDegrees, double longitudeDegrees, float accuracyMeters) {
    // TODO implement
    ALOGD("injectLocation");
    return bool {};
}

Return<void> Gnss::deleteAidingData(IGnss::GnssAidingData aidingDataFlags) {
    // TODO implement
    return Void();
}

Return<bool> Gnss::setPositionMode(IGnss::GnssPositionMode mode, IGnss::GnssPositionRecurrence recurrence, uint32_t minIntervalMs, uint32_t preferredAccuracyMeters, uint32_t preferredTimeMs) {
    // TODO implement
    ALOGD("setPositionMode %u %u %u %u",mode,recurrence,minIntervalMs,preferredAccuracyMeters);
    return true;
}

Return<sp<IAGnssRil>> Gnss::getExtensionAGnssRil() {
    // TODO implement
    ALOGD("getExtensionAGnssRil");
    return ::android::sp<::android::hardware::gnss::V1_0::IAGnssRil> {};
}

Return<sp<IGnssGeofencing>> Gnss::getExtensionGnssGeofencing() {
    // TODO implement
    ALOGD("getExtensionGnssGeofencing");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssGeofencing> {};
}

Return<sp<IAGnss>> Gnss::getExtensionAGnss() {
    // TODO implement
    ALOGD("getExtensionAGnss");
    return ::android::sp<::android::hardware::gnss::V1_0::IAGnss> {};
}

Return<sp<IGnssNi>> Gnss::getExtensionGnssNi() {
    // TODO implement
    ALOGD("getExtensionGnssNi");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssNi> {};
}

Return<sp<IGnssMeasurement>> Gnss::getExtensionGnssMeasurement() {
    // TODO implement
    ALOGD("getExtensionGnssMeasurement");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssMeasurement> {};
}

Return<sp<IGnssNavigationMessage>> Gnss::getExtensionGnssNavigationMessage() {
    // TODO implement
    ALOGD("getExtensionGnssNavigationMessage");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssNavigationMessage> {};
}

Return<sp<IGnssXtra>> Gnss::getExtensionXtra() {
    // TODO implement
    ALOGD("getExtensionXtra");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssXtra> {};
}

Return<sp<IGnssConfiguration>> Gnss::getExtensionGnssConfiguration() {
    // TODO implement
    ALOGD("getExtensionGnssConfiguration");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssConfiguration> {};
}

Return<sp<IGnssDebug>> Gnss::getExtensionGnssDebug() {
    // TODO implement
    ALOGD("getExtensionGnssDebug");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssDebug> {};
}

Return<sp<IGnssBatching>> Gnss::getExtensionGnssBatching() {
    // TODO implement
    ALOGD("getExtensionGnssBatching");
    return ::android::sp<::android::hardware::gnss::V1_0::IGnssBatching> {};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IGnss* HIDL_FETCH_IGnss(const char* name ) {
    ALOGD("HIDL_FETCH_IGnss %s",name);
    return new Gnss();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
