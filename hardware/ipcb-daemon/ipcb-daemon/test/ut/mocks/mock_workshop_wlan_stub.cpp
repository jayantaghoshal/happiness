/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "mock_workshop_wlan_stub.hpp"

#include <assert.h>

namespace gen_conn = v0::org::volvo::connectivity;

namespace Connectivity
{
WorkshopWlanStub::WorkshopWlanStub(IWorkshopWlanService *service)
    : remoteEventHandler_(this), interfaceVersion_(CommonAPI::Version(0, 1)), service_(service)
{
}

WorkshopWlanStub::RemoteEventHandler::RemoteEventHandler(WorkshopWlanStub *_defaultStub) : defaultStub_(_defaultStub)
{
}

const CommonAPI::Version &WorkshopWlanStub::getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> _client)
{
    (void)_client;
    return interfaceVersion_;
}

gen_conn::WorkshopWlanStubRemoteEvent *WorkshopWlanStub::initStubAdapter(
    const std::shared_ptr<gen_conn::WorkshopWlanStubAdapter> &_adapter)
{
    CommonAPI::Stub<gen_conn::WorkshopWlanStubAdapter, gen_conn::WorkshopWlanStubRemoteEvent>::stubAdapter_ = _adapter;
    return &remoteEventHandler_;
}

void WorkshopWlanStub::ConnectWorkshopWlanSetRequestNoReturn(const std::shared_ptr<CommonAPI::ClientId> _client,
                                                             ConnectWorkshopWlanSetRequestNoReturnReply_t reply)
{
    fprintf(stderr, "Not supposed to reach this stage in service layer tests...");
    assert(0);
}

void WorkshopWlanStub::UpdateDaCMode(const std::shared_ptr<CommonAPI::ClientId> _client,
                                     v0_conn::WorkshopWlan::DaCMode _update_dac_mode,
                                     UpdateDaCModeReply_t _reply)
{
    fprintf(stderr, "Not supposed to reach this stage in service layer tests...");
    assert(0);
}

void WorkshopWlanStub::fireConnectWorkshopWlanNotificationEvent(
    const v0_conn::WorkshopWlan::WorkshopWlanConnectionResult &connection_result)
{
    static_cast<MockWorkshopWlanStub *>(this)->fireConnectWorkshopWlanNotificationEvent(connection_result);
}

void WorkshopWlanStub::fireWorkshopWlanDetectedEvent(const v0_conn::WorkshopWlan::DaCMode &mode,
                                                     const bool &nomadic_device_present)
{
    static_cast<MockWorkshopWlanStub *>(this)->fireWorkshopWlanDetectedEvent(mode, nomadic_device_present);
}
// All other methods are virtual so no more glue needed.

}  // namespace Connectivity
