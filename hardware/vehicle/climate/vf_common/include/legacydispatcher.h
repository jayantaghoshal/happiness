/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstdint>
#include <memory>
#include "ilegacydispatcher.h"
#include "timer_manager_interface.h"

class LegacyDispatcher : public ILegacyDispatcher {
  public:
    LegacyDispatcher();

    virtual ~LegacyDispatcher();

    static void setGlobalInstanceHackTimeProvider(tarmac::timeprovider::TimerManagerInterface* globalInstance);

    /**
     * @see ILegacyDispatcher::Start
     */
    void Start(std::chrono::milliseconds timeout, std::function<void(void)> timeout_function,
               bool repeat = false) override;

    /**
     * @see ILegacyDispatcher::Startnow
     */
    void Startnow(std::function<void(void)> dispatch_function) override;

    /**
     * @see ILegacyDispatcher::Restart
     */
    void Restart(std::chrono::milliseconds timeout, bool repeat = false) override;

    /**
     * @see ILegacyDispatcher::Stop
     */
    void Cancel() override;

    /**
     * @see ILegacyDispatcher::IsRunning
     */
    bool IsRunning() const override;

  private:
    void Dispatch();

    std::unique_ptr<tarmac::timeprovider::TimerSubscriptionHandle> timer_handle_;
    std::function<void(void)> cb_func_;
};
