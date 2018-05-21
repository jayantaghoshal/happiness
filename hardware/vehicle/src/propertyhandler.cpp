/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "propertyhandler.h"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;
using namespace android;

namespace VhalPropertyHandlerHelper {
vhal20::VehiclePropConfig BoolConfig(vccvhal10::VehicleProperty property, std::vector<int> areaIds) {
    vhal20::VehiclePropConfig config;
    config.prop = vhal20::toInt(property);
    config.access = vhal20::VehiclePropertyAccess::READ_WRITE;
    config.changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE;
    config.areaConfigs.resize(areaIds.size());
    for (size_t i = 0; i < areaIds.size(); i++) {
        config.areaConfigs[i].areaId = areaIds[i];
        config.areaConfigs[i].minInt32Value = 0;
        config.areaConfigs[i].maxInt32Value = 1;
    }

    return config;
}
}

template <>
void VhalPropertyHandler<int32_t>::PushProp(int32_t value, VehiclePropertyStatus status, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

    it->second.value.int32Values[0] = value;
    it->second.status = status;
    if (registeredWithVhal) {
        pushProp(it->second);
    }
}

template <>
void VhalPropertyHandler<float>::PushProp(float value, VehiclePropertyStatus status, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

    it->second.value.floatValues[0] = value;
    it->second.status = status;
    if (registeredWithVhal) {
        pushProp(it->second);
    }
}

template <>
void VhalPropertyHandler<bool>::PushProp(bool value, VehiclePropertyStatus status, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

    it->second.value.int32Values[0] = value;
    it->second.status = status;
    if (registeredWithVhal) {
        pushProp(it->second);
    }
}

template <>
void VhalPropertyHandler<int32_t>::PushValue(int32_t value, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

    it->second.value.int32Values[0] = value;
    if (registeredWithVhal) {
        pushProp(it->second);
    }
}

template <>
void VhalPropertyHandler<float>::PushValue(float value, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

    it->second.value.floatValues[0] = value;
    if (registeredWithVhal) {
        pushProp(it->second);
    }
}

template <>
void VhalPropertyHandler<bool>::PushValue(bool value, int32_t zone) {
    if (IsGlobal()) zone = 0;  // Need better solution?

    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }

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

template <>
void VhalPropertyHandler<int32_t>::setDefaultValue(vhal20::VehiclePropValue& property_value, int32_t defalutValue) {
    property_value.value.int32Values.resize(1);
    property_value.value.int32Values[0] = defalutValue;
    property_value.status = VehiclePropertyStatus::UNAVAILABLE;
}

template <>
void VhalPropertyHandler<float>::setDefaultValue(vhal20::VehiclePropValue& property_value, float defalutValue) {
    property_value.value.floatValues.resize(1);
    property_value.value.floatValues[0] = defalutValue;
    property_value.status = VehiclePropertyStatus::UNAVAILABLE;
}

template <>
void VhalPropertyHandler<bool>::setDefaultValue(vhal20::VehiclePropValue& property_value, bool defalutValue) {
    property_value.value.int32Values.resize(1);
    property_value.value.int32Values[0] = defalutValue;
    property_value.status = VehiclePropertyStatus::UNAVAILABLE;
}
