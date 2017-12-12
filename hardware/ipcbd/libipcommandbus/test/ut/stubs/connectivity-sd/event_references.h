/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <systemd/sd-event.h>
#include <memory>

namespace Connectivity {
struct SdEventUnrefer {
    void operator()(sd_event* event) { sd_event_unref(event); }
};
using SdEventReference = std::unique_ptr<sd_event, SdEventUnrefer>;

struct SdEventSourceUnrefer {
    void operator()(sd_event_source* source) { sd_event_source_unref(source); }
};
using SdEventSourceReference = std::unique_ptr<sd_event_source, SdEventSourceUnrefer>;

}  // namespace Connectivity
