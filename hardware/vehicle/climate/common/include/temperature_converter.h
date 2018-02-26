/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include <Application_dataelement.h>
#include <utility>

namespace common
{
namespace daemon
{

class TemperatureConverter
{
public:
    TemperatureConverter();

    ~TemperatureConverter();

    double toSingle(autosar::AmbTIndcdUnit unit, double temp, autosar::HmiCmptmtTSpSpcl hiLoN) const;

    std::pair<double, autosar::HmiCmptmtTSpSpcl> fromSingle(autosar::AmbTIndcdUnit unit, double temp) const;

    std::pair<double, double> range(autosar::AmbTIndcdUnit unit) const;

private:
    bool isMCC_;
};

}
}

