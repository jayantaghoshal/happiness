/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "climate_ctrl_service.h"

#include <hidl/HidlTransportSupport.h>

#include <chrono>
#include <cstdlib>

#include <stdexcept>
#include <thread>

#include "convapi_signals_def.h"
#include "service_info.h"

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_ClimateCtrl"
#include <cutils/log.h>

using namespace ::android::hardware;
using namespace vcc::remotectrl;

int main(int argc, char* argv[]) {
    ALOGI("Convenience API climate control daemon 0.1 starting");

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
        ServiceInfo service_info{.service_name_ = "RemoteCtrl_ClimateCtrl",
                                 .service_id_ = REMOTECTRL_CLIMATECTRL_SERVICE_ID,
                                 .instance_id_ = REMOTECTRL_CLIMATECTRL_SERVICE_INSTANCE_ID,
                                 .eventgroup_id_ = REMOTECTRL_CLIMATECTRL_EVENTGROUP_ID,
                                 .methods_ = {REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE},
                                 .events_ = {REMOTECTRL_CLIMATECTRL_EVENT_ID_FANLEVELCHANGED,
                                             REMOTECTRL_CLIMATECTRL_EVENT_ID_MAX_DEFROSTER_STATECHANGED}};

        ::android::sp<remoteclimatectrl::ClimateCtrlService> climate_ctrl_service =
                new remoteclimatectrl::ClimateCtrlService(service_info);

        climate_ctrl_service->StartService();

        configureRpcThreadpool(1, true);

        if (::android::OK != climate_ctrl_service->registerAsService()) {
            throw std::runtime_error("Failed to register Service...");
        }

        joinRpcThreadpool();
    } catch (const std::exception& e) {
        ALOGE("%s", e.what());
        return EXIT_FAILURE;
    }

    ALOGD("ConvAPI ClimateCtrl is exiting...");
    return EXIT_SUCCESS;
}
