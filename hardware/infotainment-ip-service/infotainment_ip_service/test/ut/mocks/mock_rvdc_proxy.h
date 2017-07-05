/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#ifndef INFOTAINMENT_IP_SERVICE_TEST_UT_MOCKS_MOCK_RVDC_PROXY_H_
#define INFOTAINMENT_IP_SERVICE_TEST_UT_MOCKS_MOCK_RVDC_PROXY_H_

#include "v0/org/volvocars/privacypolicy/RvdcProxyBase.hpp"

#include <gmock/gmock.h>

namespace Connectivity
{
namespace gen_privacypolicy = v0::org::volvocars::privacypolicy;

class RvdcProxyMock : public gen_privacypolicy::RvdcProxyBase
{
public:
    MOCK_CONST_METHOD0(isAvailable, bool());
    MOCK_CONST_METHOD0(isAvailableBlocking, bool());
    MOCK_METHOD0(getProxyStatusEvent, CommonAPI::ProxyStatusEvent &());
    MOCK_METHOD0(getInterfaceVersionAttribute, CommonAPI::InterfaceVersionAttribute &());

    MOCK_METHOD0(getApprovalstatusesAttribute, ApprovalstatusesAttribute &());
};
}

#endif /* INFOTAINMENT_IP_SERVICE_TEST_UT_MOCKS_MOCK_RVDC_PROXY_H_ */
