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
#include <mutex>
#include <queue>
#include "connectivity-sd/event_references.h"
#include "connectivity-sd/thread_dispatcher.h"

namespace Connectivity
{
class SdEventDispatcher : public ThreadDispatcher
{
public:
    SdEventDispatcher(sd_event* sd);

    void enqueueForDispatch(std::function<void()>&& f);

private:
    void dispatchEnqueuedFunctions();

    const SdEventReference sdevent;
    const int wakeup_fd;
    SdEventSourceReference source;
    std::mutex functionQueueMutex;
    std::queue<std::function<void()>> queuedFunctions;

    static int signal_wakeup_cb(sd_event_source* s, int fd, uint32_t revents, void* userdata);
};
}
