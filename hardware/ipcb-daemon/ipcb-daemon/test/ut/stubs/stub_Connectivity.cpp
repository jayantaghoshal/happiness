/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <assert.h>
#include <memory>

#include "Connectivity.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/log_context.h"
#include "ipcommandbus/net_serializer.h"

// TODO!! Ensure that correct intervals are used for response timeouts in both directions...
//        Example: CurrentInternetSource:
//        'This signal must implement the following specific WFR timerout. WFR timeout 5800 ms.
//         Note: The signal specific WFR timeout value must be adjustable in Local Config.'
//        Would guess that there are default intervals specified for all operations in both directions. Check spec...

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_connectivity_log_context, "CON1", "FB Connectivity")

void ConnectivityX::cbCurrentInetSourceResponse(Message &msg,
                                                std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

void ConnectivityX::cbCurrentInetSourceNotification(Message &msg)
{
    (void)msg;
}

void ConnectivityX::SetInternetSource(Icb_InternetSource_t src,
                                      std::shared_ptr<CurrentInetSourceRespCallback> pCallback)
{
    (void)src;
    (void)pCallback;
}

void ConnectivityX::RequestInternetSource(std::shared_ptr<CurrentInetSourceRespCallback> pCallback)
{
    (void)pCallback;
}

Icb_InternetSource_t ConnectivityX::GetInternetSource()
{
    return Icb_InternetSource_t::e_Icb_InternetSource_none;
}

void ConnectivityX::cbInetGatewayAsClientResponse(Message &msg,
                                                  std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

void ConnectivityX::SetInetGatewayAsClient(Icb_Ecu_t ecu, std::shared_ptr<InetGatewayAsClientRespCallback> pCallback)
{
    (void)ecu;
    (void)pCallback;
}

void ConnectivityX::RequestInetGatewayAsClient(std::shared_ptr<InetGatewayAsClientRespCallback> pCallback)
{
    (void)pCallback;
}

Icb_Ecu_t ConnectivityX::GetInetGatewayAsClient()
{
    return Icb_Ecu_t::e_Icb_Ecu_none;
}

void ConnectivityX::cbConnectivityStatusResponse(Message &msg,
                                                 std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

void ConnectivityX::cbConnectivityStatusNotification(Message &msg)
{
    (void)msg;
}

void ConnectivityX::RequestConnectivityStatus(std::shared_ptr<ConnectivityStatusRespCallback> pCallback)
{
    (void)pCallback;
}

bool ConnectivityX::GetConnectivityStatus()
{
    return true;
}

void ConnectivityX::cbInetSourceCtrlBTReq(Message &msg)
{
    (void)msg;
}

void ConnectivityX::cbInetSourceCtrlBTNotificationReq(Message &msg)
{
    (void)msg;
}

void ConnectivityX::NotifyInternetSourceBT(Icb_OnOffSetting onOff)
{
    (void)onOff;
}

void ConnectivityX::RespondInternetSourceBT(const Icb_OnOffSetting onOff,
                                            const Message::Ecu ecu,
                                            const VccIpCmd::SenderHandleId senderHandleId)
{
    (void)onOff;
    (void)ecu;
    (void)senderHandleId;
}

void ConnectivityX::RespondErrorInternetSourceBT(const Icb_OpGeneric_Error_t error,
                                                 const Message::Ecu ecu,
                                                 const VccIpCmd::SenderHandleId senderHandleId)
{
    (void)error;
    (void)ecu;
    (void)senderHandleId;
}

void ConnectivityX::cbInetSourceCtrlBTSetReq(Message &msg)
{
    (void)msg;
}

void ConnectivityX::cbInetSourceCtrlUSBReq(Message &msg)
{
    (void)msg;
}

void ConnectivityX::cbInetSourceCtrlUSBNotificationReq(Message &msg)
{
    (void)msg;
}

void ConnectivityX::NotifyInternetSourceUSB(Icb_OnOffSetting onOff)
{
    (void)onOff;
}

void ConnectivityX::RespondInternetSourceUSB(const Icb_OnOffSetting onOff,
                                             const Message::Ecu ecu,
                                             const VccIpCmd::SenderHandleId senderHandleId)
{
    (void)onOff;
    (void)ecu;
    (void)senderHandleId;
}

void ConnectivityX::RespondErrorInternetSourceUSB(const Icb_OpGeneric_Error_t error,
                                                  const Message::Ecu ecu,
                                                  const VccIpCmd::SenderHandleId senderHandleId)
{
    (void)error;
    (void)ecu;
    (void)senderHandleId;
}

void ConnectivityX::cbInetSourceCtrlUSBSetReq(Message &msg)
{
    (void)msg;
}

void ConnectivityX::cbInetGatewayAsServerReq(Message &msg)
{
    (void)msg;
}

void ConnectivityX::RespondInetGatewayAsServer(const Icb_Ecu_t gateway,
                                               const Message::Ecu ecu,
                                               const VccIpCmd::SenderHandleId senderHandleId)
{
    (void)gateway;
    (void)ecu;
    (void)senderHandleId;
}

void ConnectivityX::RespondErrorInternetGatewayAsServer(const Icb_OpGeneric_Error_t error,
                                                        const Message::Ecu ecu,
                                                        const VccIpCmd::SenderHandleId senderHandleId)
{
    (void)error;
    (void)ecu;
    (void)senderHandleId;
}

void ConnectivityX::cbInetGatewayAsServerSetReq(Message &msg)
{
    (void)msg;
}

void ConnectivityX::init(IConnectivityServiceProvider *pListener)
{
    (void)pListener;
}

const char *ConnectivityX::InetSourceToString(Icb_InternetSource_t src)
{
    switch (src)
    {
        case e_Icb_InternetSource_none:
        {
            return "none";
        }
        break;
        case e_Icb_InternetSource_psim:
        {
            return "psim";
        }
        break;
        case e_Icb_InternetSource_bluetooth:
        {
            return "bluetooth";
        }
        break;
        case e_Icb_InternetSource_usb:
        {
            return "usb";
        }
        break;
        default:
        {
            return "undefined internet source enum";
        }
        break;
    }
}

const char *ConnectivityX::EcuToString(Icb_Ecu_t ecu)
{
    switch (ecu)
    {
        case e_Icb_Ecu_none:
        {
            return "none";
        }
        break;
        case e_Icb_Ecu_ihu:
        {
            return "ihu";
        }
        break;
        case e_Icb_Ecu_vcm:
        {
            return "vcm";
        }
        break;
        case e_Icb_Ecu_tem:
        {
            return "tem";
        }
        break;
        default:
        {
            return "undefined ecu enum";
        }
        break;
    }
}

}  // Connectivity
