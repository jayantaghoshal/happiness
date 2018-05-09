/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include <Application_dataelement.h>
#include <utility>

namespace common {
namespace daemon {

class TemperatureConverter {
  public:
    TemperatureConverter();

    ~TemperatureConverter();

    double toSingle(autosar::AmbTIndcdUnit unit, double temp) const;

    double fromSingle(autosar::AmbTIndcdUnit unit, double temp) const;

    std::pair<double, double> range(autosar::AmbTIndcdUnit unit) const;

    double roundToClosestWholeDegree(double temp) const;

    double roundToClosestHalfDegree(double temp) const;

  private:
    bool isMCC_;
};
}
}
