/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "audio_ctrl_service.h"
#include <cassert>
#include <future>
#include <vector>
#include "convapi_signals_def.h"

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_AudioCtrl"
#include <cutils/log.h>

namespace {
const uint64_t binder_cookie = 0x80085U;

// TODO (Abhi): Fix deserializer for signals based on SDB database and move these constants and related sanity checking
// as part of deserializer
const uint8_t REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME_PAYLOAD_LEN = 0x01U;
const uint8_t REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME_PAYLOAD_LEN = 0x02U;
}  // namespace

namespace vcc {
namespace remotectrl {
namespace remoteaudioctrl {

Return<void> AudioCtrlService::registerAudioControlHandler(
        const ::android::sp<hidl_remotectrl::ISystemRemoteAudioCtrl>& handler) {
    ALOGD("%s", __FUNCTION__);

    // TODO (Abhi): validate sender is a system java service
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (nullptr != system_service_handler_.get()) {
            ALOGE("Re-registration for binder RemoteCtrlHandler denied..");
            return Void();
        }
        system_service_handler_ = handler;
        system_service_handler_->linkToDeath(static_cast<::android::hardware::hidl_death_recipient*>(this),
                                             binder_cookie);
    }

    if (vsomeip::state_type_e::ST_REGISTERED != state_) {
        ALOGW("Waiting for VSOMEIP service registration");
        return Void();
    }

    StartOffer();
    return Void();
}

Return<void> AudioCtrlService::unregisterAudioControlHandler(
        const ::android::sp<hidl_remotectrl::ISystemRemoteAudioCtrl>& handler) {
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (system_service_handler_ != handler) {
            return Void();
        }
        system_service_handler_.clear();
    }
    StopOffer();
    return Void();
}

Return<void> AudioCtrlService::updateVolume(const hidl_remotectrl::VolumeData& data) {
    ALOGD("%s: sending Volume update", __FUNCTION__);

    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(data.context),
                                              static_cast<vsomeip::byte_t>(data.level)};

    SendNotification(REMOTECTRL_AUDIOCTRL_EVENT_ID_VOLUMESTATUS, std::move(payload_data));

    return Void();
}

Return<void> AudioCtrlService::sendGetVolumeResp(uint16_t requestIdentifier,
                                                 hidl_remotectrl::StatusCode statusCode,
                                                 const hidl_remotectrl::VolumeData& data) {
    ALOGD("%s: session[0x%04X]", __FUNCTION__, requestIdentifier);

    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(data.context),
                                              static_cast<vsomeip::byte_t>(data.level)};

    vsomeip::return_code_e status = (statusCode == hidl_remotectrl::StatusCode::SUCCESS)
                                            ? vsomeip::return_code_e::E_OK
                                            : vsomeip::return_code_e::E_NOT_OK;
    SendResponse(requestIdentifier, status, std::move(payload_data));
    return Void();
}

Return<void> AudioCtrlService::sendSetVolumeResp(uint16_t requestIdentifier, hidl_remotectrl::StatusCode statusCode) {
    ALOGD("%s: session[0x%04X]", __FUNCTION__, requestIdentifier);

    vsomeip::return_code_e status = (statusCode == hidl_remotectrl::StatusCode::SUCCESS)
                                            ? vsomeip::return_code_e::E_OK
                                            : vsomeip::return_code_e::E_NOT_OK;
    SendResponse(requestIdentifier, status);
    return Void();
}

void AudioCtrlService::serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
    if (cookie != binder_cookie) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(guard_);
        system_service_handler_.clear();
    }
    StopOffer();
}

void AudioCtrlService::OnStateChange(vsomeip::state_type_e state) {
    if (state == vsomeip::state_type_e::ST_REGISTERED) {
        ALOGD("Conv. API audio control service is now registered");
        {
            std::lock_guard<std::mutex> lock(guard_);
            if (nullptr == system_service_handler_.get()) {
                ALOGD("Java service is not registered yet. SOME/IP service offer postponed..");
                return;
            }
        }
        StartOffer();
    } else {
        ALOGD("Conv. API audio control service is deregistered");
    }
}

void AudioCtrlService::OnMessageReceive(const std::shared_ptr<vsomeip::message>& message) {
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (nullptr == system_service_handler_.get()) {
            ALOGE("VSOMEIP messages received from remote when java service is not yet registered");
            return;
        }
    }

    std::shared_ptr<vsomeip::payload> msg_payload = message->get_payload();
    if (message->get_method() == REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME &&
        REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME_PAYLOAD_LEN == msg_payload->get_length()) {
        ALOGD("Get volume request received session[0x%04X]", message->get_session());

        std::lock_guard<std::mutex> lock(guard_);
        system_service_handler_->getVolume(message->get_session(),
                                           static_cast<hidl_remotectrl::AudioContext>(msg_payload->get_data()[0]));
    } else if (message->get_method() == REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME &&
               REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME_PAYLOAD_LEN == msg_payload->get_length()) {
        ALOGD("Set volume to %d, request received session[0x%04X]", msg_payload->get_data()[1], message->get_session());

        std::lock_guard<std::mutex> lock(guard_);
        system_service_handler_->setVolume(message->get_session(),
                                           {static_cast<hidl_remotectrl::AudioContext>(msg_payload->get_data()[0]),
                                            static_cast<hidl_remotectrl::VolumeLevel>(msg_payload->get_data()[1])});
    }
}

}  // namespace remoteaudioctrl
}  // namespace remotectrl
}  // namespace vcc
