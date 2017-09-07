/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_MESSAGEDISPATCHER_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_MESSAGEDISPATCHER_H_

#include <assert.h>
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <cstdint>

#include "ipcommandbus/Message.h"
#include "ipcommandbus/TransportServicesIf.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/idiagnostics_client.h"
#include "ipcommandbus/vcc_pdu_header.h"

#ifdef ENABLE_TESTS
class MessageDispatcherFixture;
#endif  // ENABLE_TESTS

namespace Connectivity
{
/**
 * Handles messages routing to and from the TransportService.
 */
class MessageDispatcher
{
public:
#ifdef ENABLE_TESTS
    friend class ::MessageDispatcherFixture;
#endif  // ENABLE_TESTS
    typedef std::uint32_t Icb_ErrorCode;

    struct Icb_OpGeneric_Error_t
    {
        bool exists__optional__errorInfo;
        Icb_ErrorCode errorCode;
        std::uint32_t errorInfo;
    };
    // CallerData structure is provided as argument by service specific layer when sending a request
    // and is returned back upon receive of the belonging response/error/timeout.
    struct CallerData
    {
        virtual ~CallerData() = default;

        // In case of error response or timeout, errorType is set accordingly.
        // Otherwise (regular response), errorType is set to OK.
        // In case of error message having been received on ip command bus (not timeout),
        // the errorInfo is also filled with info from message.
        ITransportServices::ErrorType errorType;
        Icb_OpGeneric_Error_t errorInfo;
    };

    typedef std::function<void(Message &)> MessageCallback;

    typedef std::function<void(Message &, std::shared_ptr<CallerData>)> ResponseMessageCallback;


    MessageDispatcher(ITransportServices *transport, tarmac::eventloop::IDispatcher& dispatcher);
    void setDiagnostics(IDiagnosticsClient *diagnostics);

    /**
     * Registers the callback function to call when a response message is received.
     *
     * @param[in] serviceId                 The service id to match with
     * @param[in] operationId               The operation id  to match with
     * @param[in] messageCb                 The callback function to call
     */
    void registerResponseCallback(VccIpCmd::ServiceId serviceId,
                                  VccIpCmd::OperationId operationId,
                                  ResponseMessageCallback messageCb);

    /**
     * Registers the callback function to call when a request message is received.
     *
     * @param[in] serviceId                 The service id to match with
     * @param[in] operationId               The operation id  to match with
     * @param[in] messageCb                 The callback function to call
     */
    void registerRequestCallback(VccIpCmd::ServiceId serviceId,
                                 VccIpCmd::OperationId operationId,
                                 MessageCallback messageCb);

    /**
     * Registers the callback function to call when a set request message is received.
     *
     * @param[in] serviceId                 The service id to match with
     * @param[in] operationId               The operation id  to match with
     * @param[in] messageCb                 The callback function to call
     */
    void registerSetRequestCallback(VccIpCmd::ServiceId serviceId,
                                    VccIpCmd::OperationId operationId,
                                    MessageCallback messageCb);

    /**
     * Registers the callback function to call when a notification message is received.
     *
     * @param[in] serviceId                 The service id to match with
     * @param[in] operationId               The operation id  to match with
     * @param[in] messageCb                 The callback function to call
     */
    void registerNotificationCallback(VccIpCmd::ServiceId serviceId,
                                      VccIpCmd::OperationId operationId,
                                      MessageCallback messageCb);

    /**
     * Registers the callback function to call when a cyclic notification message is received.
     *
     * @param[in] serviceId                 The service id to match with
     * @param[in] operationId               The operation id  to match with
     * @param[in] messageCb                 The callback function to call
     */
    void registerNotificationCyclicCallback(VccIpCmd::ServiceId serviceId,
                                            VccIpCmd::OperationId operationId,
                                            MessageCallback messageCb);

    /**
     * Registers the callback function to call when a notification request message is received.
     *
     * @param[in] serviceId                 The service id to match with
     * @param[in] operationId               The operation id  to match with
     * @param[in] messageCb                 The callback function to call
     */
    void registerNotificationRequestCallback(VccIpCmd::ServiceId serviceId,
                                             VccIpCmd::OperationId operationId,
                                             MessageCallback messageCb);

    // Note: There is by design no 'registerErrorCallback'.
    //       Errors and timeouts associated with earlier sent requests are returned trough response callback.
    //       (Errors associated with earlier sent responses are currently not handled above TransportServices level...)

