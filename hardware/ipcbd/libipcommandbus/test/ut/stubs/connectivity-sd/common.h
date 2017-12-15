/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <systemd/sd-bus.h>
#include <systemd/sd-event.h>
#include <string>

namespace Connectivity {
int assert_sd_throw(const int sdStatus, const std::string& message);
}  // namespace Connectivity
