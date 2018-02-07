/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include "sensormodule.h"

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "SensorModule"

SensorModule::SensorModule(vhal20::impl::IVehicleHalImpl* vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal),
      sensor_prop_config_{{.prop = toInt(vhal20::VehicleProperty::DRIVING_STATUS),
                           .access = vhal20::VehiclePropertyAccess::READ,
                           .changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE,
                           .supportedAreas = toInt(vhal20::VehicleAreaZone::WHOLE_CABIN),
                           .areaConfigs = {{.areaId = toInt(vhal20::VehicleAreaZone::WHOLE_CABIN),
                                            .minInt32Value = 0x00,
                                            .maxInt32Value = 0x1F}}}} {}

vhal20::VehiclePropValue SensorModule::getPropertyValue(vhal20::VehicleProperty vehicle_property) {
    vhal20::VehiclePropValue vehicle_property_value;

    switch (vehicle_property) {
        case vhal20::VehicleProperty::DRIVING_STATUS: {
            vehicle_property_value.timestamp = elapsedRealtimeNano();
            vehicle_property_value.areaId = static_cast<int32_t>(vhal20::VehicleArea::GLOBAL);
            vehicle_property_value.prop = toInt(vhal20::VehicleProperty::DRIVING_STATUS);
            vehicle_property_value.value.int32Values.resize(1);
            vehicle_property_value.value.int32Values[0] = toInt(vhal20::VehicleDrivingStatus::UNRESTRICTED);
            break;
        }
        default:
            ALOGE("[%s] Invalid property: %d.", __FUNCTION__, static_cast<int>(vehicle_property));
            break;
    }
    return vehicle_property_value;
}

std::vector<vhal20::VehiclePropValue> SensorModule::getAllPropValues() {
    std::vector<vhal20::VehiclePropValue> property_values;
    property_values.push_back(getPropertyValue(vhal20::VehicleProperty::DRIVING_STATUS));
    ALOGV("[%s] Vehicle property values returned.", __FUNCTION__);
    return property_values;
}

std::vector<vhal20::VehiclePropConfig> SensorModule::listProperties() {
    ALOGV("[%s] Vehicle property configs returned.", __FUNCTION__);
    return sensor_prop_config_;
}

std::unique_ptr<vhal20::VehiclePropValue> SensorModule::getProp(const vhal20::VehiclePropValue& property_value,
                                                                vhal20::impl::Status& /*status*/) {
    ALOGV("[%s] Property ID: %d", __func__, property_value.prop);

    auto property = static_cast<vhal20::VehicleProperty>(property_value.prop);

    auto vehicle_property_value = getPropertyValue(property);

    return std::make_unique<vhal20::VehiclePropValue>(vehicle_property_value);
}
