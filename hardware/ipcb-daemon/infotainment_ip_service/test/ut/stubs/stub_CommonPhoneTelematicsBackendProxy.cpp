/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "common_phone_tele_proxy.h"
#include <v0/org/volvocars/VolvoOnCall/CommonPhoneTelematicsBackendProxy.hpp>
#include <v0/org/volvocars/VolvoOnCall/CommonPhoneTelematicsBackendProxyBase.hpp>

#include "CommonAPI/CommonAPI.hpp"

int g_common_phone_tele_proxy_function_calls = 0;
int g_common_phone_tele_proxy_available_calls = 0;
bool g_common_phone_tele_proxy_available = false;

namespace v0_conn = v0::org::volvo::connectivity;

namespace v0 {
namespace org {
namespace volvocars {
namespace VolvoOnCall {

template <>
bool CommonPhoneTelematicsBackendProxy<>::isAvailable() const
{
    g_common_phone_tele_proxy_available_calls++;
    return g_common_phone_tele_proxy_available;
}

template <>
CommonAPI::ProxyStatusEvent& CommonPhoneTelematicsBackendProxy<>::getProxyStatusEvent()
{
    static CommonAPI::ProxyStatusEvent proxy_status_event;
    return proxy_status_event;
}

template <>
std::future<CommonAPI::CallStatus> CommonPhoneTelematicsBackendProxy<>::PremiumAudioSetRequestAsync(const bool &is_required,
                                                                                                    PremiumAudioSetRequestAsyncCallback callback,
                                                                                                    const CommonAPI::CallInfo *info)
{
    (void) is_required;
    (void) callback;
    (void) info;

    g_common_phone_tele_proxy_function_calls++;
    std::future<CommonAPI::CallStatus> status;
    return status;
}

template <>
CommonPhoneTelematicsBackendProxy<>::PremiumAudioNotificationEvent& CommonPhoneTelematicsBackendProxy<>::getPremiumAudioNotificationEvent()
{
    static CommonPhoneTelematicsBackendProxy::PremiumAudioNotificationEvent notification_event;
    return notification_event;
}

} // namespace VolvoOnCall
} // namespace volvocars
} // namespace org
} // namespace v0
