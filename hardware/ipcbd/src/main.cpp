/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <IDispatcher.h>
#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>
#include <ipcommandbus/MessageDispatcher.h>
#include <ipcommandbus/TcpSocket.h>
#include <ipcommandbus/TransportServices.h>
#include <ipcommandbus/UdpSocket.h>
#include <ipcommandbus/isocket.h>
#include <sys/signalfd.h>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "diagnostics_client.h"
#include "service_manager.h"

#undef LOG_TAG
#define LOG_TAG "IpcbD"

using namespace Connectivity;
using namespace IpCmdTypes;
using namespace tarmac::eventloop;

// libhwbinder:
using namespace android::hardware;
// using android::hardware::configureRpcThreadpool;
// using android::hardware::joinRpcThreadpool;

// #define LOOPBACK_ADDRESS "127.0.0.1"
// Configure sockets
void setupSocket(std::shared_ptr<Connectivity::Socket> sock, Message::Ecu ecu) {
    const std::uint32_t sleep_time = 100000;  // sleep 100 ms

    // Wait for-ever, no need to stop since service is dependent on this to work
    std::string previousError;
    ALOGI("[Main] Setup socket for ecu %u", ecu);
    do {
        try {
            sock->setup(ecu);
            ALOGV("[Main] Setup socket for Ecu %u successfully", ecu);
            return;
        } catch (const SocketException& e) {
            if (e.what() != previousError) {
                previousError = e.what();
                ALOGE("[Main] Can not setup socket for Ecu %u, error %s, continue trying...", ecu, e.what());
            }
            usleep(sleep_time);
        }
    } while (true);
}

// Setup signal handlers
void SigTermHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("[Main] SIGTERM received...");

    IDispatcher::GetDefaultDispatcher().Stop();  // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess();       // Stop the binder
}

void SigHupHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("[Main] SIGHUP received...");
}

void SigIntHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("[Main] SIGINT received...");

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
        ALOGE("[Main] Failed to create signals: %s", strerror(errno));
        return false;
    }

    // Block signals until we have added support for them
    if ((sigprocmask(SIG_BLOCK, &signal_set_term, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_hup, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_int, nullptr) < 0)) {
        ALOGE("[Main] Failed to block signals: %s", strerror(errno));
        return false;
    }

    int termfd = signalfd(-1, &signal_set_term, 0);
    int hupfd = signalfd(-1, &signal_set_hup, 0);
    int intfd = signalfd(-1, &signal_set_int, 0);

    if (termfd < 0 || hupfd < 0 || intfd < 0) {
        ALOGE("[Main] signalfd failed: %s", strerror(errno));
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
    if (argc < 3) {
        ALOGE("[Main] Usage: ipcbd <service_name> <protocol> -optional: [ecu]"
              "\nExamples:\n 1. infotainment UDP\n"
              "\n2. iplm UDPB\n3. dim TCP DIM\n");
        return EXIT_FAILURE;
    }

    std::string service_name = argv[1];
    std::string protocol = argv[2];
    InitSignals();

    IDispatcher& dispatcher = IDispatcher::GetDefaultDispatcher();

    TransportServices transport{dispatcher, dispatcher, Message::Ecu::IHU};

    std::shared_ptr<Socket> sock;

    try {
        /*ShutdownClient shutdown_client_(sock);
          cedric::core::NodeState nsm_client_(NSM_SHUTDOWNTYPE_NORMAL |
          NSM_SHUTDOWNTYPE_FAST, 2000);
          nsm_client_.setShutdownClient(&shutdown_client_);*/

        if (protocol == "UDP") {
            sock = std::make_shared<UdpSocket>(dispatcher);
            transport.setSocket(sock.get());
            setupSocket(sock, Message::IHU);
        } else if (protocol == "UDPB") {
            sock = std::make_shared<UdpSocket>(dispatcher);
            transport.setBroadcastSocket(sock.get());
            setupSocket(sock, Message::ALL);
        } else if (protocol == "TCP") {
            if (argc < 4) {
                ALOGE("[Main] Needs ECU name. Check .rc file");
                return EXIT_FAILURE;
            }
            if (std::string(argv[3]) == "DIM") {
                sock = std::make_shared<TcpSocket>(dispatcher, Message::Ecu::DIM);
                transport.setSocket(sock.get());
            } else {
                ALOGE("[Main] Only DIM is supported currently");
                return EXIT_FAILURE;
            }
        } else {
            ALOGE("[Main] Unknown protocol specified");
            return EXIT_FAILURE;
        }

        MessageDispatcher msgDispatcher{&transport, dispatcher};
        Connectivity::ServiceManager service_manager(service_name, msgDispatcher);

        DiagnosticsClient diagnostics_client_;
        msgDispatcher.setDiagnostics(&diagnostics_client_);

        configureRpcThreadpool(1, true /*callerWillJoin*/);

        joinRpcThreadpool();

        ALOGI("[Main] exiting ...");
    } catch (const SocketException& e) {
        ALOGE("[Main] %s . Code (%s : %i)", e.what(), e.code().category().name(), e.code().value());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
