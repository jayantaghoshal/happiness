/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "Infotainment_IP_Service.manager"

#include "service_manager.h"

#include <ipcommandbus/MessageDispatcher.h>
#include <cutils/log.h>

#include "services/gnss_service.h"
// #include "services/connectivity_service.h"
// #include "services/iplm_service.h"
// #include "services/ota_service.h"
// #include "services/telematics_service.h"
// #include "services/wlan_service.h"

using namespace tarmac::eventloop;

namespace Connectivity
{
ServiceManager::ServiceManager(IDispatcher& timeProvider)
    : timeProvider{timeProvider},
      gnss_service_{}
{
}

void ServiceManager::RegisterAllBinderServices(::Connectivity::MessageDispatcher* msg_dispatcher)
{
    (void)msg_dispatcher;
    ALOGD("+ ServiceManager::RegisterAllBinderServices");

    gnss_service_.Initialize(msg_dispatcher);

    //// Register handling of services where IHU is CommonAPI server and IPBus client.

    //RegisterCommonApiService<GnssService, std::false_type>(gnss_service_, commonApiMainLoopContext_, msg_dispatcher);

    //// Register handling of services where IHU is CommonAPI client and IPBus server.

    // Note: This is a good example if adding a new service where we are Franca client / IpBus server.

    ALOGD("- ServiceManager::RegisterAllCommonApiServices");
}

void ServiceManager::UnregisterAllBinderServices()
{
    ALOGD("ServiceManager::UnregisterAllCommonApiServices");
    // Unregister CommonAPI services that we provided.
    //UnregisterCommonApiService<GnssService>(gnss_service_);
    // (Doesn't seem like unregister call is needed for CommonAPI services that we subscribe to...)
    ALOGD("ServiceManager::UnregisterAllCommonApiServices done");
}

}  // namespace Connectivity
