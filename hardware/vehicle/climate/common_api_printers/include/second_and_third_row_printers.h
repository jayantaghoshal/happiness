/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "v0/org/volvocars/climate/SecondAndThirdRow.hpp"
#include "ivi-logging.h"

namespace v0
{
namespace org
{
namespace volvocars
{
namespace climate
{

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData&          out,
                                                const SecondAndThirdRow::FanLevelRearRequest& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData&        out,
                                                const SecondAndThirdRow::FanLevelRearState& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData&              out,
                                                const SecondAndThirdRow::SecondRowClimateRequest& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData&            out,
                                                const SecondAndThirdRow::SecondRowClimateState& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::Range&      value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData&           out,
                                                const SecondAndThirdRow::TemperatureAttribute& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::HeatLevel&  value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::HeatState&  value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData&    out,
                                                const SecondAndThirdRow::HeatAttribute& value);
}
}
}
}
