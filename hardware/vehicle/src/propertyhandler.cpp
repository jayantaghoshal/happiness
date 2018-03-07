/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "propertyhandler.h"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;
using namespace android;

template <>
void VhalPropertyHandler<int32_t>::PushProp(int32_t value, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

    it->second.value.int32Values.resize(1);
    it->second.value.int32Values[0] = value;
    if (registeredWithVhal) {
        pushProp(it->second);
    }
}
template <>
void VhalPropertyHandler<float>::PushProp(float value, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

    it->second.value.floatValues.resize(1);
    it->second.value.floatValues[0] = value;
    if (registeredWithVhal) {
        pushProp(it->second);
    }
}

template <>
void VhalPropertyHandler<bool>::PushProp(bool value, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

    it->second.value.int32Values.resize(1);
    it->second.value.int32Values[0] = value;
    if (registeredWithVhal) {
        pushProp(it->second);
    }
}

template <>
int VhalPropertyHandler<int32_t>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.int32Values.size() != 1) {
        return 1;
    }

    if (!IsGlobal()) {
        auto it = values_.find(propValue.areaId);
        if (it == values_.end()) {
            return 1;
        }
    }

    int32_t value = propValue.value.int32Values[0];
    int32_t areaId = propValue.areaId;
    // TODO: weak_ptr this?
    dispatcher_->Enqueue([this, value, areaId]() {
        if (request_set_prop_) {
            request_set_prop_(value, areaId);
        }
    });
    return 0;
};
template <>
int VhalPropertyHandler<bool>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.int32Values.size() != 1) {
        return 1;
    }

    if (!IsGlobal()) {
        auto it = values_.find(propValue.areaId);
        if (it == values_.end()) {
            return 1;
        }
    }

    bool value = propValue.value.int32Values[0];
    int32_t areaId = propValue.areaId;
    // TODO: weak_ptr this?
    dispatcher_->Enqueue([this, value, areaId]() {
        if (request_set_prop_) {
            request_set_prop_(value, areaId);
        }
    });
    return 0;
};
template <>
int VhalPropertyHandler<float>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.floatValues.size() != 1) {
        return 1;
    }

    if (!IsGlobal()) {
        auto it = values_.find(propValue.areaId);
        if (it == values_.end()) {
            return 1;
        }
    }

    float value = propValue.value.floatValues[0];
    int32_t areaId = propValue.areaId;
    // TODO: weak_ptr this?
    dispatcher_->Enqueue([this, value, areaId]() {
        if (request_set_prop_) {
            request_set_prop_(value, areaId);
        }
    });
    return 0;
};

vhal20::VehiclePropConfig BoolConfig(vccvhal10::VehicleProperty property, int32_t supportedAreas) {
    vhal20::VehiclePropConfig config;
    config.prop = vhal20::toInt(property);
    config.access = vhal20::VehiclePropertyAccess::READ_WRITE;
    config.changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE;
    config.supportedAreas = supportedAreas;
    config.areaConfigs.resize(0);  // Important to not init this for bool properties!

    return config;
}