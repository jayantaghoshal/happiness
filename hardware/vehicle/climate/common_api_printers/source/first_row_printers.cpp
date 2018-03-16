/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "first_row_printers.h"

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::CleanzoneState& value) {
    switch (value) {
        case FirstRow::CleanzoneState::OFF:
            out << "OFF";
            break;
        case FirstRow::CleanzoneState::NOT_OK:
            out << "NOT_OK";
            break;
        case FirstRow::CleanzoneState::OK:
            out << "OK";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AirConditionerRequest& value) {
    switch (value) {
        case FirstRow::AirConditionerRequest::OFF:
            out << "OFF";
            break;
        case FirstRow::AirConditionerRequest::AUTO:
            out << "AUTO";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AirConditionerState& value) {
    switch (value) {
        case FirstRow::AirConditionerState::OFF:
            out << "OFF";
            break;
        case FirstRow::AirConditionerState::AUTO:
            out << "AUTO";
            break;
        case FirstRow::AirConditionerState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::AirConditionerState::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::FanLevelFrontRequest& value) {
    switch (value) {
        case FirstRow::FanLevelFrontRequest::OFF:
            out << "OFF";
            break;
        case FirstRow::FanLevelFrontRequest::LVL1:
            out << "LVL1";
            break;
        case FirstRow::FanLevelFrontRequest::LVL2:
            out << "LVL2";
            break;
        case FirstRow::FanLevelFrontRequest::LVL3:
            out << "LVL3";
            break;
        case FirstRow::FanLevelFrontRequest::LVL4:
            out << "LVL4";
            break;
        case FirstRow::FanLevelFrontRequest::LVL5:
            out << "LVL5";
            break;
        case FirstRow::FanLevelFrontRequest::MAX:
            out << "MAX";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::FanLevelFrontValue& value) {
    switch (value) {
        case FirstRow::FanLevelFrontValue::OFF:
            out << "OFF";
            break;
        case FirstRow::FanLevelFrontValue::LVL1:
            out << "LVL1";
            break;
        case FirstRow::FanLevelFrontValue::LVL2:
            out << "LVL2";
            break;
        case FirstRow::FanLevelFrontValue::LVL3:
            out << "LVL3";
            break;
        case FirstRow::FanLevelFrontValue::LVL4:
            out << "LVL4";
            break;
        case FirstRow::FanLevelFrontValue::LVL5:
            out << "LVL5";
            break;
        case FirstRow::FanLevelFrontValue::MAX:
            out << "MAX";
            break;
        case FirstRow::FanLevelFrontValue::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::FanLevelFrontValue::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AutoClimateState& value) {
    switch (value) {
        case FirstRow::AutoClimateState::NOT_VISIBLE:
            out << "NOT_VISIBLE";
            break;
        case FirstRow::AutoClimateState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::AutoClimateState::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        case FirstRow::AutoClimateState::OFF:
            out << "OFF";
            break;
        case FirstRow::AutoClimateState::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ManualRecircRequest& value) {
    switch (value) {
        case FirstRow::ManualRecircRequest::OFF:
            out << "OFF";
            break;
        case FirstRow::ManualRecircRequest::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ManualRecircState& value) {
    switch (value) {
        case FirstRow::ManualRecircState::OFF:
            out << "OFF";
            break;
        case FirstRow::ManualRecircState::ON:
            out << "ON";
            break;
        case FirstRow::ManualRecircState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::ManualRecircState::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AirDistributionAngleRequest& value) {
    switch (value) {
        case FirstRow::AirDistributionAngleRequest::FLOOR:
            out << "FLOOR";
            break;
        case FirstRow::AirDistributionAngleRequest::VENT:
            out << "VENT";
            break;
        case FirstRow::AirDistributionAngleRequest::DEFROST:
            out << "DEFROST";
            break;
        case FirstRow::AirDistributionAngleRequest::FLOOR_DEFROST:
            out << "FLOOR_DEFROST";
            break;
        case FirstRow::AirDistributionAngleRequest::FLOOR_VENT:
            out << "FLOOR_VENT";
            break;
        case FirstRow::AirDistributionAngleRequest::VENT_DEFROST:
            out << "VENT_DEFROST";
            break;
        case FirstRow::AirDistributionAngleRequest::FLOOR_VENT_DEFROST:
            out << "FLOOR_VENT_DEFROST";
            break;
        case FirstRow::AirDistributionAngleRequest::AUTO:
            out << "AUTO";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::AirDistributionAngle& value) {
    switch (value) {
        case FirstRow::AirDistributionAngle::FLOOR:
            out << "FLOOR";
            break;
        case FirstRow::AirDistributionAngle::VENT:
            out << "VENT";
            break;
        case FirstRow::AirDistributionAngle::DEFROST:
            out << "DEFROST";
            break;
        case FirstRow::AirDistributionAngle::FLOOR_DEFROST:
            out << "FLOOR_DEFROST";
            break;
        case FirstRow::AirDistributionAngle::FLOOR_VENT:
            out << "FLOOR_VENT";
            break;
        case FirstRow::AirDistributionAngle::VENT_DEFROST:
            out << "VENT_DEFROST";
            break;
        case FirstRow::AirDistributionAngle::FLOOR_VENT_DEFROST:
            out << "FLOOR_VENT_DEFROST";
            break;
        case FirstRow::AirDistributionAngle::AUTO:
            out << "AUTO";
            break;
        case FirstRow::AirDistributionAngle::DISABLED:
            out << "DISABLED";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterWindscreenRequest& value) {
    switch (value) {
        case FirstRow::ElectricDefrosterWindscreenRequest::OFF:
            out << "OFF";
            break;
        case FirstRow::ElectricDefrosterWindscreenRequest::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterWindscreenState& value) {
    switch (value) {
        case FirstRow::ElectricDefrosterWindscreenState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::ElectricDefrosterWindscreenState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        case FirstRow::ElectricDefrosterWindscreenState::OFF:
            out << "OFF";
            break;
        case FirstRow::ElectricDefrosterWindscreenState::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterRearRequest& value) {
    switch (value) {
        case FirstRow::ElectricDefrosterRearRequest::OFF:
            out << "OFF";
            break;
        case FirstRow::ElectricDefrosterRearRequest::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterRearState& value) {
    switch (value) {
        case FirstRow::ElectricDefrosterRearState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::ElectricDefrosterRearState::OFF:
            out << "OFF";
            break;
        case FirstRow::ElectricDefrosterRearState::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterPopupRequest& value) {
    switch (value) {
        case FirstRow::ElectricDefrosterPopupRequest::OFF:
            out << "OFF";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::ElectricDefrosterPopupState& value) {
    switch (value) {
        case FirstRow::ElectricDefrosterPopupState::OFF:
            out << "OFF";
            break;
        case FirstRow::ElectricDefrosterPopupState::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::MaxDefrosterRequest& value) {
    switch (value) {
        case FirstRow::MaxDefrosterRequest::OFF:
            out << "OFF";
            break;
        case FirstRow::MaxDefrosterRequest::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::MaxDefrosterState& value) {
    switch (value) {
        case FirstRow::MaxDefrosterState::OFF:
            out << "OFF";
            break;
        case FirstRow::MaxDefrosterState::ON:
            out << "ON";
            break;
        case FirstRow::MaxDefrosterState::DISABLED:
            out << "DISABLED";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::StateType& value) {
    switch (value) {
        case FirstRow::StateType::AVAILABLE:
            out << "AVAILABLE";
            break;
        case FirstRow::StateType::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        case FirstRow::StateType::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::StateType::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::Range& value) {
    out << "min: " << value.getMin() << " max: " << value.getMax();
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::TemperatureAttribute& value) {
    out << "state: " << value.getCurrState() << " temp: " << value.getValue() << " range: " << value.getRange();
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::HeatLevel& value) {
    switch (value) {
        case FirstRow::HeatLevel::OFF:
            out << "OFF";
            break;
        case FirstRow::HeatLevel::LO:
            out << "LO";
            break;
        case FirstRow::HeatLevel::MED:
            out << "MED";
            break;
        case FirstRow::HeatLevel::HI:
            out << "HI";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::HeatState& value) {
    switch (value) {
        case FirstRow::HeatState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        case FirstRow::HeatState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::HeatState::MANUAL:
            out << "MANUAL";
            break;
        case FirstRow::HeatState::AUTO:
            out << "AUTO";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::HeatAttribute& value) {
    out << "state: " << value.getHeatState() << " level: " << value.getHeatLevel();
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::VentLevel& value) {
    switch (value) {
        case FirstRow::VentLevel::OFF:
            out << "OFF";
            break;
        case FirstRow::VentLevel::LVL1:
            out << "LVL1";
            break;
        case FirstRow::VentLevel::LVL2:
            out << "LVL2";
            break;
        case FirstRow::VentLevel::LVL3:
            out << "LVL3";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::VentState& value) {
    switch (value) {
        case FirstRow::VentState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        case FirstRow::VentState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::VentState::MANUAL:
            out << "MANUAL";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::VentAttribute& value) {
    out << "state: " << value.getVentState() << " level: " << value.getVentLevel();
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::IonizerRequest& value) {
    switch (value) {
        case FirstRow::IonizerRequest::OFF:
            out << "OFF";
            break;
        case FirstRow::IonizerRequest::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::IonizerState& value) {
    switch (value) {
        case FirstRow::IonizerState::OFF:
            out << "OFF";
            break;
        case FirstRow::IonizerState::ON:
            out << "ON";
            break;
        case FirstRow::IonizerState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        case FirstRow::IonizerState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::IonizerState::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const FirstRow::TemperatureSyncState& value) {
    switch (value) {
        case FirstRow::TemperatureSyncState::DISABLED:
            out << "DISABLED";
            break;
        case FirstRow::TemperatureSyncState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        case FirstRow::TemperatureSyncState::OFF:
            out << "OFF";
            break;
        case FirstRow::TemperatureSyncState::ON:
            out << "ON";
            break;
        case FirstRow::TemperatureSyncState::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}
}  // namespace climate
}  // namespace volvocars
}  // namespace org
}  // namespace v0