/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "idefroster.h"

#include <gmock/gmock.h>

class MockDefroster : public IDefroster {
  public:
    MOCK_METHOD1(request, void(OnOff));
};
