/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"

#include "modulepropertyhandler.h"
#include "notifiable_property.h"
#include "papropertyhandler.h"
#include "utils/vf_context.h"

#include "legacydispatcher.h"

#include <gsl/gsl>
#include "Application_dataelement_synchronous.h"
#include "utils/vf_context.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class ParkingAssistActivationModule {
  public:
    ParkingAssistActivationModule(gsl::not_null<VFContext*> ctx);

  private:
    PAPropHandler<bool> PA_prop_pas_activation_;

    // Sends flexray signals
    ApplicationDataElement::DESender<autosar::ActvnOfPrkgAssi_info> pas_activation_flexray_sender_;

    // Receives flexray signals
    ApplicationDataElement::DESynchronousReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    ApplicationDataElement::DESynchronousReceiver<autosar::PrkgAssiActvSts_info> pas_activation_flexray_receiver_;
    ApplicationDataElement::DESynchronousReceiver<autosar::AutnmsPrkgActvScenario_info> active_state_AP_flexray_receiver_;

    ILegacyDispatcher& timerDispatcher_;

    // "States"
    bool is_system_error_;
    bool is_active_;
    bool pas_activation_on_;
    bool is_force_on_;

    // "Sub States"
    bool is_pas_activation_alivetimeout_;
    bool is_active_state_AP_alivetimeout_;

    void Update();
    void SetFlexray(bool value);
};
