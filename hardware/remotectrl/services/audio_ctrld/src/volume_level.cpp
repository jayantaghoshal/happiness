/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "audio_ctrl_service.h"
#include "convapi_signals_def.h"

#include <utility>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_AudioCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl;

template <>
std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>>
RemoteCtrlHalProp<static_cast<int32_t>(hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_MEDIA_VOLUME)>::
        CreateSomeIpResponse(const MessageType& type, const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    if (MessageType::GET == type) {
        return {REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, GetVolume().PackResponse(propValue)};
    } else if (MessageType::SET == type) {
        return {REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME, SetVolume().PackResponse(propValue)};
    }
    return {REMOTECTRL_AUDIOCTRL_EVENT_ID_VOLUMESTATUS, NotifyVolume().PackNotification(propValue)};
}

template <>
std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = GetVolume().UnpackRequest(msg_payload);

    ALOGD("Get Volume request received");

    return {MessageType::GET, prop_value};
}

template <>
std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = SetVolume().UnpackRequest(msg_payload);

    ALOGD("Set Volume level to %u request received", msg_payload->get_data()[0]);

    return {MessageType::SET, prop_value};
}
