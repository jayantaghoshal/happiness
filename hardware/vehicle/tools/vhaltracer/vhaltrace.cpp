/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cstdio>
#include <cstdlib>

#include <map>
#include <string>
#include <vector>

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

#include "vhal_listener.h"
#include "vhal_util.h"

void SubscribeToProperty(const ::android::sp<vhal20::IVehicle>& service,
                         const ::android::sp<VhalListener>& vhal_listener,
                         int32_t prop_id) {
    ::android::hardware::hidl_vec<vhal20::SubscribeOptions> options = {
            {.propId = prop_id, .flags = vhal20::SubscribeFlags::DEFAULT},
    };

    service->subscribe(vhal_listener, options);
}

int main(int argc, char* argv[]) {
    ::android::sp<vhal20::IVehicle> service = vhal20::IVehicle::getService();
    if (service == nullptr) {
        printf("Service is null, is android.hardware.automotive.vehicle.vcc@2.0-service running?\n");
        return 1;
    }

    std::vector<int32_t> requested_properties;
    char* tmp;

    if (argc > 1) {
        for (int index = 1; index < argc; ++index) {
            const auto requested_property = static_cast<int32_t>(strtol(argv[index], &tmp, 0));
            if (requested_property <= 0) {
                printf("Error: invalid property ID given: %i\n", requested_property);
                return 2;
            }
            requested_properties.push_back(requested_property);
        }
    }

    if (requested_properties.empty()) {
        const auto service_status = service->getAllPropConfigs(
                [=](const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& prop_configs) {
                    VhalUtil::DumpProperties(service, prop_configs);
                });
        if (!service_status.isOk()) printf("Error in getAllPropConfigs (%s)\n", service_status.description().c_str());

    } else {
        const auto service_status = service->getPropConfigs(
                requested_properties,
                [=](vhal20::StatusCode status,
                    const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& prop_config_list) {

                    if (status == vhal20::StatusCode::OK) {
                        VhalUtil::DumpProperties(service, prop_config_list);

                    } else {
                        printf(" Conf properties NOT OK (status=%s)\n", toString(status).c_str());
                    }
                });
        if (!service_status.isOk()) {
            printf("Error in getPropConfigs, description: %s)\n", service_status.description().c_str());
        }
    }

    printf("\nListning done.\n");

    ::android::sp<VhalListener> vhal_listener = new VhalListener(service);
    vhal_listener->startLinkToDeath();

    if (requested_properties.empty()) {
        auto service_status = service->getAllPropConfigs(
                [=](const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& prop_config_list) {

                    for (const vhal20::VehiclePropConfig& prop_config : prop_config_list) {
                        SubscribeToProperty(service, vhal_listener, prop_config.prop);
                    }
                });
        if (!service_status.isOk()) printf("Error in getAllPropConfigs (%s)\n", service_status.description().c_str());
    } else {
        for (auto prop_id : requested_properties) {
            SubscribeToProperty(service, vhal_listener, prop_id);
        }
    }

    printf("\nSubscribing done.\n");

    while (true) {
        sleep(999999);
    }
}
