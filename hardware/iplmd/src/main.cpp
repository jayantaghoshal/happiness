/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <IDispatcher.h>
#include <hidl/HidlTransportSupport.h>
#include <sys/signalfd.h>
#include <cstdint>
#include <cstring>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <android/log.h>
#include <hidl/Status.h>

#include "iplmService.h"

#undef LOG_TAG  // workaround #include <hidl/HidlTransportSupport.h>
#define LOG_TAG "iplmd"
#include <cutils/log.h>

using namespace tarmac::eventloop;
using namespace android::hardware;
using namespace android;
using namespace android::hardware;

// using android::hardware::configureRpcThreadpool;
// using android::hardware::joinRpcThreadpool;

// Setup signal handlers
void SigTermHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGTERM received...");

    IDispatcher::GetDefaultDispatcher().Stop();  // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess();       // Stop the binder
}

void SigHupHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGHUP received...");
}

void SigIntHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGINT received...");

    IDispatcher::GetDefaultDispatcher().Stop();  // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess();       // Stop the binder
}

bool InitSignals() {
    sigset_t signal_set_term;
    sigset_t signal_set_hup;
    sigset_t signal_set_int;

    // Create signals
    if ((sigemptyset(&signal_set_term) < 0) || (sigemptyset(&signal_set_hup) < 0) ||
        (sigemptyset(&signal_set_int) < 0) || (sigaddset(&signal_set_term, SIGTERM) < 0) ||
        (sigaddset(&signal_set_hup, SIGHUP) < 0) || (sigaddset(&signal_set_int, SIGINT) < 0)) {
        ALOGE("Failed to create signals: %s", strerror(errno));
        return false;
    }

    // Block signals until we have added support for them
    if ((sigprocmask(SIG_BLOCK, &signal_set_term, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_hup, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_int, nullptr) < 0)) {
        ALOGE("Failed to block signals: %s", strerror(errno));
        return false;
    }

    int termfd = signalfd(-1, &signal_set_term, 0);
    int hupfd = signalfd(-1, &signal_set_hup, 0);
    int intfd = signalfd(-1, &signal_set_int, 0);

    if (termfd < 0 || hupfd < 0 || intfd < 0) {
        ALOGE("signalfd failed: %s", strerror(errno));
        return false;
    }

    IDispatcher::GetDefaultDispatcher().AddFd(termfd, [termfd]() { SigTermHandler(termfd); });

    IDispatcher::GetDefaultDispatcher().AddFd(hupfd, [hupfd]() { SigHupHandler(hupfd); });

    IDispatcher::GetDefaultDispatcher().AddFd(intfd, [intfd]() { SigIntHandler(intfd); });

    return true;
}

// ===============================================================
// MAIN
int main(int argc, char* argv[]) {
    if (argc < 4) {
        ALOGE("Three argument expected ");
        return EXIT_FAILURE;
    }

    InitSignals();

    char* pch;
    std::string argument;
    std::string interface_iplm = "IIplm";
    std::string interface_vehiclecom = "IVehicleCom";
    std::string interface_lifecyclecontrol = "ILifecycleControl";
    sp<IplmService> iplmService;
    std::map<std::string, std::string> arguments;

    // Parse arguments of the format "--X=Y" ...
    // and put them in a map with X as key and Y as value
    for (int num = 1; num < argc; num++) {
        int i = 0;
        char* str = argv[num];
        pch = std::strtok(str, "-=");
        if (pch != NULL) {
            argument = pch;
            pch = std::strtok(NULL, "=");
            if (pch != NULL) {
                arguments[argument] = pch;
            }
        }
    }

    // IIplm
    argument = arguments[interface_iplm];
    if (argument != "") {
        iplmService = new IplmService(argument);
        ALOGW("iplm registered as IIplm/%s", argument.c_str());

        // IVehicleCom
        argument = arguments[interface_vehiclecom];
        if (argument != "") {
            iplmService->SubscribeVehicleCom(argument);
        } else {
            ALOGW("--%s missing", interface_vehiclecom.c_str());
        }

        // ILifecycleControl
        argument = arguments[interface_lifecyclecontrol];
        if (argument != "") {
            iplmService->ConnectLifecycleControl(argument);
        } else {
            ALOGW("--%s missing", interface_lifecyclecontrol.c_str());
        }
    } else {
        ALOGW("--%s missing", interface_iplm.c_str());
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    joinRpcThreadpool();

    return EXIT_SUCCESS;
}
