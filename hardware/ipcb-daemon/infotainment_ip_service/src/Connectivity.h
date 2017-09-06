/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <atomic>
#include <cstdint>
#include <functional>
#include <list>

#include "ipcommandbus/Message.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/vcc_pdu_header.h"
#if !defined(ENABLE_TESTS)
#include "IpService.h"
#else  // ! defined(ENABLE_TESTS)
#include "mock_ip_service.h"
#endif  // ! defined(ENABLE_TESTS)

namespace Connectivity
{
/**
 *
 */
class ConnectivityX :
#if !defined(ENABLE_TESTS)
    public IpService
#else   // ! defined(ENABLE_TESTS)
    public MockIpService
#endif  // ! defined(ENABLE_TESTS)
{
public:
    ConnectivityX()
    {
        service_name_ = "CONN";
    }

    // Implement IConnectivityServiceProvider to handle incoming messages from the VCM.
    // (Exception: Response messages are handled separately.)
    // The main purpose of this responsibility split is to be able to move the source
    // selector logic out of the ip command bus layer.
    class IConnectivityServiceProvider
    {
    public:
        virtual ~IConnectivityServiceProvider()=default;
        // Operations where IHU is _client_ node on ip command bus:

        // ConnectivityStatus-Notification
        virtual void NotifyConnectivityStatus(bool general_connectivity_available) = 0;

        // CurrentInternetSource-Notification
        virtual void NotifyCurrentInternetSource(const Icb_InternetSource_t internet_source) = 0;

        // Operations where IHU is _server_ node on ip command bus:

        // InternetGateway-Request, reply with RespondInetGatewayAsServer() or RespondErrorInternetGatewayAsServer().
        virtual void RequestInternetGateway(const Message::Ecu msg_target_ecu,
                                            const VccIpCmd::SenderHandleId sender_handle_id) = 0;

        // InternetSourceCtrlBT-Request, reply with RespondInternetSourceBT() or RespondErrorInternetSourceBT().
        virtual void RequestInternetSourceCtrlBT(const Message::Ecu msg_target_ecu,
                                                 const VccIpCmd::SenderHandleId sender_handle_id) = 0;

        // InternetSourceCtrlUSB-Request, reply with RespondInternetSourceUSB() or RespondErrorInternetSourceUSB().
        virtual void RequestInternetSourceCtrlUSB(const Message::Ecu msg_target_ecu,
                                                  const VccIpCmd::SenderHandleId sender_handle_id) = 0;

        // InternetSourceCtrlBT-SetRequest, reply with RespondInternetSourceBT() or RespondErrorInternetSourceBT().
        virtual void SetInternetSourceCtrlBT(const Icb_OnOffSetting_t on_off,
                                             const Message::Ecu msg_target_ecu,
                                             const VccIpCmd::SenderHandleId sender_handle_id) = 0;

        // InternetSourceCtrlUSB-SetRequest, reply with RespondInternetSourceUSB() or RespondErrorInternetSourceUSB().
        virtual void SetInternetSourceCtrlUSB(const Icb_OnOffSetting_t on_off,
                                              const Message::Ecu msg_target_ecu,
                                              const VccIpCmd::SenderHandleId sender_handle_id) = 0;

        // InternetGateway-SetRequest, reply with RespondInetGatewayAsServer() or RespondErrorInternetGatewayAsServer().
        virtual void SetInternetGateway(const Icb_Ecu_t gateway_ecu,
                                        const Message::Ecu msg_target_ecu,
                                        const VccIpCmd::SenderHandleId sender_handle_id) = 0;
    };

    void init(IConnectivityServiceProvider *pListener);

    //// Methods for sending messages to VCM, for services where the IHU is CLIENT node on ip command bus

    struct CurrentInetSourceRespCallback : public MessageDispatcher::CallerData
    {
        typedef std::function<void(const Icb_InternetSource_t, std::shared_ptr<CurrentInetSourceRespCallback>)>
            RespCallbackType;
        RespCallbackType callback;
    };

    void SetInternetSource(Icb_InternetSource_t src, std::shared_ptr<CurrentInetSourceRespCallback> pCallback);
    void RequestInternetSource(std::shared_ptr<CurrentInetSourceRespCallback> pCallback);
    Icb_InternetSource_t GetInternetSource();

    struct InetGatewayAsClientRespCallback : public MessageDispatcher::CallerData
    {
        typedef std::function<void(const Icb_Ecu_t, std::shared_ptr<InetGatewayAsClientRespCallback>)> RespCallbackType;
        RespCallbackType callback;
    };

    // Note: The functions here are with respect to IHU as CLIENT node, TEM as server node, .
    //       Two sets of functions exist since we support this service both as server and as client!
    void SetInetGatewayAsClient(Icb_Ecu_t ecu, std::shared_ptr<InetGatewayAsClientRespCallback> pCallback);
    void RequestInetGatewayAsClient(std::shared_ptr<InetGatewayAsClientRespCallback> pCallback);
    Icb_Ecu_t GetInetGatewayAsClient();

    struct ConnectivityStatusRespCallback : public MessageDispatcher::CallerData
    {
        typedef std::function<void(const bool, std::shared_ptr<ConnectivityStatusRespCallback>)> RespCallbackType;
        RespCallbackType callback;
    };

    // (There is no set operation for connectivity status.)
    void RequestConnectivityStatus(std::shared_ptr<ConnectivityStatusRespCallback> pCallback);
    bool GetConnectivityStatus();

    //// Methods for sending messages to VCM, for services where the IHU is SERVER node on ip command bus

    void NotifyInternetSourceBT(const Icb_OnOffSetting onOff);
    void RespondInternetSourceBT(const Icb_OnOffSetting onOff,
                                 const Message::Ecu ecu,
                                 const VccIpCmd::SenderHandleId senderHandleId);
    void RespondErrorInternetSourceBT(const Icb_OpGeneric_Error_t error,
                                      const Message::Ecu ecu,
                                      const VccIpCmd::SenderHandleId senderHandleId);

    void NotifyInternetSourceUSB(const Icb_OnOffSetting onOff);
    void RespondInternetSourceUSB(const Icb_OnOffSetting onOff,
                                  const Message::Ecu ecu,
                                  const VccIpCmd::SenderHandleId senderHandleId);
    void RespondErrorInternetSourceUSB(const Icb_OpGeneric_Error_t error,
                                       const Message::Ecu ecu,
                                       const VccIpCmd::SenderHandleId senderHandleId);

    void RespondInetGatewayAsServer(const Icb_Ecu_t gateway,
                                    const Message::Ecu ecu,
                                    const VccIpCmd::SenderHandleId senderHandleId);
    void RespondErrorInternetGatewayAsServer(const Icb_OpGeneric_Error_t error,
                                             const Message::Ecu ecu,
                                             const VccIpCmd::SenderHandleId senderHandleId);

    //// Helpers

    static const char *InetSourceToString(Icb_InternetSource_t src);
    static const char *EcuToString(Icb_Ecu_t src);

protected:
private:
    //// IHU as client node.

    // TODO. The declaration of callback functions could be automatically done
    // from a script.
    void cbCurrentInetSourceResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData);
    void cbCurrentInetSourceNotification(Message &msg);

    void cbInetGatewayAsClientResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData);

    void cbConnectivityStatusResponse(Message &msg, std::shared_ptr<MessageDispatcher::CallerData> pCallerData);
    void cbConnectivityStatusNotification(Message &msg);

    //// IHU as server node.

    void cbInetSourceCtrlBTReq(Message &msg);
    void cbInetSourceCtrlBTSetReq(Message &msg);
    void cbInetSourceCtrlBTNotificationReq(Message &msg);

    void cbInetSourceCtrlUSBReq(Message &msg);
    void cbInetSourceCtrlUSBSetReq(Message &msg);
    void cbInetSourceCtrlUSBNotificationReq(Message &msg);

    void cbInetGatewayAsServerReq(Message &msg);
    void cbInetGatewayAsServerSetReq(Message &msg);

    IConnectivityServiceProvider *m_pListener = nullptr;

    // Helpers for ASN1 codec
    // NOTE! For use when called by CommonAPI (thread) only (when sending messages)!
    ASN_BYTE m_session_buffer_common_api[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_common_api =
        ASN_Session_Create(m_session_buffer_common_api, sizeof(m_session_buffer_common_api));

    // Storage of latest values from ip command bus (notifications/responses).
    std::atomic_uint m_inetGatewayAsClient;
    std::atomic_bool m_generalConnectivityAvailable;  // (Connectivity status)

    // Storage of latest values from CommonAPI.
    std::atomic_uint m_bluetoothOnOff;
    std::atomic_uint m_usbOnOff;
    std::atomic_uint m_inetGatewayAsServer;

    std::mutex m_btSubscriberMutex;
    std::mutex m_usbSubscriberMutex;
    std::mutex m_inetGatewaySubscriberMutex;

    /// Services that we subscribe to from other nodes on ip command bus
    ///
    /// This vector is used for registering our service layer callbacks, for services that other nodes provide.
    std::vector<ClientIpCommandBusService> ipBusClientSubscriptions = {
        {Message::VCM,
         VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::CurrentInternetSource,
         std::bind(&ConnectivityX::cbCurrentInetSourceResponse, this, std::placeholders::_1, std::placeholders::_2),
         std::bind(&ConnectivityX::cbCurrentInetSourceNotification, this, std::placeholders::_1),
         false},
        {Message::VCM,
         VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::ConnectivityStatus,
         std::bind(&ConnectivityX::cbConnectivityStatusResponse, this, std::placeholders::_1, std::placeholders::_2),
         std::bind(&ConnectivityX::cbConnectivityStatusNotification, this, std::placeholders::_1),
         false}};

    // TODO: The VCM does not reply if we send notification requests for Current Internet Source and Connectivity
    // Status.
    //       (Sends ack but no reply.)
    //       It does still send notifications to us though.
    //       I.e. de-facto coded as static notifications at VCM side.
    //       Ensure with VCC/Actia expertise that this behaviour (static notifications + no need for us sending not.
    //       request) is correct...

    /// Services that we provide to other nodes on ip command bus
    ///
    /// This vector is used for registering our service layer callbacks, for services that IHU provides.
    std::vector<ServerIpCommandBusService> ipBusServerSubscriptions = {
        {VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::InternetSourceCtrlBT,
         std::bind(&ConnectivityX::cbInetSourceCtrlBTReq, this, std::placeholders::_1),
         std::bind(&ConnectivityX::cbInetSourceCtrlBTSetReq, this, std::placeholders::_1),
         std::bind(&ConnectivityX::cbInetSourceCtrlBTNotificationReq, this, std::placeholders::_1)},
        {VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::InternetSourceCtrlUSB,
         std::bind(&ConnectivityX::cbInetSourceCtrlUSBReq, this, std::placeholders::_1),
         std::bind(&ConnectivityX::cbInetSourceCtrlUSBSetReq, this, std::placeholders::_1),
         std::bind(&ConnectivityX::cbInetSourceCtrlUSBNotificationReq, this, std::placeholders::_1)},
        {VccIpCmd::ServiceId::Connectivity,
         VccIpCmd::OperationId::InternetGateway,
         std::bind(&ConnectivityX::cbInetGatewayAsServerReq, this, std::placeholders::_1),
         std::bind(&ConnectivityX::cbInetGatewayAsServerSetReq, this, std::placeholders::_1),
         nullptr}};

    // These lists contain subscribing nodes for notifications that we (IHU) are responsible for sending.
    // Static/fix subscribers must be declared directly here!
    // Additional subscribers may be added in runtime upon receive of notification request.
    const NotificationSubscriberList m_inetSourceCtrlBtStaticSubscribers = {NotificationSubscriber(
        VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetSourceCtrlBT, Message::Ecu::VCM)};
    NotificationSubscriberList m_inetSourceCtrlBtSubscribers = m_inetSourceCtrlBtStaticSubscribers;

    const NotificationSubscriberList m_inetSourceCtrlUsbStaticSubscribers = {NotificationSubscriber(
        VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetSourceCtrlUSB, Message::Ecu::VCM)};
    NotificationSubscriberList m_inetSourceCtrlUsbSubscribers = m_inetSourceCtrlUsbStaticSubscribers;

};  // class ConnectivityX

}  // namespace Connectivity

#endif  // CONNECTIVITY_H
