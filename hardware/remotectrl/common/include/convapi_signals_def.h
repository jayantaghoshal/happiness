/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstdint>
#include "exceptions.h"

namespace vcc {
namespace remotectrl {

constexpr uint8_t VCC_SOMEIP_PROTOCOL_VERSION = 0x01U;
constexpr uint8_t VCC_SOMEIP_INTERFACE_VERSION = 0x01U;
constexpr uint8_t VCC_CLIENT_ID = 0x00U;

// Currently using dummy values here
// RemoteCtrl_AudioCtrl Service
constexpr uint16_t REMOTECTRL_AUDIOCTRL_SERVICE_ID = 0x1100U;
constexpr uint16_t REMOTECTRL_AUDIOCTRL_SERVICE_INSTANCE_ID = 0x1101U;
constexpr uint16_t REMOTECTRL_AUDIOCTRL_EVENTGROUP_ID = 0x1150U;
constexpr uint16_t REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME = 0x1133U;
constexpr uint16_t REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME = 0x1144U;
constexpr uint16_t REMOTECTRL_AUDIOCTRL_EVENT_ID_VOLUMESTATUS = 0x1122U;

// RemoteCtrl_ClimateCtrl
constexpr uint16_t REMOTECTRL_CLIMATECTRL_SERVICE_ID = 0x1200U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_SERVICE_INSTANCE_ID = 0x1201U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_EVENTGROUP_ID = 0x1202U;

constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL = 0x1220U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL = 0x1221U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_EVENT_ID_FANLEVELCHANGED = 0x1222U;

constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE = 0x1270U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE = 0x1271U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_EVENT_ID_TEMPERATURECHANGED = 0x1210U;

constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE = 0x1223U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE = 0x1224U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_EVENT_ID_MAX_DEFROSTER_STATECHANGED = 0x1225U;

constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE = 0x1226U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE = 0x1227U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_EVENT_ID_AC_STATECHANGED = 0x1228U;

constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AIR_DISTRIBUTION = 0x1229U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AIR_DISTRIBUTION = 0x1230U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_EVENT_ID_AIR_DISTRIBUTIONCHANGED = 0x1231U;
// TODO (Abhi) Populate this file with all helpers and struct definitions needed for readable code until autogeneated
// header files from SDB extract are in place
//
enum class FcnAvailabilityStatus : uint8_t {
    SystemError = 0x00,
    NotPresent = 0x01,
    UserInputDisabled = 0x02,
    Available = 0x03
};

enum class Rows : uint8_t { Row_1 = 0x00, Row_2 = 0x01, Row_3 = 0x02, Row_Max = 0x03 };

enum class Seats : uint8_t { Left = 0x00, Center = 0x01, Right = 0x02, Max = 0x03 };

enum class FanLevel : uint8_t {
    FanLevel_0 = 0x00,
    FanLevel_1 = 0x01,
    FanLevel_2 = 0x02,
    FanLevel_3 = 0x03,
    FanLevel_4 = 0x04,
    FanLevel_5 = 0x05,
    FanLevel_Max = 0x06
};

enum class AirFlow : uint8_t {
    Automatic = 0x00,
    Floor = 0x01,
    Vent = 0x02,
    Floor_Vent = 0x03,
    Defrost = 0x04,
    Floor_Defrost = 0x05,
    Vent_Defrost = 0x06,
    Floor_Vent_Defrost = 0x07
};

constexpr uint8_t TEMPERATURE_MAX_VALUE = 22U;

inline void ValidateRow(const char* method_name, const uint8_t& row) {
    if (row >= static_cast<uint8_t>(Rows::Row_Max)) {
        throw RemoteCtrlParamRangeError(method_name, "Row", static_cast<uint8_t>(Rows::Row_Max), row);
    }
}

inline void ValidateSeat(const char* method_name, const uint8_t& seat) {
    if (seat >= static_cast<uint8_t>(Seats::Max)) {
        throw RemoteCtrlParamRangeError(method_name, "Seat", static_cast<uint8_t>(Seats::Max), seat);
    }
}

inline void ValidateRequstedFanLevel(const char* method_name, const uint8_t& fan_level) {
    if (fan_level > static_cast<uint8_t>(FanLevel::FanLevel_Max)) {
        throw RemoteCtrlParamRangeError(method_name, "FanLevel", 6, fan_level);
    }
}

inline void ValidateRequstedTemperature(const char* method_name, const uint8_t& temperature) {
    if (temperature > static_cast<uint8_t>(TEMPERATURE_MAX_VALUE)) {
        throw RemoteCtrlParamRangeError(method_name, "Temperature", TEMPERATURE_MAX_VALUE, temperature);
    }
}

inline void ValidateRequstedAirFlow(const char* method_name, const uint8_t& air_flow) {
    if (air_flow > static_cast<uint8_t>(AirFlow::Floor_Vent_Defrost)) {
        throw RemoteCtrlParamRangeError(
                method_name, "AirFlow", static_cast<uint8_t>(AirFlow::Floor_Vent_Defrost), air_flow);
    }
}

}  // namespace remotectrl
}  // namespace vcc
