/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "ivi-logging.h"
#include "v0/org/volvocars/climate/CommonTypes.hpp"

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const CommonTypes::ReturnCode& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const CommonTypes::TemperatureState& value);
}
}
}
}
