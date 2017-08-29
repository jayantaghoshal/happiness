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
    started_=false;
}

// The assumption is that this method is called on our dispatcher mainloop
void Gnss::updateLocation(const GnssLocation& location) {
    if (started_) {
        if (callback_!=nullptr) {
            if ( callback_->gnssLocationCb(location).isDeadObject() ) {
                // see https://source.android.com/devices/architecture/hidl-cpp/interfaces (Death recipients)
                callback_ = nullptr;
                ALOGI("callback=null due to dead client");
            }
        }
    } else {
        ALOGD("newLocationDelivered but not started -> ignored");
    }
}

// Methods from ::android::hardware::gnss::V1_0::IGnss follow.
Return<bool> Gnss::setCallback(const sp<IGnssCallback>& callback) {
    ALOGD("setCallback %d",callback!=nullptr);

    IDispatcher::EnqueueTask([this,callback](){
        callback_ = callback;
        if (callback_!=nullptr)
        {
            ALOGD("Cap+SysInfo callback");
            bool dead1 = callback_->gnssSetCapabilitesCb((uint32_t)IGnssCallback::Capabilities::SCHEDULING).isDeadObject();
            IGnssCallback::GnssSystemInfo systeminfo;
            systeminfo.yearOfHw = 2016;
            bool dead2 = callback_->gnssSetSystemInfoCb(systeminfo).isDeadObject();
            if (dead1 || dead2) {
                // see https://source.android.com/devices/architecture/hidl-cpp/interfaces (Death recipients)
                callback_=nullptr;
                ALOGI("callback=null due to dead client");
            }
        }
    });

    return true;
}

Return<bool> Gnss::start() {
    if (started_) {
        ALOGW("starting BUT already started!!");
    } else {
        ALOGD("start");
    }
    started_ = true;
    return true;
}

Return<bool> Gnss::stop() {
    ALOGD("stop");
    started_ = false;
    return true;
}

Return<void> Gnss::cleanup() {
    ALOGD("cleanup");
    if (started_) {
        ALOGD("... and also stopping");
        stop();
    }
    return Void();
}

Return<bool> Gnss::injectTime(int64_t /*timeMs*/, int64_t /*timeReferenceMs*/, int32_t /*uncertaintyMs*/) {
    // TODO implement
    ALOGD("injectTime");
    return bool {};
}

Return<bool> Gnss::injectLocation(double /*latitudeDegrees*/, double /*longitudeDegrees*/, float /*accuracyMeters*/) {
    // TODO implement
    ALOGD("injectLocation");
    return bool {};
}

Return<void> Gnss::deleteAidingData(IGnss::GnssAidingData /*aidingDataFlags*/) {
    // TODO implement
    return Void();
}

Return<bool> Gnss::setPositionMode(IGnss::GnssPositionMode mode, IGnss::GnssPositionRecurrence recurrence, uint32_t minIntervalMs, uint32_t preferredAccuracyMeters, uint32_t preferredTimeMs) {
    ALOGD("setPositionMode %hhu %u %u %u %u",mode,recurrence,minIntervalMs,preferredAccuracyMeters,preferredTimeMs);
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
