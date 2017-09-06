/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "diagnostic_management_service.h"

namespace Connectivity
{
DiagnosticManagementService::DiagnosticManagementService()
{
}
DiagnosticManagementService::~DiagnosticManagementService()
{
}
bool DiagnosticManagementService::Initialize(MessageDispatcher *message_dispatcher)
{
    return true;
}
void DiagnosticManagementService::Uninitialize()
{
}

void DiagnosticManagementService::TextFromUserToOperator(
    const std::shared_ptr<CommonAPI::ClientId> client,
    uint8_t messageId,
    std::__cxx11::string textChatMessage,
    v0::org::volvo::connectivity::DiagnosticManagementStub::TextFromUserToOperatorReply_t reply)
{
}
void DiagnosticManagementService::RequestRemoteDiagnostics(
    const std::shared_ptr<CommonAPI::ClientId> client,
    bool request,
    v0::org::volvo::connectivity::DiagnosticManagementStub::RequestRemoteDiagnosticsReply_t reply)
{
}
void DiagnosticManagementService::AbortRemoteDiagnostics(
    const std::shared_ptr<CommonAPI::ClientId> client,
    bool request,
    v0::org::volvo::connectivity::DiagnosticManagementStub::AbortRemoteDiagnosticsReply_t reply)
{
}
void DiagnosticManagementService::UpdateDaCMode(
    const std::shared_ptr<CommonAPI::ClientId> client,
    v0::org::volvo::connectivity::IpCommandBusTypes::DiagMgm_UpdateDacMode updateDaCMode,
    v0::org::volvo::connectivity::DiagnosticManagementStub::UpdateDaCModeReply_t reply)
{
}

void DiagnosticManagementService::cbRemoteDiagnosticLinkNotification(Message &msg)
{
}
void DiagnosticManagementService::cbRemoteDiagnosticInfoNotification(Message &msg)
{
}
void DiagnosticManagementService::cbCurrentJ2534SessionNotification(Message &msg)
{
}
void DiagnosticManagementService::cbCurrentDoIPStateNotification(Message &msg)
{
}
void DiagnosticManagementService::cbCurrentDoIPConnectionsNotification(Message &msg)
{
}
void DiagnosticManagementService::cbRequestUserAuthentication(Message &msg)
{
}
void DiagnosticManagementService::cbGenericResponse(Message &msg,
                                                    std::shared_ptr<MessageDispatcher::CallerData> callerData)
{
}
}
