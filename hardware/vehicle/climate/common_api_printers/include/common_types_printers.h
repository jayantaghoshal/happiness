/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "v0/org/volvocars/climate/CommonTypes.hpp"
#include "ivi-logging.h"

namespace v0
{
namespace org
{
namespace volvocars
{
namespace climate
{

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const CommonTypes::ReturnCode&       value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const CommonTypes::TemperatureState& value);
}
}
}
}
