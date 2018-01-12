/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "daisettingimpl.h"
#include "modulepropertyhandler.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class ActiveSafetyModule : public vccvhal10::impl::ModulePropertyHandler {
  public:
    ActiveSafetyModule(vhal20::impl::IVehicleHalImpl* vehicleHal,
                       android::sp<SettingsFramework::SettingsManagerHidl> settings_manager);

  private:
    DAISettingImpl m_DAISettingImpl;
};
