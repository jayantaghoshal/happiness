/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "client_info.h"

#include <vsomeip/vsomeip.hpp>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace vcc {
namespace remotectrlsim {

class RemoteCtrlClientBase {
  public:
    RemoteCtrlClientBase(const std::string& config_path, const vcc::remotectrlsim::ClientInfo& client_info);
    virtual ~RemoteCtrlClientBase();

    void EnqueueRequest(std::function<void()>&& request);

    bool WaitForResponse(const std::chrono::milliseconds& period);

    bool WaitForNotification(const std::chrono::milliseconds& period);

    void SendRequest(const uint16_t& method_id, const std::vector<vsomeip::byte_t>& payload_data);

    void StartClient();
    void StopClient();

  protected:
    virtual void OnStateChange(vsomeip::state_type_e state);

    virtual void OnAvailabilityChanged(vsomeip::service_t /*service*/,
                                       vsomeip::instance_t /*instance*/,
                                       bool is_available);

    virtual void OnMessageReceived(const std::shared_ptr<vsomeip::message>& message);

    std::shared_ptr<vsomeip::message> received_reply_;
    std::shared_ptr<vsomeip::message> received_notification_;

  private:
    const int env_set_;
    const std::shared_ptr<vsomeip::runtime> runtime_;
    const std::shared_ptr<vsomeip::application> application_;
    const ClientInfo client_info_;
    std::thread client_thread_;

    std::atomic<bool> is_available_;
    std::function<void()> enqueued_request_;

    bool response_received_ = false;
    std::mutex response_mtx_;
    std::condition_variable response_cv_;

    bool notification_received_ = false;
    std::mutex notification_mtx_;
    std::condition_variable notification_cv_;
};

}  // namespace remotectrlsim
}  // namespace vcc
