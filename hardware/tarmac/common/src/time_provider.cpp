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

    ~SubscriptionHandle() override { dispatcher_->Cancel(timer_id_); }

  private:
    const IDispatcher::JobId timer_id_;
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher_;
};
SubscriptionHandle::SubscriptionHandle(IDispatcher::JobId timer_id,
                                       std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher)
    : timer_id_(timer_id), dispatcher_(std::move(dispatcher)) {}

TimeProvider::TimeProvider(std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher)
    : dispatcher_(std::move(dispatcher)) {}

std::chrono::steady_clock::time_point TimeProvider::steady_clock_now() const {
    return std::chrono::steady_clock::now();
}

std::unique_ptr<TimerSubscriptionHandle> TimeProvider::AddSingleShotTimer(std::chrono::milliseconds delay,
                                                                          std::function<void()> func) {
    IDispatcher::JobId timer_id = dispatcher_->EnqueueWithDelay(delay, std::move(func));
    auto rval = std::make_unique<SubscriptionHandle>(timer_id, dispatcher_);
    return std::unique_ptr<TimerSubscriptionHandle>(rval.release());  //"static_pointer_cast" for unique_ptr
}

std::unique_ptr<TimerSubscriptionHandle> TimeProvider::AddPeriodicTimer(std::chrono::milliseconds time,
                                                                        std::function<void()> func) {
    IDispatcher::JobId timer_id = dispatcher_->EnqueueWithDelayCyclic(time, std::move(func));
    auto rval = std::make_unique<SubscriptionHandle>(timer_id, dispatcher_);
    return std::unique_ptr<TimerSubscriptionHandle>(rval.release());  //"static_pointer_cast" for unique_ptr
}

}  // namespace timeprovider
}  // namespace tarmac
