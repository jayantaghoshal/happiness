/*
 * Copyright 2017-2018 Volvo Car Corporation
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
#include "lsc_service.h"

#undef LOG_TAG  // workaround #include <hidl/HidlTransportSupport.h>
#define LOG_TAG "iplm_ctrl.main"
#include <cutils/log.h>

using namespace tarmac::eventloop;
using namespace android::hardware;

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

void printHelp() {
    printf("Commands:\n");
    printf("exit: Exit application\n");
    printf("help: Show this help text\n");
    printf("reg <lsc_name>: Register an LSC with name \"lsc_name\"  \n");
    printf("ureg <lsc_name>: Unregister an LSC with name \"lsc_name\"  \n");
    printf("req <lsc_name> <rg> <prio>: Request a resource group (rg) with prio (high/normal) for given lsc\n");
    printf("rel <lsc_name> <rg>: Release a resource group (rg) for given lsc\n");
}

void split(const std::string& s, char delim, std::vector<std::string>& elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

// ===============================================================
// MAIN

int main(void) {
    ALOGD("Starting iplm_ctrl");
    InitSignals();

    LscService lscService;

    configureRpcThreadpool(1, false /*callerWillJoin*/);

    for (;;) {  // same meaning as while(true){
        std::string str;
        std::cout << "iplm_ctrl >  " << std::flush;
        std::getline(std::cin, str);
        // based on the input, do something

        if (str == "") {
            continue;
        } else if (str == "exit") {
            break;
        } else if (str == "help") {
            printHelp();
        } else {
            std::vector<std::string> elems;

            split(str, ' ', elems);

            ALOGI("Command = \"%s\", num elems = %d", str.c_str(), (int)elems.size());

            for (int i = 0; i < (int)elems.size(); ++i) {
                ALOGI("elems[%d] = %s", i, elems[i].c_str());
            }

            if (elems[0] == "reg") {
                if ((int)elems.size() != 2) {
                    std::cout << "Syntax error, expecting 'reg <lsc_name>'\n";
                    continue;
                }

                lscService.RegisterLSC(
                        elems[1],
                        [](int resourceGroup, std::string resourceGroupStatus, std::string resourceGroupPrio) {
                            ALOGI("ResourceGroupStatus received: RG = %d, Status = %s, Prio = %s",
                                  resourceGroup,
                                  resourceGroupStatus.c_str(),
                                  resourceGroupPrio.c_str());
                        },
                        [](std::string ecuType, bool ecuStatus) {
                            ALOGI("NodeStatus received: ecuType = %s, Status = %d", ecuType.c_str(), ecuStatus);
                        });
                std::cout << "Registered ok \n";

            } else if (elems[0] == "ureg") {
                if ((int)elems.size() != 2) {
                    std::cout << "Syntax error, expecting 'ureg <lsc_name>'\n";
                    continue;
                }

                lscService.UnregisterLSC(elems[1]);
                std::cout << "Unregistered ok \n";

            } else if (elems[0] == "req") {
                if ((int)elems.size() == 3) {
                    std::cout << "Prio not provided, defaulting to 'normal' prio\n";
                    elems.push_back("normal");
                } else if ((int)elems.size() != 4) {
                    std::cout << "Syntax error, expecting 'req <lsc_name> <rg> <prio>'\n";
                    continue;
                }

                uint8_t rg_shift = std::stoi(elems[2]);
                ResourceGroup rg = ResourceGroup(1 << rg_shift);
                ResourceGroupPrio rgp = elems[3] == "normal" ? ResourceGroupPrio::Normal : ResourceGroupPrio::High;
                lscService.RequestResourceGroup(elems[1], rg, rgp);
                std::cout << "Request RG ok \n";

            } else if (elems[0] == "rel") {
                if ((int)elems.size() != 3) {
                    std::cout << "Syntax error, expecting 'rel <lsc_name> <rg>'\n";
                    continue;
                }

                uint8_t rg_shift = std::stoi(elems[2]);
                ResourceGroup rg = ResourceGroup(1 << rg_shift);
                lscService.ReleaseResourceGroup(elems[1], rg);
                std::cout << "Release RG ok \n";
            } else {
                // Do nothing;
            }
        }
    }

    ALOGD("exiting ...");
}
