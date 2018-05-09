/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "itemperature_logic.h"

#include <gmock/gmock.h>

class MockTemperatureLogic : public common::daemon::ITemperatureLogic {
  public:
    MOCK_METHOD1(request, void(double));
    MOCK_METHOD1(request, void(autosar::HmiCmptmtTSpSpcl));
};
