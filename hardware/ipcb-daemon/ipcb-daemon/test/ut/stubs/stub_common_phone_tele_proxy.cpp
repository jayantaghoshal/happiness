/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <ivi-logging.h>
#include "common_phone_tele_proxy.h"
#include "ipcommandbus/log_context.h"

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_common_tele_proxy_log_context, "CPTP", "voc backend proxy")

bool CommonPhoneTeleProxy::Initialize(MessageDispatcher *msg_dispatcher,
                                      std::shared_ptr<CommonAPI::MainLoopContext> commonApiMainLoopContext)
{
    (void)msg_dispatcher;
    (void)commonApiMainLoopContext;

    return true;
}

void CommonPhoneTeleProxy::AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
{
    (void)status;
}

void CommonPhoneTeleProxy::cbPremiumAudioSetRequest(Message &msg)
{
    (void)msg;
}

void SubscriptionPremiumAudioNotificationCallback(v0_conn::IpCommandBusTypes::AudioStatus audio_status)
{
    (void)audio_status;
}

void CommonPhoneTeleProxy::sendError(const VccIpCmd::OperationId operation_id,
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
