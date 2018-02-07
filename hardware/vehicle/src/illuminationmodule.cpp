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
#include "illuminationmodule.h"

#undef LOG_TAG
#define LOG_TAG "IlluminationModule"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;

using namespace ApplicationDataElement;
using namespace autosar;
using namespace android;

IlluminationHal::IlluminationHal(vhal20::impl::IVehicleHalImpl* vehicleHal) : vhal20::impl::ModuleBase(vehicleHal) {
    StartFlexraySubscribers();
}

std::unique_ptr<VehiclePropValue> IlluminationHal::getProp(const VehiclePropValue& requestedPropValue,
                                                           vhal20::impl::Status& /*status*/) {
    ALOGD("getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
    VehiclePropValue prop_value;
    switch (requestedPropValue.prop) {
        case toInt(VehicleProperty::AMBIENT_DAY_NIGHT):
            GetDayNightFromFlexray(prop_value);
            break;

        case toInt(VehicleProperty::MANUAL_ILLUMINATION):
            GetIlluminationRegulatorFromFlexray(prop_value);
            break;

        default:
            ALOGW("Unknown getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
            return nullptr;
            break;
    }
    return std::make_unique<VehiclePropValue>(prop_value);
}

std::vector<VehiclePropValue> IlluminationHal::getAllPropValues() {
    VehiclePropValue daynight;
    VehiclePropValue illumination;

    GetDayNightFromFlexray(daynight);
    GetIlluminationRegulatorFromFlexray(illumination);

    return {daynight, illumination};
}

std::vector<VehiclePropConfig> IlluminationHal::listProperties() {
    VehiclePropConfig daynightconfig;
    daynightconfig.prop = toInt(VehicleProperty::AMBIENT_DAY_NIGHT);
    daynightconfig.access = VehiclePropertyAccess::READ;
    daynightconfig.changeMode = VehiclePropertyChangeMode::ON_CHANGE;

    VehiclePropConfig illuminationconfig;
    illuminationconfig.prop = toInt(VehicleProperty::MANUAL_ILLUMINATION);
    illuminationconfig.access = VehiclePropertyAccess::READ;
    illuminationconfig.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
    illuminationconfig.supportedAreas = toInt(VehicleAreaZone::WHOLE_CABIN);
    illuminationconfig.areaConfigs.resize(1);
    illuminationconfig.areaConfigs[0].areaId = toInt(VehicleAreaZone::WHOLE_CABIN);
    illuminationconfig.areaConfigs[0].minInt32Value = 0;    // 0%
    illuminationconfig.areaConfigs[0].maxInt32Value = 100;  // 100%

    return {daynightconfig, illuminationconfig};
}

void IlluminationHal::StartFlexraySubscribers() {
    day_night_receiver_.subscribe([this]() {
        VehiclePropValue prop_value;
        if (GetDayNightFromFlexray(prop_value)) {
            ALOGI("DayNight changed to: %d", prop_value.value.int32Values[0]);
            pushProp(prop_value);
        }
    });

    illumination_receiver_.subscribe([this]() {
        VehiclePropValue prop_value;
        if (GetIlluminationRegulatorFromFlexray(prop_value)) {
            ALOGI("Illumination changed to: %d", prop_value.value.int32Values[0]);
            pushProp(prop_value);
        }
    });
}

bool IlluminationHal::GetDayNightFromFlexray(VehiclePropValue& prop_value) {
    // day/night is a boolen which is assumed to be filled in into int32 but this is actually unclear
    prop_value.timestamp = elapsedRealtimeNano();
    prop_value.areaId = 0;
    prop_value.prop = toInt(VehicleProperty::AMBIENT_DAY_NIGHT);
    prop_value.value.int32Values.resize(1);
    prop_value.value.int32Values[0] = 1;  // true/Day is a valid default value if nothing can be read
    const auto signal_state_value = day_night_receiver_.get();
    if (signal_state_value.isOk()) {
        prop_value.value.int32Values[0] = (signal_state_value.value() == TwliBriSts1::Day) ? 1 : 0;
        return true;
    } else {
        ALOGI("%s signal TwliBriSts is in error.", __FUNCTION__);
        return false;
    }
}

bool IlluminationHal::GetIlluminationRegulatorFromFlexray(VehiclePropValue& prop_value) {
    prop_value.timestamp = elapsedRealtimeNano();
    prop_value.areaId = 0;
    prop_value.prop = toInt(VehicleProperty::MANUAL_ILLUMINATION);
    prop_value.value.int32Values.resize(1);
    prop_value.value.int32Values[0] = 100;  // 100% is a valid default value if nothing can be read
    const auto signal_state_value = illumination_receiver_.get();
    if (signal_state_value.isOk()) {
        prop_value.value.int32Values[0] =
                round(signal_state_value.value() * 100.0 / 15.0);  // value from signal is 0..15
        return true;
    } else {
        ALOGI("%s signal IntrBriSts is in error.", __FUNCTION__);
        return false;
    }
}

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
