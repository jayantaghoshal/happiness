/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "diagnostic_management_client.h"

namespace Connectivity
{
DiagnosticManagementClient::DiagnosticManagementClient()
{
}
DiagnosticManagementClient::~DiagnosticManagementClient()
{
}
bool DiagnosticManagementClient::Initialize(Connectivity::MessageDispatcher *msg_dispatcher,
                                            std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context)
{
    return true;
}
bool DiagnosticManagementClient::Initialize(
    Connectivity::MessageDispatcher *msg_dispatcher,
    std::shared_ptr<gen_conn::DiagnosticManagementAuthenticationProxyBase> diagnostic_management_auth_proxy)
{
    return true;
}

void DiagnosticManagementClient::cbTextFromOperatorToUserNotification(Message &msg)
{
}
void DiagnosticManagementClient::cbRequestUserAuthentication(Message &msg)
{
}

void DiagnosticManagementClient::cbCapiRequestUserAuthResponse(
    RequestData requestInformation,
    const CommonAPI::CallStatus &callStatus,
    const gen_conn::IpCommandBusTypes::DiagMgm_UserAuthCredentials &response)
{
}
void DiagnosticManagementClient::cbCapiTextFromOperatorToUserResponse(RequestData requestInformation,
                                                                      const CommonAPI::CallStatus &callStatus)
{
}
}
