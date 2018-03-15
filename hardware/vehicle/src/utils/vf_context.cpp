/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vf_context.h"
#include <time_provider.h>

DEDispatcher::DEDispatcher(std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher) : dispatcher_{dispatcher} {}

void DEDispatcher::assertInExecutionContext() {}

void DEDispatcher::enqueueFunction(std::function<void()>&& func) { dispatcher_->Enqueue(std::move(func)); }

VFContext::VFContext(const std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                     android::sp<SettingsFramework::SettingsManagerHidl> settings,
                     DEDispatcher dataelements,
                     android::hardware::automotive::vehicle::V2_0::impl::VehicleHalImpl& vhal,
                     const vcc::LocalConfigReaderInterface* lcfg)
    : dispatcher{dispatcher},
      settings{settings},
      dataelements{dataelements},
      vhal{vhal},
      lcfg{lcfg},
      time{std::make_unique<tarmac::timeprovider::TimeProvider>(dispatcher)} {}
