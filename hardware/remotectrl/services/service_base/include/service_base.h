/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <vsomeip/vsomeip.hpp>

#include "service_info.h"

namespace vcc {
namespace remotectrl {
namespace servicebase {

class ServiceBase {
  public:
    explicit ServiceBase(const ServiceInfo& service_info);
    virtual ~ServiceBase();

    void StartService();
    void StopService();

    void StartOffer();
    void StopOffer();

    void SendNotification(vsomeip::event_t event_id, const std::vector<vsomeip::byte_t>&& payload_data);
    void SendResponse(vsomeip::request_t request_id,
                      const vsomeip::return_code_e& status,
                      const std::vector<vsomeip::byte_t>&& payload_data = {});

  protected:
    virtual bool OnMessageReceive(const std::shared_ptr<vsomeip::message>& message) = 0;

    virtual void OnStateChange(vsomeip::state_type_e state) = 0;

    vsomeip::state_type_e state_ = vsomeip::state_type_e::ST_DEREGISTERED;

  private:
    struct Transactions final {
        Transactions(vsomeip::request_t requestId, const std::shared_ptr<vsomeip::message>& message)
            : request_id_(requestId), message_(message) {}

        vsomeip::request_t request_id_{0U};
        std::shared_ptr<vsomeip::message> message_;
    };

    std::vector<Transactions>::const_iterator GetTransactionForRequestId(vsomeip::request_t request_id) const {
        std::lock_guard<std::mutex> lock{message_tracker_mutex_};
        return std::find_if(message_tracker_.cbegin(), message_tracker_.cend(), [request_id](const Transactions& tr) {
            return tr.request_id_ == request_id;
        });
    }

    void RegistrationStateChangedHook(vsomeip::state_type_e state);

    void MessageReceivedHook(const std::shared_ptr<vsomeip::message>& message);

    bool ValidateMessageHeader(const std::shared_ptr<vsomeip::message>&);

    const std::shared_ptr<vsomeip::runtime> vsomeip_runtime_ = vsomeip::runtime::get();
    const std::shared_ptr<vsomeip::application> vsomeip_appl_;

    mutable std::mutex message_tracker_mutex_;
    // TODO (Abhi): This is a basic message_tracker. This needs to be extended to track ACKs, WFA and WFR
    std::vector<Transactions> message_tracker_;
    const ServiceInfo service_info_;
    std::thread service_launcher_thread_;
};

}  // namespace servicebase
}  // namespace remotectrl
}  // namespace vcc
