/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <v1/com/ihu/Diagnostics/DiagnosticsProxyBase.hpp>
#include "CommonAPI/CommonAPI.hpp"
#include "v1/com/ihu/Diagnostics/DiagnosticsProxy.hpp"

// Globals
bool g_diagnosticsProxyAvailable = false;
std::vector<uint8_t> g_payload;

namespace v1
{
namespace com
{
namespace ihu
{
namespace Diagnostics
{
template <>
CommonAPI::ProxyStatusEvent &DiagnosticsProxy<>::getProxyStatusEvent()
{
    static CommonAPI::ProxyStatusEvent proxyStatusEvent;
    return proxyStatusEvent;
}

template <>
bool DiagnosticsProxy<>::isAvailable() const
{
    return g_diagnosticsProxyAvailable;
}

template <>
std::future<CommonAPI::CallStatus> DiagnosticsProxy<>::SubscribeOnKeysAsync(const std::vector<uint16_t> &_keys,
                                                                            SubscribeOnKeysAsyncCallback _callback,
                                                                            const CommonAPI::CallInfo *_info)
{
    (void)_keys;
    (void)_callback;
    (void)_info;
    std::promise<CommonAPI::CallStatus> promise;
    promise.set_value(CommonAPI::CallStatus::INVALID_VALUE);
    return promise.get_future();
}

template <>
std::future<CommonAPI::CallStatus> DiagnosticsProxy<>::MessageToDiagAsync(const uint16_t &_key,
                                                                          const std::vector<uint8_t> &_payload,
                                                                          MessageToDiagAsyncCallback _callback,
                                                                          const CommonAPI::CallInfo *_info)
{
    (void)_key;
    (void)_payload;
    (void)_callback;
    (void)_info;
    g_payload = _payload;
    std::promise<CommonAPI::CallStatus> promise;
    promise.set_value(CommonAPI::CallStatus::INVALID_VALUE);
    return promise.get_future();
}

}  // namespace Diagnostics
}  // namespace ihu
}  // namespace com
}  // namespace v1
