/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <assert.h>
#include <memory>

#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/log_context.h"
#include "ipcommandbus/net_serializer.h"
#include "util/type_conversion_helpers.h"
#include "wifi_accesspoint.h"

namespace Connectivity
{
WifiAccessPointService::WifiAccessPointService()
{
}

std::shared_ptr<gen_conn::WifiAccesspointStub> WifiAccessPointService::GetStub()
{
    return nullptr;
}
bool WifiAccessPointService::Initialize(MessageDispatcher* dispatcher)
{
    return true;
}

}  // Connectivity
