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
#include <vendor/volvocars/hardware/remotectrl/1.0/types.h>

#include "convapi_signals_def.h"
#include "service_base.h"

namespace vcc {
namespace remotectrl {
namespace generalsettings {

namespace hidl_remotectrl = ::vendor::volvocars::hardware::remotectrl::V1_0;

constexpr vsomeip::service_t service_id = REMOTECTRL_GENERALSETTINGS_SERVICE_ID;
constexpr vsomeip::instance_t instance_id = REMOTECTRL_GENERALSETTINGS_SERVICE_INSTANCE_ID;
constexpr vsomeip::eventgroup_t eventgroup_id = REMOTECTRL_GENERALSETTINGS_EVENTGROUP_ID;
constexpr std::array<vsomeip::method_t, 2> remotectrl_service_methods = {
        REMOTECTRL_GENERALSETTINGS_METHOD_ID_GET_CSD_STATE,
        REMOTECTRL_GENERALSETTINGS_METHOD_ID_SET_CSD_STATE,
};

constexpr std::array<vsomeip::method_t, 1> remotectrl_service_events = {
        REMOTECTRL_GENERALSETTINGS_EVENT_ID_CSD_STATECHANGED};

constexpr std::array<hidl_remotectrl::RemoteCtrlHalProperty, 1> remotectrl_hal_prop_array{
        hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_CSD_ON};

// TODO(Arjun Snehaj Lal, 14:05:2018) : populate with remaining properties handled by General Settings Service

class GeneralSettingsCtrlService final : public hidl_remotectrl::IRemoteCtrlPropertyResponse,
                                         public vcc::remotectrl::servicebase::ServiceBase,
                                         ::android::hardware::hidl_death_recipient {
  public:
    explicit GeneralSettingsCtrlService(const ServiceInfo& service_info) : ServiceBase(service_info) {}

    ::android::hardware::Return<void> sendGetPropertyResp(
            uint32_t requestIdentifier,
            const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) override;
    ::android::hardware::Return<void> notifyPropertyChanged(
            const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) override;

    ::android::hardware::Return<void> sendSetPropertyResp(
            uint32_t requestIdentifier,
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

}  // namespace generalsettings
}  // namespace remotectrl
}  // namespace vcc
