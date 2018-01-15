/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "time_provider.h"

namespace tarmac {
namespace timeprovider {

using namespace ::tarmac::eventloop;

class SubscriptionHandle final : public TimerSubscriptionHandle {
  public:
    explicit SubscriptionHandle(IDispatcher::JobId timer_id,
                                std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher);

    ~SubscriptionHandle() { dispatcher_->Cancel(timer_id_); }

  private:
    const IDispatcher::JobId timer_id_;
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher_;
};
SubscriptionHandle::SubscriptionHandle(IDispatcher::JobId timer_id,
                                       std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher)
    : timer_id_(timer_id), dispatcher_(dispatcher) {}

TimeProvider::TimeProvider(std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher) : dispatcher_(dispatcher) {}

std::chrono::steady_clock::time_point TimeProvider::steady_clock_now() const {
    return std::chrono::steady_clock::now();
}

std::chrono::system_clock::time_point TimeProvider::system_clock_now() const {
    return std::chrono::system_clock::now();
}

std::unique_ptr<TimerSubscriptionHandle> TimeProvider::add_single_shot_timer(std::chrono::milliseconds time,
                                                                             std::function<void()> func) {
    IDispatcher::JobId timer_id = dispatcher_->EnqueueWithDelay(time, std::move(func));
    auto rval = std::make_unique<SubscriptionHandle>(timer_id, dispatcher_);
    return std::unique_ptr<TimerSubscriptionHandle>(rval.release());  //"static_pointer_cast" for unique_ptr
}

std::unique_ptr<TimerSubscriptionHandle> TimeProvider::add_periodic_timer(std::chrono::milliseconds time,
                                                                          std::function<void()> func) {
    IDispatcher::JobId timer_id = dispatcher_->EnqueueWithDelay(time, std::move(func), true);
    auto rval = std::make_unique<SubscriptionHandle>(timer_id, dispatcher_);
    return std::unique_ptr<TimerSubscriptionHandle>(rval.release());  //"static_pointer_cast" for unique_ptr
}

}  // namespace timeprovider
}  // namespace tarmac
