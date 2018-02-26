/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <gen_datatypes.h>

namespace common
{
namespace daemon
{

class ITemperatureLogic
{
public:
    virtual ~ITemperatureLogic(){};

    virtual void request(double temp, autosar::HmiCmptmtTSpSpcl tempHiLoN) = 0;
};
}
}
