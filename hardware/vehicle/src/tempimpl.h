/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "Application_dataelement.h"
#include "notifiable_property.h"

using namespace ApplicationDataElement;
using namespace autosar;

class TempImpl {
  public:
    TempImpl();

    void setleftTemp(float temp_left);
    void setrightTemp(float temp_right);

    ReadOnlyNotifiableProperty<float>& temperatureLeftValue();
    ReadOnlyNotifiableProperty<float>& temperatureRightValue();

  private:
    NotifiableProperty<float> m_temp_left;
    NotifiableProperty<float> m_temp_right;
    DESender<HmiCmptmtTSp_info> HmiCmptmtTSpSender;
};