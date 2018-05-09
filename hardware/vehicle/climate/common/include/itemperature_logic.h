/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gen_datatypes.h>

namespace common {
namespace daemon {

class ITemperatureLogic {
  public:
    virtual ~ITemperatureLogic(){};

    virtual void request(double temp) = 0;
    virtual void request(autosar::HmiCmptmtTSpSpcl tempHiLoN) = 0;
};
}
}
