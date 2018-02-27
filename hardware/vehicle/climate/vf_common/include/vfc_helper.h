/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <Application_dataelement.h>
#include <chrono>

auto const VFC_TIMEOUT_DELAY_SEC = std::chrono::seconds{3};

void request_vfc(Vfc vfc, std::chrono::seconds time = VFC_TIMEOUT_DELAY_SEC);
