/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vhal_util.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

namespace VhalUtil {

std::string GetPropertyName(int32_t prop) {
    if (static_cast<vhal20::VehiclePropertyGroup>(prop & vhal20::VehiclePropertyGroup::MASK) ==
        vhal20::VehiclePropertyGroup::VENDOR) {
        return vccvhal10::toString(static_cast<vccvhal10::VehicleProperty>(prop));  // VENDOR
    } else {
        return vhal20::toString(static_cast<vhal20::VehicleProperty>(prop));  // SYSTEM
    }
}

std::string GetVehicleZoneNameByVehicleAreaAndAreaId(vhal20::VehicleArea vehicle_area, int32_t area_id) {
    std::string retval;

    switch (vehicle_area) {
        case vhal20::VehicleArea::GLOBAL:
            retval = "GLOBAL";
            break;

        case vhal20::VehicleArea::WINDOW:
            retval = vhal20::toString<vhal20::VehicleAreaWindow>(area_id);
            break;

        case vhal20::VehicleArea::MIRROR:
            retval = vhal20::toString<vhal20::VehicleAreaMirror>(area_id);
            break;

        case vhal20::VehicleArea::SEAT:
            retval = vhal20::toString<vhal20::VehicleAreaSeat>(area_id);
            break;

        case vhal20::VehicleArea::ZONE:
            retval = vhal20::toString<vhal20::VehicleAreaZone>(area_id);
            break;

        case vhal20::VehicleArea::DOOR:
            retval = vhal20::toString<vhal20::VehicleAreaDoor>(area_id);
            break;

        case vhal20::VehicleArea::MASK:  // TODO(dfloodh), unsure how to handle this

        default:
            retval = "No valid Area" + vhal20::toString(vehicle_area);
    }

    return retval;
}

void DumpPropertyValue(const ::android::sp<vhal20::IVehicle>& service, const vhal20::VehiclePropValue& property_value) {
    auto status_result = service->get(property_value, [](vhal20::StatusCode status,
                                                         const vhal20::VehiclePropValue& inner_property_value) {

        std::string name = GetPropertyName(inner_property_value.prop);

        auto vehicle_area = static_cast<vhal20::VehicleArea>(inner_property_value.prop & vhal20::VehicleArea::MASK);
        std::string area_name = GetVehicleZoneNameByVehicleAreaAndAreaId(vehicle_area, inner_property_value.areaId);

        printf("\nVehiclePropValue %s (%s)\n", name.c_str(), area_name.c_str());
        if (status != vhal20::StatusCode::OK) {
            printf("Value prop: %d = not OK (status: %s)\n", inner_property_value.prop, toString(status).c_str());
            return;
        }

        printf(" .value: ");

        int i = 1;
        const auto property_type =
                static_cast<vhal20::VehiclePropertyType>(inner_property_value.prop & vhal20::VehiclePropertyType::MASK);

        if (property_type == vhal20::VehiclePropertyType::STRING) {
            printf("  %s\n", inner_property_value.value.stringValue.c_str());
        } else {
            const auto number_of_values =
                    inner_property_value.value.bytes.size() + inner_property_value.value.int32Values.size() +
                    inner_property_value.value.int64Values.size() + inner_property_value.value.floatValues.size();

            if (number_of_values > 1) printf("\n");
            for (const auto& current_value : inner_property_value.value.bytes) {
                if (number_of_values > 1) {
                    printf("  %d: %d\n", i, current_value);
                } else {
                    printf("%d\n", current_value);
                }
                i++;
            }

            for (const auto& current_value : inner_property_value.value.int32Values) {
                if (number_of_values > 1) {
                    printf("  %d: %i\n", i, current_value);
                } else {
                    printf("%i\n", current_value);
                }
                i++;
            }
            for (const auto& current_value : inner_property_value.value.int64Values) {
                if (number_of_values > 1) {
                    printf("  %d: %li\n", i, current_value);
                } else {
                    printf("%li\n", current_value);
                }
                i++;
            }
            for (const auto& current_value : inner_property_value.value.floatValues) {
                if (number_of_values > 1) {
                    printf("  %d: %f\n", i, current_value);
                } else {
                    printf("%f\n", current_value);
                }
                i++;
            }
        }

        printf(" .timestamp: %li\n", inner_property_value.timestamp);

    });

    if (!status_result.isOk()) {
        std::string name = GetPropertyName(property_value.prop);
        printf("Failed to get property %s, desc=%s\n", name.c_str(), status_result.description().c_str());
    }
}

void DumpProperties(const ::android::sp<vhal20::IVehicle>& service,
                    const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& property_config_list) {
    for (const vhal20::VehiclePropConfig& property_config : property_config_list) {
        vhal20::VehiclePropValue requested_property_value;
        requested_property_value.prop = property_config.prop;

        const auto vehicle_property_group =
                static_cast<vhal20::VehiclePropertyGroup>(property_config.prop & vhal20::VehiclePropertyGroup::MASK);
        auto vehicle_area = static_cast<vhal20::VehicleArea>(property_config.prop & vhal20::VehicleArea::MASK);
        const auto vehicle_property_type =
                static_cast<vhal20::VehiclePropertyType>(property_config.prop & vhal20::VehiclePropertyType::MASK);

        // PROPERTY Info
        printf("\n----------------------------------------------\n");
        printf("\n\nVehicleProperty : %s", GetPropertyName(property_config.prop).c_str());
        printf("  [%s] [%s] [%s]\n", vhal20::toString(vehicle_property_group).c_str(),
               vhal20::toString(vehicle_area).c_str(), vhal20::toString(vehicle_property_type).c_str());
        printf(" .prop : %d\n", property_config.prop);

        // property CONFIG Info
        printf("\nVehiclePropConfig\n");
        printf(" .access: %d ( %s )\n", property_config.access, vhal20::toString(property_config.access).c_str());
        printf(" .changeMode: %d ( %s )\n", property_config.changeMode,
               vhal20::toString(property_config.changeMode).c_str());
        printf(" .supportedAreas: %d\n", property_config.supportedAreas);
        printf(" .configFlags: %d\n", property_config.configFlags);  // Dont know what this is for

        std::string config_array;
        for (auto value : property_config.configArray) {
            config_array += android::hardware::toString(value) + " ";
        }
        printf(" .configArray: %s\n", config_array.c_str());  // Dont know what this is for

        printf(" .configString: %s\n", property_config.configString.c_str());  // Dont know what this is for
        printf(" .minSampleRate: %f\n", property_config.minSampleRate);
        printf(" .maxSampleRate: %f\n", property_config.maxSampleRate);

        // property AREA CONFIGs Info
        if (property_config.areaConfigs.size() > 0) {
            // property AREA CONFIGs Info
            for (const vhal20::VehicleAreaConfig& area_config : property_config.areaConfigs) {
                printf("\nVehicleAreaConfigs.areaId=%d [%s]\n", area_config.areaId,
                       GetVehicleZoneNameByVehicleAreaAndAreaId(vehicle_area, area_config.areaId).c_str());

                printf(" .minInt32Value: %d\n", area_config.minInt32Value);
                printf(" .maxInt32Value: %d\n", area_config.maxInt32Value);
                printf(" .minInt64Value: %li\n", area_config.minInt64Value);
                printf(" .maxInt64Value: %li\n", area_config.maxInt64Value);
                printf(" .minFloatValue: %f\n", area_config.minFloatValue);
                printf(" .maxFloatValue: %f\n", area_config.maxFloatValue);
            }

            // property AREA VALUEs Info
            printf("\nValues:\n");
            for (const vhal20::VehicleAreaConfig& area_config : property_config.areaConfigs) {
                requested_property_value.areaId = area_config.areaId;
                DumpPropertyValue(service, requested_property_value);
            }
        } else {
            printf("\nWARNING: Bad Property. Missing areaconfig!!!!\n");
            printf("\nValues:\n");
            requested_property_value.areaId = 0;
            DumpPropertyValue(service, requested_property_value);
        }
    }
}

}  // namespace VhalUtil