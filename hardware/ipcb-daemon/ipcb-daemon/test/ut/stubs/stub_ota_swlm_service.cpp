/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <CommonAPI/CommonAPI.hpp>
#include "ota_swlm_service.h"

namespace v0_conn = v0::org::volvo::connectivity;

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_ota_swlm_service_log_context, "OTAS", "OTA SWLM Service")

// message recieved from service manager to initialize
bool OTASwlmService::Initialize(Connectivity::MessageDispatcher *msgDispatcher,
                                std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context)
{
    (void)msgDispatcher;
    (void)common_api_main_loop_context;
    return true;
}

void OTASwlmService::AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
{
}

void OTASwlmService::sendError(const VccIpCmd::OperationId operationId,
                               const Icb_ErrorCode_t errorCode,
                               const Message::Ecu ecu,
                               const VccIpCmd::SenderHandleId senderHandleId)
{
    (void)operationId;
    (void)errorCode;
    (void)ecu;
    (void)senderHandleId;
}

// recieve notification from swlm device response install
void OTASwlmService::DeviceInstallNotificationCallback(
    const v0_conn::OTATypes::InstallStatus &installStatus,
    const v0_conn::OTATypes::InstallProgress &installProgress,
    const std::vector<v0_conn::OTATypes::InstalledData> &installedList)
{
    (void)installStatus;
    (void)installProgress;
    (void)installedList;
}

// The call recieved from VCM
void OTASwlmService::cbDeviceInstallSetReq(Message &msg)
{
    (void)msg;
}

void OTASwlmService::sendDeviceInstallResponse(const CommonAPI::CallStatus &status,
                                               const v0_conn::OTATypes::InstallStatus &installStatus,
                                               const v0_conn::OTATypes::InstallProgress &installProgress,
                                               const std::vector<v0_conn::OTATypes::InstalledData> &installedList,
                                               const Message::Ecu msg_target_ecu,
                                               const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)status;
    (void)installStatus;
    (void)installProgress;
    (void)installedList;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

// we recive the info from swlm and we have to send out device install response to VCM
void OTASwlmService::DeviceInstallSetRequestCallback(const CommonAPI::CallStatus &status,
                                                     const v0_conn::OTATypes::InstallStatus &installStatus,
                                                     const v0_conn::OTATypes::InstallProgress &installProgress,
                                                     const std::vector<v0_conn::OTATypes::InstalledData> &installedList,
                                                     const Message::Ecu msg_target_ecu,
                                                     const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)status;
    (void)installStatus;
    (void)installProgress;
    (void)installedList;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void OTASwlmService::cbDeviceInstallReq(Message &msg)
{
    (void)msg;
}

void OTASwlmService::DeviceInstallRequestCallback(const CommonAPI::CallStatus &status,
                                                  const v0_conn::OTATypes::InstallStatus &installStatus,
                                                  const v0_conn::OTATypes::InstallProgress &installProgress,
                                                  const std::vector<v0_conn::OTATypes::InstalledData> &installedList,
                                                  const Message::Ecu msg_target_ecu,
                                                  const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)status;
    (void)installStatus;
    (void)installProgress;
    (void)installedList;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

// IHU recieve this message from other ecus, we need to push them in subscriber list and send them notification
void OTASwlmService::cbDeviceInstallNotificationReq(Message &msg)
{
    (void)msg;
}

void OTASwlmService::cbLocalStorageReq(Message &msg)
{
    (void)msg;
}

void OTASwlmService::LocalStorageRequestCallback(const CommonAPI::CallStatus &status,
                                                 bool available,
                                                 const Message::Ecu msg_target_ecu,
                                                 const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)status;
    (void)available;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void OTASwlmService::cbLocalStorageNotificationReq(Message &msg)
{
    (void)msg;
}

void OTASwlmService::LocalStorageNotificationCallback(bool available)
{
    (void)available;
}
}
