/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gsl/gsl>
#include "Application_dataelement_synchronous.h"
#include "daisettingimpl.h"
#include "modulepropertyhandler.h"
#include "utils/vf_context.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

class ActiveSafetyModule : public vccvhal10::impl::ModulePropertyHandler {
  public:
    ActiveSafetyModule(gsl::not_null<VFContext*> ctx);

  private:
    DAISettingImpl m_DAISettingImpl;
};
