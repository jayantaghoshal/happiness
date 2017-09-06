/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_DIAGNOSTICS_CLIENT_H
#define MOCK_DIAGNOSTICS_CLIENT_H

#include <gmock/gmock.h>
#include <services/diagnostics_client.h>

namespace Connectivity
{
namespace gen_diag = v1::com::ihu::Diagnostics;

class MockDiagnosticsClient : public DiagnosticsClient
{
public:
    MockDiagnosticsClient(CommonAPI::Timeout_t timeout = CommonAPI::DEFAULT_SEND_TIMEOUT_MS,
                          CommonAPI::Sender_t sender = 0)
        : DiagnosticsClient(timeout, sender)
    {
    }

    MOCK_METHOD2(Initialize,
                 void(Connectivity::MessageDispatcher* msg_dispatcher, const CommonAPI::ConnectionId_t& connection_id));

    MOCK_METHOD2(SetInvalidData, void(const Message::Ecu ecu, const bool error));

};

}  // namespace

#endif  // MOCK_DIAGNOSTICS_CLIENT_H
