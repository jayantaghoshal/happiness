/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef GNSS_SERVICE_H
#define GNSS_SERVICE_H

#include <atomic>
#include <cstdint>
#include <functional>
#include <list>
#include "IpService.h"
#include "ipcommandbus/Message.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/vcc_pdu_header.h"

#if defined(ENABLE_TESTS)
#include "../../test/ut/mocks/mock_ip_service.h"
class GnssServiceFixture;
#endif

namespace Connectivity
{

/**
 *
 */
class GnssService :
#if defined(ENABLE_TESTS)
    public ::testing::NiceMock<MockIpService>
#else
    public IpService
#endif
{
#ifdef ENABLE_TESTS
    friend class ::GnssServiceFixture;
#endif
    friend class ServiceManager;

public:
    GnssService();

    /**
     * Initialize ConnectivityService and the internal Connectivity functional block.
     *
     * @param[in] msg_dispatcher  Message dispatcher to use with Connectivity.
     */
    bool Initialize(::Connectivity::MessageDispatcher *msgDispatcher);

    /**
     * Uninitialize ConnectivityService.
     */
    void Uninitialize();

    //// Methods for sending messages to VCM, for services where the IHU is CLIENT node on ip command bus

    //// Helpers

    // TODO: Implement enum-to-string functions for enums and use in debug logs...
    // static const char * XxxToString(Icb_Xxx_t xxx);

protected:

    void cbGNSSPositionDataNotification(Message &msg);

    /**
     * description: The UTC time provided by the VCM, provided by GNSS (GPS, etc).
     */
    /// Sends a broadcast event for UtcTime.
    void fireUtcTimeEvent(const Icb_DateTime_t &src_utc_time);

    /**
     * description: The GPS time provided by the VCM. Prefer UTC time unless you explicitly

     *   need this type of time.
     */
    /// Sends a broadcast event for GpsTime.
    void fireGpsTimeEvent(const Icb_GPSSystemTime_t &src_gps_system_time);

    /**
     * description: The raw geographical position provided by the VCM.
            Note that this type
     *   of position is not the optimal position in most cases. Prefer to use

     *   an enhanced (map matched, predicted, ...) position provided by
     *   EnhancedPositionService instead.
     */
    /// Sends a broadcast event for GeographicalPosition.
    void fireGeographicalPositionEvent(const Icb_GeographicalPosition_t &src_geographical_position);

    /**
     * description: Get speed, horizontal and vertical velocity, provided by the VCM.
     */
    /// Sends a broadcast event for Movement.
    void fireMovementEvent(const Icb_Velocity_t &src_movement);

    /**
     * description: Heading provided by the VCM.
     */
    /// Sends a broadcast event for Heading.
    void fireHeadingEvent(const uint32_t &src_heading);

    /**
     * description: Gnss status provided by the VCM. Includes
            what type of positioning
     *   system is being used.
     */
    /// Sends a broadcast event for GnssStatus.
    void fireGnssStatusEvent(const Icb_GNSSUsage_t &src_gnss_status);

    /**
     * description: Positioning status provided by the VCM. Includes
            fix type (do we have a
     *   GNSS fix?) and if DGPS is used.
     */
    /// Sends a broadcast event for PositioningStatus.
    void firePositioningStatusEvent(const Icb_GNSSStatus_t &src_positioning_status);

    /**
     * description: Satellite information provided by the VCM. Includes
            Number of
     *   satellites visible and number of satellites used.
     */
    /// Sends a broadcast event for SatelliteUsage.
    void fireSatelliteUsageEvent(const Icb_SatelliteUsage_t &src_satellite_info);

    /**
     * description: Precision information provided by the VCM.
     */
    /// Sends a broadcast event for Precision.
    void firePrecisionEvent(const Icb_DOPValues_t &src_precision);

    /**
     * description: Receiver channels information provided by the VCM.
     */
    /// Sends a broadcast event for ReceiverChannels.
    void fireReceiverChannelsEvent(const Icb_ReceiverChannelData_t &src_receiver_channels);

    //// IHU as client node.

    //// Storage of latest values from ip command bus (notifications/responses).

    //// Storage of latest values from CommonAPI.

    //// Services that we subscribe to from other nodes on ip command bus
    ////
    //// This vector is used for registering our service layer callbacks, for services that other nodes provide.
    std::vector<ClientIpCommandBusService> ip_bus_client_subscriptions = {{
        Message::VCM,
        VccIpCmd::ServiceId::Positioning,
        VccIpCmd::OperationId::GNSSPositionData,
        nullptr,
        std::bind(&GnssService::cbGNSSPositionDataNotification, this, std::placeholders::_1),
        false  // Static notification
    }};

};  // class GnssService

}  // namespace Connectivity

#endif  // GNSS_SERVICE_H
