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

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_ClimateCtrl"
#include <cutils/log.h>

using namespace ::android::hardware;
using namespace vcc::remotectrl;
using namespace vcc::remotectrl::remoteclimatectrl;

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
        ServiceInfo service_info{
                .service_name_ = "RemoteCtrl_ClimateCtrl",
                .service_id_ = service_id,
                .instance_id_ = instance_id,
                .eventgroup_id_ = eventgroup_id,
        };

        service_info.methods_ =
                std::vector<vsomeip::method_t>(remotectrl_service_methods.cbegin(), remotectrl_service_methods.cend());

        service_info.events_ =
                std::vector<vsomeip::event_t>(remotectrl_service_events.cbegin(), remotectrl_service_events.cend());

        ::android::sp<remoteclimatectrl::ClimateCtrlService> climate_ctrl_service =
                new remoteclimatectrl::ClimateCtrlService(service_info);

        climate_ctrl_service->StartService();

        configureRpcThreadpool(1, true);

        if (::android::OK != climate_ctrl_service->registerAsService("RemoteCtrl_ClimateCtrl")) {
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
