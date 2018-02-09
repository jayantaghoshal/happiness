/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <IDispatcher.h>

#include "TransportServicesIf.h"
#include "ipcommandbus/IpCmdTypes.h"
#include "ipcommandbus/TrackMessage.h"
#include "ipcommandbus/idiagnostics_client.h"
#include "ipcommandbus/isocket.h"

namespace Connectivity {
/**
 * Transport layer implementation of the IP command bus.
 */
class TransportServices : public ITransportServices {
  public:
    TransportServices(tarmac::eventloop::IDispatcher& timeProvider, tarmac::eventloop::IDispatcher& threadDispatcher,
                      Message::Ecu selfEcu, bool useWfaTimer = true);

    static const char* ErrorTypeToCString(ErrorType error_type);

    tarmac::eventloop::IDispatcher& getThreadDispatcher() override;

    /**
     * Set the socket to use for communication.
     *
     * @param[in] pSocket           Pointer to the socket to use.
     */
    void setSocket(ISocket* pSocket);

    /**
     * Set the socket to use for broadcast communication.
     *
     * @param[in] pSocket           Pointer to the socket to use.
     */
    void setBroadcastSocket(ISocket* pSocket);

    /**
    * Sets the Diagnostics object to be used by TransportServices
    *
    * @param[in] diagnostics                pointer to an IDiagnostics object
    */
    void setDiagnostics(IDiagnosticsClient* diagnostics, tarmac::eventloop::IDispatcher* dispatcher) override;

    /**
     * Register a callback that will be called upon well formed incoming
     * request / set request / set request no return / notification request.
     * The return value from the callback determine whether an ACK shall be sent or not.
     * If false is returned, TransportServices is told that a specific ERROR message have been sent and no ACK shall be
     * emitted.
     *
     * @param[in] messageCb                 The callback.
     */
    void registerIncomingRequestCallback(std::function<bool(Message&)> messageCb) override;

    /**
     * Register a callback that will be called upon well formed incoming notification / cyclic notification.
     *
     * @param[in] messageCb                 The callback.
     */
    void registerIncomingNotificationCallback(std::function<void(Message&)> messageCb) override;

    /**
     * Register a callback that will be called upon well formed incoming response.
     *
     * @param[in] messageCb                 The callback.
     */
    void registerIncomingResponseCallback(std::function<void(Message&)> messageCb) override;

    /**
     * Register a callback that will be called upon any error / timeout related to previously sent REQUEST message.
     *
     * @param[in] messageCb                 The callback.
     */
    void registerErrorOnRequestCallback(std::function<void(Message&, ErrorType)> messageCb) override;

    /**
     * Sends a message to the default target (set in the assigned socket).
     *
     * @param[in] msg                       Message to send. TransportServices takes ownership of the message.
     */
    void sendMessage(Message&& msg) override;

    /**
     * Send an error for the given senderHandleId.
     *
     * @param[in] destination               Where to send the Pdu.
     * @param[in] pdu                       The faulty Pdu that caused the error state.
     * @param[in] errorCode                 The error to transmit.
     * @param[in] errorInfo                 Additional error information
     */
    void sendError(Message::Ecu destination, const Pdu& pdu, ErrorCode errorCode, uint16_t errorInfo = 0) override;

    static const uint8_t PROTOCOL_VERSION = 2;  ///< Which protocol version that is implemented with this class.

  protected:
    /**
     * Get the TrackMessage for a given sender handle id.
     *
     * @param[in] senderHandleId            The sender handle id
     * @return                              Pointer to TrackMessage upon success.
     *                                      nullptr if the sender handle id is not currently tracked.
     */
    TrackMessage* getTrackMessage(uint32_t senderHandleId);

    /**
     * Get the tracking info for a Pdu and stop tracking it.
     *
     * @param[in] senderHandleId            The sender handle id
     * @return                              Valid unique_ptr on success
     *                                      Invalid unique_ptr if the sender handle id is not currently tracked.
     */
    std::unique_ptr<TrackMessage> removeTrackMessage(uint32_t senderHandleId);

