/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <assert.h>

#include "mock_common_phone_tele_service_stub.h"

namespace v0_conn = v0::org::volvo::connectivity;

namespace Connectivity
{
CommonPhoneTelematicsStub::CommonPhoneTelematicsStub(ICommonPhoneTelematicsService *service)
    : remoteEventHandler_(this), interfaceVersion_(CommonAPI::Version(0, 1)), service_(service)
{
}

CommonPhoneTelematicsStub::RemoteEventHandler::RemoteEventHandler(CommonPhoneTelematicsStub *_defaultStub)
    : defaultStub_(_defaultStub)
{
}

const CommonAPI::Version &CommonPhoneTelematicsStub::getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> _client)
{
    (void)_client;

    return interfaceVersion_;
}

v0_conn::CommonPhoneTelematicsStubRemoteEvent *CommonPhoneTelematicsStub::initStubAdapter(
    const std::shared_ptr<v0_conn::CommonPhoneTelematicsStubAdapter> &_adapter)
{
    CommonAPI::Stub<v0_conn::CommonPhoneTelematicsStubAdapter,
                    v0_conn::CommonPhoneTelematicsStubRemoteEvent>::stubAdapter_ = _adapter;

    return &remoteEventHandler_;
}

/**
 * description: (0x703 CallHandling 0x02 SetRequest)
 * This operation is used to send
 * command to TEM to accept an
 * incoming call or hangup current call.
 */
void CommonPhoneTelematicsStub::CallHandlingSetRequest(const std::shared_ptr<CommonAPI::ClientId> _client,
                                                       v0_conn::IpCommandBusTypes::AssistCallAction _action,
                                                       uint8_t _call_id,
                                                       CallHandlingSetRequestReply_t _reply)
{
    (void)_client;
    (void)_action;
    (void)_reply;

    fprintf(stderr, "Not supposed to reach this stage in service layer tests...");

    assert(0);
}

}  // namespace Connectivity
