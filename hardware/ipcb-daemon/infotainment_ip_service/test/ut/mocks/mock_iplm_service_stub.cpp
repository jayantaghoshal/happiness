/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "mock_iplm_service_stub.h"

namespace Connectivity
{
IpLmServiceStub::RemoteEventHandler::RemoteEventHandler(IpLmServiceStub* _defaultStub) : defaultStub_(_defaultStub)
{
}

IpLmServiceStub::IpLmServiceStub(IIpLmService* service)
    : remoteEventHandler_(this), version_(generated::IpLmService::getInterfaceVersion()), service_(service)
{
}

IpLmServiceStub::~IpLmServiceStub()
{
}

const CommonAPI::Version& IpLmServiceStub::getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> clientId)
{
    (void)clientId;
    return version_;
}

generated::IpLmServiceStubRemoteEvent* IpLmServiceStub::initStubAdapter(
    const std::shared_ptr<generated::IpLmServiceStubAdapter>& _stubAdapter)
{
    CommonAPI::Stub<generated::IpLmServiceStubAdapter, generated::IpLmServiceStubRemoteEvent>::stubAdapter_ =
        _stubAdapter;
    return &remoteEventHandler_;
}

void IpLmServiceStub::Request(const std::shared_ptr<CommonAPI::ClientId> _client,
                              std::string _serviceName,
                              generated::IpLmService::ResourceGroup _rg,
                              generated::IpLmService::ResourcePrio _prio,
                              RequestReply_t _reply)
{
    static_cast<::MockIpLmServiceStub*>(this)->Request(_client, _serviceName, _rg, _prio, _reply);
}

void IpLmServiceStub::Release(const std::shared_ptr<CommonAPI::ClientId> _client,
                              std::string _serviceName,
                              generated::IpLmService::ResourceGroup _rg,
                              ReleaseReply_t _reply)
{
    static_cast<::MockIpLmServiceStub*>(this)->Release(_client, _serviceName, _rg, _reply);
}

void IpLmServiceStub::RegisterService(const std::shared_ptr<CommonAPI::ClientId> _client,
                                      std::string _serviceName,
                                      RegisterServiceReply_t _reply)
{
    static_cast<::MockIpLmServiceStub*>(this)->RegisterService(_client, _serviceName, _reply);
}

void IpLmServiceStub::UnRegisterService(const std::shared_ptr<CommonAPI::ClientId> _client,
                                        std::string _serviceName,
                                        RegisterServiceReply_t _reply)
{
    static_cast<::MockIpLmServiceStub*>(this)->UnRegisterService(_client, _serviceName, _reply);
}

void IpLmServiceStub::fireRgStatusEvent(const generated::IpLmService::ResourceGroup& _rg,
                                        const generated::IpLmService::ResourceGroupStatus& _rgStatus)
{
    static_cast<::MockIpLmServiceStub*>(this)->fireRgStatusEvent(_rg, _rgStatus);
}

void IpLmServiceStub::fireNodeStatusEvent(
    const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ecu& _ecu, const bool& _ecuStatus)
{
    static_cast<::MockIpLmServiceStub*>(this)->fireNodeStatusEvent(_ecu, _ecuStatus);
}

}  // namespace Connectivity
