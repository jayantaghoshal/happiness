/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_IPLM_SERVICE_STUB_H
#define MOCK_IPLM_SERVICE_STUB_H

#include <memory>
#include <v0/org/volvo/connectivity/IpLmServiceStub.hpp>
#include "gmock/gmock.h"
#include "services/iplm_service_stub.h"

namespace generated = v0_1::org::volvo::connectivity;

class MockIpLmServiceStub : public Connectivity::IpLmServiceStub
{
public:
    MockIpLmServiceStub(Connectivity::IIpLmService* service) : Connectivity::IpLmServiceStub(service)
    {
    }

    ~MockIpLmServiceStub()
    {
    }

    MOCK_METHOD1(getInterfaceVersion, CommonAPI::Version&(std::shared_ptr<CommonAPI::ClientId> clientId));

    MOCK_METHOD1(
        initStubAdapter,
        generated::IpLmServiceStubRemoteEvent*(const std::shared_ptr<generated::IpLmServiceStubAdapter>& _stubAdapter));

    MOCK_METHOD5(Request,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      std::string _serviceName,
                      generated::IpLmService::ResourceGroup _rg,
                      generated::IpLmService::ResourcePrio _prio,
                      RequestReply_t _reply));

    MOCK_METHOD4(Release,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      std::string _serviceName,
                      generated::IpLmService::ResourceGroup _rg,
                      ReleaseReply_t _reply));

    MOCK_METHOD3(RegisterService,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      std::string _serviceName,
                      RegisterServiceReply_t _reply));

    MOCK_METHOD3(UnRegisterService,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      std::string _serviceName,
                      RegisterServiceReply_t _reply));

    MOCK_METHOD2(fireRgStatusEvent,
                 void(const generated::IpLmService::ResourceGroup& _rg,
                      const generated::IpLmService::ResourceGroupStatus& _rgStatus));

    MOCK_METHOD2(fireNodeStatusEvent,
                 void(const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ecu& _ecu,
                      const bool& _ecuStatus));
};

#endif  // MOCK_IPLM_SERVICE_STUB_H
