/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "Ipcb.main"

#include <ipcommandbus/MessageDispatcher.h>
#include <ipcommandbus/TransportServices.h>
#include <ipcommandbus/UdpSocket.h>
#include <ipcommandbus/isocket.h>
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

#include "service_manager.h"

using namespace Connectivity;
using namespace IpCmdTypes;
using namespace tarmac::eventloop;

// libhwbinder:
using namespace android::hardware;
// using android::hardware::configureRpcThreadpool;
// using android::hardware::joinRpcThreadpool;

// #define LOOPBACK_ADDRESS "127.0.0.1"
// Configure sockets
void setupSocket(Connectivity::UdpSocket &sock, Message::Ecu ecu)
{
    const std::uint32_t sleep_time = 100000;  // sleep 100 ms
    std::uint32_t sleep_counter = 0;

    // Wait for-ever, no need to stop since service is dependent on this to work
    do
    {
        try
        {
            sock.setup(ecu);
            return;
        }
        catch (const SocketException &e)
        {
            usleep(sleep_time);
            sleep_counter++;

            if (sleep_counter % (1000000 / sleep_time) == 0)
            {
                ALOGW("Can not setup socket for Ecu %u, error %s, continue trying...", ecu, e.what());
            }
        }
    } while (true);
}

// Setup signal handlers
void SigTermHandler(int fd)
{
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGTERM received...");

    IDispatcher::GetDefaultDispatcher().Stop();  // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess();       // Stop the binder
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

    IDispatcher::GetDefaultDispatcher().Stop();  // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess();       // Stop the binder
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
        ALOGE("Failed to create signals: %s", strerror(errno));
        return false;
    }

    // Block signals until we have added support for them
    if ((sigprocmask(SIG_BLOCK, &signal_set_term, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_hup, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_int, nullptr) < 0))
    {
        ALOGE("Failed to block signals: %s", strerror(errno));
        return false;
    }

    int termfd = signalfd(-1, &signal_set_term, 0);
    int hupfd = signalfd(-1, &signal_set_hup, 0);
    int intfd = signalfd(-1, &signal_set_int, 0);

    if (termfd < 0 || hupfd < 0 || intfd < 0)
    {
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
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        ALOGE(
            "Usage: ipcbd <service_name> <protocol> -optional: [ecu]"
            "\nExamples:\n 1. infotainment UDP\n"
            "\n2. iplm UDPB\n3. dim TCP DIM\n");
        return 1;
    }

    std::string service_name = argv[1];
    std::string protocol = argv[2];
    InitSignals();

    IDispatcher &dispatcher = IDispatcher::GetDefaultDispatcher();

    TransportServices transport{dispatcher, dispatcher, Message::Ecu::IHU};

    try
    {
        /*ShutdownClient shutdown_client_(sock);
        cedric::core::NodeState nsm_client_(NSM_SHUTDOWNTYPE_NORMAL |
        NSM_SHUTDOWNTYPE_FAST, 2000);
        nsm_client_.setShutdownClient(&shutdown_client_);*/

        if (protocol == "UDP")
        {
            UdpSocket sock(dispatcher);
            setupSocket(sock, Message::IHU);
            transport.setSocket(&sock);
        }
        else if (protocol == "UDPB")
        {
            UdpSocket broadcastSock(dispatcher);  // Socket for broadcast
            setupSocket(broadcastSock, Message::ALL);
            transport.setBroadcastSocket(&broadcastSock);
        }
        else if (protocol == "TCP")
        {
            if (argc < 4)
            {
                ALOGE("Needs ECU name. Check .rc file");
                return 1;
            }
            // TODO: Add DIM and other ECU sockets for TCP communication
            // if (ECU == "DIM")
            // {
            //     // setup atcp socket with port and IP extracted from
            //     localconfig
            // }
            // else
            // {
            //     ALOGE("Unknown ECU specified for TCP communication")
            //     return;
            // }
        }
        else
        {
            ALOGE("Unknown protocol specified");
            return 1;
        }

        MessageDispatcher msgDispatcher{&transport, dispatcher};
        Connectivity::ServiceManager service_manager(msgDispatcher);

        configureRpcThreadpool(1, true /*callerWillJoin*/);

        joinRpcThreadpool();

        ALOGI("exiting ...");
    }
    catch (const SocketException &e)
    {
        ALOGE("%s . Code (%s : %i)", e.what(), e.code().category().name(), e.code().value());
    }
}
