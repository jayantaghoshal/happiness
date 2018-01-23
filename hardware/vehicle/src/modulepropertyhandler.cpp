/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include <algorithm>
#include <functional>
#include <memory>

#include <cutils/log.h>
#include "modulepropertyhandler.h"

#undef LOG_TAG
#define LOG_TAG "ModulePropertyHandler"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;

using namespace android;

ModulePropertyHandler::ModulePropertyHandler(vhal20::impl::IVehicleHalImpl* vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal) {}

int ModulePropertyHandler::setProp(const VehiclePropValue& propValue) {
    ALOGV("%s called for property: 0x%0x in zone %d", __FUNCTION__, propValue.prop, propValue.areaId);
    // Find the property
    auto property_it = propertyhandlers_.find(static_cast<vccvhal10::VehicleProperty>(propValue.prop));
    if (property_it != propertyhandlers_.end()) {
        // Find the zone
        auto& prop_zone_map = property_it->second.property_per_zone_handlers;
        auto prop_zone_it = prop_zone_map.find(static_cast<vhal20::VehicleAreaZone>(propValue.areaId));
        if (prop_zone_it != prop_zone_map.end()) {
            // Call set on the property handler related to this property
            prop_zone_it->second->set(propValue);
            return 0;
        }
    }
    ALOGE("SetProp called for unhandled property: %d and zone %d", propValue.prop, propValue.areaId);
    return 1;
}

std::unique_ptr<VehiclePropValue> ModulePropertyHandler::getProp(const VehiclePropValue& requestedPropValue,
                                                                 vhal20::impl::Status& /*status*/) {
    ALOGV("%s: for property: 0x%0x in zone %d", __FUNCTION__, requestedPropValue.prop, requestedPropValue.areaId);
    // Find the property
    auto property_it = propertyhandlers_.find(static_cast<vccvhal10::VehicleProperty>(requestedPropValue.prop));
    if (property_it != propertyhandlers_.end()) {
        // Find the zone
        auto& prop_zone_map = property_it->second.property_per_zone_handlers;
        auto prop_zone_it = prop_zone_map.find(static_cast<vhal20::VehicleAreaZone>(requestedPropValue.areaId));
        if (prop_zone_it != prop_zone_map.end()) {
            // Call get on the property handler related to this property
            return std::make_unique<VehiclePropValue>(prop_zone_it->second->get());
        }
    }
    ALOGE("GetProp called for unhandled property: %d and zone %d", requestedPropValue.prop, requestedPropValue.areaId);
    return nullptr;
}

std::vector<VehiclePropValue> ModulePropertyHandler::getAllPropValues() {
    std::vector<VehiclePropValue> all_prop_values;
    for (auto& property_it : propertyhandlers_) {
        auto& prop_zone_map = property_it.second.property_per_zone_handlers;
        for (auto& prop_zone_it : prop_zone_map) {
            // Call get on all the property handlers, i.e. all properties and all zones
            all_prop_values.push_back(prop_zone_it.second->get());
        }
    }
    return all_prop_values;
}

std::vector<VehiclePropConfig> ModulePropertyHandler::listProperties() {
    std::vector<VehiclePropConfig> property_configs;
    for (auto& property_it : propertyhandlers_) {
        property_configs.push_back(property_it.second.prop_config);
    }
    return property_configs;
}

ModulePropertyHandler::PropertyHandlerFloat::PropertyHandlerFloat(
        const std::function<int(float)> set_prop,
        const std::function<ReadOnlyNotifiableProperty<float>*()> notifiable_prop_getter,
        const std::function<void(vhal20::VehiclePropValue)> pushProp, vhal20::VehiclePropValue prop_value)
    : set_prop_(set_prop), notifiable_prop_getter_(notifiable_prop_getter), prop_value_(prop_value) {
    prop_value_.value.floatValues.resize(1);

    subscription_handle_ = notifiable_prop_getter()->subscribeAndCall([this, pushProp](float new_value) {
        ALOGD("Float property %d has been updated from the impl with value: %f", prop_value_.prop, new_value);

        prop_value_.timestamp = elapsedRealtimeNano();
        prop_value_.value.floatValues[0] = new_value;
        pushProp(prop_value_);
    });
}

void ModulePropertyHandler::PropertyHandlerFloat::set(const vhal20::VehiclePropValue& prop_value) {
    set_prop_(prop_value.value.floatValues[0]);
}

vhal20::VehiclePropValue ModulePropertyHandler::PropertyHandlerFloat::get() {
    prop_value_.value.floatValues[0] = notifiable_prop_getter_()->get();
    return prop_value_;
}

ModulePropertyHandler::PropertyHandlerInt32::PropertyHandlerInt32(
        const std::function<int(int)> set_prop,
        const std::function<ReadOnlyNotifiableProperty<int>*()> notifiable_prop_getter,
        const std::function<void(vhal20::VehiclePropValue)> pushProp, vhal20::VehiclePropValue prop_value)
    : set_prop_(set_prop), notifiable_prop_getter_(notifiable_prop_getter), prop_value_(prop_value) {
    prop_value_.value.int32Values.resize(1);

    subscription_handle_ = notifiable_prop_getter()->subscribeAndCall([this, pushProp](int new_value) {
        ALOGD("int property %d has been updated from the impl with value: %d", prop_value_.prop, new_value);
        prop_value_.timestamp = elapsedRealtimeNano();
        prop_value_.value.int32Values[0] = new_value;
        pushProp(prop_value_);
    });
}

void ModulePropertyHandler::PropertyHandlerInt32::set(const vhal20::VehiclePropValue& prop_value) {
    set_prop_(prop_value.value.int32Values[0]);
}

vhal20::VehiclePropValue ModulePropertyHandler::PropertyHandlerInt32::get() {
    prop_value_.value.int32Values[0] = notifiable_prop_getter_()->get();
    return prop_value_;
}

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor