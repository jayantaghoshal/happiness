/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <chrono>
#include <functional>

/**
 *LegacyDispatcher Interface
 * The dispatcher interface can only handle ONE dispatch job at a time. If you want several
 * dispatch jobs to run in parallel you need to instanciate several interfaces.
 */
class ILegacyDispatcher {
  public:
    virtual ~ILegacyDispatcher() = default;

    /**
     * Start a dispatcher that will call a specific callback upon timeout. If there is a currently running
     * dispatcher it will first be cancelled before starting the new one.
     *
     * @param[in] timeout Time until timeout, specified in milliseconds.
     * @param[in] timeout_function Callback function on timeout
     * @param[in] repeat Callback function will be repeatedly called
     * @return Returns true if a running dispatcher was cancelled before starting the new one. False otherwise.
     */
    virtual void Start(std::chrono::milliseconds timeout, std::function<void(void)> timeout_function,
                       bool repeat = false) = 0;

    /**
     * Start a dispatcher and call it immediately. This is a convenience method for Start(0,func);
     *
     * @param[in] dispatch_function Dispatch callback function
     */
    virtual void Startnow(std::function<void(void)> dispatch_function) = 0;

    /**
     * Restart the dispatcher with a new timeout value BUT using the current callback function.
     * If there is a currently running dispatcher it will first be cancelled before starting the new one.
     *
     * @param[in] timeout Time until timeout, specified in milliseconds.
     * @param[in] repeat Callback function will be repeatedly called
     * @return Returns true if a running dispatcher was cancelled before starting the new one. False otherwise.
     */
    virtual void Restart(std::chrono::milliseconds timeout, bool repeat = false) = 0;

    /**
     * Cancel the dispatcher.
     * @return Returns true if a running dispatcher was cancelled. False otherwise.
     */
    virtual void Cancel() = 0;

    /**
     * See if there is a dispatcher running or not.
     * It is in many cases more safe to check the return value of Start() / Restart() / Cancel()
     * instead of using this method!
     */
    virtual bool IsRunning() const = 0;
};
