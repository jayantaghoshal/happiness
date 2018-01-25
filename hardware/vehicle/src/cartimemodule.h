/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "ModuleBase.h"
#include "i_vehicle_hal_impl.h"

#include <IDispatcher.h>

/*
 * The CarTimeHal handles the reception of master time from the CEM and makes it available
 * via the UNIX_TIME vhal property. The time published via this property is a "verbatim" copy
 * of what is received from the CEM which means it is in localtime (not UTC)!!!
 * Clients can also write to this property (also in localtime of course) in which case the CarTimeHal will
 * start a time-change procedure against the CEM.
 * The CarTimeHal will NOT modify the system time of the Android platform!!!
 */

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;

class CarTimeHal : public vhal20::impl::ModuleBase {
  public:
    CarTimeHal(vhal20::impl::IVehicleHalImpl* vehicleHal);

    int setProp(const vhal20::VehiclePropValue& propValue) override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue) override;
    std::vector<vhal20::VehiclePropValue> getAllPropValues() override;
    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    bool GetTimeFromFlexray(vhal20::VehiclePropValue& time);
    void StartSetTimeSequenceOnFlexray(time_t timetoset);
    void StopSetTimeSequenceOnFlexray();
    void StartFlexraySubscribers();

    ApplicationDataElement::DEReceiver<autosar::TiAndDateIndcn_info> time_receiver_;
    ApplicationDataElement::DESender<autosar::SetTiAndDate_info> time_sender_;
    ApplicationDataElement::DESender<ActivateVfc_info> activatevfc_;

    tarmac::eventloop::IDispatcher::JobId settime_job_;
    time_t timetoset_;
};