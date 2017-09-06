/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_IPLM_SERVICE_H
#define MOCK_IPLM_SERVICE_H

#include <gmock/gmock.h>
#include <services/iiplm_service.h>

namespace Connectivity
{
class MockIpLmServiceStubImpl : public IIpLmService
{
public:
    MOCK_METHOD5(Request,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      std::string _serviceName,
                      generated::IpLmService::ResourceGroup _rg,
                      generated::IpLmService::ResourcePrio _prio,
                      generated::IpLmServiceStub::RequestReply_t _reply));
    MOCK_METHOD4(Release,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      std::string _serviceName,
                      generated::IpLmService::ResourceGroup _rg,
                      generated::IpLmServiceStub::ReleaseReply_t _reply));
    MOCK_METHOD3(RegisterService,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      std::string _serviceName,
                      generated::IpLmServiceStub::RegisterServiceReply_t _reply));
    MOCK_METHOD3(UnRegisterService,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      std::string _serviceName,
                      generated::IpLmServiceStub::UnRegisterServiceReply_t _reply));
    MOCK_METHOD3(GetEcuAvailibility, bool(const Message::Ecu &ecu, uint8_t &resourceGroup, uint8_t &prio));
};

}  // namespace Connectivity

#endif  // MOCK_IPLM_SERVICE_H
