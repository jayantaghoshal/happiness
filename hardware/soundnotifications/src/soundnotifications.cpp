/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>

#include "soundwrapper.h"

#include "beltreminder.h"
#include "parking_brake.h"
#include "turnindicator.h"

#undef LOG_TAG
#define LOG_TAG "SoundNotifications"

int32_t main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    ALOGI("Starting service...");

    while (!SoundNotifications::SoundWrapper::instance()->init(
            ::android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager>())) {
        usleep(1000000);  // sleep 1000ms between reties
    }
    ALOGI("Connected to Audio Manager service...");

    auto turnIndicatorModule =
            std::unique_ptr<SoundNotifications::TurnIndicator>(new SoundNotifications::TurnIndicator());
    (void)turnIndicatorModule;
    auto parkingBreakModule =
            std::unique_ptr<SoundNotifications::Chassis::ParkingBrake>(new SoundNotifications::Chassis::ParkingBrake());
    (void)parkingBreakModule;
    auto beltReminderModule = std::unique_ptr<SoundNotifications::BeltReminder>(new SoundNotifications::BeltReminder());
    (void)beltReminderModule;

    android::hardware::configureRpcThreadpool(1, true /* callerWillJoin */);
    android::hardware::joinRpcThreadpool();
    ALOGI("Leaving.");
}
