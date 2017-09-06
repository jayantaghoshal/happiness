#include "voc_backend_proxy.h"
#include <v0/org/volvocars/VolvoOnCall/VolvoOnCallBackendProxy.hpp>
#include <v0/org/volvocars/VolvoOnCall/VolvoOnCallBackendProxyBase.hpp>

#include "CommonAPI/CommonAPI.hpp"

int g_voc_backend_function_calls = 0;
int g_voc_backend_available_calls = 0;
bool g_voc_backend_proxy_available = false;

namespace v0_conn = v0::org::volvo::connectivity;

namespace v0 {
namespace org {
namespace volvocars {
namespace VolvoOnCall {

template <>
bool VolvoOnCallBackendProxy<>::isAvailable() const
{
    g_voc_backend_available_calls++;
    return g_voc_backend_proxy_available;
}

template <>
CommonAPI::ProxyStatusEvent& VolvoOnCallBackendProxy<>::getProxyStatusEvent()
{
    static CommonAPI::ProxyStatusEvent proxy_status_event;
    return proxy_status_event;
}

template <>
std::future<CommonAPI::CallStatus> VolvoOnCallBackendProxy<>::TextMessageSetRequestAsync(const v0_conn::IpCommandBusTypes::SourceStatus &source_status,
                                                                                         const std::string &source,
                                                                                         const std::string &text,
                                                                                         TextMessageSetRequestAsyncCallback callback,
                                                                                         const CommonAPI::CallInfo *info)
{
    (void) source_status;
    (void) source;
    (void) text;
    (void) callback;
    (void) info;

    g_voc_backend_function_calls++;
    std::future<CommonAPI::CallStatus> status;
    return status;
}


} // namespace VolvoOnCall
} // namespace volvocars
} // namespace org
} // namespace v0
