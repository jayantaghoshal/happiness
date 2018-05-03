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
        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AIR_DISTRIBUTION>&& /*dummy*/,
        const vhal_2_0::VehiclePropValue& requestedPropValue) {
    // Include FunctionAvailability Status
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(toConvApiStatus(requestedPropValue.status))};

    for (const auto& val : requestedPropValue.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(toConvApiFanDirection(val)));
    }

    return payload_data;
}

template <>
std::vector<vsomeip::byte_t> ClimateCtrlService::CreateSomeIpSetResponse(
        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AIR_DISTRIBUTION>&& /*dummy*/,
        const vhal_2_0::VehiclePropValue& requestedPropValue) {
    // Include FunctionAvailability Status
    return {static_cast<vsomeip::byte_t>(toConvApiStatus(requestedPropValue.status))};
}

template <>
std::vector<vsomeip::byte_t> ClimateCtrlService::CreateSomeIpNotification(
        SomeIpProp<REMOTECTRL_CLIMATECTRL_EVENT_ID_AIR_DISTRIBUTIONCHANGED>&& /*dummy*/,
        const vhal_2_0::VehiclePropValue& requestedPropValue) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(toConvApiStatus(requestedPropValue.status))};

    for (const auto& val : requestedPropValue.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(toConvApiFanDirection(val)));
    }

    return payload_data;
}

template <>
vhal_2_0::VehiclePropValue ClimateCtrlService::VhalGetPropertyReq(
        const SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AIR_DISTRIBUTION>& /*dummy*/,
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const char* method_name = "GET_FAN_DIRECTION";

    if (msg_payload->get_length() != 0) {
        throw RemoteCtrlMsgLengthError(method_name, 0, msg_payload->get_length());
    }

    vhal_2_0::VehiclePropValue prop_value{static_cast<int32_t>(vhal_2_0::VehicleProperty::HVAC_FAN_DIRECTION),
                                          std::chrono::high_resolution_clock::now().time_since_epoch().count(),
                                          static_cast<int32_t>(vhal_2_0::VehicleAreaZone::ROW_1_CENTER),
                                          vhal_2_0::VehiclePropertyStatus::AVAILABLE,
                                          {}};

    ALOGD("Get Air Distribution received");

    return prop_value;
}

template <>
vhal_2_0::VehiclePropValue ClimateCtrlService::VhalSetPropertyReq(
        const SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AIR_DISTRIBUTION>& /*dummy*/,
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const char* method_name = "SET_FAN_DIRECTION";

    if (msg_payload->get_length() != 1) {
        throw RemoteCtrlMsgLengthError(method_name, 1, msg_payload->get_length());
    }

    vhal_2_0::VehiclePropValue prop_value{static_cast<int32_t>(vhal_2_0::VehicleProperty::HVAC_FAN_DIRECTION),
                                          std::chrono::high_resolution_clock::now().time_since_epoch().count(),
                                          static_cast<int32_t>(vhal_2_0::VehicleAreaZone::ROW_1_CENTER),
                                          vhal_2_0::VehiclePropertyStatus::AVAILABLE,
                                          {}};

    const auto air_flow_state = toVhalFanDirection(msg_payload->get_data()[0]);
    prop_value.value.int32Values = android::hardware::hidl_vec<int32_t>{air_flow_state};

    ALOGD("Set Air Distribution to %u. Request received", air_flow_state);

    return prop_value;
}
