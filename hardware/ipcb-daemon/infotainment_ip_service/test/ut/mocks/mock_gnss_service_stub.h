/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_GNSS_SERVICE_STUB_H
#define MOCK_GNSS_SERVICE_STUB_H

#include <memory>
#include "gmock/gmock.h"
#include "services/gnss_service_stub.h"

namespace generated = v0_1::org::volvo::connectivity;

class MockGnssServiceStub : public Connectivity::GnssServiceStub
{
public:
    MockGnssServiceStub()
    {
    }
    ~MockGnssServiceStub()
    {
    }

    MOCK_METHOD1(getInterfaceVersion, CommonAPI::Version &(std::shared_ptr<CommonAPI::ClientId> clientId));

    MOCK_METHOD1(initStubAdapter,
                 generated::GnssServiceStubRemoteEvent *(
                     const std::shared_ptr<generated::GnssServiceStubAdapter> &_stubAdapter));

    MOCK_METHOD1(fireUtcTimeEvent, void(const generated::IpCommandBusTypes::Infotainment_IP_bus_DateTime &utc_time));

    MOCK_METHOD1(fireGpsTimeEvent,
                 void(const generated::IpCommandBusTypes::Infotainment_IP_bus_GPSSystemTime &gps_time));

    MOCK_METHOD1(fireGeographicalPositionEvent,
                 void(const generated::IpCommandBusTypes::Infotainment_IP_bus_GeographicalPosition_WGS84 &position));

    MOCK_METHOD1(fireMovementEvent, void(const generated::IpCommandBusTypes::Infotainment_IP_bus_Velocity &movement));

    MOCK_METHOD1(fireHeadingEvent, void(const uint32_t &heading));

    MOCK_METHOD1(fireGnssStatusEvent,
                 void(const generated::IpCommandBusTypes::Infotainment_IP_bus_GNSSUsage &gnss_status));

    MOCK_METHOD1(firePositioningStatusEvent,
                 void(const generated::IpCommandBusTypes::Infotainment_IP_bus_GNSSStatus &positioning_status));

    MOCK_METHOD1(fireSatelliteUsageEvent,
                 void(const generated::IpCommandBusTypes::Infotainment_IP_bus_SatelliteUsage &satellite_info));

    MOCK_METHOD1(firePrecisionEvent,
                 void(const generated::IpCommandBusTypes::Infotainment_IP_bus_DOPValues &precision));

    MOCK_METHOD1(
        fireReceiverChannelsEvent,
        void(const std::vector<generated::IpCommandBusTypes::Infotainment_IP_bus_ChannelData> &receiver_channels));

    // protected:
    //  class RemoteEventHandler : public virtual generated::GnssServiceStubRemoteEvent {
    //   public:
    //    RemoteEventHandler(GnssServiceStub *_defaultStub);
    //
    //   private:
    //    GnssServiceStub *defaultStub_;
    //  };
    //
    // private:
    //  GnssServiceStub::RemoteEventHandler remoteEventHandler_;
    //  CommonAPI::Version version_;
};

#endif  // MOCK_GNSS_SERVICE_STUB_H
