/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "CommonAPI/CommonAPI.hpp"
#include "org/genivi/EnhancedPositionService/EnhancedPositionProxy.hpp"

// Globals
int g_GetPositionInfoAsync_calls = 0;
int g_GetTimeAsync_calls = 0;
int g_enhancedPositionProxyAvailable_calls = 0;
bool g_enhancedPositionProxyAvailable = false;

namespace org
{
namespace genivi
{
namespace EnhancedPositionService
{
template <>
CommonAPI::ProxyStatusEvent &EnhancedPositionProxy<>::getProxyStatusEvent()
{
    static CommonAPI::ProxyStatusEvent proxyStatusEvent;
    return proxyStatusEvent;
}

template <>
EnhancedPositionProxyBase::PositionUpdateEvent &EnhancedPositionProxy<>::getPositionUpdateEvent()
{
    static EnhancedPositionProxyBase::PositionUpdateEvent posUpdateEvent;
    return posUpdateEvent;
}

// Only stubbed, since std::future cannot be used in mock.
template <>
std::future<CommonAPI::CallStatus> EnhancedPositionProxy<>::GetPositionInfoAsync(
    const ::org::genivi::EnhancedPositionService::EnhancedPositionServiceTypes::Bitmask &_valuesToReturn,
    GetPositionInfoAsyncCallback _callback,
    const CommonAPI::CallInfo *_info)
{
    (void)_valuesToReturn;
    (void)_callback;
    (void)_info;
    g_GetPositionInfoAsync_calls++;
    std::promise<CommonAPI::CallStatus> promise;
    promise.set_value(CommonAPI::CallStatus::INVALID_VALUE);
    return promise.get_future();
}

// Only stubbed, since std::future cannot be used in mock.
template <>
std::future<CommonAPI::CallStatus> EnhancedPositionProxy<>::GetTimeAsync(GetTimeAsyncCallback _callback,
                                                                         const CommonAPI::CallInfo *_info)
{
    (void)_callback;
    (void)_info;
    g_GetTimeAsync_calls++;
    std::promise<CommonAPI::CallStatus> promise;
    promise.set_value(CommonAPI::CallStatus::INVALID_VALUE);
    return promise.get_future();
}

template <>
bool EnhancedPositionProxy<>::isAvailable() const
{
    g_enhancedPositionProxyAvailable_calls++;
    return g_enhancedPositionProxyAvailable;
}

}  // namespace EnhancedPositionService
}  // namespace genivi
}  // namespace org
