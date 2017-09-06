/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
/*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#include <assert.h>
#include <memory>

#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/log_context.h"
#include "ipcommandbus/net_serializer.h"
#include "ota_service.h"
#include "util/type_conversion_helpers.h"

namespace v0_conn = v0::org::volvo::connectivity;

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_ota_log_context, "OTA", "Service for the OTA interface")

OTAService::OTAService()
{
    IpService::service_name_ = "OTA";
}

bool OTAService::Initialize(Connectivity::MessageDispatcher *msgDispatcher)
{
    (void)msgDispatcher;
    return true;
}

void OTAService::Uninitialize()
{
}

void OTAService::DownloadStatusRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                       OTAStub::DownloadStatusRequestReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

void OTAService::DownloadSoftwareSetRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                            std::vector<v0_conn::OTATypes::UUID> _downloadList,
                                            OTAStub::DownloadSoftwareSetRequestReply_t _reply)
{
    (void)_client;
    (void)_downloadList;
    (void)_reply;
}

void OTAService::DeletedSoftwareSetRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                           std::vector<v0_conn::OTATypes::PartIdentifier> _deletedSoftware,
                                           OTAStub::DeletedSoftwareSetRequestReply_t _reply)
{
    (void)_client;
    (void)_deletedSoftware;
    (void)_reply;
}

void OTAService::CancelDownloadSetRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                          v0_conn::OTATypes::UUID _software,
                                          OTAStub::CancelDownloadSetRequestReply_t _reply)
{
    (void)_client;
    (void)_software;
    (void)_reply;
}

void OTAService::cbOTADownloadStatusNotification(Message &msg)
{
    (void)msg;
}

void OTAService::cbOTADownloadStatusResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

void OTAService::cbOTADownloadSoftwareResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

void OTAService::cbOTADeletedSoftwareResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

void OTAService::cbOTACancelDownloadResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

}  // namespace Connectivity
