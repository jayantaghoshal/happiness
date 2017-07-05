/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "services/connectivity_service.h"

#include <functional>
#include <iostream>
#include <memory>

#include <Connectivity.h>
//#include <ipcommandbus/MessageDispatcher.h>
//#include <ipcommandbus/Timer.h>

//#include <v0/org/volvo/connectivity/ConnectivityServiceStub.hpp>
//#include <v0/org/volvocars/source_selector/SourceSelectorBackendProxy.hpp>
#include "services/connectivity_service_stub.h"

#include "../util/type_conversion_helpers.h"

static const std::uint32_t RECONNECT_INTERVAL = 2000;

extern "C" {
#include "infotainmentIpBus.h"
}

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_connectivity_service_log_context, "CON0", "Connectivity Service")

//====-----------------------------------------------------------------------------------------=========
// Constructor and destructor
//
// Thread context: Main thread
//====-----------------------------------------------------------------------------------------=========
ConnectivityService::ConnectivityService() : source_selector_available_(false)
{
}

// This constructor is intended for UNIT TESTS ONLY, hence it's private.
ConnectivityService::ConnectivityService(std::shared_ptr<generated::ConnectivityServiceStub> ipc_stub)
{
}

ConnectivityService::~ConnectivityService()
{
}
//====-----------------------------------------------------------------------------------------=========
// Public methods
//
// Thread context: Main thread
//====-----------------------------------------------------------------------------------------=========
bool ConnectivityService::Initialize(MessageDispatcher *message_dispatcher,
                                     std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context)
{
    (void)message_dispatcher;
    (void)common_api_main_loop_context;
    return true;
}

void ConnectivityService::AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
{
    (void)status;
}

void ConnectivityService::InetSourceCtrlBtNotificationCallback(
    const generated::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting on_off)
{
    (void)on_off;
}

void ConnectivityService::InetSourceCtrlUsbNotificationCallback(
    const generated::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting on_off)
{
    (void)on_off;
}

void ConnectivityService::Uninitialize()
{
}

