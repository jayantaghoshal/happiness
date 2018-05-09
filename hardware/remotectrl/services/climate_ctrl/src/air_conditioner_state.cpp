/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "climate_ctrl_service.h"
#include "convapi_signals_def.h"

#include <utility>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_ClimateCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl;

template <>
std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>>
RemoteCtrlHalProp<static_cast<int32_t>(hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_AC_ON)>::
        CreateSomeIpResponse(const MessageType& type, const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    if (MessageType::GET == type) {
        return {REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE, GetACState().PackResponse(propValue)};
    } else if (MessageType::SET == type) {
        return {REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE, SetACState().PackResponse(propValue)};
    }
    return {REMOTECTRL_CLIMATECTRL_EVENT_ID_AC_STATECHANGED, NotifyACState().PackNotification(propValue)};
}

template <>
std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = GetACState().UnpackRequest(msg_payload);

    ALOGD("Get AC State received");

    return {MessageType::GET, prop_value};
}

template <>
std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = SetACState().UnpackRequest(msg_payload);

    ALOGD("Set AC State to %u. Request received", msg_payload->get_data()[0]);

    return {MessageType::SET, prop_value};
}
