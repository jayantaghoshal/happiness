/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "Infotainment_IP_Service.manager"

#include "service_manager.h"

#include <ipcommandbus/MessageDispatcher.h>
#include <cutils/log.h>

using namespace tarmac::eventloop;

namespace Connectivity
{
ServiceManager::ServiceManager(IDispatcher& timeProvider)
    : timeProvider_{timeProvider}
{
}

void ServiceManager::RegisterAllBinderServices(::Connectivity::MessageDispatcher* msg_dispatcher)
{
    ALOGD("ServiceManager::RegisterAllBinderServices");

    msg_dispatcher->setDiagnostics(&diagnostics_client_);
}

void ServiceManager::UnregisterAllBinderServices()
{
    ALOGD("ServiceManager::UnregisterAllBinderServices");
}

}  // namespace Connectivity
