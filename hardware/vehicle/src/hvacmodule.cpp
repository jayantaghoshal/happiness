/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include <functional>
#include <memory>
#include <vector>

#include <cutils/log.h>
#include "hvacmodule.h"

#undef LOG_TAG
#define LOG_TAG "HvacModule"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;

HvacModule::HvacModule(::android::hardware::automotive::vehicle::V2_0::impl::IVehicleHalImpl* vehicleHal)
    : impl::ModuleBase(vehicleHal), m_fanLevel(0), m_fanLevelImpl(m_fanLevel), m_tempImpl(m_temperature) {
    initHandledProperties();

    m_fanLevelHandle = m_fanLevel.subscribeAndCall([this](int32_t fanLevel) {
        ALOGD("FanLevel has been updated from the fanlevelimpl. Update the vhal property with fanLevel: %d", fanLevel);

        auto propIt = m_propValues.find(propertyKey(VehicleProperty::HVAC_FAN_SPEED, VehicleAreaZone::ROW_1));
        if (propIt == m_propValues.end()) {
            ALOGE("VehicleProperty::HVAC_FAN_SPEED has not been registred in init of HvacModule");
            return;
        }

        // Update the map and and notify property store
        auto& propValInMap = propIt->second;
        propValInMap.value.int32Values[0] = m_fanLevel.get();
        VehiclePropValue propValue = propValInMap;
        pushProp(propValue);
    });

    m_temperatureLeftHandle = m_temperature.subscribeAndCall([this](float temp_left) {
        ALOGD("temp_left has been updated from the tempimpl. Update the vhal property with temp_left: %f", temp_left);
        auto propIt =
                m_propValues.find(propertyKey(VehicleProperty::HVAC_TEMPERATURE_SET, VehicleAreaZone::ROW_1_LEFT));
        if (propIt == m_propValues.end()) {
            ALOGE("VehicleProperty::HVAC_TEMPERATURE_SET has not been registred in init of HvacModule");
            return;
        }

        // Notifying subscribers
        auto& propValInMap = propIt->second;
        propValInMap.value.floatValues[0] = m_temperature.get();
        VehiclePropValue propValue = propValInMap;
        pushProp(propValue);
    });

    m_temperatureRightHandle = m_temperature.subscribeAndCall([this](float temp_right) {
        ALOGD("temp_right has been updated from the tempimpl. Update the vhal property with temp_right: %f",
              temp_right);
        auto propIt =
                m_propValues.find(propertyKey(VehicleProperty::HVAC_TEMPERATURE_SET, VehicleAreaZone::ROW_1_RIGHT));
        if (propIt == m_propValues.end()) {
            ALOGE("VehicleProperty::HVAC_TEMPERATURE_SET has not been registred in init of HvacModule");
            return;
        }

        // Notifying subscribers
        auto propValInMap = propIt->second;
        propValInMap.value.floatValues[0] = m_temperature.get();
        VehiclePropValue propValue = propValInMap;
        pushProp(propValue);
    });
}

int HvacModule::setProp(const VehiclePropValue& propValue) {
    ALOGD("Setprop: %d", propValue.prop);
    switch (propValue.prop) {
        case toInt(VehicleProperty::HVAC_FAN_SPEED): {
            m_fanLevelImpl.setFanLevel(propValue.value.int32Values[0]);
            return 0;
        }
        case toInt(VehicleProperty::HVAC_TEMPERATURE_SET): {
            if (propValue.value.floatValues.size() != 1) {
                ALOGW("HvacModule setProp HVAC_TEMPERATURE_SET floatvalues.size !=1");
                return 1;
            }
            switch (propValue.areaId) {
                case toInt(VehicleAreaZone::ROW_1_LEFT):
                    m_tempImpl.setleftTemp(propValue.value.floatValues[0]);
                    break;
                case toInt(VehicleAreaZone::ROW_1_RIGHT):
                    m_tempImpl.setrightTemp(propValue.value.floatValues[0]);
                    break;
                default: {
                    ALOGE("Trying to set property not handled by hvacmodule");
                    return 1;
                }
            }
            return 0;
        }
        default: {
            ALOGE("Trying to set property not handled by hvacmodule: %d", propValue.prop);
            return 1;
        }
    }
}

