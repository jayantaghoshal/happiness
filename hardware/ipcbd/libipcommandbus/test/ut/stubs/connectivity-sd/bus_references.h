/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <systemd/sd-bus.h>
#include <memory>
#include <string>

namespace Connectivity {
struct SdBusCleaner {
    void operator()(sd_bus* sd_bus) { sd_bus_unref(sd_bus); }
};

struct SdBusSlotCleaner {
    void operator()(sd_bus_slot* sd_bus_slot) { sd_bus_slot_unref(sd_bus_slot); }
};

}  // namespace Connectivity
