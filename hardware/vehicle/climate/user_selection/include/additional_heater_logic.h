/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "notifiable_property.h"
#include "settings_proxy.h"

#include <Application_dataelement.h>
#include <vcc/localconfig.h>
#include <v0/org/volvocars/climate/UserSelection.hpp>

using UserSelectionGen = v0::org::volvocars::climate::UserSelection;

class AdditionalHeaterLogic {
  public:
    AdditionalHeaterLogic(const vcc::LocalConfigReaderInterface* lcfg,
                          NotifiableProperty<UserSelectionGen::OffOnSelection>& additionalHeater,
                          std::unique_ptr<SettingsProxy<int, SettingsFramework::UserScope::USER,
                                                        SettingsFramework::UserScope::NOT_USER_RELATED>>
                                  additionalHeaterSetting);
    void request(UserSelectionGen::OffOnType);

  private:
    void sendSignal();

    NotifiableProperty<UserSelectionGen::OffOnSelection>& additionalHeater_;
    std::unique_ptr<
            SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
            additionalHeaterSetting_;

    ApplicationDataElement::DESender<autosar::HeatrDurgDrvgReqd_info> HeatrDurgDrvgReqd_;

    bool additionalHeaterAvaiable_;
};
