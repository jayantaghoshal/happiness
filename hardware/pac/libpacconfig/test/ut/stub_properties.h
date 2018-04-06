/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstdint>

extern "C" {
int8_t property_get_bool(const char* key, int8_t default_value);

void property_set_stub_value(int8_t value);

void property_reset_stub_value(void);
}
