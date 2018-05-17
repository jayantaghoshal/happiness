/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <cutils/properties.h>
#include "property_names.h"

int stub_property_set(const char* key, const char* value);
void stub_property_reset();