    /**
    * Will add a message to the MessageDispatcher queue for further distribution to TransportServices
    * MessageDispatcher takes ownership of the message.
    *
    * @param[in] msg                        The message to send
    * @pCallerData[in] pCallerData          For response messages, the caller data may optionally be set.
    *                                       The data will then be returned upon receiving response,
    *                                       or error/timeout.
    */
    void sendMessage(Message &&msg, std::shared_ptr<CallerData> pCallerData = nullptr);


protected:
    /**
     * Container for information about a registered message receiver.
     */
    struct RegInfo
    {
        RegInfo(VccIpCmd::ServiceId serviceId, VccIpCmd::OperationId operationId, ResponseMessageCallback messageCb)
            : serviceId(serviceId),
              operationId(operationId),
              operationType(VccIpCmd::OperationType::RESPONSE),
              messageCbResp(messageCb)
        {
            assert(messageCbResp);
        }

        RegInfo(VccIpCmd::ServiceId serviceId,
                VccIpCmd::OperationId operationId,
                VccIpCmd::OperationType operationType,
                MessageCallback messageCb)
            : serviceId(serviceId), operationId(operationId), operationType(operationType), messageCb(messageCb)
        {
            assert(messageCb);
        }

        VccIpCmd::ServiceId serviceId;
        VccIpCmd::OperationId operationId;
        VccIpCmd::OperationType operationType;
        ResponseMessageCallback messageCbResp;  // Responses and errors
        MessageCallback messageCb;              // All other message types
    };

private:
    /**
     * Registers the callback function to call when a specific message is received.
     * NOTE! Don't use for response messages!
     *
     * @param[in] serviceId                 The service id to match with
     * @param[in] operationId               The operation id  to match with
     * @param[in] operationType             The operation  to match with
     * @param[in] messageCb                 The callback function to call
     */
    void registerMessageCallback(VccIpCmd::ServiceId serviceId,
                                 VccIpCmd::OperationId operationId,
                                 VccIpCmd::OperationType operationType,
                                 MessageCallback messageCb);

    /**
     * Callback funciton called when a new message has been added to the send queue
     *
     */
    void IPCBThread_sendPendingMessages(void);

    void DecodeGenericError(Message &msg, Icb_OpGeneric_Error_t &errorReturn);

    std::vector<RegInfo>::iterator FindReceiver(const std::function<bool(const RegInfo &)> &predicate)
    {
        return std::find_if(m_registeredReceivers.begin(), m_registeredReceivers.end(), predicate);
    }

    std::vector<RegInfo>::iterator FindReceiver(const Message &msg)
    {
        return FindReceiver([&msg](const RegInfo &ri) {
            return msg.pdu.header.service_id == ri.serviceId && msg.pdu.header.operation_id == ri.operationId &&
                   msg.pdu.header.operation_type == ri.operationType;
        });
    }

    bool ReceiverExists(const std::function<bool(const RegInfo &)> &predicate)
    {
        return FindReceiver(predicate) != m_registeredReceivers.end();
    }


    tarmac::eventloop::IDispatcher& wakeUpApplicationThread;
    ITransportServices *m_transport = nullptr;    ///< The TransportServices used for sending and receiving.
    IDiagnosticsClient *m_diagnostics = nullptr;  ///< Interface for reporting to diagnostics service.

    std::mutex m_msgQueueMutex;      ///< Send message queue mutex
    std::queue<Message> m_msgQueue;  ///< Send message queue

    std::mutex m_registeredReceiversMutex;
    std::vector<RegInfo> m_registeredReceivers;  ///< Contain all registered receivers of messages. (Accessed from both TransportService-thread and App-thread)

    typedef std::map<VccIpCmd::SenderHandleId, std::shared_ptr<CallerData>> RequestsMap;
    RequestsMap m_requestsMap;  //USed to associate pCallerData (Only accessed from App-thread)



    bool IPCBThread_cbIncomingRequest(Message &msg);
    void AppThread_cbIncomingRequest(MessageCallback cb, Message &msg);

    void IPCBThread_cbIncomingNotification(Message &msg);
    void Appthread_cbIncomingNotification(Message &msg);

    void IPCBThread_cbIncomingResponse(Message &msg);
    void AppThread_cbIncomingResponse(Message &msg, ResponseMessageCallback cb);

    void IPCBThread_cbIncomingError(Message &msg, ITransportServices::ErrorType eType);
    void AppThread_cbIncomingError(Message &msg, ITransportServices::ErrorType eType);
};

}  // Connectivity

#endif  //_CONNECTIVITY_LIBIPCOMMANDBUS_MESSAGEDISPATCHER_H_
