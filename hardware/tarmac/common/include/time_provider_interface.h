/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <chrono>

namespace tarmac {
namespace timeprovider {

class TimerProviderInterface {
  public:
    virtual ~TimerProviderInterface() = default;

    virtual std::chrono::steady_clock::time_point steady_clock_now() const = 0;
};

}  // namespace timeprovider
}  // namespace tarmac
