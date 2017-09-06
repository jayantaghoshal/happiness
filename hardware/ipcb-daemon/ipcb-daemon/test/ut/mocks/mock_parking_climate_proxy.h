/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <v0/org/volvocars/climate/ParkingClimateProxy.hpp>
#include <v0/org/volvocars/climate/ParkingClimateProxyBase.hpp>
#include "stubs/stub_capi_attribute.hpp"

int g_climate_timer_function_calls;
int g_climate_timer_available_calls;
bool g_climate_timer_proxy_available;

namespace v0
{
namespace org
{
namespace volvocars
{
namespace climate
{
// template <>
// bool ParkingClimateProxy<>::Initialize(Connectivity::MessageDispatcher *msgDispatcher,
//                                const CommonAPI::ConnectionId_t& connection_id)
//{
//  (void)msgDispatcher;
//  (void)connection_id;
//  return true;
//}

template <>
bool ParkingClimateProxy<>::isAvailable() const
{
    g_climate_timer_available_calls++;
    return g_climate_timer_proxy_available;
}

template <>
CommonAPI::ProxyStatusEvent &ParkingClimateProxy<>::getProxyStatusEvent()
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
    (void)_timestamp;
    (void)_list;
    (void)_callback;
    (void)_info;

    g_climate_timer_function_calls++;
    std::future<CommonAPI::CallStatus> status;
    return status;
}

template <>
CommonAPI::ObservableReadonlyAttribute<::v0::org::volvocars::climate::ParkingClimate::ParkingTimers>
    &ParkingClimateProxy<>::getTimerListAttribute()
{
    static MockCommonApiAttribute<::v0::org::volvocars::climate::ParkingClimate::ParkingTimers> timerListAttribute;
    return timerListAttribute;
}

// template <>
// std::shared_ptr<ParkingClimateProxy> ParkingClimateProxy<>::get(void)
//{
//    g_climate_timer_function_calls++;
//    ParkingClimateProxy proxy_;
//    std::shared_ptr ret = &proxy_;
//    return ret;
//}

}  // namespace climate
}  // namespace volvo
}  // namespace org
}  // namespace v0
