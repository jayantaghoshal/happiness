/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "climate_ctrl_service.h"
#include "convapi_signals_def.h"
#include "vhal_conv_mappings.h"

#include <string>
#include <unordered_map>
#include <utility>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_ClimateCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl::remoteclimatectrl;
using namespace vcc::remotectrl;
using namespace vhal_2_0;

template <>
std::vector<vsomeip::byte_t> ClimateCtrlService::CreateSomeIpResponse(
        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE>&& /*dummy*/,
        const vhal_2_0::VehiclePropValue& requestedPropValue) {
    // Include FunctionAvailability Status
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(toConvApiStatus(requestedPropValue.status))};

    for (const auto& val : requestedPropValue.value.floatValues) {
        int temperature = (val - 16.5f) * 2;
        payload_data.push_back(static_cast<vsomeip::byte_t>(temperature));
    }

    return payload_data;
}

template <>
std::vector<vsomeip::byte_t> ClimateCtrlService::CreateSomeIpSetResponse(
        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE>&& /*dummy*/,
        const vhal_2_0::VehiclePropValue& requestedPropValue) {
    // Include FunctionAvailability Status
    return {static_cast<vsomeip::byte_t>(toConvApiStatus(requestedPropValue.status))};
}

template <>
std::vector<vsomeip::byte_t> ClimateCtrlService::CreateSomeIpNotification(
        SomeIpProp<REMOTECTRL_CLIMATECTRL_EVENT_ID_TEMPERATURECHANGED>&& /*dummy*/,
        const vhal_2_0::VehiclePropValue& requestedPropValue) {
    auto row_pos_pair = toRowAndPosition(requestedPropValue.areaId);

    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(toConvApiStatus(requestedPropValue.status)),
                                              static_cast<vsomeip::byte_t>(row_pos_pair.first),
                                              static_cast<vsomeip::byte_t>(row_pos_pair.second)};

    for (const auto& val : requestedPropValue.value.floatValues) {
        int temperature = (val - 16.5f) * 2;
        payload_data.push_back(static_cast<vsomeip::byte_t>(temperature));
    }

    return payload_data;
}

template <>
vhal_2_0::VehiclePropValue ClimateCtrlService::VhalGetPropertyReq(
        const SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE>& /*dummy*/,
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const char* method_name = "GET_TEMPERATURE";

    if (msg_payload->get_length() != 2) {
        throw RemoteCtrlMsgLengthError(method_name, 2, msg_payload->get_length());
    }

    vhal_2_0::VehiclePropValue prop_value{static_cast<int32_t>(vhal_2_0::VehicleProperty::HVAC_TEMPERATURE_SET),
                                          std::chrono::high_resolution_clock::now().time_since_epoch().count(),
                                          static_cast<int32_t>(vhal_2_0::VehicleAreaZone::ROW_4_RIGHT),  // dummy
                                          vhal_2_0::VehiclePropertyStatus::AVAILABLE,
                                          {}};

    const auto row = msg_payload->get_data()[0];
    const auto pos = msg_payload->get_data()[1];

    ValidateRow(method_name, row);
    ValidateSeat(method_name, pos);

    prop_value.areaId = static_cast<int32_t>(toVhalAreaZone(row, pos));

    ALOGD("Get temperature for row %u and position %u, request received for VhalAreaZone %d",
          row,
          pos,
          prop_value.areaId);

    return prop_value;
}

template <>
vhal_2_0::VehiclePropValue ClimateCtrlService::VhalSetPropertyReq(
        const SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE>& /*dummy*/,
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const char* method_name = "SET_TEMPERATURE";

    if (msg_payload->get_length() != 3) {
        throw RemoteCtrlMsgLengthError(method_name, 3, msg_payload->get_length());
    }

    vhal_2_0::VehiclePropValue prop_value{static_cast<int32_t>(vhal_2_0::VehicleProperty::HVAC_TEMPERATURE_SET),
                                          std::chrono::high_resolution_clock::now().time_since_epoch().count(),
                                          static_cast<int32_t>(vhal_2_0::VehicleAreaZone::ROW_4_RIGHT),  // dummy value
                                          vhal_2_0::VehiclePropertyStatus::AVAILABLE,
                                          {}};

    const auto row = msg_payload->get_data()[0];
    const auto pos = msg_payload->get_data()[1];
    const auto temperature = msg_payload->get_data()[2];

    ValidateRow(method_name, row);
    ValidateSeat(method_name, pos);
    ValidateRequstedTemperature(method_name, temperature);

    float offset_temperature = 16.5f + temperature / 2.0f;

    prop_value.areaId = static_cast<int32_t>(toVhalAreaZone(row, pos));
    prop_value.value.floatValues = android::hardware::hidl_vec<float>{offset_temperature};

    ALOGD("Set temperature to %u for row %u and position %u, request received for VhalAreaZone %d",
          temperature,
          row,
          pos,
          prop_value.areaId);

    return prop_value;
}
