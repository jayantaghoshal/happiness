/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "time_provider.h"

#include "IDispatcher.h"

using namespace tarmac::eventloop;

class SubscriptionHandle final : public TimerSubscriptionHandle {
  public:
    explicit SubscriptionHandle(IDispatcher::JobId timer_id);
    ~SubscriptionHandle() { IDispatcher::GetDefaultDispatcher().Cancel(timer_id_); }

  private:
    const IDispatcher::JobId timer_id_;
};
SubscriptionHandle::SubscriptionHandle(IDispatcher::JobId timer_id) : timer_id_(timer_id) {}

// TimeProvider::TimeProvider() : ITimeProvider() {}

std::chrono::steady_clock::time_point TimeProvider::steady_clock_now() const {
    return std::chrono::steady_clock::now();
}

std::chrono::system_clock::time_point TimeProvider::system_clock_now() const {
    return std::chrono::system_clock::now();
}

std::unique_ptr<TimerSubscriptionHandle> TimeProvider::add_single_shot_timer(std::chrono::milliseconds time,
                                                                             std::function<void()> func) {
    IDispatcher::JobId timer_id = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(time, std::move(func));
    auto rval = std::make_unique<SubscriptionHandle>(timer_id);
    return std::unique_ptr<TimerSubscriptionHandle>(rval.release());  //"static_pointer_cast" for unique_ptr
}

std::unique_ptr<TimerSubscriptionHandle> TimeProvider::add_periodic_timer(std::chrono::milliseconds time,
                                                                          std::function<void()> func) {
    IDispatcher::JobId timer_id = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(time, std::move(func), true);
    auto rval = std::make_unique<SubscriptionHandle>(timer_id);
    return std::unique_ptr<TimerSubscriptionHandle>(rval.release());  //"static_pointer_cast" for unique_ptr
}
