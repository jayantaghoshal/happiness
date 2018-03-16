/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "time_provider_stub.h"

#include <algorithm>
#include <iterator>
#include <memory>

using namespace tarmac::timeprovider;

std::chrono::steady_clock::time_point TimeProviderStub::steady_clock_now() const { return steady_now; }

void TimeProviderStub::sleep_for(const std::chrono::milliseconds duration) {
    const auto end_time = steady_now + duration;

    {
        std::unique_lock<std::recursive_mutex> lock(timersMutex);

        std::list<InternalSubscriptionHandle>::iterator i = timers.begin();
        while (i != std::end(timers)) {
            if (i->fire_at > end_time) break;
            steady_now = i->fire_at;

            {
                std::shared_ptr<SubscriptionData> x = i->meta.lock();
                if (x != nullptr) {
                    x->fire();
                }
            }
            i = timers.erase(i);
        }
    }

    steady_now = end_time;
}

std::unique_ptr<tarmac::timeprovider::TimerSubscriptionHandle> TimeProviderStub::AddSingleShotTimer(
        std::chrono::milliseconds time,
        std::function<void()> func) {
    if (time < std::chrono::milliseconds(0)) {
        throw std::runtime_error("invalid duration");
        // return nullptr;
    }
    if (func == nullptr) {
        throw std::runtime_error("No func set");
        // return nullptr;
    }

    {
        std::unique_lock<std::recursive_mutex> lock(timersMutex);
        std::chrono::steady_clock::time_point fireAtTime = steady_clock_now() + time;

        auto insertion_point =
                std::find_if(std::begin(timers), std::end(timers), [fireAtTime](const InternalSubscriptionHandle& sub) {
                    return (sub.fire_at >= fireAtTime);
                });

        std::shared_ptr<SubscriptionData> meta = std::make_shared<SubscriptionData>(func);
        std::list<InternalSubscriptionHandle>::iterator insertedIt =
                timers.emplace(insertion_point, fireAtTime, std::weak_ptr<SubscriptionData>(meta));
        (void)insertedIt;
        std::unique_ptr<tarmac::timeprovider::TimerSubscriptionHandle> rval(new UserSubscriptionHandle(meta));
        return rval;
    }
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InternalSubscriptionHandle
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TimeProviderStub::InternalSubscriptionHandle::InternalSubscriptionHandle(
        std::chrono::steady_clock::time_point fire_at,
        std::weak_ptr<TimeProviderStub::SubscriptionData> meta)
    : fire_at(fire_at), meta(meta) {}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UserSubscriptionHandle
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TimeProviderStub::UserSubscriptionHandle::UserSubscriptionHandle(
        std::shared_ptr<TimeProviderStub::SubscriptionData> meta)
    : TimerSubscriptionHandle(), meta(meta) {}

TimeProviderStub::UserSubscriptionHandle::~UserSubscriptionHandle() { meta->unsubscribe(); }

TimeProviderStub::SubscriptionData::SubscriptionData(std::function<void()> func) : func(func) {}

void TimeProviderStub::SubscriptionData::fire() {
    std::unique_lock<std::recursive_mutex> lock(fireMutex);
    if (expired) return;
    func();
}

void TimeProviderStub::SubscriptionData::unsubscribe() {
    std::unique_lock<std::recursive_mutex> lock(fireMutex);
    expired = true;
}

std::unique_ptr<tarmac::timeprovider::TimerSubscriptionHandle> TimeProviderStub::AddPeriodicTimer(
        std::chrono::milliseconds interval,
        std::function<void()> func) {
    std::shared_ptr<ProxyHandle> proxy = std::make_shared<TimeProviderStub::ProxyHandle>(interval, func, *this);
    std::unique_ptr<PeriodicTimerSubscriptionHandle> rval(new PeriodicTimerSubscriptionHandle(proxy));
    proxy->handle = AddSingleShotTimer(interval,
                                       std::bind(&TimeProviderStub::ProxyHandle::executeAndRegisterNextPeriod, proxy));
    return rval;
}

TimeProviderStub::PeriodicTimerSubscriptionHandle::PeriodicTimerSubscriptionHandle(
        std::shared_ptr<TimeProviderStub::ProxyHandle> handle)
    : handle(handle) {}

TimeProviderStub::PeriodicTimerSubscriptionHandle::~PeriodicTimerSubscriptionHandle() { handle->handle.reset(nullptr); }

TimeProviderStub::ProxyHandle::ProxyHandle(std::chrono::milliseconds interval,
                                           std::function<void()> func,
                                           tarmac::timeprovider::TimerManagerInterface& timeProvider)
    : interval(interval), func(func), timeProvider(timeProvider) {}

void TimeProviderStub::ProxyHandle::executeAndRegisterNextPeriod() {
    // NOTE: need to keep ProxyHandle alive while performing callback and while replacing handle
    //      because (keepRefToProxy->handle == this) which could be the only owner of "this"
    std::shared_ptr<ProxyHandle> keepMeAlive = this->shared_from_this();

    // Start the second timer before calling the callback-func, in case the callback cancels itself.
    handle = timeProvider.AddSingleShotTimer(
            interval, std::bind(&ProxyHandle::executeAndRegisterNextPeriod, this->shared_from_this()));

    func();
}
