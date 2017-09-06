/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <services/common_phone_tele_service.h>

namespace v0_conn = v0::org::volvo::connectivity;

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_common_phone_tel_log_context, "CPTELE", "Service for CommonPhoneTelematics")

CommonPhoneTelematicsService::CommonPhoneTelematicsService()
{
    IpService::service_name_ = "CPTELE";
}

bool CommonPhoneTelematicsService::Initialize(MessageDispatcher *msg_dispatcher)
{
    (void)msg_dispatcher;
    return true;
}

void CommonPhoneTelematicsService::Uninitialize()
{
}

void CommonPhoneTelematicsService::CallHandlingSetRequest(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    v0_conn::IpCommandBusTypes::AssistCallAction _action,
    uint8_t _call_id,
    v0_conn::CommonPhoneTelematicsStub::CallHandlingSetRequestReply_t _reply)
{
    (void)_client;
    (void)_action;
    (void)_call_id;
    (void)_reply;
}

void CommonPhoneTelematicsService::cbCommonPhoneTelematicsCallHandlingResponse(
    Message &msg, std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)msg;
    (void)p_caller_data;
}
}  // namespace Connectivity
