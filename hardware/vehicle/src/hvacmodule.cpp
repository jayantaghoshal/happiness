/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include <cutils/log.h>
#include <functional>
#include <memory>

#include "hvacmodule.h"
#include "i_vehicle_hal_impl.h"

#undef LOG_TAG
#define LOG_TAG "HvacModule"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace std::placeholders;

using namespace android;

HvacModule::HvacModule(::android::hardware::automotive::vehicle::V2_0::impl::IVehicleHalImpl* vehicleHal)
    : ModulePropertyHandler(vehicleHal), m_fanLevelImpl(), m_tempImpl() {
    {  // Temperature, HVAC_TEMPERATURE_SET
        PropertyAndConfig temperature_property;

        // Config for all zones
        temperature_property.prop_config.prop = toInt(VehicleProperty::HVAC_TEMPERATURE_SET);
        temperature_property.prop_config.access = VehiclePropertyAccess::READ_WRITE;
        temperature_property.prop_config.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        temperature_property.prop_config.supportedAreas =
                toInt(VehicleAreaZone::ROW_1_LEFT) | VehicleAreaZone::ROW_1_RIGHT;
        temperature_property.prop_config.areaConfigs.resize(2);
        temperature_property.prop_config.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1_LEFT);
        temperature_property.prop_config.areaConfigs[0].minFloatValue = 17;  // TODO carconfig?
        temperature_property.prop_config.areaConfigs[0].maxFloatValue = 27;  // TODO carconfig?
        temperature_property.prop_config.areaConfigs[1].areaId = toInt(VehicleAreaZone::ROW_1_RIGHT);
        temperature_property.prop_config.areaConfigs[1].minFloatValue = 17;  // TODO carconfig?
        temperature_property.prop_config.areaConfigs[1].maxFloatValue = 27;  // TODO carconfig?

        // Temp common
        VehiclePropValue prop_value;
        prop_value.prop = toInt(VehicleProperty::HVAC_TEMPERATURE_SET);

        {  // Left temperature
            prop_value.areaId = toInt(VehicleAreaZone::ROW_1_LEFT);

            // Put Helper for tempLeft in map
            temperature_property.property_per_zone_handlers[static_cast<VehicleAreaZone>(prop_value.areaId)] =
                    std::make_unique<PropertyHandlerFloat>(
                            [this](float left_temp) {
                                m_tempImpl.setleftTemp(left_temp);
                                return 0;
                            },
                            [this]() { return m_tempImpl.temperatureLeftValue(); },
                            [this](VehiclePropValue prop_value) { pushProp(prop_value); },
                            prop_value);
        }
        {  // Right temperature
            prop_value.areaId = toInt(VehicleAreaZone::ROW_1_RIGHT);

            // Put Helper for tempRight in map
            temperature_property.property_per_zone_handlers[static_cast<VehicleAreaZone>(prop_value.areaId)] =
                    std::make_unique<PropertyHandlerFloat>(
                            [this](float right_temp) {
                                m_tempImpl.setrightTemp(right_temp);
                                return 0;
                            },
                            [this]() { return m_tempImpl.temperatureRightValue(); },
                            [this](VehiclePropValue prop_value) { pushProp(prop_value); },
                            prop_value);
        }

        // Property handler configured, put it in map
        propertyhandlers_[static_cast<vccvhal10::VehicleProperty>(prop_value.prop)] = std::move(temperature_property);
    }

    {  // Fan level, HVAC_FAN_SPEED
        PropertyAndConfig fan_property;

        // Config for all zones
        fan_property.prop_config.prop = toInt(VehicleProperty::HVAC_FAN_SPEED);
        fan_property.prop_config.access = VehiclePropertyAccess::READ_WRITE;
        fan_property.prop_config.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        fan_property.prop_config.supportedAreas = toInt(VehicleAreaZone::ROW_1);
        fan_property.prop_config.areaConfigs.resize(1);
        fan_property.prop_config.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1);
        fan_property.prop_config.areaConfigs[0].minInt32Value = 0;  // corresponds to FAN_MIN
        fan_property.prop_config.areaConfigs[0].maxInt32Value = 6;  // corresponds to FAN_MAX

        // Fan
        vhal20::VehiclePropValue prop_value;
        prop_value.prop = toInt(VehicleProperty::HVAC_FAN_SPEED);
        prop_value.areaId = toInt(VehicleAreaZone::ROW_1);

        // Put Helper for fanLevel in map
        fan_property.property_per_zone_handlers[static_cast<VehicleAreaZone>(prop_value.areaId)] =
                std::make_unique<PropertyHandlerInt32>(
                        [this](int fan_speed) {
                            m_fanLevelImpl.setFanLevel(fan_speed);
                            return 0;
                        },
                        [this]() { return m_fanLevelImpl.fanLevelValue(); },
                        [this](vhal20::VehiclePropValue prop_value) { pushProp(prop_value); },
                        prop_value);

        // Property handler configured, put it in map
        propertyhandlers_[static_cast<vccvhal10::VehicleProperty>(prop_value.prop)] = std::move(fan_property);
    }
}
