/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "papropertyhandler.h"

namespace PaPropHandlerHelper {
vhal20::VehiclePropConfig BoolConfig(vccvhal10::VehicleProperty property) {
    return VhalPropertyHandlerHelper::BoolConfig(property);
}
}  // namespace PaPropHandlerHelper
