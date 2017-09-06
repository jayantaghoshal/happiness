/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_MESSAGEDISPATCHER_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_MESSAGEDISPATCHER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <map>
#include "ipcommandbus/Message.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/vcc_pdu_header.h"

namespace Connectivity
{
class MockMessageDispatcher : public MessageDispatcher
{
public:
    MockMessageDispatcher();
    MockMessageDispatcher(ITransportServices *transport, ThreadDispatcher *dispatcher);
    MOCK_METHOD1(setTransport, void(ITransportServices *transport));

    MOCK_METHOD1(setDiagnostics, void(IDiagnosticsClient *diagnostics));

    // NOTE!!! Avoid mocking the registerXXXXXCallback_mocked functions, use simulateMessageReceived()-function instead.
    MOCK_METHOD3(registerResponseCallback_mocked,
                 void(VccIpCmd::ServiceId serviceId,
                      VccIpCmd::OperationId operationId,
                      ResponseMessageCallback messageCb));

    MOCK_METHOD3(registerRequestCallback_mocked,
                 void(VccIpCmd::ServiceId serviceId, VccIpCmd::OperationId operationId, MessageCallback messageCb));

    MOCK_METHOD3(registerSetRequestCallback_mocked,
                 void(VccIpCmd::ServiceId serviceId, VccIpCmd::OperationId operationId, MessageCallback messageCb));

    MOCK_METHOD3(registerNotificationCallback_mocked,
                 void(VccIpCmd::ServiceId serviceId, VccIpCmd::OperationId operationId, MessageCallback messageCb));

    MOCK_METHOD3(registerNotificationCyclicCallback_mocked,
                 void(VccIpCmd::ServiceId serviceId, VccIpCmd::OperationId operationId, MessageCallback messageCb));

    MOCK_METHOD3(registerNotificationRequestCallback_mocked,
                 void(VccIpCmd::ServiceId serviceId, VccIpCmd::OperationId operationId, MessageCallback messageCb));
    /*
        // ...TO DO!! Remove or modify depending on solution for returning error responses...
        MOCK_METHOD3(registerErrorCallback,
                     void(VccIpCmd::ServiceId serviceId, VccIpCmd::OperationId operationId,
                          MessageCallback messageCb));
    */
    MOCK_METHOD2(sendMessage, void(const Message &msg, std::shared_ptr<CallerData> callerData));

    void simulateMessageReceived(Message &m, std::shared_ptr<CallerData> callerData);

    std::vector<RegInfo> m_registeredReceivers;
    std::map<VccIpCmd::SenderHandleId, std::shared_ptr<CallerData>> userCallerData;
};
}

#endif  // _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_MESSAGEDISPATCHER_H_
