/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "convapi_signals_def.h"
#include "general_settings_ctrl_service.h"

#include <utility>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_GeneralCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl;

template <>
std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>>
RemoteCtrlHalProp<static_cast<int32_t>(hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_CSD_ON)>::
        CreateSomeIpResponse(const MessageType& type, const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    if (MessageType::GET == type) {
        return {REMOTECTRL_GENERALSETTINGS_METHOD_ID_GET_CSD_STATE, GetCSDState().PackResponse(propValue)};
    } else if (MessageType::SET == type) {
        return {REMOTECTRL_GENERALSETTINGS_METHOD_ID_SET_CSD_STATE, SetCSDState().PackResponse(propValue)};
    }
    return {REMOTECTRL_GENERALSETTINGS_EVENT_ID_CSD_STATECHANGED, NotifyCSDState().PackNotification(propValue)};
}

template <>
std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_GENERALSETTINGS_METHOD_ID_GET_CSD_STATE>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = GetCSDState().UnpackRequest(msg_payload);

    ALOGD("Get CSD State received");

    return {MessageType::GET, prop_value};
}

template <>
std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_GENERALSETTINGS_METHOD_ID_SET_CSD_STATE>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = SetCSDState().UnpackRequest(msg_payload);

    ALOGD("Set CSD State to %u. Request received", msg_payload->get_data()[0]);

    return {MessageType::SET, prop_value};
}
