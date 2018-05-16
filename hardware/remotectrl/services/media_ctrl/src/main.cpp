/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "media_ctrl_service.h"

#include <hidl/HidlTransportSupport.h>

#include <chrono>
#include <cstdlib>

#include <stdexcept>
#include <thread>

#include "convapi_signals_def.h"
#include "service_info.h"

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_MediaCtrl"
#include <cutils/log.h>

using namespace ::android::hardware;
using namespace vcc::remotectrl;

int main(int argc, char* argv[]) {
    ALOGI("Convenience API media control daemon 0.1 starting");

    if (argc < 2) {
        ALOGE("Expects vsomeip configuration file location as argument");
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
        return EXIT_FAILURE;
    }

    const char* environment_variable = "VSOMEIP_CONFIGURATION";
    const char* environment_value = argv[1];

    setenv(environment_variable, environment_value, 1 /*override*/);
    try {
        ServiceInfo service_info{.service_name_ = "RemoteCtrl_MediaCtrl",
                                 .service_id_ = REMOTECTRL_MEDIACTRL_SERVICE_ID,
                                 .instance_id_ = REMOTECTRL_MEDIACTRL_SERVICE_INSTANCE_ID,
                                 .eventgroup_id_ = REMOTECTRL_MEDIACTRL_EVENTGROUP_ID,
                                 .methods_ = {REMOTECTRL_MEDIACTRL_METHOD_ID_SETMEDIAPLAYERPLAYBACK},
                                 .events_ = {REMOTECTRL_MEDIACTRL_EVENT_ID_MEDIAPLAYERPLAYBACKSTATUS}};

        ::android::sp<remotemediactrl::MediaCtrlService> media_ctrl_service =
                new remotemediactrl::MediaCtrlService(service_info);

        media_ctrl_service->StartService();

        configureRpcThreadpool(1, true);

        if (::android::OK != media_ctrl_service->registerAsService(service_info.service_name_)) {
            throw std::runtime_error("Failed to register Service...");
        }

        joinRpcThreadpool();
    } catch (const std::exception& e) {
        ALOGE("%s", e.what());
        return EXIT_FAILURE;
    }

    ALOGD("ConvAPI MediaCtrl is exiting...");
    return EXIT_SUCCESS;
}
