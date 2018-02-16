/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "carconfig_mock.h"
#include <cassert>

static CarConfigMock* carConfigMock;

CarConfigMock::CarConfigMock() {
    assert(carConfigMock == nullptr);
    carConfigMock = this;
}

CarConfigMock::~CarConfigMock() { carConfigMock = nullptr; }

namespace carconfig {
std::int32_t init() {
    assert(carConfigMock != nullptr);
    return carConfigMock->init();
}

std::uint8_t getValue(std::uint32_t parameterNumber) {
    assert(carConfigMock != nullptr);
    return carConfigMock->getValue(parameterNumber);
}

std::uint8_t getRawValue(std::uint32_t parameterNumber) {
    assert(carConfigMock != nullptr);
    return carConfigMock->getRawValue(parameterNumber);
}

bool isConfigured() {
    assert(carConfigMock != nullptr);
    return carConfigMock->isConfigured();
}
}  // namespace carconfig
