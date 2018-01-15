/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "IDispatcher.h"
#include "itime_provider.h"

#include <chrono>
#include <functional>
#include <memory>

namespace tarmac {
namespace timeprovider {

//
// NOTE:
//  TimeProvider underneath uses Dispatcher-class as dispatcher for its timers.
//
class TimeProvider final : public ITimeProvider {
  public:
    TimeProvider(std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher);
    std::chrono::steady_clock::time_point steady_clock_now() const override;
    std::chrono::system_clock::time_point system_clock_now() const override;
    std::unique_ptr<TimerSubscriptionHandle> add_single_shot_timer(std::chrono::milliseconds time,
                                                                   std::function<void()> func) override;
    std::unique_ptr<TimerSubscriptionHandle> add_periodic_timer(std::chrono::milliseconds time,
                                                                std::function<void()> func) override;

  private:
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher_;
};
}  // namespace timeprovider
}  // namespace tarmac