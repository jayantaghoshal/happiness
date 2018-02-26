/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "vfc_helper.h"

#include <chrono>
#include <algorithm>

void request_vfc(Vfc vfc, std::chrono::seconds time)
{
    auto const requestTime = std::min(static_cast<int>(time.count()), 60);

    static ApplicationDataElement::DESender<ActivateVfc_info> activateVfc;
    activateVfc.send({ vfc, static_cast<uint8_t>(requestTime)});
}
