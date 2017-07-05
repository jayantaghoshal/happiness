/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <ivi-logging.h>
#include "ipcommandbus/log_context.h"
#include "voc_backend_proxy.h"

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_voc_backend_log_context, "VOCB", "voc backend proxy")

bool VOCBackendProxy::Initialize(MessageDispatcher *msg_dispatcher,
                                 std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context)
{
    (void)msg_dispatcher;
    (void)common_api_main_loop_context;

    return true;
}

void VOCBackendProxy::AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
{
    (void)status;
}

void VOCBackendProxy::cbTextMessageSetRequest(Message &msg)
{
    (void)msg;
}

void VOCBackendProxy::sendError(const VccIpCmd::OperationId operation_id,
                                const Icb_ErrorCode_t error_code,
                                const Message::Ecu ecu,
                                const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)operation_id;
    (void)error_code;
    (void)ecu;
    (void)sender_handle_id;
}
}  // namespace Connectivity
