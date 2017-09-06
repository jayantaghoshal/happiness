/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "gnss_service_stub.h"

#include <memory>

namespace Connectivity
{
GnssServiceStub::RemoteEventHandler::RemoteEventHandler(GnssServiceStub *_defaultStub) : defaultStub_(_defaultStub)
{
}

GnssServiceStub::GnssServiceStub() : remoteEventHandler_(this), version_(generated::GnssService::getInterfaceVersion())
{
}

GnssServiceStub::~GnssServiceStub()
{
}

// ----------------------------------------------------------------------------------------------------
// Implementation of GnssServiceStubStub
// ----------------------------------------------------------------------------------------------------
const CommonAPI::Version &GnssServiceStub::getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> clientId)
{
    (void)clientId;
    return version_;
}

generated::GnssServiceStubRemoteEvent *GnssServiceStub::initStubAdapter(
    const std::shared_ptr<generated::GnssServiceStubAdapter> &stub_adapter)
{
    CommonAPI::Stub<generated::GnssServiceStubAdapter, generated::GnssServiceStubRemoteEvent>::stubAdapter_ =
        stub_adapter;
    return &remoteEventHandler_;
}

void GnssServiceStub::fireUtcTimeEvent(const generated::IpCommandBusTypes::Infotainment_IP_bus_DateTime &utc_time)
{
    getStubAdapter()->fireUtcTimeEvent(utc_time);
}

void GnssServiceStub::fireGpsTimeEvent(const generated::IpCommandBusTypes::Infotainment_IP_bus_GPSSystemTime &gps_time)
{
    getStubAdapter()->fireGpsTimeEvent(gps_time);
}

void GnssServiceStub::fireGeographicalPositionEvent(
    const generated::IpCommandBusTypes::Infotainment_IP_bus_GeographicalPosition_WGS84 &position)
{
    getStubAdapter()->fireGeographicalPositionEvent(position);
}

void GnssServiceStub::fireMovementEvent(const generated::IpCommandBusTypes::Infotainment_IP_bus_Velocity &movement)
{
    getStubAdapter()->fireMovementEvent(movement);
}

void GnssServiceStub::fireHeadingEvent(const uint32_t &heading)
{
    getStubAdapter()->fireHeadingEvent(heading);
}

void GnssServiceStub::fireGnssStatusEvent(
    const generated::IpCommandBusTypes::Infotainment_IP_bus_GNSSUsage &gnss_status)
{
    getStubAdapter()->fireGnssStatusEvent(gnss_status);
}

void GnssServiceStub::firePositioningStatusEvent(
    const generated::IpCommandBusTypes::Infotainment_IP_bus_GNSSStatus &positioning_status)
{
    getStubAdapter()->firePositioningStatusEvent(positioning_status);
}

void GnssServiceStub::fireSatelliteUsageEvent(
    const generated::IpCommandBusTypes::Infotainment_IP_bus_SatelliteUsage &satellite_info)
{
    getStubAdapter()->fireSatelliteUsageEvent(satellite_info);
}

void GnssServiceStub::firePrecisionEvent(const generated::IpCommandBusTypes::Infotainment_IP_bus_DOPValues &precision)
{
    getStubAdapter()->firePrecisionEvent(precision);
}

void GnssServiceStub::fireReceiverChannelsEvent(
    const std::vector<generated::IpCommandBusTypes::Infotainment_IP_bus_ChannelData> &receiver_channels)
{
    getStubAdapter()->fireReceiverChannelsEvent(receiver_channels);
}

}  // namespace Connectivity
