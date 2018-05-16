/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "convapi_signals_def.h"
#include "media_ctrl_service.h"

#include <cassert>
#include <utility>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_MediaCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl;

template <>
std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>>
RemoteCtrlHalProp<static_cast<int32_t>(hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_MEDIA_STREAM_CONTROL)>::
        CreateSomeIpResponse(const MessageType& type, const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    // Include FunctionAvailability Status
    if (MessageType::GET == type) {
        assert(0);
    } else if (MessageType::SET == type) {
        return {REMOTECTRL_MEDIACTRL_METHOD_ID_SETMEDIAPLAYERPLAYBACK,
                SetStreamPlayBackStatus().PackResponse(propValue)};
    }
    return {REMOTECTRL_MEDIACTRL_EVENT_ID_MEDIAPLAYERPLAYBACKSTATUS,
            NotifyStreamPlayBackStatus().PackNotification(propValue)};
}

template <>
std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue>
RemoteProp<REMOTECTRL_MEDIACTRL_METHOD_ID_SETMEDIAPLAYERPLAYBACK>::RemoteCtrlHalPropertyReq(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    const auto prop_value = SetStreamPlayBackStatus().UnpackRequest(msg_payload);

    ALOGD("Set playback status to %u. Request received", msg_payload->get_data()[0]);

    return {MessageType::SET, prop_value};
}
