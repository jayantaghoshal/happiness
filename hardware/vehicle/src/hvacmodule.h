/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "fanlevelimpl.h"
#include "modulepropertyhandler.h"
#include "tempimpl.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class HvacModule : public vccvhal10::impl::ModulePropertyHandler {
  public:
    HvacModule(vhal20::impl::IVehicleHalImpl* vehicleHal);

  private:
    FanLevelImpl m_fanLevelImpl;
    TempImpl m_tempImpl;
};