void ConnectivityService::RequestInternetGateway(const Message::Ecu msg_target_ecu,
                                                 const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::RequestInternetSourceCtrlBT(const Message::Ecu msg_target_ecu,
                                                      const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::RequestInternetSourceCtrlUSB(const Message::Ecu msg_target_ecu,
                                                       const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::NotifyConnectivityStatus(BOOL general_connectivity_available)
{
    (void)general_connectivity_available;
}

void ConnectivityService::NotifyCurrentInternetSource(const Icb_InternetSource_t internet_source)
{
    (void)internet_source;
}

void ConnectivityService::SetInternetSourceCtrlBT(const Icb_OnOffSetting_t on_off,
                                                  const Message::Ecu msg_target_ecu,
                                                  const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)on_off;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::SetInternetSourceCtrlUSB(const Icb_OnOffSetting_t on_off,
                                                   const Message::Ecu msg_target_ecu,
                                                   const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)on_off;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::SetInternetGateway(const Icb_Ecu_t gateway_ecu,
                                             const Message::Ecu msg_target_ecu,
                                             const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)gateway_ecu;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}
//====-----------------------------------------------------------------------------------------=========
// END IConnectivityServiceProvider
// Thread context: Main thread
//====-----------------------------------------------------------------------------------------=========

//====-----------------------------------------------------------------------------------------=========
// IConnectivityServiceProxy
//
// These methods will get called by the Common API runtime, triggered by external RPC
// from the source selector application.
//
// Thread context: Common API thread
//====-----------------------------------------------------------------------------------------=========
void ConnectivityService::GetCurrentInternetSource(
    const std::shared_ptr<CommonAPI::ClientId> client_id,
    generated::ConnectivityServiceStub::GetCurrentInternetSourceReply_t common_api_reply_callback)
{
    (void)client_id;
    (void)common_api_reply_callback;
}

void ConnectivityService::SetCurrentInternetSource(
    const std::shared_ptr<CommonAPI::ClientId> client_id,
    generated::IpCommandBusTypes::Infotainment_IP_bus_InternetSource current_internet_source,
    ConnectivityServiceStub::SetCurrentInternetSourceReply_t common_api_reply_callback)
{
    (void)client_id;
    (void)current_internet_source;
    (void)common_api_reply_callback;
}

void ConnectivityService::GetConnectivityStatus(
    const std::shared_ptr<CommonAPI::ClientId> client_id,
    ConnectivityServiceStub::GetConnectivityStatusReply_t common_api_reply_callback)
{
    (void)client_id;
    (void)common_api_reply_callback;
}
//====-----------------------------------------------------------------------------------------=========
// END IConnectivityServiceProxy
// Thread context: Common API thread
//====-----------------------------------------------------------------------------------------=========

//====-----------------------------------------------------------------------------------------=========
// Callbacks for IConnectivityServiceProxy call replies
//
// These callbacks will be invoked by the Connectivity Functional block, triggered by incoming responses
// from the VCM.
//
// Thread context: Main thread
//====-----------------------------------------------------------------------------------------=========
void ConnectivityService::OnGetCurrentInternetSourceResponse(
    const Icb_InternetSource_t internet_source,
    std::shared_ptr<ConnectivityX::CurrentInetSourceRespCallback> ipcb_callback_obj,
    generated::ConnectivityServiceStub::GetCurrentInternetSourceReply_t common_api_reply_callback)
{
    (void)ipcb_callback_obj;
    (void)common_api_reply_callback;
}

void ConnectivityService::OnSetCurrentInternetSourceResponse(
    const Icb_InternetSource_t internet_source,
    std::shared_ptr<ConnectivityX::CurrentInetSourceRespCallback> ipcb_callback_obj,
    generated::ConnectivityServiceStub::SetCurrentInternetSourceReply_t common_api_reply_callback)
{
    (void)internet_source;
    (void)ipcb_callback_obj;
    (void)common_api_reply_callback;
}

void ConnectivityService::OnGetConnectivityStatusResponse(
    const BOOL general_connectivity_available,
    std::shared_ptr<ConnectivityX::ConnectivityStatusRespCallback> ipcb_callback_obj,
    generated::ConnectivityServiceStub::GetConnectivityStatusReply_t common_api_reply_callback)
{
    (void)general_connectivity_available;
    (void)ipcb_callback_obj;
    (void)common_api_reply_callback;
}

//====-----------------------------------------------------------------------------------------=========
// Callbacks for SourceSelectorBackendProxy call replies
//
// These callbacks will be invoked by the Common API Runtime, triggered by incoming responses
// from the source selector.
//
// Thread context: Common API thread
//====-----------------------------------------------------------------------------------------=========
void ConnectivityService::OnRequestInternetGateway(
    const CommonAPI::CallStatus &call_status,
    const generated::IpCommandBusTypes::Infotainment_IP_bus_Ecu &internet_gateway_ecu,
    const Message::Ecu msg_target_ecu,
    const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)call_status;
    (void)internet_gateway_ecu;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::OnRequestInternetSourceCtrlBt(
    const CommonAPI::CallStatus &call_status,
    const generated::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting &on_off,
    const Message::Ecu msg_target_ecu,
    const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)call_status;
    (void)on_off;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::OnRequestInternetSourceCtrlUsb(
    const CommonAPI::CallStatus &call_status,
    const generated::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting &on_off,
    const Message::Ecu msg_target_ecu,
    const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)call_status;
    (void)on_off;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::OnSetInternetSourceCtrlBT(
    const CommonAPI::CallStatus &call_status,
    const generated::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting &on_off,
    const Message::Ecu msg_target_ecu,
    const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)call_status;
    (void)on_off;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::OnSetInternetSourceCtrlUSB(
    const CommonAPI::CallStatus &call_status,
    const generated::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting &on_off,
    const Message::Ecu msg_target_ecu,
    const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)call_status;
    (void)on_off;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}

void ConnectivityService::OnSetInternetGateway(
    const CommonAPI::CallStatus &call_status,
    const generated::IpCommandBusTypes::Infotainment_IP_bus_Ecu &internet_gateway_ecu,
    const Message::Ecu msg_target_ecu,
    const VccIpCmd::SenderHandleId sender_handle_id)
{
    (void)call_status;
    (void)internet_gateway_ecu;
    (void)msg_target_ecu;
    (void)sender_handle_id;
}
}  // namespace Connectivity
