/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "notifiable_property.h"
#include "settings_proxy.h"
#include "v0/org/volvocars/climate/CommonTypes.hpp"
#include "v0/org/volvocars/climate/FirstRow.hpp"

#include <Application_dataelement.h>

namespace first_row {
using FirstRowGen = v0::org::volvocars::climate::FirstRow;
using CommonTypesGen = v0::org::volvocars::climate::CommonTypes;

class IonizerLogic final {
  public:
    IonizerLogic(NotifiableProperty<FirstRowGen::IonizerState>& ionizer,
                 SettingsProxy<bool, SettingsFramework::UserScope::NOT_USER_RELATED,
                               SettingsFramework::UserScope::NOT_USER_RELATED>& ionizerSetting);

    ~IonizerLogic() = default;

    CommonTypesGen::ReturnCode request(FirstRowGen::IonizerRequest request);

  private:
    bool signalsOk() const;
    void activationCheck();
    void update();

    NotifiableProperty<FirstRowGen::IonizerState>& ionizer_;
    SettingsProxy<bool, SettingsFramework::UserScope::NOT_USER_RELATED, SettingsFramework::UserScope::NOT_USER_RELATED>&
            ionizerSetting_;

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModeSignal_;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climaActvSignal_;
    ApplicationDataElement::DESender<autosar::AirClngReq_info> airClngReqSignal_;

    bool carConfigOk_;
    bool isActive_;
    bool isOk_;

    using Mutex = std::mutex;
    Mutex mutex_;
};
}
