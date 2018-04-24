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

constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE = 0x1223U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE = 0x1224U;
constexpr uint16_t REMOTECTRL_CLIMATECTRL_EVENT_ID_MAX_DEFROSTER_STATECHANGED = 0x1225U;

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

}  // namespace remotectrl
}  // namespace vcc
