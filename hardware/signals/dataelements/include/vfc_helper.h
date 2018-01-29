/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <chrono>
#include "Application_dataelement.h"

namespace ApplicationDataElement {
namespace Helper {

auto const VFC_TIMEOUT_DELAY_SEC = std::chrono::seconds{3};
void request_vfc(Vfc vfc, std::chrono::seconds time = VFC_TIMEOUT_DELAY_SEC);

}  // namespace Helper
}  // namespace ApplicationDataElement