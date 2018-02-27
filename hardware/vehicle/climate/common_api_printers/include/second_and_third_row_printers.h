/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "ivi-logging.h"
#include "v0/org/volvocars/climate/SecondAndThirdRow.hpp"

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::FanLevelRearRequest& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::FanLevelRearState& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::SecondRowClimateRequest& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::SecondRowClimateState& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::Range& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::TemperatureAttribute& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::HeatLevel& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::HeatState& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::HeatAttribute& value);
}
}
}
}
