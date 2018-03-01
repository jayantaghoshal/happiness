/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vhal_util.h"
#include <iostream>

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;
using namespace std;
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

template <typename TValue, typename TExtraCast = TValue>
void DumpArray(const ::android::hardware::hidl_vec<TValue>& values,
               bool include_array_index,
               const std::string& indentation) {
    int i = 0;
    for (const auto& current_value : values) {
        if (include_array_index) {
            cout << indentation << i << ": " << static_cast<TExtraCast>(current_value) << endl;
        } else {
            cout << static_cast<TExtraCast>(current_value) << endl;
        }
        i++;
    }
}

void GetAndDumpPropertyValue(const ::android::sp<vhal20::IVehicle>& service,
                             const vhal20::VehiclePropValue& property_value,
                             const std::string indentation) {
    std::string name = GetPropertyName(property_value.prop);
    auto status_result = service->get(
            property_value,
            [indentation, name](vhal20::StatusCode status, const vhal20::VehiclePropValue& inner_property_value) {
                if (status != vhal20::StatusCode::OK) {
                    cout << indentation << "Failed to get value for " << name << ", status: " << toString(status)
                         << endl;
                    return;
                }
                DumpPropertyValue(inner_property_value, indentation);
            });
    if (!status_result.isOk()) {
        cout << indentation << "Failed to get property " << name << ", reason: " << status_result.description() << endl;
    }
}

void DumpPropertyValue(const vhal20::VehiclePropValue& property_value, const std::string indentation) {
    std::string name = GetPropertyName(property_value.prop);
    auto vehicle_area = static_cast<vhal20::VehicleArea>(property_value.prop & vhal20::VehicleArea::MASK);
    std::string area_name = GetVehicleZoneNameByVehicleAreaAndAreaId(vehicle_area, property_value.areaId);

    cout << indentation << "VehiclePropValue " << name << " " << area_name << endl;

    cout << indentation << " .value: ";  // Deliberate no endl to show value on same line in

    const auto property_type =
            static_cast<vhal20::VehiclePropertyType>(property_value.prop & vhal20::VehiclePropertyType::MASK);

    if (property_type == vhal20::VehiclePropertyType::STRING) {
        cout << indentation << "   " << property_value.value.stringValue << endl;
    } else {
        const auto number_of_values = property_value.value.bytes.size() + property_value.value.int32Values.size() +
                                      property_value.value.int64Values.size() + property_value.value.floatValues.size();

        if (number_of_values > 1) {
            cout << endl;
        }
        // Cast bytes to int to avoid printing as char
        DumpArray<uint8_t, uint32_t>(property_value.value.bytes, (number_of_values > 1), indentation + "    ");
        DumpArray(property_value.value.int32Values, (number_of_values > 1), indentation + "   ");
        DumpArray(property_value.value.int64Values, (number_of_values > 1), indentation + "    ");
        DumpArray(property_value.value.floatValues, (number_of_values > 1), indentation + "    ");
        if (number_of_values == 0) {
            cout << "ERROR: NO VALUES IN PROPERTY" << endl;
        }
    }
    cout << indentation << " .timestamp: " << property_value.timestamp << endl;
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
        printf("\n\nVehicleProperty : %s [%s] [%s] [%s] [propid=0x%08x]\n",
               GetPropertyName(property_config.prop).c_str(),
               vhal20::toString(vehicle_property_group).c_str(),
               vhal20::toString(vehicle_area).c_str(),
               vhal20::toString(vehicle_property_type).c_str(),
               property_config.prop);

        // property CONFIG Info
        printf("\n");
        printf("   VehiclePropConfig\n");
        printf("    .access: %d ( %s )\n", property_config.access, vhal20::toString(property_config.access).c_str());
        printf("    .changeMode: %d ( %s )\n",
               property_config.changeMode,
               vhal20::toString(property_config.changeMode).c_str());
        printf("    .supportedAreas: %d\n", property_config.supportedAreas);
        printf("    .configFlags: %d\n", property_config.configFlags);  // Dont know what this is for

        std::string config_array;
        for (auto value : property_config.configArray) {
            config_array += android::hardware::toString(value) + " ";
        }
        printf("    .configArray: %s\n", config_array.c_str());                   // Dont know what this is for
        printf("    .configString: %s\n", property_config.configString.c_str());  // Dont know what this is for
        printf("    .minSampleRate: %f\n", property_config.minSampleRate);
        printf("    .maxSampleRate: %f\n", property_config.maxSampleRate);

        // property AREA CONFIGs Info
        if (property_config.areaConfigs.size() > 0) {
            // property AREA CONFIGs Info
            for (const vhal20::VehicleAreaConfig& area_config : property_config.areaConfigs) {
                printf("\n");
                printf("   VehicleAreaConfigs\n");
                printf("    .areaId=%d [%s]\n",
                       area_config.areaId,
                       GetVehicleZoneNameByVehicleAreaAndAreaId(vehicle_area, area_config.areaId).c_str());
                printf("    .minInt32Value: %d\n", area_config.minInt32Value);
                printf("    .maxInt32Value: %d\n", area_config.maxInt32Value);
                printf("    .minInt64Value: %li\n", area_config.minInt64Value);
                printf("    .maxInt64Value: %li\n", area_config.maxInt64Value);
                printf("    .minFloatValue: %f\n", area_config.minFloatValue);
                printf("    .maxFloatValue: %f\n", area_config.maxFloatValue);
            }

            // property AREA VALUEs Info
            printf("\n");
            printf("   Values:\n");
            for (const vhal20::VehicleAreaConfig& area_config : property_config.areaConfigs) {
                requested_property_value.areaId = area_config.areaId;
                GetAndDumpPropertyValue(service, requested_property_value, "      ");
            }
        } else {
            printf("\n   WARNING: Bad Property. Missing areaconfig!!!!\n");
            printf("\n   Values:\n");
            requested_property_value.areaId = 0;
            GetAndDumpPropertyValue(service, requested_property_value, "      ");
        }
    }
}

}  // namespace VhalUtil