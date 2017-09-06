/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/log_context.h"
#include "ipcommandbus/net_serializer.h"
#include "workshop_wlan_service.hpp"

// NOTE! Don't change this path!
// For unit tests and coverage to work as intended,
// the path must also be valid relative to the stubs directory.
#include "util/type_conversion_helpers.h"

namespace v0_conn = v0::org::volvo::connectivity;

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_workshop_wlan_log_context, "WSWL", "Service for the WorksopWLAN interface")

WorkshopWlanService::WorkshopWlanService()
{
}

bool WorkshopWlanService::Initialize(MessageDispatcher *msg_dispatcher)
{
    (void)msg_dispatcher;
    return true;
}

void WorkshopWlanService::Uninitialize()
{
}

void WorkshopWlanService::ConnectWorkshopWlanSetRequestNoReturn(
    const std::shared_ptr<CommonAPI::ClientId> client,
    v0_conn::WorkshopWlanStub::ConnectWorkshopWlanSetRequestNoReturnReply_t reply)
{
    (void)client;
    (void)reply;
}

void WorkshopWlanService::UpdateDaCMode(const std::shared_ptr<CommonAPI::ClientId> client,
                                        v0_conn::WorkshopWlan::DaCMode update_dac_mode,
                                        v0_conn::WorkshopWlanStub::UpdateDaCModeReply_t reply)
{
    (void)client;
    (void)update_dac_mode;
    (void)reply;
}
void WorkshopWlanService::fireConnectWorkshopWlanNotificationEvent(
    const v0_conn::WorkshopWlan::WorkshopWlanConnectionResult &connection_result)
{
    (void)connection_result;
}

void WorkshopWlanService::fireWorkshopWlanDetectedEvent(const v0_conn::WorkshopWlan::DaCMode &mode,
                                                        const bool &nomadic_device_present)
{
    (void)mode;
    (void)nomadic_device_present;
}

void WorkshopWlanService::cbConnectWorkshopWlanNotification(Message &msg)
{
    (void)msg;
}

void WorkshopWlanService::cbWorkshopWlanDetectedNotification(Message &msg)
{
    (void)msg;
}
}  // namespace Connectivity
