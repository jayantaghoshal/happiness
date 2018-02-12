/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "ModuleBase.h"
#include "i_vehicle_hal_impl.h"
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {
namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class ConnectedSafety : public vhal20::impl::ModuleBase {
  public:
    ConnectedSafety(vhal20::impl::IVehicleHalImpl* vehicleHal,
                    android::sp<SettingsFramework::SettingsManagerHidl> settings_manager);

    int setProp(const vhal20::VehiclePropValue& propValue) override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& status) override;
    std::vector<vhal20::VehiclePropValue> getAllPropValues() override;
    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    SettingsFramework::Setting<autosar::OnOff1, SettingsFramework::UserScope::USER> sConnectedSafety_;

    ApplicationDataElement::DESender<autosar::RoadFricIndcnActv_info> roadFricIndcnActv_;
    ApplicationDataElement::DEReceiver<autosar::RoadFricIndcnSts_info> roadFricIndcnSts_;
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1_;

    autosar::OnOffPen autosarConnSafety_;
    autosar::OnOff1 connectedSafetyValue_;
    SettingsFramework::ProfileIdentifier activeProfile_;
    vhal20::VehiclePropValue connectedSafetyPropValue_;

    bool isAvailable_;

    void mainLoop();
};

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
