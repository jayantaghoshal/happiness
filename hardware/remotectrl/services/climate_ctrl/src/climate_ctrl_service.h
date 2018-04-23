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

#include "service_base.h"

namespace vcc {
namespace remotectrl {
namespace remoteclimatectrl {

namespace vhal_2_0 = ::android::hardware::automotive::vehicle::V2_0;
namespace hidl_remotectrl = ::vendor::volvocars::hardware::remotectrl::V1_0;

class ClimateCtrlService final : public hidl_remotectrl::IRemoteCtrlPropertyResponse,
                                 public vcc::remotectrl::servicebase::ServiceBase,
                                 ::android::hardware::hidl_death_recipient {
  public:
    explicit ClimateCtrlService(const ServiceInfo& service_info) : ServiceBase(service_info) {}

    ::android::hardware::Return<void> sendGetPropertyResp(
            uint16_t requestIdentifier,
            const vhal_2_0::VehiclePropValue& requestedPropValue) override;

    ::android::hardware::Return<void> sendSetPropertyResp(
            uint16_t requestIdentifier,
            const vhal_2_0::VehiclePropValue& requestedPropValue) override;

    ::android::hardware::Return<void> notifyPropertyChanged(const vhal_2_0::VehiclePropValue& propValue) override;

    ::android::hardware::Return<void> registerRemoteCtrlPropertyHandler(
            const ::android::sp<hidl_remotectrl::IRemoteCtrlProperty>& handler) override;

    ::android::hardware::Return<void> unregisterRemoteCtrlPropertyHandler(
            const ::android::sp<hidl_remotectrl::IRemoteCtrlProperty>& handler) override;

  private:
    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override;

    void OnStateChange(vsomeip::state_type_e state);

    void OnMessageReceive(const std::shared_ptr<vsomeip::message>& message);

    template <typename T>
    std::vector<vsomeip::byte_t> CreateSomeIpResponse(T&& /*dummy*/,
                                                      const vhal_2_0::VehiclePropValue& requestedPropValue);

    template <typename T>
    std::vector<vsomeip::byte_t> CreateSomeIpSetResponse(T&& /*dummy*/,
                                                         const vhal_2_0::VehiclePropValue& requestedPropValue);

    template <typename T>
    std::vector<vsomeip::byte_t> CreateSomeIpNotification(T&& /*dummy*/,
                                                          const vhal_2_0::VehiclePropValue& requestedPropValue);

    template <typename T>
    vhal_2_0::VehiclePropValue VhalSetPropertyReq(const T& /*dummy*/,
                                                  const std::shared_ptr<vsomeip::payload>& msg_payload);

    template <typename T>
    vhal_2_0::VehiclePropValue VhalGetPropertyReq(const T& /*dummy*/,
                                                  const std::shared_ptr<vsomeip::payload>& msg_payload);

    std::mutex guard_;
    ::android::sp<hidl_remotectrl::IRemoteCtrlProperty> system_service_handler_;
};

}  // namespace remoteclimatectrl
}  // namespace remotectrl
}  // namespace vcc
