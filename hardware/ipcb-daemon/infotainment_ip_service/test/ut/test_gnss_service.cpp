/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <memory>
#include "connectivity-sd/mock_time_provider.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/TransportServices.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_gnss_service_stub.h"
#include "mocks/mock_ip_service.h"
#include "pl/asn_base/asn_base.h"
#include "services/gnss_service.h"
#include "util/type_conversion_helpers.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

class GnssServiceFixture : public ::testing::Test
{
public:
    GnssServiceFixture() : gnssService_{}
    {
    }

    void SetUp()
    {
        gnssService_.gnss_service_stub_ = p_gnss_service_stub_;
    }

    Connectivity::GnssService gnssService_;
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcher_;

    std::shared_ptr<NiceMock<MockGnssServiceStub>> p_gnss_service_stub_ =
        std::make_shared<NiceMock<MockGnssServiceStub>>();

    // Wrapper methods for private members in GnssService

    std::shared_ptr<Connectivity::gen_conn::GnssServiceStub> GetStub()
    {
        return gnssService_.GetStub();
    }

    void cbGNSSPositionDataNotification(Connectivity::Message &msg)
    {
        gnssService_.cbGNSSPositionDataNotification(msg);
    }

    void fireUtcTimeEvent(const Icb_DateTime_t &src_utc_time)
    {
        gnssService_.fireUtcTimeEvent(src_utc_time);
    }

    void fireGpsTimeEvent(const Icb_GPSSystemTime_t &src_gps_system_time)
    {
        gnssService_.fireGpsTimeEvent(src_gps_system_time);
    }

    void fireGeographicalPositionEvent(const Icb_GeographicalPosition_t &src_geographical_position)
    {
        gnssService_.fireGeographicalPositionEvent(src_geographical_position);
    }

    void fireMovementEvent(const Icb_Velocity_t &src_movement)
    {
        gnssService_.fireMovementEvent(src_movement);
    }

    void fireHeadingEvent(const uint32_t &src_heading)
    {
        gnssService_.fireHeadingEvent(src_heading);
    }

    void fireGnssStatusEvent(const Icb_GNSSUsage_t &src_gnss_status)
    {
        gnssService_.fireGnssStatusEvent(src_gnss_status);
    }

    void firePositioningStatusEvent(const Icb_GNSSStatus_t &src_positioning_status)
    {
        gnssService_.firePositioningStatusEvent(src_positioning_status);
    }

    void fireSatelliteUsageEvent(const Icb_SatelliteUsage_t &src_satellite_info)
    {
        gnssService_.fireSatelliteUsageEvent(src_satellite_info);
    }

    void firePrecisionEvent(const Icb_DOPValues_t &src_precision)
    {
        gnssService_.firePrecisionEvent(src_precision);
    }

    void fireReceiverChannelsEvent(const Icb_ReceiverChannelData_t &src_receiver_channels)
    {
        gnssService_.fireReceiverChannelsEvent(src_receiver_channels);
    }
};

// Original wrapper used in the old transport layer unit tests:
// This setup lead to quite long failure logs with the actual fault being a little too well disguised.
// MATCHER_P(LambdaMatcher, func, "") { bool ret=false; *result_listener << func(arg, ret); return ret; }

// Instead we here catch the faults with EXPECT_EQ, EXPECT_TRUE, etc which give a compact and clear error message upon
// failure.
// It may look weird that this wrapper always returns true, but if an expect check fails in func, the parent test will
// automatically fail as well.
MATCHER_P(LambdaMatcher, func, "")
{
    func(arg);
    return true;
}

TEST_F(GnssServiceFixture, Initialize)
{
    EXPECT_CALL(gnssService_, RegisterClient(_)).WillOnce(Return(false));
    EXPECT_FALSE(gnssService_.Initialize(&messageDispatcher_));

    EXPECT_CALL(gnssService_, RegisterClient(_)).WillOnce(Return(true));
    EXPECT_TRUE(gnssService_.Initialize(&messageDispatcher_));

    // Should do nothing. Run here to increase coverage.
    gnssService_.Uninitialize();
}
