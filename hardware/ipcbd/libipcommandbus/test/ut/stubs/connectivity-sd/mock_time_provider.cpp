/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "mock_time_provider.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace Connectivity {
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeProviderMock
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::chrono::steady_clock::time_point TimeProviderStub::steady_clock_now() const { return steady_now; }

std::chrono::system_clock::time_point TimeProviderStub::system_clock_now() const { return system_now; }

void TimeProviderStub::sleep_for(const std::chrono::milliseconds duration) {
    const auto steady_now_before = steady_now;
    const auto system_now_before = system_now;
    const auto end_time = steady_now + duration;
    const auto end_time_system = system_now + duration;

    {
        std::unique_lock<std::recursive_mutex> lock(timersMutex);

        std::list<InternalSubscriptionHandle>::iterator i = timers.begin();
        while (i != std::end(timers)) {
            if (i->fire_at > end_time) break;
            steady_now = i->fire_at;
            system_now = system_now_before + (i->fire_at - steady_now_before);

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
    system_now = end_time_system;
}
std::unique_ptr<TimerSubscriptionHandle> TimeProviderStub::add_single_shot_timer(std::chrono::milliseconds time,
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

        auto insertion_point = std::find_if(
                std::begin(timers), std::end(timers),
                [fireAtTime](const InternalSubscriptionHandle& sub) { return (sub.fire_at >= fireAtTime); });

        std::shared_ptr<SubscriptionData> meta = std::make_shared<SubscriptionData>(func);
        std::list<InternalSubscriptionHandle>::iterator insertedIt =
                timers.emplace(insertion_point, fireAtTime, std::weak_ptr<SubscriptionData>(meta));
        std::unique_ptr<TimerSubscriptionHandle> rval(new UserSubscriptionHandle(meta));
        return rval;
    }
}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InternalSubscriptionHandle
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TimeProviderStub::InternalSubscriptionHandle::InternalSubscriptionHandle(
        std::chrono::_V2::steady_clock::time_point fire_at, std::weak_ptr<TimeProviderStub::SubscriptionData> meta)
    : fire_at(fire_at), meta(meta) {}

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UserSubscriptionHandle
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TimeProviderStub::UserSubscriptionHandle::UserSubscriptionHandle(
        std::shared_ptr<TimeProviderStub::SubscriptionData> meta)
    : TimerSubscriptionHandle(), meta(meta) {}

TimeProviderStub::UserSubscriptionHandle::~UserSubscriptionHandle() { meta->unsubscribe(); }

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SubscriptionData
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
}
