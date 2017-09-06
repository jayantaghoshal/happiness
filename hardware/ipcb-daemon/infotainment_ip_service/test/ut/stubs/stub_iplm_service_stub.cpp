/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <services/iplm_service_stub.h>

namespace Connectivity
{
IpLmServiceStub::RemoteEventHandler::RemoteEventHandler(IpLmServiceStub *_defaultStub) : defaultStub_(_defaultStub)
{
}
IpLmServiceStub::IpLmServiceStub(IIpLmService *service)
    : remoteEventHandler_(this), version_(generated::IpLmService::getInterfaceVersion()), service_(service)
{
}

IpLmServiceStub::~IpLmServiceStub()
{
}
const CommonAPI::Version &IpLmServiceStub::getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> clientId)
{
    (void)clientId;
    return version_;
}

generated::IpLmServiceStubRemoteEvent *IpLmServiceStub::initStubAdapter(
    const std::shared_ptr<generated::IpLmServiceStubAdapter> &_stubAdapter)
{
    (void)_stubAdapter;
    return nullptr;
}

void IpLmServiceStub::Request(const std::shared_ptr<CommonAPI::ClientId> _client,
                              std::string _serviceName,
                              generated::IpLmService::ResourceGroup _rg,
                              generated::IpLmService::ResourcePrio _prio,
                              IpLmServiceStub::RequestReply_t _reply)
{
    (void)_client;
    (void)_serviceName;
    (void)_rg;
    (void)_prio;
    (void)_reply;
}

void IpLmServiceStub::Release(const std::shared_ptr<CommonAPI::ClientId> _client,
                              std::string _serviceName,
                              generated::IpLmService::ResourceGroup _rg,
                              IpLmServiceStub::ReleaseReply_t _reply)
{
    (void)_client;
    (void)_serviceName;
    (void)_rg;
    (void)_reply;
}

void IpLmServiceStub::RegisterService(const std::shared_ptr<CommonAPI::ClientId> _client,
                                      std::string _serviceName,
                                      RegisterServiceReply_t _reply)
{
    (void)_client;
    (void)_serviceName;
    (void)_reply;
}

void IpLmServiceStub::UnRegisterService(const std::shared_ptr<CommonAPI::ClientId> _client,
                                        std::string _serviceName,
                                        UnRegisterServiceReply_t _reply)
{
    (void)_client;
    (void)_serviceName;
    (void)_reply;
}

void IpLmServiceStub::fireRgStatusEvent(const generated::IpLmService::ResourceGroup &_rg,
                                        const generated::IpLmService::ResourceGroupStatus &_rgStatus)
{
    (void)_rg;
    (void)_rgStatus;
}

void IpLmServiceStub::fireNodeStatusEvent(
    const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ecu &_ecu, const bool &_ecuStatus)
{
    (void)_ecu;
    (void)_ecuStatus;
}

}  // namespace Connectivity
