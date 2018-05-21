/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cutils/log.h>
#include <vendor/volvocars/hardware/gps/1.0/ILocationCallback.h>
#include <vendor/volvocars/hardware/gps/1.0/ILocationUpdate.h>
#include <iostream>
#include <iterator>
#include <mutex>
#include <vector>

using ::android::sp;
using ::vendor::volvocars::hardware::gps::V1_0::ILocationUpdate;
using ::vendor::volvocars::hardware::gps::V1_0::ILocationCallback;
using ::vendor::volvocars::hardware::gps::V1_0::GnssTimeLocinfo;
using ::android::hardware::Return;
using ::android::hardware::Void;

class GnssTimeLocService : public ILocationUpdate {
  public:
    GnssTimeLocService();
    GnssTimeLocService(const GnssTimeLocService&) = delete;
    GnssTimeLocService& operator=(const GnssTimeLocService&) = delete;
    virtual ~GnssTimeLocService();

    Return<bool> requestGNSSLocationUpdates(const sp<ILocationCallback>& callbackHandler) override;
    void PerformGnssTimeLocCb(const GnssTimeLocinfo& timelocation);
    bool StartService();

  private:
    std::vector<android::sp<ILocationCallback>> loc_cb_t;

    void OnGnssTimeLocUpdates(const GnssTimeLocinfo& timelocation);
};