/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "ModuleBase.h"

#include "Application_dataelement.h"
#include "HisipClient.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;

/**
 *  @brief Sensor module
 */
class SensorModule : public vhal20::impl::ModuleBase {
  public:
    SensorModule(vhal20::impl::IVehicleHalImpl* vehicleHal);

    ~SensorModule() = default;

    std::vector<vhal20::VehiclePropValue> getAllPropValues() override;
    std::vector<vhal20::VehiclePropConfig> listProperties() override;
    /* Callbacks */
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& propValue,
                                                      vhal20::impl::Status& status) override;

  private:
    const std::vector<vhal20::VehiclePropConfig> sensor_prop_config_;

    bool GetDrivingStatus(vhal20::VehiclePropValue& driving_status);
    bool GetIgnitionState(vhal20::VehiclePropValue& ignition_state);
    bool GetGearSelection(vhal20::VehiclePropValue& gear_selection);

    void StartFlexraySubscribers();
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> veh_mod_receiver_;
};
