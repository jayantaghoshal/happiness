/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once
#include <systemd/sd-bus.h>
#include <systemd/sd-event.h>
#include <string>

namespace Connectivity {
int assert_sd_throw(const int sdStatus, const std::string& message);
}  // namespace Connectivity
