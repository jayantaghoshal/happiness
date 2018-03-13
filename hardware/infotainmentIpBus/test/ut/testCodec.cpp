/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>

#include "asn_codec.h"

#define LOG_TAG "iipbus.ut"
#include <cutils/log.h>

class TypeConvHelpersTest : public ::testing::Test {
  protected:
    void verifyAndClear() {}
};

/**
 * Encode/Decode one message.
 * Do NOT use session, evaluate complexity
 */
TEST_F(TypeConvHelpersTest, EncodeWOSession) {
    //------------------ Prepare data for encoding ---------------------
    ALOGI("Starting prepare data!");
    Icb_GNSSData_t gnssPositionData;

    Icb_DateTime_t time = {
            2017,  // utcTime.year
            10,    // utcTime.month
            5,     // utcTime.day
            8,     // utcTime.hour
            1,     // utcTime.minute
            2      // utcTime.second
    };
    gnssPositionData.utcTime = &time;

    Icb_GPSSystemTime_t gpsTime = {
            40,    // gpsTime.weekNumber
            12345  // gpsTime.timeOfWeek
    };
    gnssPositionData.gpsTime = &gpsTime;

    Icb_CoordinatesLongLat_t longLat = {
            (int)(57.708870 * INT_MAX) / 360,  // longLat.longitude (degrees/360*2^32)
            (int)(11.974560 * INT_MAX) / 360,  // longLat.latitude (degrees/360*2^32)
    };
    Icb_GeographicalPosition_t position = {
            &longLat,
            21  // position.altitude
    };
    gnssPositionData.position = &position;

    Icb_Velocity_t movement = {
            99,                // movement.speed
            22,                // movement.horizontalVelocity
            (int)(0.5 * 1000)  // movement.verticalVelocity (1/1000 m/s)
    };
    gnssPositionData.movement = &movement;

    gnssPositionData.heading = 234;

    Icb_GNSSUsage_t gnssStatus = {
            true,  // gnssStatus.gpsIsUsed
            true,  // gnssStatus.glonassIsUsed
            true,  // gnssStatus.galileoIsUsed
            true,  // gnssStatus.sbasIsUsed
            true,  // gnssStatus.qzssL1IsUsed
            true   // gnssStatus.qzssL1SAIFIsUsed
    };
    gnssPositionData.gnssStatus = &gnssStatus;

    Icb_GNSSStatus_t positioningStatus = {
            Icb_GnssFixType_t::e_Icb_GnssFixType_fix3D,  // positioningStatus.fixType
            true,                                        // positioningStatus.dgpsIsUsed
            true                                         // positioningStatus.selfEphemerisDataUsage
    };
    gnssPositionData.positioningStatus = &positioningStatus;

    Icb_NrOfSatellitesPerSystem_t nrOfSatellitesVisible = {
            11,  // nrOfSatellitesVisible.gps
            12,  // nrOfSatellitesVisible.glonass
            13,  // nrOfSatellitesVisible.galileo
            14,  // nrOfSatellitesVisible.sbas
            15,  // nrOfSatellitesVisible.qzssL1
            16   // nrOfSatellitesVisible.qzssL1SAIF
    };
    Icb_NrOfSatellitesPerSystem_t nrOfSatellitesUsed = {
            1,  // nrOfSatellitesUsed.gps
            2,  // nrOfSatellitesUsed.glonass
            3,  // nrOfSatellitesUsed.galileo
            4,  // nrOfSatellitesUsed.sbas
            5,  // nrOfSatellitesUsed.qzssL1
            6   // nrOfSatellitesUsed.qzssL1SAIF
    };
    Icb_SatelliteUsage_t satelliteInfo = {&nrOfSatellitesVisible, &nrOfSatellitesUsed};
    gnssPositionData.satelliteInfo = &satelliteInfo;

    Icb_DOPValues_t precision = {
            123,  // precision.hdop
            124,  // precision.vdop
            125,  // precision.pdop
            126   // precision.tdop
    };
    gnssPositionData.precision = &precision;

    Icb_ChannelCorrectionData_t data_1 = {
            -1,  // pseudoRangeMetres
            2,   // pseudoRangeMillimetres
            -3,  // rangeRate
            -4,  // pseudoRangeCorrectionData
            5    // selfEphemerisPredictionTime
    };
    Icb_ExtendedChannelData_t extendedData_1 = {
            Icb_ExtendedChannelData_Choices::e_Icb_ExtendedChannelData_data,  // choice
            nullptr,                                                          // notSupported
            &data_1};
    Icb_ChannelData_t channelData[1] = {
            {1,                                                                              // prn
             Icb_SatelliteTrackingStatusType_t::e_Icb_SatelliteTrackingStatusType_tracking,  // trackingStatus
             10,                                                                             // svacc
             20,                                                                             // snr
             30,                                                                             // azimuthAngle
             40,                                                                             // elevationAngle
             &extendedData_1}};
    Icb_ChannelData channelData_p = &channelData[0];
    Icb_ReceiverChannelData_t receiverChannels = {0,  // min and max taken from Icb_ReceiverChannelData_EncodedSize()
                                                  127,
                                                  1,
                                                  &channelData_p};

    gnssPositionData.receiverChannels = &receiverChannels;

    Icb_OpGNSSPositionData_Response_t gnssPos = {&gnssPositionData};

    //------------------ Encode ---------------------
    ALOGI("Starting encode data!");
    std::vector<uint8_t> payload;

    InfotainmentIpBus::Utils::encodeMessage(
            &gnssPos, Icb_OpGNSSPositionData_Response_Encode, Icb_OpGNSSPositionData_Response_EncodedSize, &payload);

    //------------------ Decode ---------------------
    ALOGI("Starting decode data!");
    ASN_BYTE m_session_buffer_msgd[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_msgd;
    m_session_msgd = ASN_Session_Create(m_session_buffer_msgd, sizeof(m_session_buffer_msgd));

    Icb_OpGNSSPositionData_Response decodedGnssPos = nullptr;

    InfotainmentIpBus::Utils::DecodeMessage(payload,
                                            m_session_msgd,
                                            decodedGnssPos,
                                            Icb_OpGNSSPositionData_Response_Create,
                                            Icb_OpGNSSPositionData_Response_Decode);

    //------------------ Compare ---------------------
    ALOGI("Starting compare data!");

    EXPECT_EQ(decodedGnssPos->gnssPositionData->position->longLat->longitude,
              gnssPos.gnssPositionData->position->longLat->longitude);
    EXPECT_EQ(decodedGnssPos->gnssPositionData->position->longLat->latitude,
              gnssPos.gnssPositionData->position->longLat->latitude);
    EXPECT_EQ(decodedGnssPos->gnssPositionData->position->altitude, gnssPos.gnssPositionData->position->altitude);
    EXPECT_EQ(decodedGnssPos->gnssPositionData->receiverChannels->actual_size,
              gnssPos.gnssPositionData->receiverChannels->actual_size);
    EXPECT_EQ(decodedGnssPos->gnssPositionData->receiverChannels->data_array[0]
                      ->extendedData->data->pseudoRangeCorrectionData,
              gnssPos.gnssPositionData->receiverChannels->data_array[0]->extendedData->data->pseudoRangeCorrectionData);
}

/**
 * Encode/Decode one message.
 * Use session to test complexity
 */
TEST_F(TypeConvHelpersTest, EncodeWithSession) {
    ASN_BYTE m_session_buffer_send[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_send;
    m_session_send = ASN_Session_Create(m_session_buffer_send, sizeof(m_session_buffer_send));

    //------------------ Prepare data for encoding ---------------------
    ALOGI("Starting prepare data!");

    Icb_OpGNSSPositionData_Response gnssPos = Icb_OpGNSSPositionData_Response_Create(m_session_send);

    gnssPos->gnssPositionData->utcTime->year = 2017;
    gnssPos->gnssPositionData->utcTime->month = 10;
    gnssPos->gnssPositionData->utcTime->day = 5;
    gnssPos->gnssPositionData->utcTime->hour = 8;
    gnssPos->gnssPositionData->utcTime->minute = 1;
    gnssPos->gnssPositionData->utcTime->second = 2;

    gnssPos->gnssPositionData->gpsTime->weekNumber = 40;
    gnssPos->gnssPositionData->gpsTime->timeOfWeek = 12345;

    gnssPos->gnssPositionData->position->longLat->longitude = (int)(57.708870 * INT_MAX) / 360;
    gnssPos->gnssPositionData->position->longLat->latitude = (int)(11.974560 * INT_MAX) / 360;
    gnssPos->gnssPositionData->position->altitude = 21;

    gnssPos->gnssPositionData->movement->speed = 99;
    gnssPos->gnssPositionData->movement->horizontalVelocity = 22;
    gnssPos->gnssPositionData->movement->verticalVelocity = (int)(0.5 * 1000);

    gnssPos->gnssPositionData->heading = 234;

    gnssPos->gnssPositionData->gnssStatus->gpsIsUsed = true;
    gnssPos->gnssPositionData->gnssStatus->glonassIsUsed = true;
    gnssPos->gnssPositionData->gnssStatus->galileoIsUsed = true;
    gnssPos->gnssPositionData->gnssStatus->sbasIsUsed = true;
    gnssPos->gnssPositionData->gnssStatus->qzssL1IsUsed = true;
    gnssPos->gnssPositionData->gnssStatus->qzssL1SAIFIsUsed = true;

    gnssPos->gnssPositionData->positioningStatus->fixType = Icb_GnssFixType_t::e_Icb_GnssFixType_fix3D;
    gnssPos->gnssPositionData->positioningStatus->dgpsIsUsed = true;
    gnssPos->gnssPositionData->positioningStatus->selfEphemerisDataUsage = true;

    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesVisible->gps = 11;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesVisible->glonass = 12;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesVisible->galileo = 13;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesVisible->sbas = 14;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesVisible->qzssL1 = 15;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesVisible->qzssL1SAIF = 16;

    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesUsed->gps = 1;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesUsed->glonass = 2;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesUsed->galileo = 3;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesUsed->sbas = 4;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesUsed->qzssL1 = 5;
    gnssPos->gnssPositionData->satelliteInfo->nrOfSatellitesUsed->qzssL1SAIF = 6;

    gnssPos->gnssPositionData->precision->hdop = 123;
    gnssPos->gnssPositionData->precision->vdop = 124;
    gnssPos->gnssPositionData->precision->pdop = 125;
    gnssPos->gnssPositionData->precision->tdop = 126;

    Icb_ReceiverChannelData_SetSize(gnssPos->gnssPositionData->receiverChannels, m_session_send, 1);

    gnssPos->gnssPositionData->receiverChannels->data_array[0]->prn = 1;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->trackingStatus =
            Icb_SatelliteTrackingStatusType_t::e_Icb_SatelliteTrackingStatusType_tracking;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->svacc = 10;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->snr = 20;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->azimuthAngle = 30;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->elevationAngle = 40;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->extendedData->choice =
            Icb_ExtendedChannelData_Choices::e_Icb_ExtendedChannelData_data;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->extendedData->data =
            Icb_ChannelCorrectionData_Create(m_session_send);
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->extendedData->data->pseudoRangeMetres = -1;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->extendedData->data->pseudoRangeMillimetres = 2;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->extendedData->data->rangeRate = -3;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->extendedData->data->pseudoRangeCorrectionData = -3;
    gnssPos->gnssPositionData->receiverChannels->data_array[0]->extendedData->data->selfEphemerisPredictionTime = 5;

    //------------------ Encode ---------------------
    ALOGI("Starting encode data!");
    std::vector<uint8_t> payload;

    InfotainmentIpBus::Utils::encodeMessage(
            gnssPos, Icb_OpGNSSPositionData_Response_Encode, Icb_OpGNSSPositionData_Response_EncodedSize, &payload);

    //------------------ Decode ---------------------
    ALOGI("Starting decode data!");

    ASN_BYTE m_session_buffer_rcv[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_rcv;
    m_session_rcv = ASN_Session_Create(m_session_buffer_rcv, sizeof(m_session_buffer_rcv));

    Icb_OpGNSSPositionData_Response decodedGnssPos = nullptr;

    InfotainmentIpBus::Utils::DecodeMessage(payload,
                                            m_session_rcv,
                                            decodedGnssPos,
                                            Icb_OpGNSSPositionData_Response_Create,
                                            Icb_OpGNSSPositionData_Response_Decode);

    //------------------ Compare ---------------------
    ALOGI("Starting compare data!");

    EXPECT_EQ(decodedGnssPos->gnssPositionData->position->longLat->longitude,
              gnssPos->gnssPositionData->position->longLat->longitude);
    EXPECT_EQ(decodedGnssPos->gnssPositionData->position->longLat->latitude,
              gnssPos->gnssPositionData->position->longLat->latitude);
    EXPECT_EQ(decodedGnssPos->gnssPositionData->position->altitude, gnssPos->gnssPositionData->position->altitude);
    EXPECT_EQ(decodedGnssPos->gnssPositionData->receiverChannels->actual_size,
              gnssPos->gnssPositionData->receiverChannels->actual_size);
    EXPECT_EQ(
            decodedGnssPos->gnssPositionData->receiverChannels->data_array[0]
                    ->extendedData->data->pseudoRangeCorrectionData,
            gnssPos->gnssPositionData->receiverChannels->data_array[0]->extendedData->data->pseudoRangeCorrectionData);
}
