#pragma once
#include "itime_provider.h"

#include <chrono>
#include <functional>
#include <memory>
//
// NOTE:
//  TimeProvider underneath uses Dispatcher-class as dispatcher for its timers.
//
class TimeProvider final : public ITimeProvider {
  public:
    std::chrono::steady_clock::time_point steady_clock_now() const override;
    std::chrono::system_clock::time_point system_clock_now() const override;
    std::unique_ptr<TimerSubscriptionHandle> add_single_shot_timer(std::chrono::milliseconds time,
                                                                   std::function<void()> func) override;
    std::unique_ptr<TimerSubscriptionHandle> add_periodic_timer(std::chrono::milliseconds time,
                                                                std::function<void()> func) override;
};
