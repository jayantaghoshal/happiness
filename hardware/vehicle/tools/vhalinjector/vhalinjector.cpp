/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

std::string GetPropertyName(int32_t prop) {
    if (static_cast<vhal20::VehiclePropertyGroup>(prop & vhal20::VehiclePropertyGroup::MASK) ==
        vhal20::VehiclePropertyGroup::VENDOR) {
        return vccvhal10::toString(static_cast<vccvhal10::VehicleProperty>(prop));  // VENDOR
    } else {
        return vhal20::toString(static_cast<vhal20::VehicleProperty>(prop));  // SYSTEM
    }
}

void SetProperty(vhal20::VehiclePropValue& property_value, const std::string& value,
                 ::android::sp<vhal20::IVehicle> service) {
    const auto property_type =
            static_cast<vhal20::VehiclePropertyType>(property_value.prop & vhal20::VehiclePropertyType::MASK);

    printf("Prop type: %s\n", vhal20::toString(property_type).c_str());

    if (property_type == vhal20::VehiclePropertyType::STRING) {
        printf("Not implemented.\n");
        return;

        // Was never able to get the commented code below to work, hence the "Not implemented" for this
        // VehiclePropertyType
        // property_value.value.stringValue = ::android::hardware::hidl_string(value);

    } else if (property_type == vhal20::VehiclePropertyType::BOOLEAN ||
               property_type == vhal20::VehiclePropertyType::INT32) {
        const auto int32_value = std::stoi(value);
        property_value.value.int32Values = ::android::hardware::hidl_vec<int32_t>{int32_value};

    } else if (property_type == vhal20::VehiclePropertyType::FLOAT) {
        const auto float_value = std::stof(std::string(value));
        property_value.value.floatValues = ::android::hardware::hidl_vec<float>{float_value};

    } else if (property_type == vhal20::VehiclePropertyType::INT64) {
        const auto int64_value = std::stol(std::string(value));
        property_value.value.int64Values = ::android::hardware::hidl_vec<int64_t>{int64_value};

    } else if (property_type == vhal20::VehiclePropertyType::INT32_VEC) {
        printf("Not implemented.\n");
        return;

        // Was never able to get the commented code below to work, hence the "Not implemented" for this
        // VehiclePropertyType
        // std::vector<int32_t> vect;
        // std::stringstream stream(value);
        // unsigned long element_id;

        // while (stream >> element_id) {
        //     vect.push_back(element_id);

        //     if (stream.peek() == ',') stream.ignore();
        // }

        // for (element_id = 0; element_id < vect.size(); element_id++) {
        //     printf("element id: %d\n", vect.at(element_id));
        // }

        // // No effect on property atm.
        // property_value.value.int32Values = ::android::hardware::hidl_vec<int32_t>{vect};

        // // No effect on property atm.
        // property_value.value.int32Values.resize(2);
        // property_value.value.int32Values[0] = 1;
        // property_value.value.int32Values[1] = 1;

    } else if (property_type == vhal20::VehiclePropertyType::BYTES) {
        printf("Not implemented.\n");
        return;

        // Was never able to get the commented code below to work, hence the "Not implemented" for this
        // VehiclePropertyType
        // TODO make the below work (data-> android::hardware::hidl_vec<unsigned char>)
        // property_value.value.bytes = value.c_str();

    } else if (property_type == vhal20::VehiclePropertyType::FLOAT_VEC) {
        printf("Not implemented.\n");
        return;

        // Was never able to get the commented code below to work, hence the "Not implemented" for this
        // VehiclePropertyType
        // const auto floatvec_value = std::stof(std::string(value));
        // property_value.value.floatValues = ::android::hardware::hidl_vec<float>{floatvec_value};

    } else if (property_type == vhal20::VehiclePropertyType::COMPLEX ||
               property_type == vhal20::VehiclePropertyType::MASK) {
        printf("Not implemented.\n");
        return;
    }

    service->set(property_value);
    printf("Value set");
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Error: Need 2-3 arguments [property name] [wanted property value] [area id (optional, default is 0 as "
               "in GLOBAL )], got: %d",
               argc);
        return 2;
    }

    ::android::sp<vhal20::IVehicle> service = vhal20::IVehicle::getService();
    if (service == nullptr) {
        printf("Service is null, is android.hardware.automotive.vehicle.vcc@2.0-service running?\n");
        return 1;
    }

    const auto value_for_property = std::string(argv[2]);

    const auto service_status = service->getAllPropConfigs(
            [=](const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& prop_configs) {

                bool match_found = false;

                for (const vhal20::VehiclePropConfig& property_config : prop_configs) {
                    if (argv[1] == GetPropertyName(property_config.prop)) {
                        match_found = true;

                        vhal20::VehiclePropValue property_value;
                        property_value.prop = property_config.prop;

                        if (argc == 4) {
                            const auto area_id = std::stoi(std::string(argv[3]));
                            property_value.areaId = area_id;

                        } else {  // No areaId is defaulted as areaId = 0 (GLOBAL).
                            property_value.areaId = 0;
                        }

                        printf("Prop id: %d\n", property_config.prop);
                        printf("area id: %d\n", property_value.areaId);

                        SetProperty(property_value, value_for_property, service);
                    }
                }
                if (!match_found) {
                    printf("No matched property found.\n");
                }
            });
    if (!service_status.isOk()) printf("Error in getAllPropConfigs (%s)\n", service_status.description().c_str());

    printf("\nWait for 1 sec, making sure all async stuff is done before terminating...\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    printf("\nDone.\n");
}