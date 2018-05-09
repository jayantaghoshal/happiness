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
RemoteCtrlHalProp<static_cast<int32_t>(hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_FAN_SPEED)>::
        CreateSomeIpResponse(const BaseProp::MessageType& type,
                             const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    if (MessageType::GET == type) {
        return {REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL, GetFanLevel().PackResponse(propValue)};
    } else if (MessageType::SET == type) {
        return {REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL, SetFanLevel().PackResponse(propValue)};
    }
    return {REMOTECTRL_CLIMATECTRL_EVENT_ID_FANLEVELCHANGED, NotifyFanLevel().PackNotification(propValue)};
}

template <>
std::pair<BaseProp::MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = GetFanLevel().UnpackRequest(msg_payload);

    ALOGD("Get fan level for row %u and position %u, request received for VhalAreaZone %d",
          msg_payload->get_data()[0],
          msg_payload->get_data()[1],
          prop_value.areaId);

    return {BaseProp::MessageType::GET, prop_value};
}

template <>
std::pair<BaseProp::MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = SetFanLevel().UnpackRequest(msg_payload);

    ALOGD("Set fan level to %u for row %u and position %u, request received for VhalAreaZone %d",
          msg_payload->get_data()[2],
          msg_payload->get_data()[0],
          msg_payload->get_data()[1],
          prop_value.areaId);

    return {BaseProp::MessageType::SET, prop_value};
}
