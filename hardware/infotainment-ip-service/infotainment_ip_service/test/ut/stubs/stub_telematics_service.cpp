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
#include "util/type_conversion_helpers.h"

#include "telematics_service.h"

namespace v0_conn = v0::org::volvo::connectivity;

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_telematics_log_context, "TELE", "Service for Telematics")

TelematicsService::TelematicsService()
{
    IpService::service_name_ = "TELE";
}

bool TelematicsService::Initialize(MessageDispatcher *msgDispatcher)
{
    (void)msgDispatcher;

    return true;
}

void TelematicsService::Uninitialize()
{
}

void TelematicsService::SoHPacketSendSetRequestNoReturn(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    v0_conn::IpCommandBusTypes::SBTrigger _trigger,
    std::string _text,
    v0_conn::IpCommandBusTypes::SBWarning _mainCause,
    std::vector<v0_conn::IpCommandBusTypes::SBWarning> _warnings,
    v0_conn::TelematicsStub::SoHPacketSendSetRequestNoReturnReply_t _reply)
{
    (void)_client;
    (void)_trigger;
    (void)_text;
    (void)_mainCause;
    (void)_warnings;
    (void)_reply;
}

void TelematicsService::TelematicSettingsSetRequest(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    ::v0::org::volvo::connectivity::IpCommandBusTypes::TEM_TelematicSettings _inTelematicSetting,
    v0_conn::TelematicsStub::TelematicSettingsSetRequestReply_t _reply)
{
    (void)_client;
    (void)_inTelematicSetting;
    (void)_reply;
}

void TelematicsService::ECallSettingsSetRequest(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    ::v0_1::org::volvo::connectivity::IpCommandBusTypes::TEM_PreferredEcalls _inPreferredEcalls,
    v0_conn::TelematicsStub::ECallSettingsSetRequestReply_t _reply)
{
    (void)_client;
    (void)_inPreferredEcalls;
    (void)_reply;
}

void TelematicsService::SubscriptionActivationSetRequest(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    v0_conn::IpCommandBusTypes::TEM_ActivationStatusChange _newStatus,
    uint32_t _activationCode,
    v0_conn::TelematicsStub::SubscriptionActivationSetRequestReply_t _reply)
{
    (void)_client;
    (void)_newStatus;
    (void)_activationCode;
    (void)_reply;
}

void TelematicsService::PositionDataRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                            v0_conn::TelematicsStub::PositionDataRequestReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

void TelematicsService::ServiceActivationSetRequest(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    v0_conn::IpCommandBusTypes::TEM_OnCallService _service,
    v0_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting _action,
    v0_conn::TelematicsStub::ServiceActivationSetRequestReply_t _reply)
{
    (void)_client;
    (void)_service;
    (void)_action;
    (void)_reply;
}

void TelematicsService::SVTStatusRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                         v0_conn::TelematicsStub::SVTStatusRequestReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

void TelematicsService::TNStatusRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                        v0_conn::TelematicsStub::TNStatusRequestReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

void TelematicsService::fireSoHPackageUploadedNotificationEvent(
    const std::string &_packetId, const v0_conn::IpCommandBusTypes::GenericOkStatus &_result)
{
    (void)_packetId;
    (void)_result;
}

void TelematicsService::fireSubscriptionActivationNotificationEvent(
    const v0_conn::IpCommandBusTypes::TEM_ActivationStatus &_status)
{
    (void)_status;
}

void TelematicsService::fireRescueStatusNotificationEvent(
    const v0_conn::IpCommandBusTypes::TEM_RescueStatus &_raStatus,
    const v0_conn::IpCommandBusTypes::TEM_RescueStatus &_eaStatus,
    const v0_conn::IpCommandBusTypes::TEM_RescueStatus &_sdnStatus,
    const v0_conn::IpCommandBusTypes::TEM_VoiceStatus &_voiceStatus,
    const v0_conn::IpCommandBusTypes::SourceStatus &_voiceSource,
    const v0_conn::IpCommandBusTypes::TEM_MessageStatus &_messageStatus,
    const v0_conn::IpCommandBusTypes::TEM_ButtonStatus &_buttonStatus,
    const v0_conn::IpCommandBusTypes::TEM_PSAPStatus &_psapConfirmStatus,
    const v0_conn::IpCommandBusTypes::GenericOkStatus &_backupAudioStatus)
{
    (void)_raStatus;
    (void)_eaStatus;
    (void)_sdnStatus;
    (void)_voiceStatus;
    (void)_voiceSource;
    (void)_messageStatus;
    (void)_buttonStatus;
    (void)_psapConfirmStatus;
    (void)_backupAudioStatus;
}

void TelematicsService::fireSVTStatusNotificationEvent(const v0_conn::IpCommandBusTypes::SecurityStatus &_svtStatus)
{
    (void)_svtStatus;
}

void TelematicsService::fireTNStatusNotificationEvent(const v0_conn::IpCommandBusTypes::SecurityStatus &_tnStatus)
{
    (void)_tnStatus;
}

void TelematicsService::cbTelematicsSoHPackageUploadedNotification(Message &msg)
{
    (void)msg;
}

void TelematicsService::cbTelematicsSubscriptionActivationResponse(
    Message &msg, std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)msg;
    (void)p_caller_data;
}

void TelematicsService::cbTelematicsPositionDataResponse(Message &msg,
                                                         std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)msg;
    (void)p_caller_data;
}

void TelematicsService::cbTelematicsServiceActivationResponse(
    Message &msg, std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)msg;
    (void)p_caller_data;
}

void TelematicsService::cbTelematicsSubscriptionActivationNotification(Message &msg)
{
    (void)msg;
}

void TelematicsService::cbTelematicsRescueStatusNotification(Message &msg)
{
    (void)msg;
}

void TelematicsService::cbTelematicsSVTStatusResponse(Message &msg,
                                                      std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)msg;
    (void)p_caller_data;
}

void TelematicsService::cbTelematicsSVTStatusNotification(Message &msg)
{
    (void)msg;
}

void TelematicsService::cbTelematicsTNStatusResponse(Message &msg,
                                                     std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)msg;
    (void)p_caller_data;
}

void TelematicsService::cbTelematicsTNStatusNotification(Message &msg)
{
    (void)msg;
}

void TelematicsService::cbTelematicSettingstResponse(Message &msg,
                                                     std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)msg;
    (void)p_caller_data;
}

void TelematicsService::cbECallSettingsResponse(Message &msg,
                                                std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)msg;
    (void)p_caller_data;
}
}  // namespace Connectivity
