/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <chrono>
#include <functional>
#include <memory>
#include "IDispatcher.h"
#include "timer_manager_interface.h"

namespace tarmac {
namespace timeprovider {

//
// NOTE:
//  TimeProvider underneath uses Dispatcher-class as dispatcher for its timers.
//
class TimeProvider final : public TimerManagerInterface {
  public:
    TimeProvider(std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher);
    std::chrono::steady_clock::time_point steady_clock_now() const override;

    std::unique_ptr<TimerSubscriptionHandle> AddSingleShotTimer(std::chrono::milliseconds delay,
                                                                std::function<void()> func) override;
    std::unique_ptr<TimerSubscriptionHandle> AddPeriodicTimer(std::chrono::milliseconds time,
                                                              std::function<void()> func) override;

  private:
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher_;
};
}  // namespace timeprovider
}  // namespace tarmac
