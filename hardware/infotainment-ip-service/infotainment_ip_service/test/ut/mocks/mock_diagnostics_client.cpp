/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "mock_diagnostics_client.h"

namespace Connectivity
{
namespace gen_diag = v1::com::ihu::Diagnostics;

void DiagnosticsClient::Initialize(Connectivity::MessageDispatcher* msg_dispatcher,
                                   const CommonAPI::ConnectionId_t& connection_id)
{
    static_cast<MockDiagnosticsClient*>(this)->Initialize(msg_dispatcher, connection_id);
}

void DiagnosticsClient::SetInvalidData(const Message::Ecu ecu, const bool error)
{
    static_cast<MockDiagnosticsClient*>(this)->SetInvalidData(ecu, error);
}

void DiagnosticsClient::SetIpLinkManagerInstance(IpLmServiceStubImpl* iplm_instance)
{
    static_cast<MockDiagnosticsClient*>(this)->SetIpLinkManagerInstance(iplm_instance);
}

}  // namespace
