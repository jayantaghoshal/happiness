/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <assert.h>
#include <memory>

#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/log_context.h"
#include "ipcommandbus/net_serializer.h"
#include "wlan_service.h"

// NOTE! Don't change this path!
// For unit tests and coverage to work as intended,
// the path must also be valid relative to the stubs directory.
#include "util/type_conversion_helpers.h"

// TODO!! Ensure that correct intervals are used for response timeouts in both directions...
//        Example: CurrentInternetSource:
//        'This signal must implement the following specific WFR timerout. WFR timeout 5800 ms.
//         Note: The signal specific WFR timeout value must be adjustable in Local Config.'
//        Would guess that there are default intervals specified for all operations in both directions. Check spec...

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_wlan_log_context, "WLAN", "Service for the WLAN interface")

WlanService::WlanService()
{
}

bool WlanService::Initialize(Connectivity::MessageDispatcher *msgDispatcher)
{
    (void)msgDispatcher;
    return true;
}

void WlanService::Uninitialize()
{
}
/**
 * description: (0x040D WLANSTAStatus.Request)
 */
void WlanService::WlanStaStatus(const std::shared_ptr<CommonAPI::ClientId> _client,
                                WlanStub::WlanStaStatusReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

void WlanService::cbWlanStaStatusResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)p_caller_data;
}

void WlanService::cbWlanStaStatusNotification(Message &msg)
{
    (void)msg;
}

void WlanService::cbWlanStaConnectResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)pCallerData;
}

void WlanService::cbWlanStaConnectNotification(Message &msg)
{
    (void)msg;
}

void WlanService::cbWlanStaDisconnectResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)pCallerData;
}

/**
 * description: (0x0400 WLANMode.Request)
    Request for the current WLAN mode.
 */
void WlanService::WlanModeRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                  WlanStub::WlanModeRequestReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

/**
 * description: (0x0400 WLANMode.SetRequest)
    Requests to set a new WLAN mode, and confirms
 *   the success of this action with a response containing the mode.
 */
void WlanService::WlanModeSetRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                     gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode _inWlanMode,
                                     WlanStub::WlanModeSetRequestReply_t _reply)
{
    (void)_client;
    (void)_inWlanMode;
    (void)_reply;
}

void WlanService::cbWlanModeResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)p_caller_data;
}

void WlanService::cbWlanModeNotification(Message &msg)
{
    (void)msg;
}

/**
 * description: (0x0408 WLANSTAConnect.SetRequest)
        Connect to a WiFi network.
 */
void WlanService::WlanStaConnectSetRequest(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _inSsid,
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanExternalPassword _inWlanPassword,
    WlanStub::WlanStaConnectSetRequestReply_t _reply)
{
    (void)_client;
    (void)_inSsid;
    (void)_inWlanPassword;
    (void)_reply;
}

/**
 * description: (0x0409 WLANSTADisconnect.OpGeneric-Request)
        The client requests VCM
 *   (Wi-Fi STA) to disconnect from the currently connected external AP.
 */
void WlanService::WlanStaDisconnectRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                           WlanStub::WlanStaDisconnectRequestReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

/**
 * description: (0x040E WLANSTARememberedNetwks.OpGeneric-Request)
        When Wi-Fi STA gets
 *   successfully connected to an external network for the first time,
        the
 *   external network automatically becomes a remembered network.
        VCM
 *   (Wi-Fi STA) shall provide the remembered network list to IHU HMI on request.

 *          Remembered network list is ordered by most recent remembered network on
 *   the top and so on.
        The list at maximum can contain 50 networks.

 *      The oldest remembered network will be dropped to accommodate the latest
 *   remembered network if the maximum number has already reached.
 */
void WlanService::WlanStaRememberedNetwksRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                                 WlanStub::WlanStaRememberedNetwksRequestReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

void WlanService::cbWlanStaRememberedNetwksResponse(Message &msg,
                                                    std::shared_ptr<MessageDispatcher::CallerData> p_caller_data)
{
    (void)p_caller_data;
}

/**
 * description: (0x040F WLANSTAForgetRememberedNetwk.SetRequest)
        This operation is
 *   used when user request VCM (Wi-Fi) to forget a remembered network.

 *   VCM (Wi-Fi) forgets the network by deleting login information and removing it
 *   from the remembered networks list.
        The information will be shown in
 *   HMI.
        There are two scenarios related to this operation:
        - If
 *   the network is an available network in network list, it will still be
 *   displayed as available.
        - If user wishes to forget a remembered
 *   network to which the Wi-Fi STA is currently connected, Wi-Fi STA will be
 *   disconnected as well.
 */
void WlanService::WlanStaForgetRememberedNetwkSetRequest(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid _inSsidToForget,
    WlanStub::WlanStaForgetRememberedNetwkSetRequestReply_t _reply)
{
    (void)_client;
    (void)_inSsidToForget;
    (void)_reply;
}

void WlanService::cbWlanStaForgetRememberedNetwkResponse(Message &msg,
                                                         std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

void WlanService::cbWlanChannelFreqInfoNotification(Message &msg)
{
    (void)msg;
}

}  // Connectivity
