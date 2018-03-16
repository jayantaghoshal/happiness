/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <v0/org/volvocars/climate/ParkingClimate.hpp>
#include "ivi-logging.h"

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::ParkingClimateState& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::ParkingClimateRequest& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::DateTimeStruct& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::PreConditioningSettingRequest& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::PreConditioningSettingState& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::PreCleaningNotificationState& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::WeekdayEnum& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::TimerStateEnum& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::TimerTypeEnum& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::ParkingTimerStruct& value);
logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::ParkingTimers& value);
}
}
}
}
