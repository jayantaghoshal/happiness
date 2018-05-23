/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gsl/gsl>
#include "Application_dataelement.h"
#include "Application_dataelement_synchronous.h"
#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"
#include "notifiable_property.h"
#include "utils/vf_context.h"

enum class DAISettingType : int32_t { NOTHING = 0, VISUAL = 1, VISUAL_AND_SOUND = 2 };

extern bool isDriveAwayInfoEnabledInCarConfig();

class DAISettingImpl {
  public:
    DAISettingImpl(gsl::not_null<VFContext*> ctx);
    void setDAISetting(int32_t settings_value);
    ReadOnlyNotifiableProperty<int32_t>* DAISetting();

  private:
    SettingsFramework::Setting<DAISettingType, SettingsFramework::UserScope::USER> sDAISetting_;
    NotifiableProperty<int32_t> daiSetting_;

    ApplicationDataElement::DESynchronousReceiver<autosar::VehModMngtGlbSafe1_info> vehmod_flexray_receiver_;
    ApplicationDataElement::DESender<autosar::DriveAwayInfoActvReq_info> infoActvSender_;
    ApplicationDataElement::DESender<autosar::DriveAwayInfoSoundWarnActvReq_info> infoSoundWarnActvSender_;
    autosar::OnOff1 autosarInfoActv_;
    autosar::OnOff1 autosarSoundWarnActv_;
};