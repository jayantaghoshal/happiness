/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <v0/org/volvo/connectivity/OTASwlmProxy.hpp>
#include "CommonAPI/CommonAPI.hpp"

bool g_ota_swlm_available_ = false;
int g_ota_swlm_available_calls = 0;
int g_ota_swlm_function_called = 0;

namespace v0
{
namespace org
{
namespace volvo
{
namespace connectivity
{
template <>
bool OTASwlmProxy<>::isAvailable() const
{
    g_ota_swlm_available_calls++;
    return g_ota_swlm_available_;
}

template <>
CommonAPI::ProxyStatusEvent &OTASwlmProxy<>::getProxyStatusEvent()
{
    static CommonAPI::ProxyStatusEvent proxyStatusEvent;
    return proxyStatusEvent;
}

template <>
OTASwlmProxyBase::DeviceInstallNotificationEvent &OTASwlmProxy<>::getDeviceInstallNotificationEvent()
{
    static OTASwlmProxyBase::DeviceInstallNotificationEvent deviceInstallNotifEvent;
    return deviceInstallNotifEvent;
}

template <>
OTASwlmProxyBase::LocalStorageNotificationEvent &OTASwlmProxy<>::getLocalStorageNotificationEvent()
{
    static OTASwlmProxyBase::LocalStorageNotificationEvent localStorageNotifEvent;
    return localStorageNotifEvent;
}

template <>
std::future<CommonAPI::CallStatus> OTASwlmProxy<>::DeviceInstallRequestAsync(
    DeviceInstallRequestAsyncCallback _callback, const CommonAPI::CallInfo *_info)
{
    g_ota_swlm_function_called++;
    std::future<CommonAPI::CallStatus> status;
    return status;
}

template <>
std::future<CommonAPI::CallStatus> OTASwlmProxy<>::DeviceInstallSetRequestAsync(
    const ::v0::org::volvo::connectivity::OTATypes::InstallCommand &_installCommand,
    const std::vector<::v0::org::volvo::connectivity::OTATypes::InstallationData> &_installList,
    DeviceInstallSetRequestAsyncCallback _callback,
    const CommonAPI::CallInfo *_info)
{
    g_ota_swlm_function_called++;
    std::future<CommonAPI::CallStatus> status;
    return status;
}

template <>
std::future<CommonAPI::CallStatus> OTASwlmProxy<>::LocalStorageRequestAsync(LocalStorageRequestAsyncCallback _callback,
                                                                            const CommonAPI::CallInfo *_info)
{
    g_ota_swlm_function_called++;
    std::future<CommonAPI::CallStatus> status;
    return status;
}
}
}
}
}
