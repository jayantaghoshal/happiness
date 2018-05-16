/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "convapi_signals_def.h"

namespace vcc {
namespace remotectrl {

struct RowSeatPair {
    Rows first;
    Seats second;
};

struct ZonedRowSeat {
    hidl_remotectrl::VehicleAreaZone first;
    RowSeatPair second;
};

// clang-format off
constexpr std::array<ZonedRowSeat, 9> area_row_seat_map = {{
           {hidl_remotectrl::VehicleAreaZone::ROW_1_LEFT, {Rows::Row_1, Seats::Left}},
           {hidl_remotectrl::VehicleAreaZone::ROW_1_CENTER, {Rows::Row_1, Seats::Center}},
           {hidl_remotectrl::VehicleAreaZone::ROW_1_RIGHT, {Rows::Row_1, Seats::Right}},
           {hidl_remotectrl::VehicleAreaZone::ROW_2_LEFT, {Rows::Row_2, Seats::Left}},
           {hidl_remotectrl::VehicleAreaZone::ROW_2_CENTER, {Rows::Row_2, Seats::Center}},
           {hidl_remotectrl::VehicleAreaZone::ROW_2_RIGHT, {Rows::Row_2, Seats::Right}},
           {hidl_remotectrl::VehicleAreaZone::ROW_3_LEFT, {Rows::Row_3, Seats::Left}},
           {hidl_remotectrl::VehicleAreaZone::ROW_3_CENTER, {Rows::Row_3, Seats::Center}},
           {hidl_remotectrl::VehicleAreaZone::ROW_3_RIGHT, {Rows::Row_3, Seats::Right}}
        }};
// clang-format on

hidl_remotectrl::VehicleAreaZone toVhalAreaZone(const uint8_t& row, const uint8_t& pos) {
    const auto& it =
            std::find_if(area_row_seat_map.cbegin(), area_row_seat_map.cend(), [row, pos](const ZonedRowSeat& item) {
                return static_cast<uint8_t>(item.second.first) == row &&
                       static_cast<uint8_t>(item.second.second) == pos;
            });
    if (it == area_row_seat_map.cend()) {
        throw RemoteCtrlConversionError(std::string("Row ") + std::to_string(row) + " Seat " + std::to_string(pos) +
                                        " do not map to VhalArea");
    }
    return it->first;
}

std::pair<Rows, Seats> toRowAndPosition(const int32_t& area_id) {
    const auto& it =
            std::find_if(area_row_seat_map.cbegin(), area_row_seat_map.cend(), [area_id](const ZonedRowSeat& item) {
                return area_id == static_cast<int32_t>(item.first);
            });
    if (it == area_row_seat_map.cend()) {
        throw RemoteCtrlConversionError("area_id: " + std::to_string(area_id) +
                                        " do not match to Conv API row and seat");
    }
    return {it->second.first, it->second.second};
}

hidl_remotectrl::RemoteCtrlHalPropertyValue RemoteCtrlSignal::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    if (msg_payload->get_length() != expected_length_) {
        throw RemoteCtrlMsgLengthError(method_name_, expected_length_, msg_payload->get_length());
    }
    hidl_remotectrl::RemoteCtrlHalPropertyValue prop_value{
            prop_,
            static_cast<int32_t>(hidl_remotectrl::VehicleAreaZone::ROW_1_CENTER),
            hidl_remotectrl::RemoteCtrlHalPropertyStatus::AVAILABLE,
            {}};
    return prop_value;
}

std::vector<vsomeip::byte_t> RemoteCtrlSignal::PackResponse(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    return {static_cast<vsomeip::byte_t>(prop_value.status)};
}

hidl_remotectrl::RemoteCtrlHalPropertyValue RemoteCtrlZonalSignal::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    if (msg_payload->get_length() != expected_length_) {
        throw RemoteCtrlMsgLengthError(method_name_, expected_length_, msg_payload->get_length());
    }
    hidl_remotectrl::RemoteCtrlHalPropertyValue prop_value{
            prop_,
            static_cast<int32_t>(hidl_remotectrl::VehicleAreaZone::ROW_4_RIGHT),
            hidl_remotectrl::RemoteCtrlHalPropertyStatus::AVAILABLE,
            {}};
    const auto row = msg_payload->get_data()[0];
    const auto pos = msg_payload->get_data()[1];
    ValidateRow(method_name_, row);
    ValidateSeat(method_name_, pos);
    prop_value.areaId = static_cast<int32_t>(toVhalAreaZone(row, pos));
    return prop_value;
}

std::vector<vsomeip::byte_t> RemoteCtrlZonalSignal::PackResponse(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    return {static_cast<vsomeip::byte_t>(prop_value.status)};
}

hidl_remotectrl::RemoteCtrlHalPropertyValue SetFanLevel::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    auto prop_value = RemoteCtrlZonalSignal::UnpackRequest(msg_payload);
    const auto fan_level = msg_payload->get_data()[2];
    ValidateRequstedFanLevel(method_name_, fan_level);
    prop_value.value.int32Values = android::hardware::hidl_vec<int32_t>{fan_level};
    return prop_value;
}

std::vector<vsomeip::byte_t> GetFanLevel::PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