std::unique_ptr<VehiclePropValue> HvacModule::getProp(const VehiclePropValue& requestedPropValue) {
    ALOGD("getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
    auto propIt = m_propValues.find(propertyKey(requestedPropValue));

    if (propIt == m_propValues.end()) {
        ALOGE("HvacModule trying to get unhandled property: %d", static_cast<int>(requestedPropValue.prop));
        return nullptr;
    }

    auto propValInMap = propIt->second;
    auto propValue = std::make_unique<VehiclePropValue>(propValInMap);

    return propValue;
}

std::vector<VehiclePropValue> HvacModule::getAllPropValues() {
    std::vector<VehiclePropValue> propValues;
    for (auto const& prop : m_propValues) {
        propValues.push_back(prop.second);
    }

    return propValues;
}

std::vector<VehiclePropConfig> HvacModule::listProperties() {
    std::vector<VehiclePropConfig> propConfigs;
    {
        VehiclePropConfig propConfig;

        // Config of HVAC_FAN_SPEED
        propConfig.prop = toInt(VehicleProperty::HVAC_FAN_SPEED);
        propConfig.access = VehiclePropertyAccess::READ_WRITE;
        propConfig.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        propConfig.supportedAreas = toInt(VehicleAreaZone::ROW_1);
        propConfig.areaConfigs.resize(1);
        propConfig.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1);
        propConfig.areaConfigs[0].minInt32Value = 0;  // corresponds to FAN_MIN
        propConfig.areaConfigs[0].maxInt32Value = 6;  // corresponds to FAN_MAX

        propConfigs.push_back(propConfig);
    }
    {
        VehiclePropConfig propConfig;

        propConfig.prop = toInt(VehicleProperty::HVAC_TEMPERATURE_SET);
        propConfig.access = VehiclePropertyAccess::READ_WRITE;
        propConfig.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        propConfig.supportedAreas = VehicleAreaZone::ROW_1_LEFT | VehicleAreaZone::ROW_1_RIGHT;
        propConfig.areaConfigs.resize(2);
        propConfig.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1_LEFT);
        propConfig.areaConfigs[0].minFloatValue = 16;
        propConfig.areaConfigs[0].maxFloatValue = 28;
        propConfig.areaConfigs[1].areaId = toInt(VehicleAreaZone::ROW_1_RIGHT);
        propConfig.areaConfigs[1].minFloatValue = 16;
        propConfig.areaConfigs[1].maxFloatValue = 28;

        propConfigs.push_back(propConfig);
    }
    return propConfigs;
}

void HvacModule::initHandledProperties() {
    {
        VehiclePropValue fanspeed;

        // Config of HVAC_FAN_SPEED
        fanspeed.prop = toInt(VehicleProperty::HVAC_FAN_SPEED);
        fanspeed.value.int32Values.resize(1);
        fanspeed.areaId = toInt(VehicleAreaZone::ROW_1);
        fanspeed.value.int32Values[0] = m_fanLevel.get();

        m_propValues[propertyKey(fanspeed)] = fanspeed;
    }
    {
        VehiclePropValue templeft;

        templeft.prop = toInt(VehicleProperty::HVAC_TEMPERATURE_SET);
        templeft.value.floatValues.resize(1);
        templeft.areaId = toInt(VehicleAreaZone::ROW_1_LEFT);
        templeft.value.floatValues[0] = m_fanLevel.get();

        m_propValues[propertyKey(templeft)] = templeft;
    }
    {
        VehiclePropValue tempright;

        tempright.prop = toInt(VehicleProperty::HVAC_TEMPERATURE_SET);
        tempright.value.floatValues.resize(1);
        tempright.areaId = toInt(VehicleAreaZone::ROW_1_RIGHT);
        tempright.value.floatValues[0] = m_fanLevel.get();

        m_propValues[propertyKey(tempright)] = tempright;
    }
}
