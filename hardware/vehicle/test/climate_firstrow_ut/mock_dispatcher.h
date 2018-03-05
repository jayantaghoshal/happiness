/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "ilegacydispatcher.h"

#include <gmock/gmock.h>

/**
 * @class Mock Dispatcher
 */
class MockDispatcher : public ILegacyDispatcher {
  public:
    MOCK_METHOD3(Start, void(std::chrono::milliseconds, std::function<void(void)>, bool));

    MOCK_METHOD1(Startnow, void(std::function<void(void)>));

    MOCK_METHOD2(Restart, void(std::chrono::milliseconds, bool));

    MOCK_METHOD0(Cancel, void());

    MOCK_CONST_METHOD0(IsRunning, bool());
};
