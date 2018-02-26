/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dispatcher.h"

#include <glib.h>
#include <mutex>

using namespace std::chrono_literals;

struct DispatchInstance
{
    Dispatcher*          dispatcher;
    std::recursive_mutex mutex;
};

Dispatcher::Dispatcher()
    : dispatchInstance_{}
    , cb_func_{}
    , gsource{ nullptr }
{
}

Dispatcher::~Dispatcher()
{
    Cancel();
}

bool Dispatcher::Start(std::chrono::milliseconds timeout, std::function<void(void)> timeout_function, bool repeat)
{
    const bool didCancel = Cancel();

    cb_func_                      = timeout_function;
    dispatchInstance_             = std::make_shared<DispatchInstance>(); // create a shared DispatchInstance for "this"
    dispatchInstance_->dispatcher = this;

    // Also create a shared instance (increase ref cnt) that will be sent to glib (and de-referenced in destroyNotify
    std::shared_ptr<DispatchInstance>* tmpshrd = new std::shared_ptr<DispatchInstance>(dispatchInstance_);

    gsource = createTimeoutSource(timeout.count(), tmpshrd, repeat);
    return didCancel;
}

void Dispatcher::Startnow(std::function<void(void)> dispatch_function)
{
    Start(0ms, dispatch_function, false);
}

bool Dispatcher::Restart(std::chrono::milliseconds timeout, bool repeat)
{
    return Start(timeout, cb_func_, repeat);
}

bool Dispatcher::Cancel()
{
    bool didCancel = false; // default to: didn't need to cancel dispatcher
    if (dispatchInstance_)
    {
        // this will keep dispatchInstance_ alive even after dispatchInstance_ = nullptr below
        std::shared_ptr<DispatchInstance> tmpDi = dispatchInstance_;
        dispatchInstance_                       = nullptr;
        { // lock scope
            std::lock_guard<std::recursive_mutex> safeLock(tmpDi->mutex);
            tmpDi->dispatcher = nullptr;
            if (gsource)
            {
                if (!g_source_is_destroyed(gsource))
                    g_source_destroy(gsource);
                g_source_unref(gsource);
                gsource   = nullptr;
                didCancel = true;
            }
        }
    }
    return didCancel;
}

bool Dispatcher::IsRunning() const
{
    return gsource != nullptr;
}

void Dispatcher::Dispatch()
{
    if (cb_func_)
        cb_func_();
}

void Dispatcher::destroyNotify(gpointer data)
{
    auto const* di = static_cast<std::shared_ptr<DispatchInstance>*>(data);

    {
        std::lock_guard<std::recursive_mutex> safeLock((*di)->mutex);
        if ((*di)->dispatcher && (*di)->dispatcher->gsource != nullptr)
        {
            g_source_unref((*di)->dispatcher->gsource);
            (*di)->dispatcher->gsource = nullptr;
        }
    } // lock scope

    delete di; // This deletes the share_ptr instance (and not the DispatchInstance directly)
}

GSource* Dispatcher::createTimeoutSource(guint timeoutMilliSeconds, gpointer data, bool repeat)
{
    GSource* gs = (timeoutMilliSeconds > 0) ? g_timeout_source_new(timeoutMilliSeconds) : g_idle_source_new();
    g_source_set_priority(gs, G_PRIORITY_DEFAULT);

    const auto proxy = repeat ? Dispatcher::cbProxyRepeat : Dispatcher::cbProxySingle;
    g_source_set_callback(gs, proxy, data, Dispatcher::destroyNotify);

    g_source_attach(gs, nullptr);
    return gs;
}

void Dispatcher::cbProxy(gpointer data)
{
    std::shared_ptr<DispatchInstance>& di = *static_cast<std::shared_ptr<DispatchInstance>*>(data);

    std::lock_guard<std::recursive_mutex> safeLock(di->mutex);
    if (di->dispatcher)
        di->dispatcher->Dispatch();
}

gboolean Dispatcher::cbProxySingle(gpointer data)
{
    cbProxy(data);
    return G_SOURCE_REMOVE;
}

gboolean Dispatcher::cbProxyRepeat(gpointer data)
{
    cbProxy(data);
    return G_SOURCE_CONTINUE;
}
