/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "rvdc_client.h"

namespace Connectivity
{
RvdcClient::RvdcClient()
{
}
RvdcClient::~RvdcClient()
{
}
void RvdcClient::Initialize(MessageDispatcher *msg_dispatcher,
                            std::shared_ptr<CommonAPI::MainLoopContext> context,
                            std::shared_ptr<gen_privacypolicy::RvdcProxyBase> proxy)
{
}
void RvdcClient::cbApprovalStatusRequest(Message &msg)
{
}
void RvdcClient::cbApprovalStatusReponse(RequestData requestInformation,
                                         const gen_privacypolicy::Rvdc::ApprovalStatuses &status,
                                         const CommonAPI::CallStatus &callStatus)
{
}
void RvdcClient::ApprovalStatusNotification(const gen_privacypolicy::Rvdc::ApprovalStatuses &status)
{
}
}
