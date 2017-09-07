/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once
#include <assert.h>
#include <sys/eventfd.h>
#include <systemd/sd-event.h>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include "connectivity-sd/sd_event_dispatcher.h"

namespace Connectivity
{
class SdEventThread
{
public:
    SdEventThread()
    {
        std::promise<void> dispatcherCreated;
        std::future<void> dispatcherCreatedFut = dispatcherCreated.get_future();
        t = std::thread{[&]() {
            int sdStatus = sd_event_default(&sd);
            assert(sdStatus);
            d = std::make_unique<SdEventDispatcher>(sd);
            sd_event_loop(sd);
        }};
        dispatcherCreatedFut.wait();
    }
    ~SdEventThread()
    {
        d->enqueueForDispatch([&]() { sd_event_exit(sd, 0); });
        t.join();
    }

private:
    std::thread t;
    sd_event* sd;
    std::unique_ptr<SdEventDispatcher> d;
};
}
