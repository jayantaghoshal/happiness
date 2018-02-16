/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <cstdint>
#include "carconfig.h"

class CarConfigMock {
  public:
    CarConfigMock();
    ~CarConfigMock();

    MOCK_METHOD0(init, std::int32_t());
    MOCK_METHOD1(getValue, std::uint8_t(std::uint32_t));
    MOCK_METHOD1(getRawValue, std::uint8_t(std::uint32_t));
    MOCK_METHOD0(isConfigured, bool());
};
