#pragma once

#include "notifiable_property.h"

class FanLevelImpl {
  public:
    FanLevelImpl(NotifiableProperty<int32_t>& fanLevel);

    void setFanLevel(int32_t fanLevel);

  private:
    NotifiableProperty<int32_t>& m_fanLevel;
};