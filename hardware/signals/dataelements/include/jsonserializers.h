/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cutils/log.h>
#include "gen_jsonencdec.h"

// ==================================================================
// The following two json serialize/deserialize shall be in another file
template <typename S>
std::string serialize(const DataElemValue<S>& dev) {
    using value_type = typename S::data_elem_type;

    json jsonObject;
    if (dev.isOk()) jsonObject["value"] = autosar::toJson<value_type>(dev.value());
    jsonObject["timestamp"] = dev.timestamp();
    jsonObject["state"] = (int)dev.state();
    jsonObject["type"] = S::TypeName();
    if (dev.isError()) jsonObject["errorCode"] = dev.errorCode();

    return jsonObject.dump();
}

template <typename S>
DataElemValue<S> deserialize(const std::string& ba) {
    using value_type = typename S::data_elem_type;

    json object = json::parse(ba);

    const long long timestamp = object["timestamp"].get<long long>();

    json::iterator typeIter = object.find("type");
    if (typeIter == object.end()) {
        return DataElemValue<S>::ERROR(-1, timestamp);  // mandatory type missing
    }

    const std::string typeName = typeIter.value().get<std::string>();
    if (typeName != S::TypeName()) {
        ALOG(LOG_WARN, "DE_JsonSerializer", "Is trying to deserialize %s into %s", typeName.c_str(), S::TypeName());
        return DataElemValue<S>::ERROR(-1, timestamp);
    }

    json::iterator stateIter = object.find("state");
    if (stateIter == object.end()) {
        return DataElemValue<S>::ERROR(-1, timestamp);  // mandatory state missing
    }

    DataElemValueBase::State state = (DataElemValueBase::State)stateIter.value().get<int>();
    switch (state) {
        case DataElemValueBase::State::OK: {
            const value_type value = autosar::fromJson<value_type>(object["value"]);
            return DataElemValue<S>::OK(value, timestamp);
            break;
        }

        case DataElemValueBase::State::ERROR:
            return DataElemValue<S>::ERROR(object["errorCode"].get<int>(), timestamp);
            break;

        default:
            return DataElemValue<S>::ERROR(-2, timestamp);  // unknown state
    }
}
