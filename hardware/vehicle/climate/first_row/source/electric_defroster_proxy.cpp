/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "electric_defroster_proxy.h"

ElectricDefrosterProxy* ElectricDefrosterProxy::getInstance() {
    static ElectricDefrosterProxy* instance_ = nullptr;
    if (nullptr == instance_) {
        instance_ = new ElectricDefrosterProxy();
    }
    return instance_;
}

ElectricDefrosterProxy::ElectricDefrosterProxy()
    : front_(autosar::ActrReq::Off), rear_(autosar::ActrReq::Off), mirror_(autosar::ActrReq::Off) {}

void ElectricDefrosterProxy::updateElectricDefrosterRear(autosar::ActrReq rear, autosar::ActrReq mirror) {
    std::lock_guard<std::mutex> locker(mutex_);

    rear_ = rear;
    mirror_ = mirror;
    send();
}

void ElectricDefrosterProxy::updateElectricDefrosterWindscreen(autosar::ActrReq front) {
    std::lock_guard<std::mutex> locker(mutex_);

    front_ = front;
    send();
}

void ElectricDefrosterProxy::send() {
    autosar::HmiDefrstElecReq request{front_, rear_, mirror_};
    hmiDefrosterRequest.send(request);
}
