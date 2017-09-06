/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <services/diagnostics_client.h>

namespace Connectivity
{
namespace gen_diag = v1::com::ihu::Diagnostics;

LOG_DECLARE_DEFAULT_CONTEXT(g_diagnostics_client_log_context, "DIAG", "Diagnostics Client")

void DiagnosticsClient::Initialize(Connectivity::MessageDispatcher *msg_dispatcher,
                                   std::shared_ptr<CommonAPI::MainLoopContext> commonApiMainloopContext)
{
    (void)msg_dispatcher;
    (void)commonApiMainloopContext;
}

void DiagnosticsClient::AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
{
    (void)status;
}
void DiagnosticsClient::SubscribeOnKeys(const std::vector<uint16_t> &keys)
{
    (void)keys;
}
void DiagnosticsClient::SubscribeOnKeysAsyncCallback(const CommonAPI::CallStatus &call_status)
{
    (void)call_status;
}
void DiagnosticsClient::SendMessageToDiag(const uint16_t key, const std::vector<uint8_t> &payload)
{
    (void)key;
    (void)payload;
}

void DiagnosticsClient::MessageToDiagAsyncCallback(const CommonAPI::CallStatus &call_status)
{
    (void)call_status;
}
void DiagnosticsClient::SetInvalidDataFromTEM(const bool error)
{
    (void)error;
}
void DiagnosticsClient::SetInvalidDataFromVCM(const bool error)
{
    (void)error;
}
void DiagnosticsClient::SetInvalidData(const Message::Ecu ecu, const bool error)
{
    (void)ecu;
    (void)error;
}

}  // namespace
