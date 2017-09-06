/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once

#include "v0/org/volvo/connectivity/DiagnosticManagementAuthenticationProxyBase.hpp"
#include "v0/org/volvo/connectivity/IpCommandBusTypes.hpp"

#include <gmock/gmock.h>

namespace Connectivity
{
namespace gen_conn = v0::org::volvo::connectivity;
using IPT = gen_conn::IpCommandBusTypes;

class DiagnosticManagementAuthProxyMock : public gen_conn::DiagnosticManagementAuthenticationProxyBase
{
public:
    MOCK_CONST_METHOD0(isAvailable, bool());
    MOCK_CONST_METHOD0(isAvailableBlocking, bool());
    MOCK_METHOD0(getProxyStatusEvent, CommonAPI::ProxyStatusEvent &());
    MOCK_METHOD0(getInterfaceVersionAttribute, CommonAPI::InterfaceVersionAttribute &());

    MOCK_METHOD6(RequestUserAuthentication,
                 void(const std::string &,
                      const std::string &,
                      const uint32_t &,
                      CommonAPI::CallStatus &,
                      IPT::DiagMgm_UserAuthCredentials &,
                      const CommonAPI::CallInfo *));

    MOCK_METHOD5(RequestUserAuthenticationAsync,
                 std::future<CommonAPI::CallStatus>(const std::string &,
                                                    const std::string &,
                                                    const uint32_t &,
                                                    RequestUserAuthenticationAsyncCallback,
                                                    const CommonAPI::CallInfo *));

    MOCK_METHOD3(TextFromOperatorToUser,
                 void(const std::string &, CommonAPI::CallStatus &, const CommonAPI::CallInfo *));

    MOCK_METHOD3(TextFromOperatorToUserAsync,
                 std::future<CommonAPI::CallStatus>(const std::string &,
                                                    TextFromOperatorToUserAsyncCallback,
                                                    const CommonAPI::CallInfo *));
};
}
