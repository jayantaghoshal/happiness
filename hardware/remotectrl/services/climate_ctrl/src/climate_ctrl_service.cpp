/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "climate_ctrl_service.h"
#include "convapi_signals_def.h"

#include <memory>
#include <utility>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_ClimateCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl::remoteclimatectrl;
using namespace vcc::remotectrl;

using ::android::hardware::Return;
using ::android::hardware::Void;

namespace {

constexpr uint64_t binder_cookie = 0x80085U;

template <int I>
std::unique_ptr<BaseProp> createObject(const uint16_t& method_id) {
    if (remotectrl_service_methods[I] == method_id) {
        return std::make_unique<RemoteProp<remotectrl_service_methods[I]>>();
    }
    return createObject<I + 1>(method_id);
}

template <>
std::unique_ptr<BaseProp> createObject<remotectrl_service_methods.size()>(const uint16_t& /*method_id*/) {
    return std::unique_ptr<BaseProp>();
}

template <int I>
std::unique_ptr<BaseProp> createObject(const hidl_remotectrl::RemoteCtrlHalProperty& prop) {
    if (remotectrl_hal_prop_array[I] == prop) {
        return std::make_unique<RemoteCtrlHalProp<static_cast<int>(remotectrl_hal_prop_array[I])>>();
    }
    return createObject<I + 1>(prop);
}

template <>
std::unique_ptr<BaseProp> createObject<remotectrl_hal_prop_array.size()>(
        const hidl_remotectrl::RemoteCtrlHalProperty& /*prop*/) {
    return std::unique_ptr<BaseProp>();
}

}  // namespace

Return<void> ClimateCtrlService::registerRemoteCtrlPropertyHandler(
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

Return<void> ClimateCtrlService::unregisterRemoteCtrlPropertyHandler(
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

Return<void> ClimateCtrlService::sendGetPropertyResp(uint32_t requestIdentifier,
                                                     const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    ALOGD("%s: (0x%08X)", __func__, requestIdentifier);

    try {
        const auto prop = createObject<0>(propValue.prop);
        if (prop) {
            std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>> payload_data =
                    prop->CreateSomeIpResponse(MessageType::GET, propValue);
            SendResponse(requestIdentifier, vsomeip::return_code_e::E_OK, std::move(payload_data.second));
        } else {
            ALOGW("%s: Unhandled Prop [%d]", __func__, propValue.prop);
        }
    } catch (const RemoteCtrlError& e) {
        ALOGE("%s", e.what());
    }

    return Void();
}

Return<void> ClimateCtrlService::sendSetPropertyResp(uint32_t requestIdentifier,
                                                     const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    ALOGD("%s: (0x%08X)", __func__, requestIdentifier);

    try {
        const auto prop = createObject<0>(propValue.prop);
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

Return<void> ClimateCtrlService::notifyPropertyChanged(const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) {
    ALOGD("%s", __func__);

    try {
        const auto prop = createObject<0>(propValue.prop);
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

void ClimateCtrlService::serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
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

void ClimateCtrlService::OnStateChange(vsomeip::state_type_e state) {
    if (state == vsomeip::state_type_e::ST_REGISTERED) {
        ALOGD("Conv. API climate control service is now registered");
        {
            std::lock_guard<std::mutex> lock(guard_);
            if (nullptr == system_service_handler_.get()) {
                ALOGD("Java service is not registered yet. SOME/IP service offer postponed..");
                return;
            }
        }
        StartOffer();
    } else {
        ALOGD("Conv. API climate control service is deregistered");
    }
}

bool ClimateCtrlService::OnMessageReceive(const std::shared_ptr<vsomeip::message>& message) {
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (nullptr == system_service_handler_.get()) {
            ALOGE("VSOMEIP messages received from remote when java service is not yet registered");
            return false;
        }
    }

    try {
        const auto prop_obj = createObject<0>(message->get_method());
        if (prop_obj) {
            const auto prop_value = prop_obj->RemoteCtrlHalPropertyReq(message->get_payload());

            if (prop_value.first == MessageType::GET) {
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->getProperty(message->get_request(), prop_value.second);
                }
            } else if (prop_value.first == MessageType::SET) {
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
