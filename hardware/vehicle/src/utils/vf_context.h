/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <Application_dataelement_synchronous.h>
#include <IDispatcher.h>
#include <libsettings/settingsmanagerhidl.h>
#include <timer_manager_interface.h>
#include <vcc/localconfig.h>
#include "VehicleHalImpl.h"

class DEDispatcher final : public ApplicationDataElement::DataElementsExecutionContext {
  public:
    DEDispatcher(std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher);
    void assertInExecutionContext() override;
    void enqueueFunction(std::function<void()>&& func) override;

  private:
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher_;
};

class VFContext {
  public:
    VFContext(const std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
              android::sp<SettingsFramework::SettingsManagerHidl> settings,
              DEDispatcher dataelements,
              android::hardware::automotive::vehicle::V2_0::impl::VehicleHalImpl& vhal,
              const vcc::LocalConfigReaderInterface* lcfg);

    const std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher;
    const android::sp<SettingsFramework::SettingsManagerHidl> settings;
    DEDispatcher dataelements;
    android::hardware::automotive::vehicle::V2_0::impl::VehicleHalImpl& vhal;
    const vcc::LocalConfigReaderInterface* lcfg;
    const std::unique_ptr<tarmac::timeprovider::TimerManagerInterface> time;
};
