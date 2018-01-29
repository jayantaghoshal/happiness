/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "ECD_dataelement.h"

#include <vendor/volvocars/hardware/profiles/1.0/ICarProfileManager.h>
#include <vendor/volvocars/hardware/profiles/1.0/IKeyPairResponseHandler.h>
#include <vendor/volvocars/hardware/profiles/1.0/IProfileChangedHandler.h>
#include <vendor/volvocars/hardware/profiles/1.0/types.h>

#undef LOG_TAG
#define LOG_TAG "ProfilesTests"

using namespace vendor::volvocars::hardware::profiles::V1_0;
using namespace autosar;
using namespace std::chrono_literals;

struct KeyPairResponseHandler : public IKeyPairResponseHandler {
    android::hardware::Return<void> keySearchCompleted(KeyPairStatus result) override {
        success_ = result == KeyPairStatus::Success;
        ALOGD("KeyPairResponseHandler called with status %d", static_cast<int>(result));
        return android::hardware::Void();
    }
    bool success_;
};

class ProfilesTests : public ::testing::Test {
  public:
    ProfilesTests() {
        // TODO (TS) Replace this stop and start when we have an alternative way to reset the settings
        // NOTE: Starting server without any existing DB usually takes 150ms, need to take this into account in tests
        system("stop settingstorage-hidl-server");      // NOLINT
        system("rm /data/vendor/vehiclesettings.db*");  // NOLINT
        system("start settingstorage-hidl-server");     // NOLINT
        system("stop vehicle-signals-daemon");          // NOLINT
        std::this_thread::sleep_for(300ms);

        profile_manager_ = ICarProfileManager::getService();
        if (profile_manager_ == nullptr) {
            ALOGW("profileManager service is null, is vendor::volvocars::hardware::profiles::V1_0-service running?\n");
        }
    };

    ~ProfilesTests() override {
        system("start vehicle-signals-daemon");  // NOLINT
    }

    ::android::sp<ICarProfileManager> profile_manager_;

    ECDDataElement::DEInjector<ProfPenSts1_info> prof_pen_sts1_receiver_injector_;
    ECDDataElement::DEInjector<KeyReadStsToProfCtrl_info> key_read_sts_injector_;
    ECDDataElement::DEInjector<NrOfKeyAvl_info> nr_of_key_injector_;

    ECDDataElement::DESink<ProfChg_info> prof_chg_sink_;
    ECDDataElement::DESink<KeyReadReqFromSetgMgr_info> key_read_req_sink_;
};

TEST_F(ProfilesTests, pairCurrentProfileToKey) {
    android::hardware::Return<int16_t> nr_of_unused = profile_manager_->getNrOfUnusedProfiles();
    if (!nr_of_unused.isOk()) ALOGE("Dead object: getNrOfUnusedProfiles");
    ALOGI("Nr of unused profiles %d", (int)nr_of_unused);

    // Create a new android id and connect to availabel profile
    std::string new_android_user{"12345"};
    auto status = profile_manager_->pairAndroidUserToUnusedVehicleProfile(new_android_user);
    if (!status.isOk()) ALOGE("Dead object: pairAndroidUserToUnusedVehicleProfile");

    auto nr_of_unused2 = profile_manager_->getNrOfUnusedProfiles();
    if (!nr_of_unused2.isOk()) ALOGE("Dead object: getNrOfUnusedProfiles");
    ALOGI("Nr of unused2 profiles %d", (int)nr_of_unused2);

    EXPECT_EQ(nr_of_unused - 1, nr_of_unused2);

    ::android::sp<KeyPairResponseHandler> onKeySearchCompleted = new KeyPairResponseHandler();
    KeyLocation searchLocation = KeyLocation::AllZones;
    auto status2 = profile_manager_->pairCurrentProfileToKey(searchLocation, onKeySearchCompleted);
    if (!status2.isOk()) {
        ALOGW("hidl error");
    }
    std::this_thread::sleep_for(300ms);

    EXPECT_EQ(KeyLocn1::KeyLocnAllInt, key_read_req_sink_.get().value());

    // inject reponse from CEM for successful key search
    KeyReadStsToProfCtrl key_paired_sts_from_cem;
    key_paired_sts_from_cem.KeyId = autosar::KeyId1::Key0;
    key_paired_sts_from_cem.Boolean = true;
    key_read_sts_injector_.inject(key_paired_sts_from_cem);
    std::this_thread::sleep_for(300ms);
    EXPECT_EQ(true, onKeySearchCompleted->success_);

    ALOGD("tc done");  // TODO (TS) remove log
}