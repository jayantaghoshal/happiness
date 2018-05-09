/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "service_base.h"
#include <set>
#include <stdexcept>
#include <thread>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_ServiceBase"
#include <cutils/log.h>

namespace vcc {
namespace remotectrl {
namespace servicebase {

ServiceBase::ServiceBase(const ServiceInfo& service_info)
    : vsomeip_appl_(vsomeip_runtime_->create_application(service_info.service_name_)), service_info_(service_info) {
    // Initialize service application
    if (!vsomeip_appl_->init()) {
        throw std::runtime_error("Couldn't initialize Convenience API service");
    }

    vsomeip_appl_->register_state_handler(
            std::bind(&ServiceBase::RegistrationStateChangedHook, this, std::placeholders::_1));

    for (const auto& method : service_info_.methods_) {
        // register messgage handler for get_volume signal
        vsomeip_appl_->register_message_handler(
                service_info_.service_id_,
                service_info_.instance_id_,
                method,
                std::bind(&ServiceBase::MessageReceivedHook, this, std::placeholders::_1));
    }
}

ServiceBase::~ServiceBase() {
    StopOffer();
    StopService();
    service_launcher_thread_.join();
}

void ServiceBase::StartService() {
    ALOGD("Starting service %s", service_info_.service_name_);
    service_launcher_thread_ = std::thread([this]() { vsomeip_appl_->start(); });
}

void ServiceBase::StopService() {
    ALOGD("Stopping service %s", service_info_.service_name_);
    vsomeip_appl_->unregister_state_handler();
    for (const auto& method : service_info_.methods_) {
        // register messgage handler for get_volume signal
        vsomeip_appl_->unregister_message_handler(service_info_.service_id_, service_info_.instance_id_, method);
    }
    vsomeip_appl_->stop();
}

void ServiceBase::StartOffer() {
    ALOGD("Start Offer for %s: ServiceId[0x%04X] InstanceId[0x%04X]",
          service_info_.service_name_,
          service_info_.service_id_,
          service_info_.instance_id_);

    vsomeip_appl_->offer_service(service_info_.service_id_, service_info_.instance_id_);

    for (const auto& event : service_info_.events_) {
        vsomeip_appl_->offer_event(service_info_.service_id_,
                                   service_info_.instance_id_,
                                   event,
                                   std::set<vsomeip::eventgroup_t>({service_info_.eventgroup_id_}),
                                   false);
    }
}

void ServiceBase::StopOffer() {
    ALOGD("Stop Offer for %s: ServiceId[0x%04X] InstanceId[0x%04X]",
          service_info_.service_name_,
          service_info_.service_id_,
          service_info_.instance_id_);

    for (const auto& event : service_info_.events_) {
        vsomeip_appl_->stop_offer_event(service_info_.service_id_, service_info_.instance_id_, event);
    }

    vsomeip_appl_->stop_offer_service(service_info_.service_id_, service_info_.instance_id_);
}

void ServiceBase::SendNotification(vsomeip::event_t event_id, const std::vector<vsomeip::byte_t>&& payload_data) {
    ALOGD("SendNotification: service[%s] ServiceId[0x%04X] InstanceId[0x%04X], EventId[0x%04X]",
          service_info_.service_name_,
          service_info_.service_id_,
          service_info_.instance_id_,
          event_id);

    std::shared_ptr<vsomeip::payload> pl = vsomeip_runtime_->create_payload();
    pl->set_data(payload_data);

    vsomeip_appl_->notify(service_info_.service_id_, service_info_.instance_id_, event_id, pl);
}

void ServiceBase::SendResponse(vsomeip::request_t request_id,
                               const vsomeip::return_code_e& status,
                               const std::vector<vsomeip::byte_t>&& payload_data) {
    ALOGD("SendResponse: service[%s] ServiceId[0x%04X], InstanceId[0x%04X] requestId[0x%08X]",
          service_info_.service_name_,
          service_info_.service_id_,
          service_info_.instance_id_,
          request_id);

    const auto& message = GetTransactionForRequestId(request_id);

    if (message == message_tracker_.cend()) {
        ALOGE("Message with request_id[0x%08X] not found. Response not sent", request_id);
        return;
    }

    std::shared_ptr<vsomeip::message> response = vsomeip_runtime_->create_response(message->message_);
    response->set_return_code(status);

    std::shared_ptr<vsomeip::payload> pl = vsomeip_runtime_->create_payload();
    pl->set_data(payload_data);
    response->set_payload(pl);

    vsomeip_appl_->send(response);

    {
        std::lock_guard<std::mutex> lock{message_tracker_mutex_};
        // If service do not call SendResponse should message be removed after WFR?
        ALOGV("Removing tracking message with requestId[0x%08X]", request_id);
        message_tracker_.erase(message);
    }
}

void ServiceBase::RegistrationStateChangedHook(vsomeip::state_type_e state) {
    state_ = state;

    ALOGD("%s: %s state %d", __FUNCTION__, service_info_.service_name_, static_cast<uint8_t>(state));
    OnStateChange(state_);
}

void ServiceBase::MessageReceivedHook(const std::shared_ptr<vsomeip::message>& message) {
    if (!ValidateMessageHeader(message)) {
        return;
    }

    ALOGD("%s: %s", __FUNCTION__, service_info_.service_name_);
    // Track messages if service implementation has confirmed validity of message
    if (OnMessageReceive(message)) {
        auto request_id = message->get_request();
        std::lock_guard<std::mutex> lock{message_tracker_mutex_};
        ALOGV("Tracking message with requestId[0x%08X]", request_id);
        message_tracker_.emplace_back(request_id, message);
    }
}

bool ServiceBase::ValidateMessageHeader(const std::shared_ptr<vsomeip::message>& message) {
    std::shared_ptr<vsomeip::message> response = vsomeip_runtime_->create_response(message);

    if (message->get_message_type() != vsomeip::message_type_e::MT_REQUEST) {
        ALOGE("Invalid message type %hhu", message->get_message_type());
        response->set_return_code(vsomeip::return_code_e::E_WRONG_MESSAGE_TYPE);
        vsomeip_appl_->send(response, true);
        return false;
    }

    return true;
}

}  // namespace servicebase
}  // namespace remotectrl
}  // namespace vcc
