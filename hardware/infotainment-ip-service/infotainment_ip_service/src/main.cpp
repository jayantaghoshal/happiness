/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "Infotainment_IP_Service.main"

#include <ipcommandbus/MessageDispatcher.h>
#include <ipcommandbus/TransportServices.h>
#include <ipcommandbus/UdpSocket.h>
#include <ipcommandbus/isocket.h>
//#include <ipcommandbus/shutdown_client.h>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <cutils/log.h>
#include <IDispatcher.h>

#include "util/local_config.h"
#include "main_loop.h"
#include "service_manager.h"

using namespace Connectivity;
using namespace VccIpCmd;
using namespace tarmac::eventloop;


// #define LOOPBACK_ADDRESS "127.0.0.1"

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

int main(void)
{
    InfotainmentIpService::Utils::loadLocalConfig();

    MainLoop ipCommandBusThread;
    //IDispatcher& applicationThread = IDispatcher::GetDefaultDispatcher();

    //TimeProviderSd ipCommandBusTimeProvider{*ipCommandBusThread.event_};
    //SdEventDispatcher ipCommandBusThreadDispatcher{ipCommandBusThread.event_.get()};
    Connectivity::TransportServices transport{ipCommandBusThread.GetDispatcher(), ipCommandBusThread.GetDispatcher(), Message::Ecu::IHU};

    try
    {
        Connectivity::UdpSocket sock(ipCommandBusThread.GetDispatcher());
        Connectivity::UdpSocket broadcastSock(ipCommandBusThread.GetDispatcher());  // Socket for broadcast

        transport.setSocket(&sock);
        transport.setBroadcastSocket(&broadcastSock);

        /*ShutdownClient shutdown_client_(sock);
        cedric::core::NodeState nsm_client_(NSM_SHUTDOWNTYPE_NORMAL | NSM_SHUTDOWNTYPE_FAST, 2000);
        nsm_client_.setShutdownClient(&shutdown_client_);*/

        setupSocket(sock, Message::IHU);
        setupSocket(broadcastSock, Message::ALL);

        std::promise<void> servicesRegistered;
        std::future<void> servicesRegisteredFut = servicesRegistered.get_future();
        //SdEventDispatcher &appDispatcher = applicationThread.getDispatcher();
        Connectivity::MessageDispatcher msgDispatcher{&transport, ipCommandBusThread.GetDispatcher()};

        // TODO: unique_ptr Quick hack to keep these alive but allow init in the other thread, should be kept in a
        // separate class.
        //std::unique_ptr<Connectivity::TimeProviderSd> applicationTimeProvider;
        //std::unique_ptr<Connectivity::CommonAPIMainLoopHandler> commonApi_mainLoop_handler;
        std::unique_ptr<ServiceManager> service_manager;

        ipCommandBusThread.GetDispatcher().Enqueue([&]() {
            //sd_event *sd;
            //sd_event_default(&sd);
            //applicationTimeProvider = std::make_unique<Connectivity::TimeProviderSd>(*sd);

            //commonApi_mainLoop_handler = std::make_unique<Connectivity::CommonAPIMainLoopHandler>();
            //std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
            //    std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");
            //commonApi_mainLoop_handler->Init(common_api_main_loop_context_);

            service_manager =
                std::make_unique<Connectivity::ServiceManager>(ipCommandBusThread.GetDispatcher());
            service_manager->RegisterAllBinderServices(&msgDispatcher);
            servicesRegistered.set_value();
        });

        servicesRegisteredFut.wait();  // Must wait for ServiceManager to finish init to set DiagnosticsClient
        ipCommandBusThread.Run();
    }
    catch (const SocketException &e)
    {
        ALOGE("%s . Code (%s : %i)", e.what(), e.code().category().name(), e.code().value());
    }
}
