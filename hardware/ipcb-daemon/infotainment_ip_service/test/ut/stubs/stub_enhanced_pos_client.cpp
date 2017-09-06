/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <connectivity-sd/time_provider.h>
#include <ipcommandbus/log_context.h>
#include <ivi-logging.h>
#include <unistd.h>
#include <CommonAPI/CommonAPI.hpp>
#include <memory>
#include "enhanced_pos_client.h"
#include "util/type_conversion_helpers.h"

using namespace Connectivity;

namespace ePos = org::genivi::EnhancedPositionService;

LOG_DECLARE_DEFAULT_CONTEXT(g_enhanced_pos_client_log_context, "EPOS", "Enhanced Position Client Service")

EnhancedPosClient::EnhancedPosClient(ITimeProvider &timeProvider) : timeProvider{timeProvider}
{
}

void EnhancedPosClient::Initialize(Connectivity::MessageDispatcher *msg_dispatcher,
                                   std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context)
{
    (void)msg_dispatcher;
    (void)common_api_main_loop_context;
}

void EnhancedPosClient::PosUpdateCallback(const ePos::EnhancedPositionServiceTypes::Bitmask bit_mask)
{
    (void)bit_mask;
}

void EnhancedPosClient::GetPosAsyncCallback(const CommonAPI::CallStatus &call_status,
                                            const ePos::EnhancedPositionServiceTypes::Timestamp &timestamp_ms,
                                            const ePos::EnhancedPositionServiceTypes::PositionInfo &pos_info)
{
    (void)call_status;
    (void)timestamp_ms;
    (void)pos_info;
}

void EnhancedPosClient::GetPosAsyncCallbackData(const ePos::EnhancedPositionServiceTypes::PositionInfo &pos_info)
{
    (void)pos_info;
}

void EnhancedPosClient::GetTimeAsyncCallback(const CommonAPI::CallStatus &call_status,
                                             const ePos::EnhancedPositionServiceTypes::Timestamp &timestamp_ms,
                                             const ePos::EnhancedPositionServiceTypes::TimeInfo &time_info)
{
    (void)call_status;
    (void)timestamp_ms;
    (void)time_info;
}

void EnhancedPosClient::AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
{
}

void EnhancedPosClient::DrTimerEvent()
{
}

void EnhancedPosClient::UpdateDrPos(Icb_DRPositionData p_pos_data)
{
    (void)p_pos_data;
}

void EnhancedPosClient::SendDrPos(const VccIpCmd::ServiceId service_id,
                                  const VccIpCmd::OperationId operation_id,
                                  const VccIpCmd::OperationType operation_type,
                                  const Message::Ecu ecu,
                                  const uint8_t seq_nr)
{
    (void)service_id;
    (void)operation_id;
    (void)operation_type;
    (void)ecu;
    (void)seq_nr;
}

void EnhancedPosClient::DrPositionCopy(Icb_DRPositionData p_src, Icb_DRPositionData p_dest)
{
    (void)p_src;
    (void)p_dest;
}

void EnhancedPosClient::cbDrPositionReq(Message &msg)
{
    (void)msg;
}

void EnhancedPosClient::cbDrPositionNotificationReq(Message &msg)
{
    (void)msg;
}
