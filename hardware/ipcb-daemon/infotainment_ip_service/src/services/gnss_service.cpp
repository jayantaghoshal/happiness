/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "Infotainment_IP_Service.gnss"
#include <assert.h>
#include <memory>
#include <cutils/log.h>

#include "gnss_service.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/net_serializer.h"
#include "util/type_conversion_helpers.h"

using namespace android::hardware::gnss::V1_0::implementation;
using namespace android::hardware::gnss::V1_0;
using namespace InfotainmentIpService::Utils;

namespace Connectivity
{
static const std::chrono::milliseconds s_position_request_timeout_ms_{2000};

GnssService::GnssService()
{
    //// Init for IpService base class.
    //// Fix me: Update IpService constructor with arguments and do work there. Avoid fiddling with details here and in
    /// other services...

    IpService::service_name_ = "GNSS";

    expect_location_accuracy_ = false; // set this according to which VCM ver you are currently using
    location_.timestamp = 0;
    location_.gnssLocationFlags = 0;
}

bool GnssService::Initialize(Connectivity::MessageDispatcher *msgDispatcher)
{
    //// Init for IpService base class.
    //// Fix me: Update IpService::Init() with arguments and do work there. Avoid fiddling with details here and in
    /// other services...
    IpService::setDispatcher(msgDispatcher);

    // Register our service, both client- and server part.
    if (!IpService::RegisterClient(ip_bus_client_subscriptions))
    {
        ALOGE("Can not register as client");
        return false;
    }

    android::status_t status = gnss_.registerAsService();

    if (status != android::OK) {
        ALOGE("Failed to register Gnss binder service: %d",status);
    } else {
        ALOGI("Gnss binder service register ok");
    }

    return true;
}

void GnssService::Uninitialize()
{
}

void GnssService::cbGNSSPositionDataNotification(Message &msg)
{
    Icb_OpGNSSPositionData_Response p = nullptr;

    ALOGD("cbGNSSPositionDataNotification %04X.%04X.%02d 0x%08X. size: %d",
                               (int)msg.pdu.header.service_id,
                               (int)msg.pdu.header.operation_id,
                               (int)msg.pdu.header.operation_type,
                               msg.pdu.header.sender_handle_id,
                               (int)msg.pdu.payload.size());

    if (DecodeMessage(msg, p, Icb_OpGNSSPositionData_Response_Create, Icb_OpGNSSPositionData_Response_Decode))
    {
        // All ok.
        const int64_t mssince1970 = ToMsSince1970(p->gnssPositionData->utcTime);
        ALOGD("   UTC: %d-%d-%d %d:%d:%d:%ld",
                                   p->gnssPositionData->utcTime->year,
                                   p->gnssPositionData->utcTime->month,
                                   p->gnssPositionData->utcTime->day,
                                   p->gnssPositionData->utcTime->hour,
                                   p->gnssPositionData->utcTime->minute,
                                   p->gnssPositionData->utcTime->second,
                                   mssince1970);

#ifdef OpGNSSPositionData_PRINT_ENABLED
        Icb_OpGNSSPositionData_Response_Print(p);
#endif

        bool sendnow = false;
        if (!expect_location_accuracy_) {
            // we don't expect any accuracy signal to arrive so lets send this right now
            // We set some dummy values  for horizontal accuracy
            // otherwise Android completely filters out the location samples.
            location_.gnssLocationFlags |= (uint16_t)GnssLocationFlags::HAS_HORIZONTAL_ACCURACY;
            location_.horizontalAccuracyMeters = 10.0;
            sendnow=true;
        } else if (mssince1970==location_.timestamp) {
            // We seem to have already received the accuracy signal so lets send now
            sendnow=true;
        } else if (expect_location_accuracy_ && location_.timestamp!=0) {
            // This is just to detect misconfigurations; that we expect accuracy but we dont seem to get them
            ALOGW("Received cbGNSSPositionDataNotification but no accuracy in sight");
        }
        location_.timestamp = mssince1970;

        location_.gnssLocationFlags |=
            (uint16_t)GnssLocationFlags::HAS_LAT_LONG |
            (uint16_t)GnssLocationFlags::HAS_ALTITUDE |
            (uint16_t)GnssLocationFlags::HAS_SPEED |
            (uint16_t)GnssLocationFlags::HAS_BEARING;

        location_.latitudeDegrees  = FixedPoint32ToDegreesD(p->gnssPositionData->position->longLat->latitude);
        location_.longitudeDegrees = FixedPoint32ToDegreesD(p->gnssPositionData->position->longLat->longitude);
        location_.altitudeMeters = p->gnssPositionData->position->altitude / 10.0;   // dm -> meters
        location_.speedMetersPerSec = p->gnssPositionData->movement->speed / 1000.0; // mm/s -> m/s
        location_.bearingDegrees = p->gnssPositionData->heading / 100.0;             // 1/100 degrees -> degrees
        ALOGD("lat=%.4lf , long=%.4lf",location_.latitudeDegrees,location_.longitudeDegrees);


        // TODO these shall maybe also be handled
        // we call them here now just to get the logging
        fireGnssStatusEvent(*(p->gnssPositionData->gnssStatus));
        firePositioningStatusEvent(*(p->gnssPositionData->positioningStatus));
        fireSatelliteUsageEvent(*(p->gnssPositionData->satelliteInfo));
        firePrecisionEvent(*(p->gnssPositionData->precision));
        fireReceiverChannelsEvent(*(p->gnssPositionData->receiverChannels));


        if (sendnow) {
            ALOGD("location send to gnss, triggered by location");
            gnss_.updateLocation(location_);
            location_.timestamp = 0;
            location_.gnssLocationFlags = 0;
        }
    }
}

void GnssService::cbGNSSPositionDataAccuracyNotification(Message &msg)
{
    Icb_OpGNSSPositionDataAccuracy_Response p = nullptr;

    ALOGD("cbGNSSPositionDataAccuracyNotification %04X.%04X.%02d 0x%08X. size: %d",
                               (int)msg.pdu.header.service_id,
                               (int)msg.pdu.header.operation_id,
                               (int)msg.pdu.header.operation_type,
                               msg.pdu.header.sender_handle_id,
                               (int)msg.pdu.payload.size());

    if (DecodeMessage(msg, p, Icb_OpGNSSPositionDataAccuracy_Response_Create, Icb_OpGNSSPositionDataAccuracy_Response_Decode))
    {
        // All ok.
        const int64_t mssince1970 = ToMsSince1970(p->gnssPositionDataAccuracy->utcTime);
        ALOGD("   UTC: %d-%d-%d %d:%d:%d:%ld",
                                   p->gnssPositionDataAccuracy->utcTime->year,
                                   p->gnssPositionDataAccuracy->utcTime->month,
                                   p->gnssPositionDataAccuracy->utcTime->day,
                                   p->gnssPositionDataAccuracy->utcTime->hour,
                                   p->gnssPositionDataAccuracy->utcTime->minute,
                                   p->gnssPositionDataAccuracy->utcTime->second,
                                   mssince1970);

#ifdef OpGNSSPositionDataAccuracy_PRINT_ENABLED
        Icb_OpGNSSPositionDataAccuracy_Response_Print(p);
#endif
        bool sendnow = false;
        if (mssince1970==location_.timestamp) {
            // We seem to have already received the location signal so lets send now
            sendnow=true;
        }
        location_.timestamp = mssince1970;

        if (!expect_location_accuracy_) {
            ALOGW("Strange, we dont expect accuracy but still receives it!!");
        }

        location_.gnssLocationFlags |=
            (uint16_t)GnssLocationFlags::HAS_HORIZONTAL_ACCURACY |
            (uint16_t)GnssLocationFlags::HAS_VERTICAL_ACCURACY;
        // we can currently not provide speed and bearing accuracy since it is not available from the VCM

        // In Android we can only report horizontal accuracy but what we receive are values for
        // the lat and long axis. Now we take "max" of these but maybe "min" or "mean" could be used!?
        double lat_accuracy = p->gnssPositionDataAccuracy->accuracy->sdLatitude/100.0; // cm -> m
        double long_accuracy = p->gnssPositionDataAccuracy->accuracy->sdLongitud/100.0;// cm -> m
        location_.horizontalAccuracyMeters = std::max(lat_accuracy,long_accuracy);

        location_.verticalAccuracyMeters = p->gnssPositionDataAccuracy->accuracy->sdAltitude/100.0; // cm -> m

        if (sendnow) {
            ALOGD("location send to gnss, triggered by accuracy");
            gnss_.updateLocation(location_);
            location_.timestamp=0;
            location_.gnssLocationFlags = 0;
        }
    }
}


void GnssService::fireGnssStatusEvent(const Icb_GNSSUsage_t &src_gnss_status)
{
    ALOGV(
        "not impl Source gpsIsUsed: %u, glonass: %u, galileo: %u, sbas: %u, "
        "qzssL1: %u, qzssL1SAIF: %u",
        src_gnss_status.gpsIsUsed,
        src_gnss_status.glonassIsUsed,
        src_gnss_status.galileoIsUsed,
        src_gnss_status.sbasIsUsed,
        src_gnss_status.qzssL1IsUsed,
        src_gnss_status.qzssL1SAIFIsUsed);
}

void GnssService::firePositioningStatusEvent(const Icb_GNSSStatus_t &src_positioning_status)
{
    ALOGV("not impl Source fixType: %u, dgpsIsUsed: %u, selfEphemerisDataUsage: %u",
                                 src_positioning_status.fixType,
                                 src_positioning_status.dgpsIsUsed,
                                 src_positioning_status.selfEphemerisDataUsage);
}

void GnssService::fireSatelliteUsageEvent(const Icb_SatelliteUsage_t &src_satellite_info)
{
    ALOGV(
        "not impl Source nof. satellites visible gps: %u, glonass: %u, galileo: %u, sbas: %u, "
        "qzssL1: %u, qzssL1SAIF: %u",
        src_satellite_info.nrOfSatellitesVisible->gps,
        src_satellite_info.nrOfSatellitesVisible->glonass,
        src_satellite_info.nrOfSatellitesVisible->galileo,
        src_satellite_info.nrOfSatellitesVisible->sbas,
        src_satellite_info.nrOfSatellitesVisible->qzssL1,
        src_satellite_info.nrOfSatellitesVisible->qzssL1SAIF);

    ALOGV(
        "not impl Source nof. satellites used gps: %u, glonass: %u, galileo: %u, sbas: %u, "
        "qzssL1: %u, qzssL1SAIF: %u",
        src_satellite_info.nrOfSatellitesUsed->gps,
        src_satellite_info.nrOfSatellitesUsed->glonass,
        src_satellite_info.nrOfSatellitesUsed->galileo,
        src_satellite_info.nrOfSatellitesUsed->sbas,
        src_satellite_info.nrOfSatellitesUsed->qzssL1,
        src_satellite_info.nrOfSatellitesUsed->qzssL1SAIF);
}

void GnssService::firePrecisionEvent(const Icb_DOPValues_t &src_precision)
{
    ALOGV("not impl Source precision hdop: %u, vdop: %u, pdop: %u, tdop: %u",
                                 src_precision.hdop,
                                 src_precision.vdop,
                                 src_precision.pdop,
                                 src_precision.tdop);
}

void GnssService::fireReceiverChannelsEvent(const Icb_ReceiverChannelData_t &src_receiver_channels)
{
    //std::vector<generated::IpCommandBusTypes::Infotainment_IP_bus_ChannelData> dest_receiver_channels;

    // The below statements do not work since there is a bug in the asn.1 compiler
    // that reverses min and max size. We only need actual_size.
    // assert(src_receiver_channels.actual_size <= src_receiver_channels.max_size);
    // assert(src_receiver_channels.actual_size >= src_receiver_channels.min_size);

    //dest_receiver_channels.reserve(src_receiver_channels.actual_size);

    for (std::size_t i = 0; i < src_receiver_channels.actual_size; ++i)
    {
        assert(src_receiver_channels.data_array[i]);
        const Icb_ChannelData_t &current_src_channel_data = *(src_receiver_channels.data_array[i]);

        ALOGV(
            "not impl Source channel %u - prn: %u, trackingStatus (enum): %u, svacc: %u, snr: %u, azimuthAngle: %u, "
            "elevationAngle: %u.  VCC STD INTEGERS!",
            (unsigned int)i,
            (unsigned int)current_src_channel_data.prn,
            (unsigned int)current_src_channel_data.trackingStatus,
            (unsigned int)current_src_channel_data.svacc,
            (unsigned int)current_src_channel_data.snr,
            (unsigned int)current_src_channel_data.azimuthAngle,
            (unsigned int)current_src_channel_data.elevationAngle);
/*
        generated::IpCommandBusTypes::Infotainment_IP_bus_SatelliteTrackingStatusType dest_tracking_status(
            static_cast<generated::IpCommandBusTypes::Infotainment_IP_bus_SatelliteTrackingStatusType::Literal>(
                current_src_channel_data.trackingStatus));

        generated::IpCommandBusTypes::Infotainment_IP_bus_ExtendedChannelData_Choices::Literal
            dest_extended_channel_data_choice(
                static_cast<generated::IpCommandBusTypes::Infotainment_IP_bus_ExtendedChannelData_Choices::Literal>(
                    current_src_channel_data.extendedData->choice));

        generated::IpCommandBusTypes::Infotainment_IP_bus_ExtendedChannelData dest_extended_channel_data;
        dest_extended_channel_data.setChoice(dest_extended_channel_data_choice);

        // The ASN.1 data for current_src_channel_data.extendedData->data is OPTIONAL, controlled by the
        // enumeration value in current_src_channel_data.extendedData->choice.
        // For more information, read up on ASN.1 CHOICE.
        if (dest_extended_channel_data.getChoice() ==
            generated::IpCommandBusTypes::Infotainment_IP_bus_ExtendedChannelData_Choices::
                kInfotainment_IP_bus_ExtendedChannelData_data)
        {
            generated::IpCommandBusTypes::Infotainment_IP_bus_ChannelCorrectionData dest_channel_correction_data(
                current_src_channel_data.extendedData->data->pseudoRangeMetres,
                current_src_channel_data.extendedData->data->pseudoRangeMillimetres,
                current_src_channel_data.extendedData->data->rangeRate,
                current_src_channel_data.extendedData->data->pseudoRangeCorrectionData,
                current_src_channel_data.extendedData->data->selfEphemerisPredictionTime);

            dest_extended_channel_data.setData(dest_channel_correction_data);
        }

        generated::IpCommandBusTypes::Infotainment_IP_bus_ChannelData dest_channel_data_entry(
            current_src_channel_data.prn,
            dest_tracking_status,
            current_src_channel_data.svacc,
            current_src_channel_data.snr,
            current_src_channel_data.azimuthAngle,
            current_src_channel_data.elevationAngle,
            dest_extended_channel_data);

        dest_receiver_channels.push_back(dest_channel_data_entry);*/
    }
}

}  // Connectivity
