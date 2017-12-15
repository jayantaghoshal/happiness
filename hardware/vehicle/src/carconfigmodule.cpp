/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include <vector>

#include <cutils/log.h>
#include "carconfig.h"
#include "carconfigmodule.h"

#undef LOG_TAG
#define LOG_TAG "VccVhalModuleCarConfig"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {

CarConfigHal::CarConfigHal(::android::hardware::automotive::vehicle::V2_0::impl::IVehicleHalImpl *vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal), carconfigs(carconfig::getNumberOfCCParameters()) {
    // Get all carconfigs
    for (int i = 0; i < carconfig::getNumberOfCCParameters(); i++) {
        carconfigs[i] = carconfig::getValue(i + 1);
    }
}

std::vector<vhal20::VehiclePropValue> CarConfigHal::getAllPropValues() {
    ALOGD("CarConfigHal: getAllPropValues");
    std::vector<vhal20::VehiclePropValue> props;
    vhal20::VehiclePropValue propValue;
    propValue.value.bytes.resize(carconfigs.size());

    propValue.timestamp = android::elapsedRealtimeNano();
    propValue.areaId = 0;

    propValue.prop = vhal20::toInt(vccvhal10::VehicleProperty::CARCONFIG);
    propValue.value.bytes = carconfigs;
    props.push_back(propValue);

    return props;
}

std::vector<vhal20::VehiclePropConfig> CarConfigHal::listProperties() {
    vhal20::VehiclePropConfig propConfig{};
    propConfig.prop = vhal20::toInt(vccvhal10::VehicleProperty::CARCONFIG);
    propConfig.access = vhal20::VehiclePropertyAccess::READ;
    propConfig.changeMode = vhal20::VehiclePropertyChangeMode::STATIC;

    return std::vector<vhal20::VehiclePropConfig>{propConfig};
}

}  // impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
