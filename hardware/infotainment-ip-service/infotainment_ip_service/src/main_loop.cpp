/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "IIPS_Main_Loop"

#include "main_loop.h"
#include <cassert>
#include <cutils/log.h>
#include <sys/signalfd.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>


using namespace tarmac::eventloop;

// TODO, "android" shall be changed to "android::hardware" when we are using Android-O
namespace hal = android;

namespace Connectivity
{

void MainLoop::SigTermHandler(int fd)
{
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGTERM received...");

    event_.Stop(); // stop our own IDispatcher mainloop
    hal::IPCThreadState::self()->stopProcess(); // Stop the binder
}

void MainLoop::SigHupHandler(int fd)
{
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGHUP received...");
}

void MainLoop::SigIntHandler(int fd)
{
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGINT received...");

    event_.Stop(); // stop our own IDispatcher mainloop
    hal::IPCThreadState::self()->stopProcess(); // Stop the binder
}

MainLoop::MainLoop(void) : event_{IDispatcher::GetDefaultDispatcher()}
{
    if (!InitSignals())
    {
        ALOGE("Failed to init signals!!"); // TODO what shall we do??
    }
}

bool MainLoop::InitSignals()
{
    int ret_val;

    sigset_t signal_set_term;
    sigset_t signal_set_hup;
    sigset_t signal_set_int;

    // Create signals
    if ((sigemptyset(&signal_set_term) < 0) || (sigemptyset(&signal_set_hup) < 0) ||
        (sigemptyset(&signal_set_int) < 0) || (sigaddset(&signal_set_term, SIGTERM) < 0) ||
        (sigaddset(&signal_set_hup, SIGHUP) < 0) || (sigaddset(&signal_set_int, SIGINT) < 0))
    {
        ALOGE("Failed to create signals: %s",strerror(errno));
        return false;
    }

    // Block signals until we have added support for them
    if ((sigprocmask(SIG_BLOCK, &signal_set_term, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_hup, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_int, nullptr) < 0))
    {
        ALOGE("Failed to block signals: %s",strerror(errno));
        return false;
    }

    int termfd = signalfd(-1, &signal_set_term, 0);
    int hupfd  = signalfd(-1, &signal_set_hup, 0);
    int intfd  = signalfd(-1, &signal_set_int, 0);

    if (termfd<0 || hupfd<0 || intfd<0) {
        ALOGE("signalfd failed: %s",strerror(errno));
        return false;
    }

    event_.AddFd(termfd, [this,termfd]() {
        this->SigTermHandler(termfd);
    });

    event_.AddFd(hupfd, [this,hupfd]() {
        this->SigHupHandler(hupfd);
    });

    event_.AddFd(intfd, [this,intfd]() {
        this->SigIntHandler(intfd);
    });

    return true;
}

bool MainLoop::Run()
{
    // join with the binder thread pool. This is a blocking call until the binder is stopped
    hal::ProcessState::self()->startThreadPool();
    ALOGI("Starting mainloop");
    hal::IPCThreadState::self()->joinThreadPool();

    ALOGI("Mainloop exiting");

    return true;
}

IDispatcher& MainLoop::GetDispatcher() {
    return event_;
}


}  // namespace Connectivity
