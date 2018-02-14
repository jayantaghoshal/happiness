/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <IDispatcher.h>
#include <hidl/HidlTransportSupport.h>
#include <sys/signalfd.h>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "Application_dataelement.h"
#include "lscservice.h"

#undef LOG_TAG  // workaround #include <hidl/HidlTransportSupport.h>
#define LOG_TAG "lscd.service"
#include <cutils/log.h>

using namespace tarmac::eventloop;
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
    int ret_val;

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

int main(void) {
    ALOGV("Starting lscd");
    InitSignals();

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> _vehModMngtGlbSafe1;

    sp<LscService> lscservice1 = new LscService;
    sp<LscService> lscservice2 = new LscService;
    sp<LscService> lscservice3 = new LscService;
    bool drivingActive = false;
    const hidl_string& lscName("Lsc1");

    _vehModMngtGlbSafe1.subscribe([&_vehModMngtGlbSafe1, &lscservice1, &lscservice2, &lscservice3, &lscName,
                                   &drivingActive]() {
        const auto signal_state_value = _vehModMngtGlbSafe1.get();
        if (!drivingActive && signal_state_value.isOk() &&
            signal_state_value.value().UsgModSts == autosar::UsgModSts1::UsgModDrvg)  // 13 == Driving
        {
            ALOGI("Request signal received");
            lscservice1->RegisterLSC("Lsc1");
            lscservice2->RegisterLSC("Lsc2");
            lscservice3->RegisterLSC("Lsc3");

            lscservice1->RequestResourceGroup(lscName, ResourceGroup::ResourceGroup1, ResourceGroupPrio::Normal);
            drivingActive = true;
        } else if (drivingActive && signal_state_value.isOk() &&
                   signal_state_value.value().UsgModSts != autosar::UsgModSts1::UsgModDrvg) {
            ALOGI("ReleaseResourceGroup signal received :");
            lscservice1->ReleaseResourceGroup(lscName, ResourceGroup::ResourceGroup1);
            lscservice1->UnregisterLSC("Lsc1");
            lscservice2->UnregisterLSC("Lsc2");
            lscservice3->UnregisterLSC("Lsc3");
            drivingActive = false;
        }
        ALOGD("UsgModSts1: %d", signal_state_value.value().UsgModSts);
    });

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    joinRpcThreadpool();

    ALOGD("exiting ...");
}
