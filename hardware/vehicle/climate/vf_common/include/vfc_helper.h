/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <chrono>
#include <Application_dataelement.h>

auto const VFC_TIMEOUT_DELAY_SEC = std::chrono::seconds{ 3 };

void request_vfc(Vfc vfc, std::chrono::seconds time = VFC_TIMEOUT_DELAY_SEC);
