/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "ModuleBase.h"
#include "i_vehicle_hal_impl.h"
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {
namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class ActiveUserProfileHal : public vhal20::impl::ModuleBase {
  public:
    ActiveUserProfileHal(vhal20::impl::IVehicleHalImpl* vehicleHal);

    int setProp(const vhal20::VehiclePropValue& propValue) override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& status) override;
    std::vector<vhal20::VehiclePropValue> getAllPropValues() override;
    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    void initHandledProperties();
    bool getUserProfileFromFlexray(vhal20::VehiclePropValue& active_user_profile);

    ApplicationDataElement::DEReceiver<autosar::ProfPenSts1_info> prof_pen_sts1_receiver_;
    ApplicationDataElement::DESender<autosar::ProfChg_info> prof_chg_sender_;

    vhal20::VehiclePropValue active_user_profile_;
};

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
