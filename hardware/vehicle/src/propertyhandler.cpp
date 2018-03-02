/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "propertyhandler.h"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;

using namespace android;

template <>
void VhalPropertyHandler<int32_t>::myPushProp(int32_t value) {
    value_.value.int32Values.resize(1);
    value_.value.int32Values[0] = value;
    pushProp(value_);
}
template <>
void VhalPropertyHandler<bool>::myPushProp(bool value) {
    value_.value.int32Values.resize(1);
    value_.value.int32Values[0] = value;
    pushProp(value_);
}

template <>
void VhalPropertyHandler<float>::myPushProp(float value) {
    value_.value.floatValues.resize(1);
    value_.value.floatValues[0] = value;
    pushProp(value_);
}

template <>
int VhalPropertyHandler<int32_t>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.int32Values.size() != 1) {
        return 1;
    }
    int value = propValue.value.int32Values[0];
    // TODO: weak_ptr this?
    dispatcher_->Enqueue([this, value]() {
        if (request_set_prop_) {
            request_set_prop_(value);
        }
    });
    return 0;
};

template <>
int VhalPropertyHandler<bool>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.int32Values.size() != 1) {
        return 1;
    }
    bool value = propValue.value.int32Values[0];
    // TODO: weak_ptr this?
    dispatcher_->Enqueue([this, value]() {
        if (request_set_prop_) {
            request_set_prop_(value);
        }
    });
    return 0;
};
template <>
int VhalPropertyHandler<float>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.floatValues.size() != 1) {
        return 1;
    }
    int value = propValue.value.floatValues[0];
    // TODO: weak_ptr this?
    dispatcher_->Enqueue([this, value]() {
        if (request_set_prop_) {
            request_set_prop_(value);
        }
    });
    return 0;
};

template <>
void MultiZonePropertyHandler<int32_t>::myPushProp(int32_t value, int32_t zone) {
    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }
    it->second.value.int32Values.resize(1);
    it->second.value.int32Values[0] = value;
    pushProp(it->second);
}
template <>
void MultiZonePropertyHandler<float>::myPushProp(float value, int32_t zone) {
    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }
    it->second.value.floatValues.resize(1);
    it->second.value.floatValues[0] = value;
    pushProp(it->second);
}

template <>
void MultiZonePropertyHandler<bool>::myPushProp(bool value, int32_t zone) {
    auto it = values_.find(zone);
    if (it == values_.end()) {
        return;
    }
    it->second.value.int32Values.resize(1);
    it->second.value.int32Values[0] = value;
    pushProp(it->second);
}

template <>
int MultiZonePropertyHandler<int32_t>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.int32Values.size() != 1) {
        return 1;
    }

    auto it = values_.find(propValue.areaId);
    if (it == values_.end()) {
        return 1;
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
int MultiZonePropertyHandler<bool>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.int32Values.size() != 1) {
        return 1;
    }

    auto it = values_.find(propValue.areaId);
    if (it == values_.end()) {
        return 1;
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
int MultiZonePropertyHandler<float>::setProp(const vhal20::VehiclePropValue& propValue) {
    if (propValue.value.floatValues.size() != 1) {
        return 1;
    }

    auto it = values_.find(propValue.areaId);
    if (it == values_.end()) {
        return 1;
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
