/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <assert.h>
#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>

#include "ipcommandbus/IpCmdTypes.h"
#include "ipcommandbus/Message.h"
#include "ipcommandbus/TransportServicesIf.h"
#include "ipcommandbus/idiagnostics_client.h"
#include "ipcommandbus/vcc_pdu_header.h"

#ifdef ENABLE_TESTS
class MessageDispatcherFixture;
#endif  // ENABLE_TESTS

namespace Connectivity {
/**
 * Handles messages routing to and from the TransportService.
 */
class MessageDispatcher {
  public:
#ifdef ENABLE_TESTS
    friend class ::MessageDispatcherFixture;
#endif  // ENABLE_TESTS
    typedef std::uint32_t Icb_ErrorCode;

    struct Icb_OpGeneric_Error_t {
        bool exists__optional__errorInfo;
        Icb_ErrorCode errorCode;
        std::uint32_t errorInfo;
    };

    typedef std::function<bool(Message &)> ResponseMessageCallback;

    // CallerData structure is provided as argument by service specific layer when sending a request
    // and is returned back upon receive of the belonging response/error/timeout.
    struct CallerData {
        virtual ~CallerData() = default;

        ResponseMessageCallback responseCallback;

        // In case of error response or timeout, errorType is set accordingly.
        // Otherwise (regular response), errorType is set to OK.
        // In case of error message having been received on ip command bus (not timeout),
        // the errorInfo is also filled with info from message.
        ITransportServices::ErrorType errorType;
        Icb_OpGeneric_Error_t errorInfo;
    };

    typedef std::function<bool(Message &, uint64_t &)> MessageCallback;

    MessageDispatcher(ITransportServices *transport, tarmac::eventloop::IDispatcher &dispatcher);
    void setDiagnostics(IDiagnosticsClient *diagnostics);

    /**
     * Registers the callback function to call when a specific message is received.
     * NOTE! Don't use for response messages!
     *
     * @param[in] serviceId                 The service id to match with
     * @param[in] operationId               The operation id  to match with
     * @param[in] operationType             The operation  to match with
     * @param[in] messageCb                 The callback function to call
     */
    uint64_t registerMessageCallback(IpCmdTypes::ServiceId serviceId, IpCmdTypes::OperationId operationId,
                                     IpCmdTypes::OperationType operationType, MessageCallback messageCb);

    bool unregisterCallback(uint64_t registeredReceiverId);

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
    struct RegInfo {
        RegInfo(IpCmdTypes::ServiceId serviceId, IpCmdTypes::OperationId operationId,
                IpCmdTypes::OperationType operationType, uint64_t registeredReceiverId, MessageCallback messageCb)
            : serviceId(serviceId),
              operationId(operationId),
              operationType(operationType),
              registeredReceiverId(registeredReceiverId),
              messageCb(messageCb) {
            assert(messageCb);
        }

        IpCmdTypes::ServiceId serviceId;
        IpCmdTypes::OperationId operationId;
        IpCmdTypes::OperationType operationType;
        uint64_t registeredReceiverId;
        MessageCallback messageCb;
    };

  private:
    /**
     * Callback funciton called when a new message has been added to the send queue
     *
     */
    void IPCBThread_sendPendingMessages(void);

    void DecodeGenericError(Message &msg, Icb_OpGeneric_Error_t &errorReturn);

    std::vector<RegInfo>::iterator FindReceiver(const std::function<bool(const RegInfo &)> &predicate) {
        return std::find_if(m_registeredReceivers.begin(), m_registeredReceivers.end(), predicate);
    }

    std::vector<RegInfo> FindReceivers(const std::function<bool(const RegInfo &)> &predicate) {
        std::vector<RegInfo> foundReceivers;
        std::vector<RegInfo>::iterator start = m_registeredReceivers.begin();
        while (true) {
            std::vector<RegInfo>::iterator receiver = std::find_if(start, m_registeredReceivers.end(), predicate);
            if (receiver == m_registeredReceivers.end()) {
                break;
            }

            foundReceivers.push_back(*receiver);
            start = receiver + 1;
        }

        return foundReceivers;
    }

    std::vector<RegInfo>::iterator FindReceiver(const Message &msg) {
        return FindReceiver([&msg](const RegInfo &ri) {
            return msg.pdu.header.service_id == ri.serviceId && msg.pdu.header.operation_id == ri.operationId &&
                   msg.pdu.header.operation_type == ri.operationType;
        });
    }

    std::vector<RegInfo> FindReceivers(const Message &msg) {
        return FindReceivers([&msg](const RegInfo &ri) {
            return msg.pdu.header.service_id == ri.serviceId && msg.pdu.header.operation_id == ri.operationId &&
                   msg.pdu.header.operation_type == ri.operationType;
        });
    }

    bool ReceiverExists(const std::function<bool(const RegInfo &)> &predicate) {
        return FindReceiver(predicate) != m_registeredReceivers.end();
    }

    tarmac::eventloop::IDispatcher &wakeUpApplicationThread;
    ITransportServices *m_transport = nullptr;    ///< The TransportServices used for sending and receiving.
    IDiagnosticsClient *m_diagnostics = nullptr;  ///< Interface for reporting to diagnostics service.

    std::mutex m_msgQueueMutex;      ///< Send message queue mutex
    std::queue<Message> m_msgQueue;  ///< Send message queue

    std::mutex m_registeredReceiversMutex;
    std::vector<RegInfo> m_registeredReceivers;  ///< Contain all registered receivers of messages. (Accessed from both
                                                 /// TransportService-thread and App-thread)
    uint64_t m_registeredReceiverIds = 1;        ///< Id to find lambda receiver function (Start at 1, 0 == error)

    typedef std::map<IpCmdTypes::SenderHandleId, std::shared_ptr<CallerData>> RequestsMap;
    RequestsMap m_requestsMap;  // USed to associate pCallerData (Only accessed from App-thread)

    bool IPCBThread_cbIncomingRequest(Message &msg);
    void AppThread_cbIncomingRequest(RegInfo ri, Message &msg);

    void IPCBThread_cbIncomingNotification(Message &msg);
    void Appthread_cbIncomingNotification(Message &msg);

    void IPCBThread_cbIncomingResponse(Message &msg);
    void AppThread_cbIncomingResponse(Message &msg);

    void IPCBThread_cbIncomingError(Message &msg, ITransportServices::ErrorType eType);
    void AppThread_cbIncomingError(Message &msg, ITransportServices::ErrorType eType);
};

}  // Connectivity
