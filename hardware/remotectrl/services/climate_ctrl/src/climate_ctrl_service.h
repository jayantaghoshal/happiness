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
namespace remoteclimatectrl {

namespace hidl_remotectrl = ::vendor::volvocars::hardware::remotectrl::V1_0;

constexpr vsomeip::service_t service_id = REMOTECTRL_CLIMATECTRL_SERVICE_ID;
constexpr vsomeip::instance_t instance_id = REMOTECTRL_CLIMATECTRL_SERVICE_INSTANCE_ID;
constexpr vsomeip::eventgroup_t eventgroup_id = REMOTECTRL_CLIMATECTRL_EVENTGROUP_ID;

constexpr std::array<vsomeip::method_t, 10> remotectrl_service_methods = {
        REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AIR_DISTRIBUTION,
        REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AIR_DISTRIBUTION};

constexpr std::array<vsomeip::method_t, 5> remotectrl_service_events = {
        REMOTECTRL_CLIMATECTRL_EVENT_ID_FANLEVELCHANGED,
        REMOTECTRL_CLIMATECTRL_EVENT_ID_MAX_DEFROSTER_STATECHANGED,
        REMOTECTRL_CLIMATECTRL_EVENT_ID_AC_STATECHANGED,
        REMOTECTRL_CLIMATECTRL_EVENT_ID_TEMPERATURECHANGED,
        REMOTECTRL_CLIMATECTRL_EVENT_ID_AIR_DISTRIBUTIONCHANGED};

constexpr std::array<hidl_remotectrl::RemoteCtrlHalProperty, 5> remotectrl_hal_prop_array{
        hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_FAN_SPEED,
        hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_MAX_DEFROST_ON,
        hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_AC_ON,
        hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_TEMPERATURE,
        hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_FAN_DIRECTION};

class ClimateCtrlService final : public hidl_remotectrl::IRemoteCtrlPropertyResponse,
                                 public vcc::remotectrl::servicebase::ServiceBase,
                                 ::android::hardware::hidl_death_recipient {
  public:
    explicit ClimateCtrlService(const ServiceInfo& service_info) : ServiceBase(service_info) {}

    ::android::hardware::Return<void> sendGetPropertyResp(
            uint32_t requestIdentifier,
            const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) override;

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

}  // namespace remoteclimatectrl
}  // namespace remotectrl
}  // namespace vcc
