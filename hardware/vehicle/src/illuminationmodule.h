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

class IlluminationHal : public vhal20::impl::ModuleBase {
  public:
    IlluminationHal(vhal20::impl::IVehicleHalImpl* vehicleHal);

    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue) override;
    std::vector<vhal20::VehiclePropValue> getAllPropValues() override;
    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    bool GetDayNightFromFlexray(vhal20::VehiclePropValue& day_night);
    bool GetIlluminationRegulatorFromFlexray(vhal20::VehiclePropValue& illumination);
    void StartFlexraySubscribers();

    ApplicationDataElement::DEReceiver<autosar::TwliBriSts_info> day_night_receiver_;
    ApplicationDataElement::DEReceiver<autosar::IntrBriSts_info> illumination_receiver_;
};

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
