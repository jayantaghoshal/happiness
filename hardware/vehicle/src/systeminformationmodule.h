/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "ModuleBase.h"
#include "android/hardware/automotive/vehicle/2.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;

using namespace ApplicationDataElement;
using namespace autosar;

class SystemInformationModule : public vhal20::impl::ModuleBase {
  public:
    SystemInformationModule(vhal20::impl::IVehicleHalImpl *vehicle_hal);
    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    void StartSubscribe();
    void onVin1BlockReceived(const DataElemValue<Vin_info> &vin_info);
    std::string asStdString(const autosar::Vin1 &block1, const autosar::Vin1 &block2, const autosar::Vin1 &block3);
    bool hasByteErrors(const autosar::Vin1 &block);
    bool hasNonPrintableCharacters(const autosar::Vin1 &block);

  private:
    DEReceiver<Vin_info> vin_info_receiver_;

    autosar::Vin1 m_block1;
    autosar::Vin1 m_block2;
    autosar::Vin1 m_block3;
    bool m_block1Received = false;
    bool m_block2Received = false;
    bool m_block3Received = false;
    std::string m_vin;
};
