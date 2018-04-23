/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/automotive/vehicle/2.0/types.h>
#include "convapi_signals_def.h"

#include <utility>

namespace vcc {
namespace remotectrl {
namespace remoteclimatectrl {

namespace vhal_2_0 = ::android::hardware::automotive::vehicle::V2_0;

template <uint16_t I>
struct SomeIpProp {
    enum { PROP = I };
};

FcnAvailabilityStatus toConvApiStatus(const vhal_2_0::VehiclePropertyStatus& vhal_status);

std::pair<Rows, Seats> toRowAndPosition(const int32_t& area_id);

vhal_2_0::VehicleAreaZone toVhalAreaZone(const uint8_t& row, const uint8_t& pos);

}  // namespace remoteclimatectrl
}  // namespace remotectrl
}  // namespace vcc
