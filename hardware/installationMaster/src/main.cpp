/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <sys/signalfd.h>
#include <memory>

#include "installation_master_daemon.h"

#undef LOG_TAG
#define LOG_TAG "InstMast"

// Setup signal handlers
void SigTermHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGTERM received...");
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

    return true;
}

using namespace com::volvocars::installationmaster;

// ===============================================================
// MAIN
int main(void) {
    InitSignals();

    std::unique_ptr<InstallationMasterDaemon> installation_master_daemon = std::make_unique<InstallationMasterDaemon>();

    ALOGI("exiting ...");

    return 0;
}