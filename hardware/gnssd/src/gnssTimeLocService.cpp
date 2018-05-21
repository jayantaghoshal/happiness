/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "gnssTimeLocService.h"
#define LOG_TAG "GnssD.timelocservice"

GnssTimeLocService::GnssTimeLocService() {}

bool GnssTimeLocService::StartService() {
    bool blret = false;

    const android::status_t status = registerAsService();
    if (status != ::android::OK) {
        ALOGE("Failed to register VCC GnssTimeLoc binder service: %d", status);
    } else {
        ALOGD("VCC GnssTimeLoc binder service register Success");
        blret = true;
    }

    return blret;
}

Return<bool> GnssTimeLocService::requestGNSSLocationUpdates(const sp<ILocationCallback>& callbackHandler) {
    loc_cb_t.push_back(callbackHandler);
    return true;
}

void GnssTimeLocService::PerformGnssTimeLocCb(const GnssTimeLocinfo& timelocation) {
    OnGnssTimeLocUpdates(timelocation);
}

void GnssTimeLocService::OnGnssTimeLocUpdates(const GnssTimeLocinfo& timelocation) {
    auto it = loc_cb_t.begin();
    while (it != loc_cb_t.end()) {
        auto result = (*it)->OnGnssLocationUpdate(timelocation);
        result.isOk();
        if (result.isDeadObject()) {
            ALOGE("[ILocation] Callback function does not exist or Client dead ");
            it = loc_cb_t.erase(it);
            continue;
        }
        it++;
    }
}
GnssTimeLocService::~GnssTimeLocService() {}
