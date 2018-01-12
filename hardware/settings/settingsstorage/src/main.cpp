/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>
#include "SettingsStorage.h"

#undef LOG_TAG
#define LOG_TAG "SettingsStorage"

using namespace vendor::volvocars::hardware::settings::V1_0::implementation;

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    android::sp<SettingsStorage> service = new SettingsStorage();

    // Configure a thread pool of 1, this means that the HIDL
    // server calls is done from one thread.
    android::hardware::configureRpcThreadpool(1, true /* callerWillJoin */);

    ALOGI("Registering as service...");
    android::status_t status = service->registerAsService();
    if (status != android::OK) {
        ALOGE("Failed to register SettingsStorage binder service: %d", status);
        return -1;
    }

    ALOGI("Registering as service done.");
    android::hardware::joinRpcThreadpool();
}
