/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <services/iplm_service.h>

#include <algorithm>
#include <bitset>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>

#include <NodeStateTypes.h>
#include <ivi-logging.h>

#include <ipcommandbus/MessageDispatcher.h>
#include <ipcommandbus/log_context.h>
#include "util/local_config.h"

namespace Connectivity
{
namespace generated = v0::org::volvo::connectivity;

LOG_DECLARE_DEFAULT_CONTEXT(g_iplm_service_log_context, "LM00", "IP Link Manager Service")

IpLmServiceStubImpl::IpLmServiceStubImpl(ITimeProvider &timeProvider)
    : iplm_service_stub_(std::make_shared<IpLmServiceStub>(this)), timeProvider{timeProvider}
{
    service_name_ = "IPLM";
}

bool IpLmServiceStubImpl::Initialize(Connectivity::MessageDispatcher * /*msgDispatcher*/)
{
    return true;
}

void IpLmServiceStubImpl::Uninitialize()
{
}
/**
 * description: Request the given Resource Group
 */
void IpLmServiceStubImpl::Request(const std::shared_ptr<CommonAPI::ClientId> /*_client*/,
                                  std::string /*_serviceName*/,
                                  generated::IpLmService::ResourceGroup /*_rg*/,
                                  generated::IpLmService::ResourcePrio /*_prio*/,
                                  generated::IpLmServiceStub::RequestReply_t /*_reply*/)
{
}

/**
 * description: Release the given Resource Group
 */
void IpLmServiceStubImpl::Release(const std::shared_ptr<CommonAPI::ClientId> /*_client*/,
                                  std::string /*_serviceName*/,
                                  generated::IpLmService::ResourceGroup /*_rg*/,
                                  generated::IpLmServiceStub::ReleaseReply_t /*_reply*/)
{
}

void IpLmServiceStubImpl::RegisterService(const std::shared_ptr<CommonAPI::ClientId> /*_client*/,
                                          std::string /*_serviceName*/,
                                          generated::IpLmServiceStub::RegisterServiceReply_t /*_reply*/)
{
}

void IpLmServiceStubImpl::UnRegisterService(const std::shared_ptr<CommonAPI::ClientId> /*_client*/,
                                            std::string /*_serviceName*/,
                                            generated::IpLmServiceStub::UnRegisterServiceReply_t /*_reply*/)
{
}

void IpLmServiceStubImpl::CbLmBroadcast(Message & /*msg*/)
{
}
bool IpLmServiceStubImpl::GetEcuAvailibility(const Message::Ecu & /*ecu*/,
                                             uint8_t & /*resourceGroup*/,
                                             uint8_t & /*prio*/)
{
    return true;
}

void IpLmServiceStubImpl::CreateAndSendIpActivityMessage()
{
}
void IpLmServiceStubImpl::RequestMonitoringTimeout(EcuId ecu)
{
}

void IpLmServiceStubImpl::ActivityTimeout()
{
}

void IpLmServiceStubImpl::FlexrayWakeupTimeout()
{
}

bool IpLmServiceStubImpl::SendFlexrayWakeup(ResourceGroup /*_rg*/, Prio /*_prio*/)
{
    return true;
}

bool IpLmServiceStubImpl::SetNsmSessionState()
{
    return true;
}

const char *IpLmServiceStubImpl::ToString(ResourceGroup /*r*/)
{
    return "";
}

const std::string IpLmServiceStubImpl::ToString(Action /*a*/)
{
    return "";
}

const char *IpLmServiceStubImpl::ToString(Prio /*a*/)
{
    return "";
}

}  // namespace Connectivity
