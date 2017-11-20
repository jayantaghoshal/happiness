
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
    : impl::ModuleBase(vehicleHal), m_fanLevel(0), m_fanLevelImpl(m_fanLevel) {
    initHandledProperties();

    m_fanLevelHandle = m_fanLevel.subscribeAndCall([this](int32_t fanLevel) {
        ALOGD("FanLevel has been updated from the fanlevelimpl. Update the vhal property with fanLevel: %d", fanLevel);

        auto propIt = m_propValues.find(VehicleProperty::HVAC_FAN_SPEED);
        if (propIt == m_propValues.end()) {
            ALOGE("VehicleProperty::HVAC_FAN_SPEED has not been registred in init of HvacModule");
            return;
        }

        // Update the map and and notify property store
        auto propValInMap = propIt->second;
        propValInMap.value.int32Values[0] = m_fanLevel.get();
        VehiclePropValue propValue = propValInMap;
        pushProp(propValue);
    });
}

int HvacModule::setProp(const VehiclePropValue& propValue) {
    switch (propValue.prop) {
        case toInt(VehicleProperty::HVAC_FAN_SPEED): {
            ALOGV("HvacModule setProp HVAC_FAN_SPEED to: %d", propValue.value.int32Values[0]);
            m_fanLevelImpl.setFanLevel(propValue.value.int32Values[0]);
            return 0;
        }
        default: {
            ALOGE("Trying to set property not handled by hvacmodule: %d", propValue.prop);
            return 1;
        }
    }
}

std::unique_ptr<VehiclePropValue> HvacModule::getProp(const VehiclePropValue& requestedPropValue) {
    ALOGV("HvacModule getProp: %d", static_cast<int>(requestedPropValue.prop));
    auto propIt = m_propValues.find(static_cast<VehicleProperty>(requestedPropValue.prop));

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

    return propConfigs;
}

void HvacModule::initHandledProperties() {
    VehiclePropValue propValue;

    // Config of HVAC_FAN_SPEED
    propValue.prop = toInt(VehicleProperty::HVAC_FAN_SPEED);
    propValue.value.int32Values.resize(1);
    propValue.areaId = toInt(VehicleAreaZone::ROW_1);
    propValue.value.int32Values[0] = m_fanLevel.get();

    m_propValues[VehicleProperty::HVAC_FAN_SPEED] = propValue;
}