    /**
     * Transmit a given Pdu.
     *
     * @param[in] destination               Where to send the Pdu.
     * @param[in] pdu                       Pdu to transmit
     */
    void sendPdu(Message::Ecu destination, const Pdu& pdu);

    /**
     * Transmit an ACK for a given Pdu
     *
     * @param[in] destination               Where to send the Pdu.
     * @param[in] pdu                       Pdu to ACK
     */
    void sendAck(Message::Ecu destination, const Pdu& pdu);

    /**
     * Generates an error for the given senderHandleId.
     *
     * @param[out] errorPdu                 The generated error pdu.
     * @param[in] pdu                       The faulty Pdu that caused the error state.
     * @param[in] errorCode                 The error to transmit.
     * @param[in] errorInfo                 Additional error information
     */
    void generateErrorPdu(Pdu& errorPdu, const Pdu& pdu, ErrorCode errorCode, uint16_t errorInfo);

    /**
     * Called when there is data available on the given socket.
     * Will extract Pdu and relay them for processing.
     * @param[in] socket                    The unicast/broadcast socket.
      */
    void handleInData(ISocket* socket);

    /**
     * Called when there is data available on the broadcast socket.
     * Will extract Pdu and relay them for processing.
     */
    void handleInBroadcastData(void);

    /**
      * Called when there is data available on the unicast socket.
      * Will extract Pdu and relay them for processing.
      */
    void handleInUnicastData(void);

    /**
     * Process an incoming Pdu.
     *
     * @param[in] pdu                       The Pdu to process.
     * @param[in] sourceEcu                 Ecu that sent the Pdu
     */
    void processIncomingPdu(Pdu&& pdu, Message::Ecu sourceEcu);

    /**
     * Process an incoming ACK.
     *
     * @param[in] pdu                       The incoming ACK Pdu.
     * @param[in] sourceEcu                 Ecu that sent the Pdu
     */
    void handleIncomingAck(const Pdu& pdu, Message::Ecu sourceEcu);

    /**
     * Process an incoming ERROR.
     *
     * @param[in] pdu                       The incoming ERROR Pdu
     */
    void handleIncomingError(const Pdu& pdu);

    /**
     * Called when a message timeout occurrs.
     *
     * @param[in] tm                        Reference to the message track structure that has to handle the time out.
     */
    void messageTimeout(TrackMessage& tm, IpCmdTypes::SenderHandleId id);

    /// Handle to the callback to call when we get a well formed incoming request.
    std::function<bool(Message&)> m_incomingRequestCb;

    /// Handle to the callback to call when we get a well formed incoming (cyclic) notification
    std::function<void(Message&)> m_incomingNotificationCb;

    /// Handle to the callback to call when we get a well formed incoming response
    std::function<void(Message&)> m_incomingResponseCb;

    ///  Handle to the callback when an error occurred associated with a REQUEST that we previously sent.
    std::function<void(Message&, ErrorType)> m_errorOnRequestCb;

    /// Handle to the callback when an error occurred associated with a RESPONSE that we previously sent.
    //  Currently not handled. See TODO in handleIncomingError...
    // std::function<void(Message&, ErrorType)> m_errorOnResponsetCb;

    tarmac::eventloop::IDispatcher& timeProvider;
    tarmac::eventloop::IDispatcher& threadDispatcher;
    ISocket* m_pSocket = nullptr;           ///< Socket to use for unicast communication.
    ISocket* m_pBroadcastSocket = nullptr;  ///< Socket to use for broadcast communication.

    IDiagnosticsClient* m_diagnostics = nullptr;  ///< Interface for reporting to diagnostics service.
    tarmac::eventloop::IDispatcher* m_applicationThreadDispatcher = nullptr;

    // ...Ideally change to a container type more suitable for the purpose...
    std::vector<std::unique_ptr<TrackMessage>> m_pendingMessages;  ///< Contain all currently tracked messages.

    // Should WFA (Wait For Ack) timer be used or not
    const bool m_useWfaTimer;

    const Message::Ecu selfEcu;
};

}  // Connectivity
