/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstdint>
#include "exceptions.h"

#include <vendor/volvocars/hardware/remotectrl/1.0/types.h>
#include <vsomeip/vsomeip.hpp>

namespace vcc {
namespace remotectrl {

namespace hidl_remotectrl = ::vendor::volvocars::hardware::remotectrl::V1_0;

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

enum class MessageType { GET, SET, NOTIFICATION };
struct BaseProp {
    virtual ~BaseProp() = default;
    virtual std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue> RemoteCtrlHalPropertyReq(
            const std::shared_ptr<vsomeip::payload>& /*msg_payload*/) {
        return {};
    };

    virtual std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>> CreateSomeIpResponse(
            const MessageType& /*type*/,
            const hidl_remotectrl::RemoteCtrlHalPropertyValue& /*propValue*/) {
        return {};
    };
};

template <int I>
struct RemoteProp : BaseProp {
    enum { PROP = I };
    std::pair<MessageType, hidl_remotectrl::RemoteCtrlHalPropertyValue> RemoteCtrlHalPropertyReq(
            const std::shared_ptr<vsomeip::payload>& msg_payload) override;
};

template <int I>
struct RemoteCtrlHalProp : BaseProp {
    enum { PROP = I };
    std::pair<vsomeip::method_t, std::vector<vsomeip::byte_t>> CreateSomeIpResponse(
            const MessageType& type,
            const hidl_remotectrl::RemoteCtrlHalPropertyValue& propValue) override;
};

struct RemoteCtrlSignal {
    RemoteCtrlSignal(const char* method_name,
                     uint32_t expected_length,
                     const hidl_remotectrl::RemoteCtrlHalProperty& prop)
        : method_name_(method_name), expected_length_(expected_length), prop_(prop) {}
    virtual ~RemoteCtrlSignal() = default;
    virtual hidl_remotectrl::RemoteCtrlHalPropertyValue UnpackRequest(
            const std::shared_ptr<vsomeip::payload>& msg_payload);
    virtual std::vector<vsomeip::byte_t> PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);

    const char* method_name_;
    const uint32_t expected_length_;
    const hidl_remotectrl::RemoteCtrlHalProperty prop_;
};

struct RemoteCtrlZonalSignal {
    RemoteCtrlZonalSignal(const char* method_name,
                          uint32_t expected_len,
                          const hidl_remotectrl::RemoteCtrlHalProperty& prop)
        : method_name_(method_name), expected_length_(expected_len), prop_(prop) {}
    virtual ~RemoteCtrlZonalSignal() = default;
    virtual hidl_remotectrl::RemoteCtrlHalPropertyValue UnpackRequest(
            const std::shared_ptr<vsomeip::payload>& msg_payload);
    virtual std::vector<vsomeip::byte_t> PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);

    const char* method_name_;
    const uint32_t expected_length_;
    const hidl_remotectrl::RemoteCtrlHalProperty prop_;
};

enum class Rows : uint8_t { Row_1 = 0x00, Row_2 = 0x01, Row_3 = 0x02, Row_Max = 0x03 };
inline void ValidateRow(const char* method_name, const uint8_t& row) {
    if (row >= static_cast<uint8_t>(Rows::Row_Max)) {
        throw RemoteCtrlParamRangeError(method_name, "Row", static_cast<uint8_t>(Rows::Row_Max), row);
    }
}

enum class Seats : uint8_t { Left = 0x00, Center = 0x01, Right = 0x02, Max = 0x03 };
inline void ValidateSeat(const char* method_name, const uint8_t& seat) {
    if (seat >= static_cast<uint8_t>(Seats::Max)) {
        throw RemoteCtrlParamRangeError(method_name, "Seat", static_cast<uint8_t>(Seats::Max), seat);
    }
}

inline void ValidateRequstedFanLevel(const char* method_name, const uint8_t& fan_level) {
    if (fan_level > static_cast<uint8_t>(hidl_remotectrl::FanLevel::FANLEVEL_6)) {
        throw RemoteCtrlParamRangeError(
                method_name, "FanLevel", static_cast<uint8_t>(hidl_remotectrl::FanLevel::FANLEVEL_6), fan_level);
    }
}
struct GetFanLevel : RemoteCtrlZonalSignal {
    GetFanLevel()
        : RemoteCtrlZonalSignal("GET_FANLEVEL",
                                0x02U,
                                hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_FAN_SPEED) {}
    std::vector<vsomeip::byte_t> PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);
    // NOTE: inherits default implementation for UnpackRequest
};
struct SetFanLevel : RemoteCtrlZonalSignal {
    SetFanLevel()
        : RemoteCtrlZonalSignal("SET_FANLEVEL",
                                0x03U,
                                hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_FAN_SPEED) {}
    hidl_remotectrl::RemoteCtrlHalPropertyValue UnpackRequest(
            const std::shared_ptr<vsomeip::payload>& msg_payload) override;
    // NOTE: inherits default implementation for PackResponse
};
struct NotifyFanLevel {
    std::vector<vsomeip::byte_t> PackNotification(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);
};

