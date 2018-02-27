/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "ivi-logging.h"
#include "v0/org/volvocars/climate/FirstRow.hpp"

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::CleanzoneState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AirConditionerRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AirConditionerState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::FanLevelFrontRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::FanLevelFrontValue& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AutoClimateState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ManualRecircRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ManualRecircState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AirDistributionAngleRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AirDistributionAngle& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterWindscreenRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterWindscreenState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterRearRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterRearState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterPopupRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterPopupState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::MaxDefrosterRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::MaxDefrosterState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::StateType& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const FirstRow::Range& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::TemperatureAttribute& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::HeatLevel& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::HeatState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::HeatAttribute& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::VentLevel& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::VentState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::VentAttribute& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::IonizerRequest& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::IonizerState& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::FanLevelFrontValue& value);

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::TemperatureSyncState& value);
}
}
}
}