std::vector<vsomeip::byte_t> NotifyFanLevel::PackNotification(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    auto row_pos_pair = toRowAndPosition(prop_value.areaId);
    payload_data.push_back(static_cast<vsomeip::byte_t>(row_pos_pair.first));
    payload_data.push_back(static_cast<vsomeip::byte_t>(row_pos_pair.second));
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

hidl_remotectrl::RemoteCtrlHalPropertyValue SetTemperature::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    auto prop_value = RemoteCtrlZonalSignal::UnpackRequest(msg_payload);
    const auto temperature = msg_payload->get_data()[2];
    ValidateRequstedTemperature(method_name_, temperature);
    float offset_temperature = 16.5f + temperature / 2.0f;
    prop_value.value.floatValues = android::hardware::hidl_vec<float>{offset_temperature};
    return prop_value;
}

std::vector<vsomeip::byte_t> GetTemperature::PackResponse(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.floatValues) {
        int temperature = (val - 16.5f) * 2;
        payload_data.push_back(static_cast<vsomeip::byte_t>(temperature));
    }
    return payload_data;
}

std::vector<vsomeip::byte_t> NotifyTemperature::PackNotification(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    auto row_pos_pair = toRowAndPosition(prop_value.areaId);
    payload_data.push_back(static_cast<vsomeip::byte_t>(row_pos_pair.first));
    payload_data.push_back(static_cast<vsomeip::byte_t>(row_pos_pair.second));
    for (const auto& val : prop_value.value.floatValues) {
        int temperature = (val - 16.5f) * 2;
        payload_data.push_back(static_cast<vsomeip::byte_t>(temperature));
    }
    return payload_data;
}

hidl_remotectrl::RemoteCtrlHalPropertyValue SetMaxDefrostState::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    auto prop_value = RemoteCtrlSignal::UnpackRequest(msg_payload);
    const auto defrost_state = msg_payload->get_data()[0];
    ValidateRequestedMaxDefrost(method_name_, defrost_state);
    prop_value.value.int32Values = android::hardware::hidl_vec<int32_t>{defrost_state};
    return prop_value;
}

std::vector<vsomeip::byte_t> GetMaxDefrostState::PackResponse(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

std::vector<vsomeip::byte_t> NotifyMaxDefrostState::PackNotification(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

hidl_remotectrl::RemoteCtrlHalPropertyValue SetACState::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    auto prop_value = RemoteCtrlSignal::UnpackRequest(msg_payload);
    const auto ac_state = msg_payload->get_data()[0];
    ValidateRequestedACState(method_name_, ac_state);
    prop_value.value.int32Values = android::hardware::hidl_vec<int32_t>{ac_state};
    return prop_value;
}

std::vector<vsomeip::byte_t> GetACState::PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

std::vector<vsomeip::byte_t> NotifyACState::PackNotification(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

hidl_remotectrl::RemoteCtrlHalPropertyValue SetAirDistribution::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    auto prop_value = RemoteCtrlSignal::UnpackRequest(msg_payload);
    const auto air_flow = msg_payload->get_data()[0];
    ValidateRequstedAirFlow(method_name_, air_flow);
    prop_value.value.int32Values = android::hardware::hidl_vec<int32_t>{air_flow};
    return prop_value;
}

std::vector<vsomeip::byte_t> GetAirDistribution::PackResponse(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

std::vector<vsomeip::byte_t> NotifyAirDistribution::PackNotification(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

hidl_remotectrl::RemoteCtrlHalPropertyValue SetVolume::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    auto prop_value = RemoteCtrlSignal::UnpackRequest(msg_payload);
    // const auto context = msg_payload->get_data()[0];
    const auto volume = msg_payload->get_data()[1];
    // TODO (Abhi) validateContext
    ValidateRequstedVolume(method_name_, volume);
    prop_value.value.int32Values = android::hardware::hidl_vec<int32_t>{volume};
    return prop_value;
}

std::vector<vsomeip::byte_t> GetVolume::PackResponse(const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

std::vector<vsomeip::byte_t> NotifyVolume::PackNotification(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status), 0x01U};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

hidl_remotectrl::RemoteCtrlHalPropertyValue SetStreamPlayBackStatus::UnpackRequest(
        const std::shared_ptr<vsomeip::payload>& msg_payload) {
    auto prop_value = RemoteCtrlSignal::UnpackRequest(msg_payload);
    const auto playback_status = msg_payload->get_data()[0];
    ValidateStreamPlayBackStatus(method_name_, playback_status);
    prop_value.value.int32Values = android::hardware::hidl_vec<int32_t>{playback_status};
    return prop_value;
}

std::vector<vsomeip::byte_t> NotifyStreamPlayBackStatus::PackNotification(
        const hidl_remotectrl::RemoteCtrlHalPropertyValue& prop_value) {
    std::vector<vsomeip::byte_t> payload_data{static_cast<vsomeip::byte_t>(prop_value.status)};
    for (const auto& val : prop_value.value.int32Values) {
        payload_data.push_back(static_cast<vsomeip::byte_t>(val));
    }
    return payload_data;
}

}  // namespace remotectrl
}  // namespace vcc
