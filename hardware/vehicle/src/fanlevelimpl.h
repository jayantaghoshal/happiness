/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "notifiable_property.h"

class FanLevelImpl {
  public:
    FanLevelImpl();

    void setFanLevel(int32_t fanLevel);
    ReadOnlyNotifiableProperty<int32_t>& fanLevelValue();

  private:
    NotifiableProperty<int32_t> m_fanLevel;
};