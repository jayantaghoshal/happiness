/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "itemperature_logic.h"

#include <gmock/gmock.h>

class MockTemperatureLogic : public common::daemon::ITemperatureLogic {
  public:
    MOCK_METHOD2(request, void(double, autosar::HmiCmptmtTSpSpcl));
};
