/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <v0/org/volvocars/climate/ParkingClimateProxy.hpp>
#include <v0/org/volvocars/climate/ParkingClimateProxyBase.hpp>
#include "climate_timer_proxy.h"

#include "CommonAPI/CommonAPI.hpp"

/*
int g_climate_timer_function_calls = 0;
int g_climate_timer_available_calls = 0;
bool g_climate_timer_proxy_available = false;

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {
*/
namespace Connectivity
{
bool ClimateTimerProxy::Initialize(MessageDispatcher *msgDispatcher,
                                   std::shared_ptr<CommonAPI::MainLoopContext> commonApiMainLoopContext)
{
    (void)msgDispatcher;
    (void)commonApiMainLoopContext;

    return true;
}

void ClimateTimerProxy::cbGenericSettingSynchReq(Message &msg)
{
    (void)msg;
}
void ClimateTimerProxy::cbGenericSettingSynchSetReq(Message &msg)
{
    (void)msg;
}
void ClimateTimerProxy::cbGenericSettingSynchNotificationReq(Message &msg)
{
    (void)msg;
}

// template <>
// bool ParkingClimateProxy<>::Initialize(Connectivity::MessageDispatcher *msgDispatcher,
//                                const CommonAPI::ConnectionId_t& connection_id)
//{
//  (void)msgDispatcher;
//  (void)connection_id;
//  return true;
//}
/*
template <>
bool ParkingClimateProxy<>::isAvailable() const
{
    g_climate_timer_available_calls++;
    return g_climate_timer_proxy_available;
}

template <>
CommonAPI::ProxyStatusEvent& ParkingClimateProxy<>::getProxyStatusEvent()
{
    static CommonAPI::ProxyStatusEvent proxy_status_event;
    return proxy_status_event;
}

template <>
std::future<CommonAPI::CallStatus> ParkingClimateProxy<>::requestTimerListAsync(
        const ParkingClimate::DateTimeStruct &_timestamp,
        const ParkingClimate::ParkingTimers &_list,
        ParkingClimateProxyBase::RequestTimerListAsyncCallback _callback,
        const CommonAPI::CallInfo *_info)
{
    (void) _timestamp;
    (void) _list;
    (void) _callback;
    (void) _info;

    g_climate_timer_function_calls++;
    std::future<CommonAPI::CallStatus> status;
    return status;
}
*/
// template <>
// std::shared_ptr<ParkingClimateProxy> ParkingClimateProxy<>::get(void)
//{
//    g_climate_timer_function_calls++;
//    ParkingClimateProxy proxy_;
//    std::shared_ptr ret = &proxy_;
//    return ret;
//}
}  // namespace connectivity
/*
} // namespace climate
} // namespace volvo
} // namespace org
} // namespace v0
*/