constexpr uint8_t TEMPERATURE_MAX_VALUE = 22U;
inline void ValidateRequstedTemperature(const char* method_name, const uint8_t& temperature) {
    if (temperature > static_cast<uint8_t>(TEMPERATURE_MAX_VALUE)) {
        throw RemoteCtrlParamRangeError(method_name, "Temperature", TEMPERATURE_MAX_VALUE, temperature);
    }
}
struct GetTemperature : RemoteCtrlZonalSignal {
    GetTemperature()
        : RemoteCtrlZonalSignal("GET_TEMPERATURE",
                                0x02U,
                                hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_TEMPERATURE) {}
    std::vector<vsomeip::byte_t> PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);
    // NOTE: inherits default implementation for UnpackRequest
};
struct SetTemperature : RemoteCtrlZonalSignal {
    SetTemperature()
        : RemoteCtrlZonalSignal("SET_TEMPERATURE",
                                0x03U,
                                hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_TEMPERATURE) {}
    hidl_remotectrl::RemoteCtrlHalPropertyValue UnpackRequest(
            const std::shared_ptr<vsomeip::payload>& msg_payload) override;
    // NOTE: inherits default implementation for PackResponse
};
struct NotifyTemperature {
    std::vector<vsomeip::byte_t> PackNotification(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);
};

// MaxDefrostState messaging
inline void ValidateRequestedMaxDefrost(const char* method_name, const uint8_t& max_defrost_state) {
    if (max_defrost_state > static_cast<uint8_t>(hidl_remotectrl::MaxDefrostState::ON)) {
        throw RemoteCtrlParamRangeError(method_name,
                                        "MaxDefrostState",
                                        static_cast<uint8_t>(hidl_remotectrl::MaxDefrostState::ON),
                                        max_defrost_state);
    }
}
struct GetMaxDefrostState : RemoteCtrlSignal {
    GetMaxDefrostState()
        : RemoteCtrlSignal("GET_MAX_DEFROSTER_STATE",
                           0x00U,
                           hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_MAX_DEFROST_ON) {}
    std::vector<vsomeip::byte_t> PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);
    // NOTE: inherits default implementation for UnpackRequest
};
struct SetMaxDefrostState : RemoteCtrlSignal {
    SetMaxDefrostState()
        : RemoteCtrlSignal("SET_MAX_DEFROSTER_STATE",
                           0x01U,
                           hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_MAX_DEFROST_ON) {}
    hidl_remotectrl::RemoteCtrlHalPropertyValue UnpackRequest(
            const std::shared_ptr<vsomeip::payload>& msg_payload) override;
    // NOTE: inherits default implementation for PackResponse
};
struct NotifyMaxDefrostState {
    std::vector<vsomeip::byte_t> PackNotification(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);
};

// AirConditionState messaging
inline void ValidateRequestedACState(const char* method_name, const uint8_t& ac_defrost_state) {
    if (ac_defrost_state > static_cast<uint8_t>(hidl_remotectrl::ACState::ON)) {
        throw RemoteCtrlParamRangeError(
                method_name, "ACState", static_cast<uint8_t>(hidl_remotectrl::ACState::ON), ac_defrost_state);
    }
}
struct GetACState : RemoteCtrlSignal {
    GetACState()
        : RemoteCtrlSignal("GET_AC_STATE", 0x00U, hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_AC_ON) {}
    std::vector<vsomeip::byte_t> PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) override;
    // NOTE: inherits default implementation for UnpackRequest
};
struct SetACState : RemoteCtrlSignal {
    SetACState()
        : RemoteCtrlSignal("SET_AC_STATE", 0x01U, hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_AC_ON) {}
    hidl_remotectrl::RemoteCtrlHalPropertyValue UnpackRequest(
            const std::shared_ptr<vsomeip::payload>& msg_payload) override;
    // NOTE: inherits default implementation for PackResponse
};
struct NotifyACState {
    std::vector<vsomeip::byte_t> PackNotification(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);
};

// Air Distribution messaging
inline void ValidateRequstedAirFlow(const char* method_name, const uint8_t& air_flow) {
    if (air_flow > static_cast<uint8_t>(hidl_remotectrl::AirFlow::FLOOR_VENT_DEFROST)) {
        throw RemoteCtrlParamRangeError(
                method_name, "AirFlow", static_cast<uint8_t>(hidl_remotectrl::AirFlow::FLOOR_VENT_DEFROST), air_flow);
    }
}
struct GetAirDistribution : RemoteCtrlSignal {
    GetAirDistribution()
        : RemoteCtrlSignal("GET_FAN_DIRECTION",
                           0x00U,
                           hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_FAN_DIRECTION) {}
    std::vector<vsomeip::byte_t> PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) override;
    // NOTE: inherits default implementation for UnpackRequest
};
struct SetAirDistribution : RemoteCtrlSignal {
    SetAirDistribution()
        : RemoteCtrlSignal("SET_FAN_DIRECTION",
                           0x01U,
                           hidl_remotectrl::RemoteCtrlHalProperty::REMOTECTRLHAL_HVAC_FAN_DIRECTION) {}
    hidl_remotectrl::RemoteCtrlHalPropertyValue UnpackRequest(
            const std::shared_ptr<vsomeip::payload>& msg_payload) override;
    // NOTE: inherits default implementation for PackResponse
};
struct NotifyAirDistribution {
    std::vector<vsomeip::byte_t> PackNotification(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value);
};

}  // namespace remotectrl
}  // namespace vcc
