/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <memory>
#include "connectivity_service.h"
#include "connectivity_service_stub.h"

namespace Connectivity
{
ConnectivityServiceStub::RemoteEventHandler::RemoteEventHandler(ConnectivityServiceStub *_defaultStub)
    : defaultStub_(_defaultStub)
{
}

ConnectivityServiceStub::ConnectivityServiceStub(IConnectivityServiceProxy *proxy)
    : remoteEventHandler_(this), version_(generated::ConnectivityService::getInterfaceVersion()), proxy_(proxy)
{
}

ConnectivityServiceStub::~ConnectivityServiceStub()
{
}
// ----------------------------------------------------------------------------------------------------
// Implementation of ConnectivityServiceStubStub
// ----------------------------------------------------------------------------------------------------
const CommonAPI::Version &ConnectivityServiceStub::getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> clientId)
{
    (void)clientId;
    return version_;
}

generated::ConnectivityServiceStubRemoteEvent *ConnectivityServiceStub::initStubAdapter(
    const std::shared_ptr<generated::ConnectivityServiceStubAdapter> &stub_adapter)
{
    (void)stub_adapter;
    return nullptr;
}

void ConnectivityServiceStub::GetCurrentInternetSource(const std::shared_ptr<CommonAPI::ClientId> _client,
                                                       ConnectivityServiceStub::GetCurrentInternetSourceReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

void ConnectivityServiceStub::SetCurrentInternetSource(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    generated::IpCommandBusTypes::Infotainment_IP_bus_InternetSource _currentInternetSource,
    ConnectivityServiceStub::SetCurrentInternetSourceReply_t _reply)
{
    (void)_client;
    (void)_currentInternetSource;
    (void)_reply;
}

void ConnectivityServiceStub::GetConnectivityStatus(const std::shared_ptr<CommonAPI::ClientId> _client,
                                                    ConnectivityServiceStub::GetConnectivityStatusReply_t _reply)
{
    (void)_client;
    (void)_reply;
}

void ConnectivityServiceStub::fireCurrentInternetSourceNotificationEvent(
    const generated::IpCommandBusTypes::Infotainment_IP_bus_InternetSource &_currentInternetSource)
{
    (void)_currentInternetSource;
}

void ConnectivityServiceStub::fireConnectivityStatusNotificationEvent(const bool &_generalConnectivityAvailable)
{
    (void)_generalConnectivityAvailable;
}

}  // namespace Connectivity
