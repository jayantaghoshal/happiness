/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "parking_climate_printers.h"

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::ParkingClimateState& value) {
    switch (value) {
        case ParkingClimate::ParkingClimateState::OFF:
            out << "OFF";
            break;
        case ParkingClimate::ParkingClimateState::ON:
            out << "ON";
            break;
        case ParkingClimate::ParkingClimateState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        case ParkingClimate::ParkingClimateState::DISABLED:
            out << "DISABLED";
            break;
        default:
            out << "NOT MAPPED: " << value;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::ParkingClimateRequest& value) {
    switch (value) {
        case ParkingClimate::ParkingClimateRequest::OFF:
            out << "OFF";
            break;
        case ParkingClimate::ParkingClimateRequest::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::DateTimeStruct& value) {
    out << "Year: " << value.getYear() << "Month: " << value.getMonth() << "Day: " << value.getDay()
        << "Hour: " << value.getHour() << "Minutes: " << value.getMinute();

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::PreConditioningSettingRequest& value) {
    switch (value) {
        case ParkingClimate::PreConditioningSettingRequest::OFF:
            out << "OFF";
            break;
        case ParkingClimate::PreConditioningSettingRequest::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::PreConditioningSettingState& value) {
    switch (value) {
        case ParkingClimate::PreConditioningSettingState::OFF:
            out << "OFF";
            break;
        case ParkingClimate::PreConditioningSettingState::ON:
            out << "ON";
            break;
        case ParkingClimate::PreConditioningSettingState::NOT_PRESENT:
            out << "NOT_PRESENT";
            break;
        default:
            out << "NOT MAPPED: " << value;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::PreCleaningNotificationState& value) {
    switch (value) {
        case ParkingClimate::PreCleaningNotificationState::NOT_VISIBLE:
            out << "NOT_VISIBLE";
            break;
        case ParkingClimate::PreCleaningNotificationState::COMPLETED:
            out << "COMPLETED";
            break;
        case ParkingClimate::PreCleaningNotificationState::LOW_BATTERY_ERROR:
            out << "LOW_BATTERY_ERROR";
            break;
        case ParkingClimate::PreCleaningNotificationState::INTERRUPTED:
            out << "INTERRUPTED";
            break;
        default:
            out << "NOT MAPPED: " << value;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::WeekdayEnum& value) {
    switch (value) {
        case ParkingClimate::WeekdayEnum::MONDAY:
            out << "MONDAY ";
            break;
        case ParkingClimate::WeekdayEnum::TUESDAY:
            out << "TUESDAY ";
            break;
        case ParkingClimate::WeekdayEnum::WEDNESDAY:
            out << "WEDNESDAY ";
            break;
        case ParkingClimate::WeekdayEnum::THURSDAY:
            out << "THURSDAY ";
            break;
        case ParkingClimate::WeekdayEnum::FRIDAY:
            out << "FRIDAY ";
            break;
        case ParkingClimate::WeekdayEnum::SATURDAY:
            out << "SATURDAY ";
            break;
        case ParkingClimate::WeekdayEnum::SUNDAY:
            out << "SUNDAY ";
            break;
        default:
            out << "NOT MAPPED: " << value;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::WeekdaysVec& value) {
    for (auto day : value) {
        out << day;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::TimerStateEnum& value) {
    switch (value) {
        case ParkingClimate::TimerStateEnum::OFF:
            out << "OFF";
            break;
        case ParkingClimate::TimerStateEnum::ON:
            out << "ON";
            break;
        default:
            out << "NOT MAPPED: " << value;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::TimerTypeEnum& value) {
    switch (value) {
        case ParkingClimate::TimerTypeEnum::DAY:
            out << "DAY";
            break;
        case ParkingClimate::TimerTypeEnum::REPEATED:
            out << "REPEATED";
            break;
        case ParkingClimate::TimerTypeEnum::SINGLE:
            out << "SINGLE";
            break;
        default:
            out << "NOT MAPPED: " << value;
            break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::ParkingTimerStruct& value) {
    out << "id:" << value.getTimerId() << " type:" << value.getTimerType() << " active:" << value.getActive()
        << " date:" << value.getDateTime() << " days:" << value.getWeekdays();

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out,
                                                const ParkingClimate::ParkingTimers& value) {
    out << "{ ";

    for (auto const& item : value) {
        out << "[" << item << "]";
    }

    out << " } ";

    return out;
}
}  // namespace climate
}  // namespace volvocars
}  // namespace org
}  // namespace v0
