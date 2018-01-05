/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

std::string propName(int32_t prop) {
    std::string propName = vhal20::toString<vhal20::VehicleProperty>(prop);
    // fallback to Vendor's properties
    if (static_cast<vhal20::VehiclePropertyGroup>(prop & vhal20::VehiclePropertyGroup::MASK) ==
        vhal20::VehiclePropertyGroup::VENDOR) {
        propName = vccvhal10::toString<vccvhal10::VehicleProperty>(prop);
    }
    return propName;
}

void dumpPropValue(::android::sp<vhal20::IVehicle> service, const vhal20::VehiclePropValue& pValue) {
    service->get(pValue, [](vhal20::StatusCode status, const vhal20::VehiclePropValue& propValue) {
        if (status != vhal20::StatusCode::OK) {
            printf("\n");
            return;
        }

        printf("\nProperty name: %s\n", propName(propValue.prop).c_str());
        printf("Property id: %d\n", propValue.prop);
        printf("StatusCode: %s\n", vhal20::toString(status).c_str());
        printf("Property value:\n");
        int i = 1;
        const vhal20::VehiclePropertyType prop_type =
                static_cast<vhal20::VehiclePropertyType>(propValue.prop & vhal20::VehiclePropertyType::MASK);
        if (prop_type == vhal20::VehiclePropertyType::BYTES) {
            for (const auto& ccVal : propValue.value.bytes) {
                printf("  %d: %d\n", i, ccVal);
                i++;
            }
        } else if (prop_type == vhal20::VehiclePropertyType::STRING) {
            printf("  %s\n", propValue.value.stringValue.c_str());
        } else if (prop_type == vhal20::VehiclePropertyType::INT32) {
            for (const auto& ccVal : propValue.value.int32Values) {
                printf("  %d: %i\n", i, ccVal);
                i++;
            }
        } else if (prop_type == vhal20::VehiclePropertyType::INT64) {
            for (const auto& ccVal : propValue.value.int64Values) {
                printf("  %d: %li\n", i, ccVal);
                i++;
            }
        } else if (prop_type == vhal20::VehiclePropertyType::FLOAT) {
            for (const auto& ccVal : propValue.value.floatValues) {
                printf("  %d: %f\n", i, ccVal);
                i++;
            }
        }
    });
}

void dumpPropConfigs(::android::sp<vhal20::IVehicle> service,
                     const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& propConfigs) {
    for (const vhal20::VehiclePropConfig& conf : propConfigs) {
        vhal20::VehiclePropValue requestedPropValue;
        requestedPropValue.prop = conf.prop;
        dumpPropValue(service, requestedPropValue);
        if (conf.areaConfigs.size() > 0) {
            printf("Area configs for property: %s\n", propName(conf.prop).c_str());
            for (const vhal20::VehicleAreaConfig& areaConf : conf.areaConfigs) {
                printf("  %s\n", vhal20::toString(areaConf).c_str());
            }
        }
    }
}

int main(int argc, char* argv[]) {
    ::android::sp<vhal20::IVehicle> service = vhal20::IVehicle::getService();
    if (service == nullptr) {
        printf("Service is null, is android.hardware.automotive.vehicle.vcc@2.0-service running?\n");
        return 1;
    }

    std::vector<int32_t> requestedProperties;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            const int32_t reqProp = static_cast<int32_t>(atoi(argv[i]));
            if (reqProp <= 0) {
                printf("Error: invalid property ID given: %i\n", reqProp);
                return 2;
            }
            requestedProperties.push_back(reqProp);
        }
    }

    if (requestedProperties.size() == 0) {
        service->getAllPropConfigs([=](const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& propConfigs) {
            dumpPropConfigs(service, propConfigs);
        });
    } else {
        service->getPropConfigs(requestedProperties,
                                [=](vhal20::StatusCode status,
                                    const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& propConfigs) {
                                    if (status == vhal20::StatusCode::OK) {
                                        dumpPropConfigs(service, propConfigs);
                                    }
                                });
    }
}
