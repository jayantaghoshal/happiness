/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "second_and_third_row_printers.h"
#include "common_types_printers.h"

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::FanLevelRearRequest& value) {
    switch (value) {
        case SecondAndThirdRow::FanLevelRearRequest::LVL1:
            out << "LVL1";
            break;
        case SecondAndThirdRow::FanLevelRearRequest::LVL2:
            out << "LVL2";
            break;
        case SecondAndThirdRow::FanLevelRearRequest::LVL3:
            out << "LVL3";
            break;
        case SecondAndThirdRow::FanLevelRearRequest::LVL4:
            out << "LVL4";
            break;
        case SecondAndThirdRow::FanLevelRearRequest::LVL5:
            out << "LVL5";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::FanLevelRearState& value) {
    switch (value) {
        case SecondAndThirdRow::FanLevelRearState::LVL1:
            out << "LVL1";
            break;
        case SecondAndThirdRow::FanLevelRearState::LVL2:
            out << "LVL2";
            break;
        case SecondAndThirdRow::FanLevelRearState::LVL3:
            out << "LVL3";
            break;
        case SecondAndThirdRow::FanLevelRearState::LVL4:
            out << "LVL4";
            break;
        case SecondAndThirdRow::FanLevelRearState::LVL5:
            out << "LVL5";
            break;
        case SecondAndThirdRow::FanLevelRearState::OFF:
            out << "OFF";
            break;
        case SecondAndThirdRow::FanLevelRearState::DISABLED:
            out << "DISABLED";
            break;
        case SecondAndThirdRow::FanLevelRearState::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        case SecondAndThirdRow::FanLevelRearState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::SecondRowClimateRequest& value) {
    switch (value) {
        case SecondAndThirdRow::SecondRowClimateRequest::OFF:
            out << "OFF";
            break;
        case SecondAndThirdRow::SecondRowClimateRequest::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::SecondRowClimateState& value) {
    switch (value) {
        case SecondAndThirdRow::SecondRowClimateState::OFF:
            out << "OFF";
            break;
        case SecondAndThirdRow::SecondRowClimateState::ON:
            out << "ON";
            break;
        case SecondAndThirdRow::SecondRowClimateState::DISABLED:
            out << "DISABLED";
            break;
        case SecondAndThirdRow::SecondRowClimateState::SYSTEM_ERROR:
            out << "SYSTEM_ERROR";
            break;
        case SecondAndThirdRow::SecondRowClimateState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::Range& value) {
    out << "min: " << value.getMin() << " max: " << value.getMax();
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::TemperatureAttribute& value) {
    out << "state: " << value.getCurrState() << " temp: " << value.getValue() << " range: " << value.getRange();
    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::HeatLevel& value) {
    switch (value) {
        case SecondAndThirdRow::HeatLevel::OFF:
            out << "OFF";
            break;
        case SecondAndThirdRow::HeatLevel::LO:
            out << "LO";
            break;
        case SecondAndThirdRow::HeatLevel::MED:
            out << "MED";
            break;
        case SecondAndThirdRow::HeatLevel::HI:
            out << "HI";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::HeatState& value) {
    switch (value) {
        case SecondAndThirdRow::HeatState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        case SecondAndThirdRow::HeatState::DISABLED:
            out << "DISABLED";
            break;
        case SecondAndThirdRow::HeatState::MANUAL:
            out << "MANUAL";
            break;
        default:
            out << "NOT MAPPED: " << value.value_;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const SecondAndThirdRow::HeatAttribute& value) {
    out << "state: " << value.getHeatState() << " level: " << value.getHeatLevel();
    return out;
}
}  // namespace climate
}  // namespace volvocars
}  // namespace org
}  // namespace v0
