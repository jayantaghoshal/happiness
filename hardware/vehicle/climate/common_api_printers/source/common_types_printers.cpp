/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "common_types_printers.h"

namespace v0
{
namespace org
{
namespace volvocars
{
namespace climate
{

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const CommonTypes::ReturnCode& value)
{
    switch (value)
    {
    case CommonTypes::ReturnCode::SUCCESS:
        out << "SUCCESS";
        break;
    case CommonTypes::ReturnCode::FUNCTION_NOT_PRESENT:
        out << "FUNCTION_NOT_PRESENT";
        break;
    case CommonTypes::ReturnCode::SIGNAL_FAULT:
        out << "SIGNAL_FAULT";
        break;
    case CommonTypes::ReturnCode::FUNCTION_IS_DISABLED:
        out << "FUNCTION_IS_DISABLED";
        break;
    case CommonTypes::ReturnCode::BAD_ARGUMENTS:
        out << "BAD_ARGUMENTS";
        break;
    case CommonTypes::ReturnCode::INVALID_REQUEST:
        out << "INVALID_REQUEST";
        break;
    case CommonTypes::ReturnCode::NOT_READY:
        out << "NOT_READY";
        break;
    case CommonTypes::ReturnCode::TIMESTAMP_INVALID:
        out << "TIMESTAMP_INVALID";
        break;
    default:
        out << "NOT MAPPED: " << static_cast<int>(value);
        break;
    }

    return out;
}

logging::DefaultLogContext::LogData& operator<<(logging::DefaultLogContext::LogData& out, const CommonTypes::TemperatureState& value)
{
    switch (value)
    {
    case CommonTypes::TemperatureState::AVAILABLE:
        out << "AVAILABLE";
        break;
    case CommonTypes::TemperatureState::DISABLED:
        out << "DISABLED";
        break;
    case CommonTypes::TemperatureState::NOT_PRESENT:
        out << "NOT_PRESENT";
        break;
    case CommonTypes::TemperatureState::SYSTEM_ERROR:
        out << "SYSTEM_ERROR";
        break;
    default:
        out << "NOT MAPPED: " << static_cast<int>(value);
        break;
    }

    return out;
}

}
}
}
}
