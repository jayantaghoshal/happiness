/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vfc_helper.h"

#include <algorithm>
#include <chrono>

namespace ApplicationDataElement {
namespace Helper {

void request_vfc(Vfc vfc, std::chrono::seconds time) {
    auto const requestTime = std::min(static_cast<int>(time.count()), 60);

    static DESender<ActivateVfc_info> activateVfc;
    activateVfc.send({vfc, static_cast<uint8_t>(requestTime)});
}
}  // namespace Helper
}  // namespace ApplicationDataElement