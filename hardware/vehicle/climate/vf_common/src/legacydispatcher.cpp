/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "legacydispatcher.h"
#include <mutex>
#include "time_provider.h"

using namespace std::chrono_literals;

namespace {
static tarmac::timeprovider::TimerManagerInterface* globalInstanceHackTimeProvider = nullptr;

tarmac::timeprovider::TimerManagerInterface& GetGlobalTimeProvider() {
    if (globalInstanceHackTimeProvider == nullptr) {
        throw std::runtime_error("Global legacydispatcher must be initialized");
    }
    return *globalInstanceHackTimeProvider;
}
}  // namespace

void LegacyDispatcher::setGlobalInstanceHackTimeProvider(tarmac::timeprovider::TimerManagerInterface* globalInstance) {
    if (globalInstanceHackTimeProvider != nullptr) {
        throw std::runtime_error("Legacydispatcher initialized twice");
    }
    globalInstanceHackTimeProvider = globalInstance;
}

LegacyDispatcher::LegacyDispatcher() {}

LegacyDispatcher::~LegacyDispatcher() { Cancel(); }

void LegacyDispatcher::Start(std::chrono::milliseconds timeout, std::function<void()> timeout_function, bool repeat) {
    cb_func_ = timeout_function;

    if (repeat) {
        timer_handle_ = GetGlobalTimeProvider().AddSingleShotTimer(timeout, [this]() { Dispatch(); });
    } else {
        timer_handle_ = GetGlobalTimeProvider().AddPeriodicTimer(timeout, [this]() { Dispatch(); });
    }
}

void LegacyDispatcher::Startnow(std::function<void(void)> dispatch_function) { Start(0ms, dispatch_function, false); }

void LegacyDispatcher::Restart(std::chrono::milliseconds timeout, bool repeat) { Start(timeout, cb_func_, repeat); }

void LegacyDispatcher::Cancel() { timer_handle_ = nullptr; }

bool LegacyDispatcher::IsRunning() const { return timer_handle_ != nullptr; }

void LegacyDispatcher::Dispatch() {
    if (cb_func_) cb_func_();
}
