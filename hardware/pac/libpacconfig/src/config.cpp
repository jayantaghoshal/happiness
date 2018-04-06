/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <pacconfig/config.h>

#include <cstdint>

#include <cutils/properties.h>

namespace {

namespace system_property {
// Property to check if EVS services shall be started on boot
constexpr char const* kRunEvsOnBoot = "persist.vcc.service.evs.boot";
}  // namespace system_property

namespace property_value {
constexpr int8_t kBoolFalse = 0;
}  // namespace property_value

}  // namespace

namespace pac {
namespace config {

bool PacConfig::Evs::IsEvsServicesEnabled() {
    return static_cast<bool>(property_get_bool(system_property::kRunEvsOnBoot, property_value::kBoolFalse));
}

}  // namespace config
}  // namespace pac
