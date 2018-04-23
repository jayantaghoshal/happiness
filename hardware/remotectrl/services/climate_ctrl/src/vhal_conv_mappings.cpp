/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vhal_conv_mappings.h"
#include "climate_ctrl_service.h"
#include "convapi_signals_def.h"

#include <vsomeip/vsomeip.hpp>

#include <unordered_map>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_ClimateCtrl"
#include <cutils/log.h>

using namespace vcc::remotectrl::remoteclimatectrl;
using namespace vcc::remotectrl;
using namespace vhal_2_0;

namespace {

struct RowSeatPair {
    Rows first;
    Seats second;
};

struct ZonedRowSeat {
    vhal_2_0::VehicleAreaZone first;
    RowSeatPair second;
};

// clang-format off
constexpr std::array<ZonedRowSeat, 9> area_row_seat_map = {{
           {vhal_2_0::VehicleAreaZone::ROW_1_LEFT, {Rows::Row_1, Seats::Left}},
           {vhal_2_0::VehicleAreaZone::ROW_1_CENTER, {Rows::Row_1, Seats::Center}},
           {vhal_2_0::VehicleAreaZone::ROW_1_RIGHT, {Rows::Row_1, Seats::Right}},
           {vhal_2_0::VehicleAreaZone::ROW_2_LEFT, {Rows::Row_2, Seats::Left}},
           {vhal_2_0::VehicleAreaZone::ROW_2_CENTER, {Rows::Row_2, Seats::Center}},
           {vhal_2_0::VehicleAreaZone::ROW_2_RIGHT, {Rows::Row_2, Seats::Right}},
           {vhal_2_0::VehicleAreaZone::ROW_3_LEFT, {Rows::Row_3, Seats::Left}},
           {vhal_2_0::VehicleAreaZone::ROW_3_CENTER, {Rows::Row_3, Seats::Center}},
           {vhal_2_0::VehicleAreaZone::ROW_3_RIGHT, {Rows::Row_3, Seats::Right}}
        }};
// clang-format on

}  // namespace

namespace vcc {
namespace remotectrl {
namespace remoteclimatectrl {
FcnAvailabilityStatus toConvApiStatus(const vhal_2_0::VehiclePropertyStatus& vhal_status) {
    // TODO (Abhi) there is no 1:1 mapping. Need to discuss and agree with Team Ragnarok. URGENT!!!
    switch (vhal_status) {
        case vhal_2_0::VehiclePropertyStatus::AVAILABLE:
            return FcnAvailabilityStatus::Available;
        case vhal_2_0::VehiclePropertyStatus::UNAVAILABLE:
            return FcnAvailabilityStatus::UserInputDisabled;
        case vhal_2_0::VehiclePropertyStatus::ERROR:
            return FcnAvailabilityStatus::SystemError;
    }
    // return 0x01U; // Unclear what to map NOT_PRESENT to?
}

vhal_2_0::VehicleAreaZone toVhalAreaZone(const uint8_t& row, const uint8_t& pos) {
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

}  // namespace remoteclimatectrl
}  // namespace remotectrl
}  // namespace vcc
