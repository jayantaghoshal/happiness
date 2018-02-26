/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include <Application_dataelement.h>

class VFCVisibilityLogic
{
public:
    explicit VFCVisibilityLogic();

private:
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info>      VehModMngtGlbSafe1_;
    ApplicationDataElement::DEReceiver<autosar::Btn4ForUsrSwtPanFrntReq_info> bt4PanFrontReq_;
    ApplicationDataElement::DEReceiver<autosar::Btn5ForUsrSwtPanFrntReq_info> bt5PanFrontReq_;
};
