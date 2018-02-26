/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/
#pragma once

#include "idispatcher.h"
#include <cstdint>
#include <memory>

struct DispatchInstance;

// Forward decl of GSource
struct _GSource;
typedef struct _GSource GSource;

class Dispatcher : public IDispatcher
{
public:
    Dispatcher();

    virtual ~Dispatcher();

    /**
     * @see IDispatcher::Start
     */
    bool Start(std::chrono::milliseconds timeout,
               std::function<void(void)> timeout_function,
               bool                      repeat = false) override;

    /**
     * @see IDispatcher::Startnow
     */
    void Startnow(std::function<void(void)> dispatch_function) override;

    /**
     * @see IDispatcher::Restart
     */
    bool Restart(std::chrono::milliseconds timeout, bool repeat = false) override;

    /**
     * @see IDispatcher::Stop
     */
    bool Cancel() override;

    /**
     * @see IDispatcher::IsRunning
     */
    bool IsRunning() const override;

private:
    void Dispatch();

    static void cbProxy(void* data);
    static int cbProxySingle(void* data);
    static int cbProxyRepeat(void* data);
    static void destroyNotify(void* data);
    static GSource* createTimeoutSource(unsigned int timeoutMilliSeconds, void* data, bool repeat);

    std::shared_ptr<DispatchInstance> dispatchInstance_;
    std::function<void(void)>         cb_func_;
    GSource*                          gsource;
};
