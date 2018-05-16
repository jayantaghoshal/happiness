/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>
#include <mutex>
#include <vsomeip/vsomeip.hpp>

#include <vendor/volvocars/hardware/remotectrl/1.0/IRemoteCtrlProperty.h>
#include <vendor/volvocars/hardware/remotectrl/1.0/IRemoteCtrlPropertyResponse.h>

#include "convapi_signals_def.h"
#include "service_base.h"

namespace vcc {
namespace remotectrl {
namespace remotemediactrl {

namespace hidl_remotectrl = ::vendor::volvocars::hardware::remotectrl::V1_0;

class MediaCtrlService final : public hidl_remotectrl::IRemoteCtrlPropertyResponse,
                               public vcc::remotectrl::servicebase::ServiceBase,
                               ::android::hardware::hidl_death_recipient {
  public:
    explicit MediaCtrlService(const ServiceInfo& service_info) : ServiceBase(service_info) {}

    ::android::hardware::Return<void> sendGetPropertyResp(
            uint32_t /*requestIdentifier*/,
            const hidl_remotectrl::RemoteCtrlHalPropertyValue& /*propValue*/) override {
        return ::android::hardware::Void();
    }

    ::android::hardware::Return<void> sendSetPropertyResp(
            uint32_t requestIdentifier,
            const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) override;

    ::android::hardware::Return<void> notifyPropertyChanged(
            const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) override;

    ::android::hardware::Return<void> registerRemoteCtrlPropertyHandler(
            const ::android::sp<hidl_remotectrl::IRemoteCtrlProperty>& handler) override;

    ::android::hardware::Return<void> unregisterRemoteCtrlPropertyHandler(
            const ::android::sp<hidl_remotectrl::IRemoteCtrlProperty>& handler) override;

  private:
    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override;

    void OnStateChange(vsomeip::state_type_e state) override;

    bool OnMessageReceive(const std::shared_ptr<vsomeip::message>& message) override;

    std::mutex guard_;
    ::android::sp<hidl_remotectrl::IRemoteCtrlProperty> system_service_handler_;
};

}  // namespace remotemediactrl
}  // namespace remotectrl
}  // namespace vcc
