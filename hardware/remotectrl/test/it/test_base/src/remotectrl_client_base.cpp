/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "remotectrl_client_base.h"

#include <iostream>

using namespace vcc::remotectrlsim;

RemoteCtrlClientBase::RemoteCtrlClientBase(const std::string& config_path, const ClientInfo& client_info)
    : env_set_(setenv("VSOMEIP_CONFIGURATION", config_path.c_str(), 1 /*override*/)),
      runtime_(vsomeip::runtime::get()),
      application_(runtime_->create_application(client_info.app_name_)),
      client_info_(client_info) {
    if (!application_->init()) {
        throw std::runtime_error("Failed to initialize VSOME/IP application ");
    }

    application_->register_state_handler(std::bind(&RemoteCtrlClientBase::OnStateChange, this, std::placeholders::_1));

    application_->register_availability_handler(client_info_.service_id_,
                                                client_info_.instance_id_,
                                                std::bind(&RemoteCtrlClientBase::OnAvailabilityChanged,
                                                          this,
                                                          std::placeholders::_1,
                                                          std::placeholders::_2,
                                                          std::placeholders::_3));

    application_->register_message_handler(
            client_info_.service_id_,
            client_info_.instance_id_,
            vsomeip::ANY_METHOD,
            std::bind(&RemoteCtrlClientBase::OnMessageReceived, this, std::placeholders::_1));
}

RemoteCtrlClientBase::~RemoteCtrlClientBase() { StopClient(); }

void RemoteCtrlClientBase::EnqueueRequest(std::function<void()>&& request) {
    enqueued_request_ = std::move(request);
    if (is_available_.load()) {
        enqueued_request_();
        return;
    }
}

bool RemoteCtrlClientBase::WaitForResponse(const std::chrono::milliseconds& period) {
    std::unique_lock<std::mutex> lk(response_mtx_);
    auto status = response_cv_.wait_for(lk, period, [this]() { return response_received_; });
    response_received_ = false;
    return status;
}

bool RemoteCtrlClientBase::WaitForNotification(const std::chrono::milliseconds& period) {
    std::unique_lock<std::mutex> lk(notification_mtx_);
    auto status = notification_cv_.wait_for(lk, period, [this]() { return notification_received_; });
    notification_received_ = false;
    return status;
}

void RemoteCtrlClientBase::StartClient() {
    client_thread_ = std::thread([this]() { application_->start(); });
}

void RemoteCtrlClientBase::StopClient() {
    application_->clear_all_handler();
    application_->release_service(client_info_.service_id_, client_info_.instance_id_);
    application_->stop();

    if (client_thread_.joinable()) {
        client_thread_.join();
    }
}

void RemoteCtrlClientBase::OnStateChange(vsomeip::state_type_e state) {
    if (state == vsomeip::state_type_e::ST_REGISTERED) {
        application_->request_service(client_info_.service_id_, client_info_.instance_id_);
        for (const vsomeip::event_t& e : client_info_.events_) {
            application_->request_event(
                    client_info_.service_id_, client_info_.instance_id_, e, {client_info_.eventgroup_id_}, false);
        }
        application_->subscribe(client_info_.service_id_, client_info_.instance_id_, client_info_.eventgroup_id_);
    }
}

void RemoteCtrlClientBase::OnAvailabilityChanged(vsomeip::service_t /*service*/,
                                                 vsomeip::instance_t /*instance*/,
                                                 bool is_available) {
    is_available_.store(is_available);
    if (is_available) {
        enqueued_request_();
    }
}

void RemoteCtrlClientBase::OnMessageReceived(const std::shared_ptr<vsomeip::message>& message) {
    switch (message->get_message_type()) {
        case vsomeip::message_type_e::MT_RESPONSE: {
            std::cout << "Received Response Message" << std::endl;
            received_reply_ = message;
            response_received_ = true;
            response_cv_.notify_all();
            break;
        }
        case vsomeip::message_type_e::MT_NOTIFICATION: {
            std::cout << "Received Notification Message" << std::endl;
            received_notification_ = message;
            notification_received_ = true;
            notification_cv_.notify_all();
            break;
        }
        default: {
            // Dont care about the others, yet...
            std::cout << "Received Unknown Message [" << unsigned(message->get_message_type()) << "]" << std::endl;
        }
    }
}

void RemoteCtrlClientBase::SendRequest(const uint16_t& method_id, const std::vector<vsomeip::byte_t>& payload_data) {
    std::shared_ptr<vsomeip::message> request = runtime_->create_request();
    request->set_service(client_info_.service_id_);
    request->set_instance(client_info_.instance_id_);
    request->set_method(method_id);

    std::shared_ptr<vsomeip::payload> pl = runtime_->create_payload();
    pl->set_data(payload_data);
    request->set_payload(pl);
    application_->send(request);
}
