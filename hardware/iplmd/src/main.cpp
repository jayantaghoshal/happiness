#include <hidl/HidlTransportSupport.h>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <cutils/log.h>
#include <IDispatcher.h>
#include <sys/signalfd.h>

#include "iplmService.h"

#define LOG_TAG "iplmd"

using namespace tarmac::eventloop;
using namespace android::hardware;

//using android::hardware::configureRpcThreadpool;
//using android::hardware::joinRpcThreadpool;

// Setup signal handlers
void SigTermHandler(int fd)
{
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGTERM received...");

    IDispatcher::GetDefaultDispatcher().Stop(); // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess(); // Stop the binder
}

void SigHupHandler(int fd)
{
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGHUP received...");
}

void SigIntHandler(int fd)
{
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGINT received...");

    IDispatcher::GetDefaultDispatcher().Stop(); // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess(); // Stop the binder
}

bool InitSignals()
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

    IDispatcher::GetDefaultDispatcher().AddFd(termfd, [termfd]() {
        SigTermHandler(termfd);
    });

    IDispatcher::GetDefaultDispatcher().AddFd(hupfd, [hupfd]() {
        SigHupHandler(hupfd);
    });

    IDispatcher::GetDefaultDispatcher().AddFd(intfd, [intfd]() {
        SigIntHandler(intfd);
    });

    return true;
}


// ===============================================================
// MAIN
int main(void)
{
    InitSignals();

    IplmService iplmService;

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    joinRpcThreadpool();

    ALOGI("exiting ...");
}
