/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "restart_client.h"
#include <android/log.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>
#include <hidl/Status.h>
#include <vendor/delphi/lifecyclecontrol/1.0/ILifecycleControl.h>

#define LOG_TAG "carconfig_updater"

using namespace vendor::delphi::lifecyclecontrol::V1_0;

void restartClient::requestRestart() {
    ALOGI("Executing restart request..");

    const ::android::hardware::hidl_string caller_name = "carconfig";

    android::sp<ILifecycleControl> lifecyclecontrol = ILifecycleControl::getService();
    if (lifecyclecontrol == nullptr) {
        ALOGE("Cannot reach LifecycleControl Service, unable to restart the IHU!");
    } else {
        lifecyclecontrol->RequestNodeRestart(::MPRestartReason::CAR_CONFIG, caller_name);
    }
}
