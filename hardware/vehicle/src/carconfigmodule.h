/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

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

class CarConfigHal : public vhal20::impl::ModuleBase {
  public:
    CarConfigHal(vhal20::impl::IVehicleHalImpl* vehicleHal);

    std::vector<vhal20::VehiclePropValue> getAllPropValues() override;
    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    std::vector<uint8_t> carconfigs;
};

}  // impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
