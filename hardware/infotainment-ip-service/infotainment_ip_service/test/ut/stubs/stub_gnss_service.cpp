/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <assert.h>
#include <memory>

#include <connectivity-sd/time_provider.h>
#include "gnss_service.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/log_context.h"
#include "ipcommandbus/net_serializer.h"
#include "util/type_conversion_helpers.h"

// TODO: Ensure that correct intervals are used for response timeouts in both directions...

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_gnss_log_context, "GNSS", "Service for the GNSS positioning (from VCM)")

GnssService::GnssService()
{
    IpService::service_name_ = "GNSS";
    gnss_service_stub_ = std::make_shared<GnssServiceStub>();
}

bool GnssService::Initialize(Connectivity::MessageDispatcher *msgDispatcher)
{
    return true;
}

void GnssService::Uninitialize()
{
}

void GnssService::cbGNSSPositionDataNotification(Message &msg)
{
    (void)msg;
}

void GnssService::fireUtcTimeEvent(const Icb_DateTime_t &src_utc_time)
{
    (void)src_utc_time;
}

void GnssService::fireGpsTimeEvent(const Icb_GPSSystemTime_t &src_gps_system_time)
{
    (void)src_gps_system_time;
}

void GnssService::fireGeographicalPositionEvent(const Icb_GeographicalPosition_t &src_geographical_position)
{
    (void)src_geographical_position;
}

void GnssService::fireMovementEvent(const Icb_Velocity_t &src_movement)
{
    (void)src_movement;
}

void GnssService::fireHeadingEvent(const uint32_t &src_heading)
{
    (void)src_heading;
}

void GnssService::fireGnssStatusEvent(const Icb_GNSSUsage_t &src_gnss_status)
{
    (void)src_gnss_status;
}

void GnssService::firePositioningStatusEvent(const Icb_GNSSStatus_t &src_positioning_status)
{
    (void)src_positioning_status;
}

void GnssService::fireSatelliteUsageEvent(const Icb_SatelliteUsage_t &src_satellite_info)
{
    (void)src_satellite_info;
}

void GnssService::firePrecisionEvent(const Icb_DOPValues_t &src_precision)
{
    (void)src_precision;
}

void GnssService::fireReceiverChannelsEvent(const Icb_ReceiverChannelData_t &src_receiver_channels)
{
    (void)src_receiver_channels;
}

}  // Connectivity
