/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "climate_ctrl_service.h"
#include "convapi_signals_def.h"
#include "vhal_conv_mappings.h"

#include <cassert>
#include <chrono>
#include <memory>
#include <utility>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_ClimateCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl::remoteclimatectrl;
using namespace vhal_2_0;

using ::android::hardware::Return;
using ::android::hardware::Void;

namespace {
constexpr uint64_t binder_cookie = 0x80085U;
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

Return<void> ClimateCtrlService::sendGetPropertyResp(uint16_t requestIdentifier,
                                                     const vhal_2_0::VehiclePropValue& requestedPropValue) {
    ALOGD("%s: (0x%04X)", __func__, requestIdentifier);

    std::vector<vsomeip::byte_t> payload_data;
    try {
        switch (static_cast<vhal_2_0::VehicleProperty>(requestedPropValue.prop)) {
            case vhal_2_0::VehicleProperty::HVAC_FAN_SPEED: {
                payload_data = CreateSomeIpResponse(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL>(),
                                                    requestedPropValue);
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_MAX_DEFROST_ON: {
                payload_data = CreateSomeIpResponse(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE>(), requestedPropValue);
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_AC_ON: {
                payload_data = CreateSomeIpResponse(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE>(),
                                                    requestedPropValue);
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_TEMPERATURE_SET: {
                payload_data = CreateSomeIpResponse(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE>(),
                                                    requestedPropValue);
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_FAN_DIRECTION: {
                payload_data = CreateSomeIpResponse(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AIR_DISTRIBUTION>(),
                                                    requestedPropValue);
                break;
            }
            default: {
                ALOGW("%s: Unhandled Prop [%d]", __func__, requestedPropValue.prop);
                return Void();
            }
        }
    } catch (const RemoteCtrlError& e) {
        ALOGE("%s", e.what());
        return Void();
    }

    SendResponse(requestIdentifier, vsomeip::return_code_e::E_OK, std::move(payload_data));
    return Void();
}

Return<void> ClimateCtrlService::sendSetPropertyResp(uint16_t requestIdentifier,
                                                     const vhal_2_0::VehiclePropValue& requestedPropValue) {
    ALOGD("%s: (0x%04X)", __func__, requestIdentifier);

    std::vector<vsomeip::byte_t> payload_data;
    try {
        switch (static_cast<vhal_2_0::VehicleProperty>(requestedPropValue.prop)) {
            case vhal_2_0::VehicleProperty::HVAC_FAN_SPEED: {
                payload_data = CreateSomeIpSetResponse(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL>(),
                                                       requestedPropValue);
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_MAX_DEFROST_ON: {
                payload_data = CreateSomeIpSetResponse(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE>(), requestedPropValue);
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_AC_ON: {
                payload_data = CreateSomeIpSetResponse(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE>(),
                                                       requestedPropValue);
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_TEMPERATURE_SET: {
                payload_data = CreateSomeIpSetResponse(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE>(),
                                                       requestedPropValue);
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_FAN_DIRECTION: {
                payload_data = CreateSomeIpSetResponse(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AIR_DISTRIBUTION>(), requestedPropValue);
                break;
            }
            default: {
                ALOGW("%s: Unhandled Prop [%d]", __func__, requestedPropValue.prop);
                return Void();
            }
        }
    } catch (const RemoteCtrlError& e) {
        ALOGE("%s", e.what());
        return Void();
    }

    SendResponse(requestIdentifier, vsomeip::return_code_e::E_OK, std::move(payload_data));
    return Void();
}

Return<void> ClimateCtrlService::notifyPropertyChanged(const vhal_2_0::VehiclePropValue& propValue) {
    ALOGD("%s", __func__);

    std::vector<vsomeip::byte_t> payload_data;
    uint16_t event_id;
    try {
        switch (static_cast<vhal_2_0::VehicleProperty>(propValue.prop)) {
            case vhal_2_0::VehicleProperty::HVAC_FAN_SPEED: {
                payload_data = CreateSomeIpNotification(SomeIpProp<REMOTECTRL_CLIMATECTRL_EVENT_ID_FANLEVELCHANGED>(),
                                                        propValue);
                event_id = REMOTECTRL_CLIMATECTRL_EVENT_ID_FANLEVELCHANGED;
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_MAX_DEFROST_ON: {
                payload_data = CreateSomeIpNotification(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_EVENT_ID_MAX_DEFROSTER_STATECHANGED>(), propValue);
                event_id = REMOTECTRL_CLIMATECTRL_EVENT_ID_MAX_DEFROSTER_STATECHANGED;
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_AC_ON: {
                payload_data = CreateSomeIpNotification(SomeIpProp<REMOTECTRL_CLIMATECTRL_EVENT_ID_AC_STATECHANGED>(),
                                                        propValue);
                event_id = REMOTECTRL_CLIMATECTRL_EVENT_ID_AC_STATECHANGED;
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_TEMPERATURE_SET: {
                payload_data = CreateSomeIpNotification(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_EVENT_ID_TEMPERATURECHANGED>(), propValue);
                event_id = REMOTECTRL_CLIMATECTRL_EVENT_ID_TEMPERATURECHANGED;
                break;
            }
            case vhal_2_0::VehicleProperty::HVAC_FAN_DIRECTION: {
                payload_data = CreateSomeIpNotification(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_EVENT_ID_AIR_DISTRIBUTIONCHANGED>(), propValue);
                event_id = REMOTECTRL_CLIMATECTRL_EVENT_ID_AIR_DISTRIBUTIONCHANGED;
                break;
            }
            default: {
                ALOGW("%s: Unhandled Prop [%d]", __func__, propValue.prop);
                return Void();
            }
        }
    } catch (const RemoteCtrlError& e) {
        ALOGE("%s", e.what());
        return Void();
    }

    SendNotification(event_id, std::move(payload_data));
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

void ClimateCtrlService::OnMessageReceive(const std::shared_ptr<vsomeip::message>& message) {
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (nullptr == system_service_handler_.get()) {
            ALOGE("VSOMEIP messages received from remote when java service is not yet registered");
            return;
        }
    }

    std::shared_ptr<vsomeip::payload> msg_payload = message->get_payload();

    try {
        switch (message->get_method()) {
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL: {
                const auto& prop_value =
                        VhalGetPropertyReq(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->getProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL: {
                const auto& prop_value =
                        VhalSetPropertyReq(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->setProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE: {
                const auto& prop_value = VhalGetPropertyReq(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->getProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE: {
                const auto& prop_value = VhalSetPropertyReq(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->setProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE: {
                const auto& prop_value =
                        VhalGetPropertyReq(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->getProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE: {
                const auto& prop_value =
                        VhalSetPropertyReq(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->setProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE: {
                const auto& prop_value =
                        VhalGetPropertyReq(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->getProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE: {
                const auto& prop_value =
                        VhalSetPropertyReq(SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->setProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AIR_DISTRIBUTION: {
                const auto& prop_value = VhalGetPropertyReq(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AIR_DISTRIBUTION>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->getProperty(message->get_session(), prop_value);
                }
                break;
            }
            case REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AIR_DISTRIBUTION: {
                const auto& prop_value = VhalSetPropertyReq(
                        SomeIpProp<REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AIR_DISTRIBUTION>(), msg_payload);
                std::lock_guard<std::mutex> lock(guard_);
                if (nullptr != system_service_handler_.get()) {
                    system_service_handler_->setProperty(message->get_session(), prop_value);
                }
                break;
            }
            default: {
                ALOGW("Unhandled message 0x%04X", message->get_method());
                break;
            }
        }
    } catch (const RemoteCtrlError& e) {
        ALOGW("%s. Request ignored", e.what());
        return;
    }
}
