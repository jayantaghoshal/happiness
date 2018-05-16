/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "media_ctrl_service.h"
#include "convapi_signals_def.h"

#include <cassert>
#include <memory>
#include <utility>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_MediaCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl::remotemediactrl;

using ::android::hardware::Return;
using ::android::hardware::Void;

namespace {
constexpr uint64_t binder_cookie = 0x80086U;
}  // namespace

Return<void> MediaCtrlService::registerRemoteCtrlPropertyHandler(
        const ::android::sp<hidl_remotectrl::IRemoteCtrlProperty>& handler) {
    ALOGD("%s", __func__);

    // TODO (Abhi): validate sender is a system java service
    {
        std::lock_guard<std::mutex> lock(guard_);
        ALOGW_IF(nullptr != system_service_handler_.get(), "Re-registration for binder RemoteCtrlHandler received");
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

Return<void> MediaCtrlService::unregisterRemoteCtrlPropertyHandler(
        const ::android::sp<hidl_remotectrl::IRemoteCtrlProperty>& handler) {
    ALOGD("%s", __func__);

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

Return<void> MediaCtrlService::sendSetPropertyResp(uint32_t requestIdentifier,
                                                   const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    ALOGD("%s: (0x%08X)", __func__, requestIdentifier);

    try {
        const auto prop = std::make_unique<RemoteCtrlHalProp<static_cast<int>(
                hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_MEDIA_STREAM_CONTROL)>>();
        if (prop) {
            std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>> payload_data =
                    prop->CreateSomeIpResponse(MessageType::SET, propValue);
            SendResponse(requestIdentifier, vsomeip::return_code_e::E_OK, std::move(payload_data.second));
        } else {
            ALOGW("%s: Unhandled Prop [%d]", __func__, propValue.prop);
        }
    } catch (const RemoteCtrlError& e) {
        ALOGE("%s", e.what());
    }

    return Void();
}

Return<void> MediaCtrlService::notifyPropertyChanged(const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    ALOGD("%s", __func__);

    try {
        const auto prop = std::make_unique<RemoteCtrlHalProp<static_cast<int>(
                hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_MEDIA_STREAM_CONTROL)>>();
        if (prop) {
            std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>> payload_data =
                    prop->CreateSomeIpResponse(MessageType::NOTIFICATION, propValue);
            SendNotification(payload_data.first, std::move(payload_data.second));
        } else {
            ALOGW("%s: Unhandled Prop [%d]", __func__, propValue.prop);
        }
    } catch (const RemoteCtrlError& e) {
        ALOGE("%s", e.what());
    }

    return Void();
}

void MediaCtrlService::serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
    if (cookie != binder_cookie) {
        return;
    }

    ALOGI("%s", __func__);

    {
        std::lock_guard<std::mutex> lock(guard_);
        system_service_handler_.clear();
    }
    StopOffer();
}

void MediaCtrlService::OnStateChange(vsomeip::state_type_e state) {
    if (state == vsomeip::state_type_e::ST_REGISTERED) {
        ALOGD("Conv. API media control service is now registered");
        {
            std::lock_guard<std::mutex> lock(guard_);
            if (nullptr == system_service_handler_.get()) {
                ALOGD("Java service is not registered yet. SOME/IP service offer postponed..");
                return;
            }
        }
        StartOffer();
    } else {
        ALOGD("Conv. API media control service is deregistered");
    }
}

bool MediaCtrlService::OnMessageReceive(const std::shared_ptr<vsomeip::message>& message) {
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (nullptr == system_service_handler_.get()) {
            ALOGE("VSOMEIP messages received from remote when java service is not yet registered");
            return false;
        }
    }

    try {
        const auto prop_obj = std::make_unique<RemoteProp<REMOTECTRL_MEDIACTRL_METHOD_ID_SETMEDIAPLAYERPLAYBACK>>();
        if (prop_obj) {
            const auto prop_value = prop_obj->RemoteCtrlHalPropertyReq(message->get_payload());

            if (prop_value.first == MessageType::SET) {
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->setProperty(message->get_request(), prop_value.second);
                }
            }
        } else {
            ALOGW("Unhandled message 0x%04X", message->get_method());
            return false;
        }
    } catch (const RemoteCtrlError& e) {
        ALOGW("%s. Request ignored", e.what());
        return false;
    }
    return true;
}
