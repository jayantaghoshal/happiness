/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "stub_properties.h"

static int8_t stub_value = 0;

int8_t property_get_bool(const char* key, int8_t default_value) {
    if (key == nullptr || *key == '\0') {
        return default_value;
    }
    return stub_value;
}

void property_set_stub_value(int8_t value) { stub_value = value; }

void property_reset_stub_value(void) { stub_value = 0; }
