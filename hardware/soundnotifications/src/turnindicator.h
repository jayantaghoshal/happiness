/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "Application_dataelement.h"
using namespace ApplicationDataElement;
using namespace autosar;

namespace SoundNotifications {
class TurnIndicator {
  public:
    TurnIndicator();

  private:
    void injectSignals(DataElemValue<autosar::IndcrDisp1WdSts_info> indcr,
                       DataElemValue<autosar::FltIndcrTurnLeFrnt_info> FltIndcrTurnLeFrnt,
                       DataElemValue<autosar::FltIndcrTurnLeRe_info> FltIndcrTurnLeRe,
                       DataElemValue<autosar::FltIndcrTurnRiFrnt_info> FltIndcrTurnRiFrnt,
                       DataElemValue<autosar::FltIndcrTurnRiRe_info> FltIndcrTurnRiRe);

    ApplicationDataElement::DEReceiver<autosar::IndcrDisp1WdSts_info> indcrRecevier;
    ApplicationDataElement::DEReceiver<autosar::FltIndcrTurnLeFrnt_info> fltIndLeFrontReceiver;
    ApplicationDataElement::DEReceiver<autosar::FltIndcrTurnLeRe_info> fltIndLeReReceiver;
    ApplicationDataElement::DEReceiver<autosar::FltIndcrTurnRiFrnt_info> fltIndRiFrontReceiver;
    ApplicationDataElement::DEReceiver<autosar::FltIndcrTurnRiRe_info> fltIndRiReReceiver;

    autosar::IndcrSts1 previous_IndcrDisp1WdSts;
};
}
