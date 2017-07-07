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

namespace Connectivity
{
static const std::chrono::milliseconds s_position_request_timeout_ms_{2000};

GnssService::GnssService()
{
    //// Init for IpService base class.
    //// Fix me: Update IpService constructor with arguments and do work there. Avoid fiddling with details here and in
    /// other services...

    IpService::service_name_ = "GNSS";
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
        ALOGD("UTC: %d-%d-%d %d:%d:%d",
                                   p->gnssPositionData->utcTime->year,
                                   p->gnssPositionData->utcTime->month,
                                   p->gnssPositionData->utcTime->day,
                                   p->gnssPositionData->utcTime->hour,
                                   p->gnssPositionData->utcTime->minute,
                                   p->gnssPositionData->utcTime->second);

#ifdef OpGNSSPositionData_PRINT_ENABLED
        Icb_OpGNSSPositionData_Response_Print(p);
#endif

        // Notify CommonAPI listeners.
        fireUtcTimeEvent(*(p->gnssPositionData->utcTime));
        fireGpsTimeEvent(*(p->gnssPositionData->gpsTime));
        fireGeographicalPositionEvent(*(p->gnssPositionData->position));
        fireMovementEvent(*(p->gnssPositionData->movement));
        fireHeadingEvent(p->gnssPositionData->heading);
        fireGnssStatusEvent(*(p->gnssPositionData->gnssStatus));
        firePositioningStatusEvent(*(p->gnssPositionData->positioningStatus));
        fireSatelliteUsageEvent(*(p->gnssPositionData->satelliteInfo));
        firePrecisionEvent(*(p->gnssPositionData->precision));
        fireReceiverChannelsEvent(*(p->gnssPositionData->receiverChannels));
    }
}

void GnssService::fireUtcTimeEvent(const Icb_DateTime_t &src_utc_time)
{
    ALOGV("not impl Source utc time - year: %u, month: %u, day: %u, hour: %u, minute: %u, second: %u",
                                 src_utc_time.year,
                                 src_utc_time.month,
                                 src_utc_time.day,
                                 src_utc_time.hour,
                                 src_utc_time.minute,
                                 src_utc_time.second);
}

void GnssService::fireGpsTimeEvent(const Icb_GPSSystemTime_t &src_gps_system_time)
{
    ALOGV(
        "not impl Source weekNumber: %u, timeOfWeek: %u", src_gps_system_time.weekNumber, src_gps_system_time.timeOfWeek);
}

void GnssService::fireGeographicalPositionEvent(const Icb_GeographicalPosition_t &src_geographical_position)
{
    ALOGV(
        "not impl Source longitude: %d / 360*2^32 degrees, latitude: %d / 360*2^32 degrees, altitude: %d *0.1m.",
        src_geographical_position.longLat->longitude,
        src_geographical_position.longLat->latitude,
        src_geographical_position.altitude);

    // The altitude data from vcm is stored in 1/10m. We want whole meters.
    double altitude = src_geographical_position.altitude / 10.0;

    double floating_point_longitude =
        InfotainmentIpService::Utils::FixedPoint32ToDegreesD(src_geographical_position.longLat->longitude);
    double floating_point_latitude =
        InfotainmentIpService::Utils::FixedPoint32ToDegreesD(src_geographical_position.longLat->latitude);
}

void GnssService::fireMovementEvent(const Icb_Velocity_t &src_movement)
{
    ALOGV("not impl Source speed: %u, horizontalVelocity: %u, verticalVelocity: %d",
                                 src_movement.speed,
                                 src_movement.horizontalVelocity,
                                 src_movement.verticalVelocity);
}

void GnssService::fireHeadingEvent(const uint32_t &src_heading)
{
    ALOGV("not impl Source heading: %u", src_heading);
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
