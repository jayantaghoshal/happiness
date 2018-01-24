/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>
#include "turnindicator.h"

#undef LOG_TAG
#define LOG_TAG "SoundNotifications"

int32_t main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    ALOGI("Starting service...");

    auto turnIndicatorModule =
            std::unique_ptr<SoundNotifications::TurnIndicator>(new SoundNotifications::TurnIndicator());
    (void)turnIndicatorModule;
    android::hardware::configureRpcThreadpool(1, true /* callerWillJoin */);
    android::hardware::joinRpcThreadpool();
    ALOGI("Leaving.");
}
