/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include <Application_dataelement.h>
#include <mutex>

const std::string LCFG_ClimateDefrosterTimeout("Climate_defroster_timeout");

class ElectricDefrosterProxy {
  public:
    static ElectricDefrosterProxy* getInstance();

    void updateElectricDefrosterRear(autosar::ActrReq rear, autosar::ActrReq mirror);
    void updateElectricDefrosterWindscreen(autosar::ActrReq front);

  private:
    std::mutex mutex_;

    autosar::ActrReq front_;
    autosar::ActrReq rear_;
    autosar::ActrReq mirror_;

    ApplicationDataElement::DESender<autosar::HmiDefrstElecReq_info> hmiDefrosterRequest;

    ElectricDefrosterProxy();
    void send();
};
