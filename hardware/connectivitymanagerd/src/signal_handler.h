/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "signal_handler_interface.h"

#include <IDispatcher.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IMessageCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IResponseCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>
#include <mutex>

#include "VccIpCmdApi.h"

extern "C" {
#include <pl/asn_base/asn_base.h>
}

namespace vcc {
namespace conman {

namespace conman_hal_v1_0 = ::vendor::volvocars::hardware::connectivitymanager::V1_0;
namespace vehicle_com = ::vendor::volvocars::hardware::vehiclecom::V1_0;
namespace vehicle = ::vendor::volvocars::hardware::common::V1_0;
namespace android_hidl = ::android::hidl::base::V1_0;

class SignalHandler : public ISignalHandler,
                      private vehicle_com::IMessageCallback,
                      private vehicle_com::IResponseCallback,
                      private ::android::hardware::hidl_death_recipient {
  public:
    explicit SignalHandler(const std::weak_ptr<IConnectivityManager>& connectivity_manager);
    virtual ~SignalHandler() = default;

    bool RequestWifiStationMode() override;
    bool RequestSetWifiStationMode(const conman_hal_v1_0::WifiStationMode& mode) override;

  private:
    void StartSubscribe();
    bool Subscribe(const Connectivity::VccIpCmd::ServiceId& service_id,
                   const Connectivity::VccIpCmd::OperationId& operation_id,
                   const vehicle_com::OperationType& operation_type,
                   vehicle_com::IMessageCallback* message_callback);
    void UnsubscribeAll();

    ::android::hardware::Return<void> onMessageRcvd(const vehicle_com::Msg& msg) override;
    ::android::hardware::Return<void> onResponseRcvd(const vehicle_com::Msg& resp) override;
    ::android::hardware::Return<void> onErrorRcvd(const vehicle_com::Error& error) override;

    void serviceDied(uint64_t cookie, const ::android::wp<android_hidl::IBase>& who) override;

    std::weak_ptr<IConnectivityManager> connectivity_manager_;
    ::android::sp<vehicle_com::IVehicleCom> ipcb_server_;

    tarmac::eventloop::IDispatcher& dispatcher_;

    std::vector<uint64_t> subscription_ids_;

    ASN_BYTE session_buffer_msgd_[ASN_SESSION_SIZE + 2048] = {0};
    ASN_Session session_msgd_ = ASN_Session_Create(session_buffer_msgd_, sizeof(session_buffer_msgd_));

    uint8_t sequence_nr_{0};

    std::mutex guard_;
};

}  // namespace conman
}  // namespace vcc