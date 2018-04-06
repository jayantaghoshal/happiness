/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <ECD_dataelement.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <log/log.h>
#include <cinttypes>
#include <iostream>

#include "adasisFlexrayConverterImpl.h"

#include <utils/StrongPointer.h>

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>

#include "MockVmsAdasisConverter.h"

#undef LOG_TAG
#define LOG_TAG "adasisFlexrayConv.Tests"

// using namespace ApplicationDataElement;
using ::android::hardware::automotive::vehicle::V2_0::VehiclePropValue;
using ::android::hardware::automotive::vehicle::V2_0::VmsMessageType;
using ::android::hardware::automotive::vehicle::V2_0::vms::createSubscribeMessage;
using ::android::hardware::automotive::vehicle::V2_0::vms::createAvailabilityRequest;
using ::android::hardware::automotive::vehicle::V2_0::vms::createSubscriptionsRequest;

using ::android::hardware::automotive::vehicle::V2_0::vms::isValidVmsMessage;
using ::android::hardware::automotive::vehicle::V2_0::vms::parseMessageType;
using ::android::hardware::automotive::vehicle::V2_0::vms::parseData;
using ::android::hardware::automotive::vehicle::V2_0::vms::VmsLayer;
using ::geo::vms::v1::VmsLayerTypeAndVersion;
using ::geo::vms::v1::VmsPacket;
using ::geo::vms::vms_adasis_converter::CreateVmsAdasisConverter;
using ::geo::vms::vms_adasis_converter::VmsAdasisConverter;
using ::geo::vms::vms_adasis_converter::AdasisMessage;
namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
using namespace android::hardware;
using namespace testing;

class AdasisConverterUT : public Test {
  public:
    AdasisConverterUT() {
        ALOGI("in adasis test constructor");
        vccAdasisConverter = new adasisFlexrayConverterImpl();
        converter = std::make_unique<::geo::vms::vms_adasis_converter::MockVmsAdasisConverter>();
        converter_ptr = converter.get();
        vccAdasisConverter->initUnitTest(std::move(converter));
    }

    static void SetUpTestCase() {}
    ::geo::vms::vms_adasis_converter::VmsAdasisConverter::StatusCode mockGetNextMessageForType(
            ::geo::vms::vms_adasis_converter::AdasisMessageType message_type,
            AdasisMessage* message) {
        ALOGI("getNextMessage called");
        if (message_type == ::geo::vms::vms_adasis_converter::AdasisMessageType::POSITION_TYPE) {
            message->mutable_header()->mutable_counter()->set_count(1);
            message->mutable_header()->mutable_parent_path_index()->set_path_number(4);
            message->mutable_header()->mutable_offset()->set_offset_from_start_meters(0.0000);

        } else if (message_type == ::geo::vms::vms_adasis_converter::AdasisMessageType::SEGMENT_TYPE) {
            message->mutable_header()->mutable_counter()->set_count(2);
            message->mutable_header()->mutable_parent_path_index()->set_path_number(4);
            message->mutable_header()->set_retransmission(true);
        } else if (message_type == ::geo::vms::vms_adasis_converter::AdasisMessageType::STUB_TYPE) {
            message->mutable_header()->mutable_counter()->set_count(3);
            message->mutable_header()->mutable_parent_path_index()->set_path_number(4);
        } else if (message_type == ::geo::vms::vms_adasis_converter::AdasisMessageType::METADATA_TYPE) {
            message->mutable_header()->mutable_counter()->set_count(0);
            message->mutable_header()->mutable_parent_path_index()->set_path_number(4);
        } else if (message_type == ::geo::vms::vms_adasis_converter::AdasisMessageType::PROFILE_LONG_TYPE) {
            message->mutable_header()->mutable_counter()->set_count(1);
            message->mutable_header()->mutable_parent_path_index()->set_path_number(4);
        } else if (message_type == ::geo::vms::vms_adasis_converter::AdasisMessageType::PROFILE_SHORT_TYPE) {
            message->mutable_header()->mutable_counter()->set_count(2);
            message->mutable_header()->mutable_parent_path_index()->set_path_number(4);
        }
        return geo::vms::vms_adasis_converter::VmsAdasisConverter::StatusCode::kSuccess;
    }
    void SetUp() override {
        ON_CALL(*converter_ptr, GetNextMessageForType(_, _))
                .WillByDefault(Invoke(this, &AdasisConverterUT::mockGetNextMessageForType));
    }

    void TearDown() override {}

    ~AdasisConverterUT() override = default;
    adasisFlexrayConverterImpl* vccAdasisConverter;
    std::unique_ptr<::geo::vms::vms_adasis_converter::MockVmsAdasisConverter> converter;
    ::geo::vms::vms_adasis_converter::MockVmsAdasisConverter* converter_ptr;
    // static ::android::sp<MockVmsAdasisConverter> googleService;
};

TEST_F(AdasisConverterUT, sending_position_message) {
    ALOGI("Starting %s", test_info_->name());
    EXPECT_CALL(*converter_ptr,
                GetNextMessageForType(::geo::vms::vms_adasis_converter::AdasisMessageType::POSITION_TYPE, testing::_))
            .Times(1);
    vccAdasisConverter->sendPositionMessage();

    ECDDataElement::DESink<autosar::HznPosn_info> position_sink;
    ALOGI("position counter %d", position_sink.get().value().CycCntr);
    EXPECT_EQ(1, position_sink.get().value().CycCntr);

    ALOGI("position path number %d", position_sink.get().value().PahIdx);
    EXPECT_EQ(4, position_sink.get().value().PahIdx);

    EXPECT_EQ(0.000000, position_sink.get().value().Offs);

    // Pranay: Setter for these Values not availaible in adasis converter to be updated once we have the setter
    /*  ALOGI("Adasis message position index: %d", position_sink.get().value().PosnIdx);
      EXPECT_EQ(6, pposition_sink.get().value().PosnIdx);
              dataForHznPosnSignal.PosnIdx = (uint8_t) message.position().position_index();

              // TODO: As mentioned in the proto; should be followed up what this time means.
              ALOGI("Adasis message time: %lld",
      message.position().time_of_position_calculation().milliseconds_past_unix_epoch());
              dataForHznPosnSignal.PosnTiDif = (double)
      message.position().time_of_position_calculation().milliseconds_past_unix_epoch();

              // Conversion to ADASISv2 format by the VIP.
              ALOGI("Adasis message speed: %f", message.position().speed_meters_per_second());
              dataForHznPosnSignal.Spd = (double) message.position().speed_meters_per_second();

              // Conversion to ADASISv2 format by the VIP.
              ALOGI("Adasis message angle: %d", message.position().relative_heading().angle_degrees());
              dataForHznPosnSignal.RelDir = (double) message.position().relative_heading().angle_degrees();

              // Conversion to ADASISv2 format by the VIP.
              ALOGI("Adasis message position index: %d", message.position().position_index());
              dataForHznPosnSignal.PosnProblty = (double) message.position().probability().percentage();

              // No conversion needed.
              ALOGI("Adasis message percentage confidence: %d", message.position().confidence_rank());
              dataForHznPosnSignal.PosnQly = (uint8_t) message.position().confidence_rank();

              // No conversion needed.
              ALOGI("Adasis message current lane: %d", message.position().current_lane());
              dataForHznPosnSignal.LanePrsnt = (autosar::HznLanePrsnt2) message.position().current_lane();
      */
    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(AdasisConverterUT, sending_segment_message) {
    ALOGI("Starting %s", test_info_->name());
    EXPECT_CALL(*converter_ptr,
                GetNextMessageForType(::geo::vms::vms_adasis_converter::AdasisMessageType::SEGMENT_TYPE, testing::_))
            .Times(1);
    vccAdasisConverter->sendSegmentMessage();

    ECDDataElement::DESink<autosar::HznSeg_info> segment_sink;
    ALOGI("Segment counter %d", segment_sink.get().value().CycCntr);
    EXPECT_EQ(2, segment_sink.get().value().CycCntr);

    ALOGI("Segment path number %d", segment_sink.get().value().PahIdx);
    EXPECT_EQ(4, segment_sink.get().value().PahIdx);

    ALOGI("Segment txprev: %d", (uint8_t)segment_sink.get().value().TxPrev);
    EXPECT_EQ(1, (uint8_t)segment_sink.get().value().TxPrev);

    // ALOGI("Segment offset: %f", segment_sink.get().value().Offs);
    EXPECT_EQ(0.000000, segment_sink.get().value().Offs);

    // No conversion needed.
    // ALOGI("Adasis segmentmessage updatet: %d", message.header().update());
    EXPECT_EQ(0, (uint8_t)segment_sink.get().value().Upd);
    // dataForHznSegSignal.Upd = (autosar::NoYes1) message.header().update();

    EXPECT_EQ(0, segment_sink.get().value().RoadClass);

    // No conversion needed.
    // ALOGI("Adasis segmentmessage typeofway: %d", message.segment().road_attributes().form_of_way());
    //  dataForHznSegSignal.TypOfWay = (autosar::HznTypOfWay1) message.segment().road_attributes().form_of_way();
    EXPECT_EQ(0, (uint8_t)segment_sink.get().value().TypOfWay);

    EXPECT_EQ(0, (uint8_t)segment_sink.get().value().SpdLimEfc);
    EXPECT_EQ(7, (uint8_t)segment_sink.get().value().SpdLimTypEfc);
    // Conversion by Google provided utility function GetNumberOfLanesInDrivingDirection.
    // ALOGI("Adasis segmentmessage NrOfLaneInDrvgDir: %d",
    // message.segment().road_attributes().number_of_lanes_driving_dir());
    // dataForHznSegSignal.NrOfLaneInDrvgDir = (autosar::HznNrOfLaneInDrvgDir1)
    // a_utilities::GetNumberOfLanesInDrivingDirection(message.segment().road_attributes());
    EXPECT_EQ(0, (uint8_t)segment_sink.get().value().NrOfLaneInDrvgDir);
    // Conversion by Google provided utility function GetNumberOfLanesInOppositeDirection.
    // ALOGI("Adasis segmentmessage NrOfLaneInSecDir: %d",
    // message.segment().road_attributes().number_of_lanes_oppo_dir());
    // dataForHznSegSignal.NrOfLaneInSecDir = (autosar::HznNrOfLaneInSecDir1)
    // a_utilities::GetNumberOfLanesInOppositeDirection(message.segment().road_attributes());
    EXPECT_EQ(0, (uint8_t)segment_sink.get().value().NrOfLaneInSecDir);
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
    // ALOGI("Adasis segmentmessage Tnl: %d", message.segment().road_attributes().is_tunnel());
    // dataForHznSegSignal.Tnl = (autosar::HznTnl1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.segment().road_attributes().is_tunnel());
    EXPECT_EQ(3, (uint8_t)segment_sink.get().value().Tnl);
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
    // ALOGI("Adasis segmentmessage Brdg: %d", message.segment().road_attributes().is_bridge());
    // dataForHznSegSignal.Brdg = (autosar::HznBrdg1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.segment().road_attributes().is_bridge());
    EXPECT_EQ(3, (uint8_t)segment_sink.get().value().Brdg);
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
    // ALOGI("Adasis segmentmessage RoadMpl: %d", message.segment().road_attributes().is_divided_road());
    // dataForHznSegSignal.RoadMpl = (autosar::HznRoadMpl1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.segment().road_attributes().is_divided_road());
    EXPECT_EQ(3, (uint8_t)segment_sink.get().value().RoadMpl);
    // Attrribute Built up areas not supported by Google GAS, according to GAS FSD v0.5.
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool
    // ALOGI("Adasis segmentmessage RoadLiAr: %d", message.segment().road_attributes().is_built_up_area());
    // dataForHznSegSignal.RoadLiAr = (autosar::HznRoadLiAr1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.segment().road_attributes().is_built_up_area());
    EXPECT_EQ(3, (uint8_t)segment_sink.get().value().RoadLiAr);
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
    // ALOGI("Adasis segmentmessage IntscnCmplx: %d", message.segment().road_attributes().is_complex_intersection());
    // dataForHznSegSignal.IntscnCmplx = (autosar::HznIntscnCmplx1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.segment().road_attributes().is_complex_intersection());
    EXPECT_EQ(3, (uint8_t)segment_sink.get().value().IntscnCmplx);
    // Conversion to ADASISv2 format by the VIP.
    // ALOGI("Adasis segmentmessage RelProblty: %u", message.segment().probability().percentage());
    // dataForHznSegSignal.RelProblty = (double) message.segment().probability().percentage();
    EXPECT_EQ(0, (uint16_t)segment_sink.get().value().RelProblty);
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
    // ALOGI("Adasis segmentmessage PartOfCourseCalcd: %d",
    // message.segment().road_attributes().is_on_calculated_route());
    // dataForHznSegSignal.PartOfCourseCalcd = (autosar::HznPartOfCourseCalcd1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.segment().road_attributes().is_on_calculated_route());
    EXPECT_EQ(3, (uint8_t)segment_sink.get().value().PartOfCourseCalcd);

    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(AdasisConverterUT, sending_stub_message) {
    ALOGI("Starting %s", test_info_->name());
    EXPECT_CALL(*converter_ptr,
                GetNextMessageForType(::geo::vms::vms_adasis_converter::AdasisMessageType::STUB_TYPE, testing::_))
            .Times(1);
    vccAdasisConverter->sendStubMessage();

    ECDDataElement::DESink<autosar::HznEdge_info> stub_sink;
    ALOGI("Stub counter %d", stub_sink.get().value().CycCntr);
    EXPECT_EQ(3, stub_sink.get().value().CycCntr);

    ALOGI("Stub path number %d", stub_sink.get().value().PahIdx);
    EXPECT_EQ(4, stub_sink.get().value().PahIdx);

    // ALOGI("Adasis StubMessage TxPrev: %d", message.header().retransmission());
    // dataForHznEdgeSignal.TxPrev = (autosar::NoYes1) message.header().retransmission();
    EXPECT_EQ(0, (uint8_t)stub_sink.get().value().TxPrev);
    // No conversion needed.
    // ALOGI("Adasis StubMessage PahIdx: %d", message.header().parent_path_index().path_number());
    // dataForHznEdgeSignal.PahIdx = (uint8_t) message.header().parent_path_index().path_number();

    // No conversion needed.
    // ALOGI("Adasis StubMessage Offs: %f", message.header().offset().offset_from_start_meters());
    // dataForHznEdgeSignal.Offs = (uint16_t) message.header().offset().offset_from_start_meters();
    EXPECT_EQ(0.000000, stub_sink.get().value().Offs);
    // No conversion needed.
    // ALOGI("Adasis StubMessage Upd: %d", message.header().update());
    // dataForHznEdgeSignal.Upd = (autosar::NoYes1) message.header().update();
    EXPECT_EQ(0, (uint8_t)stub_sink.get().value().Upd);
    // No conversion needed.
    // ALOGI("Adasis StubMessage PahIdxNew: %d", message.stub().sub_path_index().path_number());
    // dataForHznEdgeSignal.PahIdxNew = (uint8_t) message.stub().sub_path_index().path_number();
    EXPECT_EQ(0, stub_sink.get().value().PahIdxNew);
    // Conversion to ADASISv2 format by  the VIP.
    // ALOGI("Adasis StubMessage TurnAg: %d", message.stub().turn_angle().angle_degrees());
    // dataForHznEdgeSignal.TurnAg = (double) message.stub().turn_angle().angle_degrees();
    EXPECT_EQ(0, stub_sink.get().value().TurnAg);

    // Conversion to ADASISv2 format by  the VIP.
    //       ALOGI("Adasis StubMessage RelProblty: %u", message.stub().probability().percentage());
    //     dataForHznEdgeSignal.RelProblty = (double) message.stub().probability().percentage();
    EXPECT_EQ(0, stub_sink.get().value().RelProblty);
    // No conversion needed.
    // ALOGI("Adasis StubMessage TypOfWay: %d", message.stub().road_attributes().form_of_way());
    // dataForHznEdgeSignal.TypOfWay = (autosar::HznTypOfWay1) message.stub().road_attributes().form_of_way();
    EXPECT_EQ(0, (uint8_t)stub_sink.get().value().TypOfWay);
    // Conversion by Google provided utility function GetNumberOfLanesInDrivingDirection.
    // ALOGI("Adasis StubMessage NrOfLaneInDrvgDir: %d",
    // message.stub().road_attributes().number_of_lanes_driving_dir());
    // dataForHznEdgeSignal.NrOfLaneInDrvgDir = (autosar::HznNrOfLaneInDrvgDir1)
    // a_utilities::GetNumberOfLanesInDrivingDirection(message.stub().road_attributes());
    EXPECT_EQ(0, (uint8_t)stub_sink.get().value().NrOfLaneInDrvgDir);
    // Conversion by Google provided utility function GetNumberOfLanesInOppositeDirection.
    // ALOGI("Adasis StubMessage NrOfLaneInSecDir: %d", message.stub().road_attributes().number_of_lanes_oppo_dir());
    // dataForHznEdgeSignal.NrOfLaneInSecDir = (autosar::HznNrOfLaneInSecDir1)
    // a_utilities::GetNumberOfLanesInDrivingDirection(message.stub().road_attributes());
    EXPECT_EQ(0, (uint8_t)stub_sink.get().value().NrOfLaneInSecDir);
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
    // ALOGI("Adasis StubMessage IntscnCmplx: %d", message.stub().road_attributes().is_complex_intersection());
    // dataForHznEdgeSignal.IntscnCmplx = (autosar::HznIntscnCmplx1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.stub().road_attributes().is_complex_intersection());
    EXPECT_EQ(3, (uint8_t)stub_sink.get().value().IntscnCmplx);
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool
    // ALOGI("Adasis StubMessage RiOfWay: %d", message.stub().right_of_way());
    // dataForHznEdgeSignal.RiOfWay = (autosar::HznRiOfWay1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.stub().right_of_way());
    EXPECT_EQ(3, (uint8_t)stub_sink.get().value().RiOfWay);
    // Conversion by Google provided utility function GetFunctionalRoadClass.

    // dataForHznEdgeSignal.RoadClass = (uint8_t) a_utilities::GetFunctionalRoadClass(message.stub().road_attributes());
    EXPECT_EQ(0, stub_sink.get().value().RoadClass);
    // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
    // ALOGI("Adasis StubMessage PartOfCourseCalcd: %d", message.stub().road_attributes().is_on_calculated_route());
    // dataForHznEdgeSignal.PartOfCourseCalcd = (autosar::HznPartOfCourseCalcd1)
    // a_utilities::GetBoolOrUnknownFromQualifiedBool(message.stub().road_attributes().is_on_calculated_route());
    EXPECT_EQ(3, (uint8_t)stub_sink.get().value().PartOfCourseCalcd);
    // No conversion needed.
    // ALOGI("Adasis StubMessage LstEdgeAtOffs: %d", message.stub().last_stub_at_offset());
    // dataForHznEdgeSignal.LstEdgeAtOffs = (autosar::NoYes1) message.stub().last_stub_at_offset();
    EXPECT_EQ(0, (uint8_t)stub_sink.get().value().LstEdgeAtOffs);

    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(AdasisConverterUT, sending_Metadata_Message) {
    ALOGI("Starting %s", test_info_->name());
    EXPECT_CALL(*converter_ptr,
                GetNextMessageForType(::geo::vms::vms_adasis_converter::AdasisMessageType::METADATA_TYPE, testing::_))
            .Times(1);
    vccAdasisConverter->sendHorizonMetaData();

    ECDDataElement::DESink<autosar::HznData_info> stub_metadata;
    ALOGI("Metadata counter %d", stub_metadata.get().value().CycCntr);
    EXPECT_EQ(0, stub_metadata.get().value().CycCntr);

    // ALOGI("Adasis metadatamessage CtryCode: %d", message.metadata().country_code());
    // dataForHznDataSignal.CtryCod = (uint32_t) message.metadata().country_code();
    EXPECT_EQ(0, stub_metadata.get().value().CtryCod);
    // Attribute Region code not supported by Google GAS, according to GAS FSD v0.5.
    // ALOGI("Adasis metadatamessage RegnCod: %d", message.metadata().region_code());
    // dataForHznDataSignal.RegnCod = (uint32_t) message.metadata().region_code();
    EXPECT_EQ(0, stub_metadata.get().value().RegnCod);
    // No conversion needed.
    // ALOGI("Adasis metadatamessage DrvgSide: %d", message.metadata().is_right_side_drive());
    // dataForHznDataSignal.DrvgSide = (autosar::DrvgSide1) message.metadata().is_right_side_drive();
    EXPECT_EQ(0, (uint8_t)stub_metadata.get().value().DrvgSide);
    // No conversion needed.
    // ALOGI("Adasis metadatamessage SpdUnit: %d", message.metadata().is_speed_in_mph());
    // dataForHznDataSignal.SpdUnit = (autosar::SpdUnit1) message.metadata().is_speed_in_mph();
    EXPECT_EQ(0, (uint8_t)stub_metadata.get().value().SpdUnit);
    // No conversion needed.
    //  ALOGI("Adasis metadatamessage HdTxVers: %d", message.metadata().protocol_version().major());
    // dataForHznDataSignal.HdTxVers = (uint8_t) message.metadata().protocol_version().major();
    EXPECT_EQ(0, stub_metadata.get().value().HdTxVers);
    // No conversion needed.
    //  ALOGI("Adasis metadatamessage TxVers: %d", message.metadata().protocol_version().minor());
    // dataForHznDataSignal.TxVers = (uint8_t) message.metadata().protocol_version().minor();
    EXPECT_EQ(0, stub_metadata.get().value().TxVers);
    // No conversion needed.
    // ALOGI("Adasis metadatamessage ChdTxVers: %d", message.metadata().protocol_version().patch());
    // dataForHznDataSignal.ChdTxVers = (uint8_t) message.metadata().protocol_version().patch();
    EXPECT_EQ(0, stub_metadata.get().value().ChdTxVers);

    // dataForHznDataSignal.HwVers = (uint16_t) message.metadata().provider_version().major();
    EXPECT_EQ(0, stub_metadata.get().value().HwVers);
    // No conversion needed.
    //   ALOGI("Adasis metadatamessage MapSrc: %d", message.metadata().map_provider());
    // dataForHznDataSignal.MapSrc = (autosar::HznMapSrc2) message.metadata().map_provider();
    EXPECT_EQ(0, (uint8_t)stub_metadata.get().value().MapSrc);
    // Attribute Map version year not supported by Google GAS, according to GAS FSD v0.5.
    //     ALOGI("Adasis metadatamessage YrVersOfMap: %d", message.metadata().map_version_year());
    //   dataForHznDataSignal.YrVersOfMap = (uint8_t) message.metadata().map_version_year();
    EXPECT_EQ(0, stub_metadata.get().value().YrVersOfMap);
    // Attribute Map version quarter not supported by Google GAS, according to GAS FSD v0.5.
    //   ALOGI("Adasis metadatamessage PartOfYrVersOfMap: %d", message.metadata().map_version_quarter());
    // dataForHznDataSignal.PartOfYrVersOfMap = (uint8_t) message.metadata().map_version_quarter();
    // dataForHznDataSignal.PartOfYrVersOfMap = (uint8_t) message.metadata().map_version_quarter();
    EXPECT_EQ(0, stub_metadata.get().value().PartOfYrVersOfMap);

    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(AdasisConverterUT, sending_LongHorizon_message) {
    ALOGI("Starting %s", test_info_->name());
    EXPECT_CALL(
            *converter_ptr,
            GetNextMessageForType(::geo::vms::vms_adasis_converter::AdasisMessageType::PROFILE_LONG_TYPE, testing::_))
            .Times(1);
    vccAdasisConverter->sendLongHorizonMessage();

    ECDDataElement::DESink<autosar::HznProfLong_info> stub_LongEH;
    ALOGI("Long Horizon counter %d", stub_LongEH.get().value().CycCntr);
    EXPECT_EQ(1, stub_LongEH.get().value().CycCntr);

    // ALOGI("Adasis HznProfLong TxPrev: %d", message.header().retransmission());
    //  dataForHznProfLongSignal.TxPrev = (autosar::NoYes1) message.header().retransmission();
    EXPECT_EQ(0, (uint8_t)stub_LongEH.get().value().TxPrev);
    // No conversion needed.
    //   ALOGI("Adasis HznProfLong PahIdx: %d", message.header().parent_path_index().path_number());
    // dataForHznProfLongSignal.PahIdx = (uint8_t) message.header().parent_path_index().path_number();
    EXPECT_EQ(4, stub_LongEH.get().value().PahIdx);
    // No conversion needed.
    //    ALOGI("Adasis HznProfLong Offs: %f", message.header().offset().offset_from_start_meters());
    //  dataForHznProfLongSignal.Offs = (uint16_t) message.header().offset().offset_from_start_meters();
    EXPECT_EQ(0.000000, stub_LongEH.get().value().Offs);
    // No conversion needed.
    // ALOGI("Adasis HznProfLong Upd: %d", message.header().update());
    // dataForHznProfLongSignal.Upd = (autosar::NoYes1) message.header().update();
    // dataForHznProfLongSignal.Upd = (autosar::NoYes1) message.header().update();
    EXPECT_EQ(0, (uint8_t)stub_LongEH.get().value().Upd);
    // No conversion needed.
    // ALOGI("Adasis HznProfLong ProfTyp: %d", message.profile_long().profile_type());
    // dataForHznProfLongSignal.ProfTyp = (autosar::HznProfLongTyp) message.profile_long().profile_type();
    EXPECT_EQ(0, (uint8_t)stub_LongEH.get().value().ProfTyp);
    // No conversion needed.
    //  ALOGI("Adasis HznProfLong NodCtrl: %d", message.profile_long().control_point());
    // dataForHznProfLongSignal.NodCtrl = (autosar::NoYes1) message.profile_long().control_point();
    EXPECT_EQ(0, (uint8_t)stub_LongEH.get().value().NodCtrl);
    ALOGI("Finishing %s", test_info_->name());
}

TEST_F(AdasisConverterUT, sending_ShortHorizon_message) {
    ALOGI("Starting %s", test_info_->name());
    EXPECT_CALL(
            *converter_ptr,
            GetNextMessageForType(::geo::vms::vms_adasis_converter::AdasisMessageType::PROFILE_SHORT_TYPE, testing::_))
            .Times(1);
    vccAdasisConverter->sendShortHorizonMessage();

    ECDDataElement::DESink<autosar::HznProfSho_info> stub_ShortEH;
    ALOGI("Short Horizon counter %d", stub_ShortEH.get().value().CycCntr);
    EXPECT_EQ(2, stub_ShortEH.get().value().CycCntr);

    //  ALOGI("Adasis HznProfSho TxPrev: %d", message.header().retransmission());
    //   dataForHznProfShoSignal.TxPrev = (autosar::NoYes1) message.header().retransmission();
    EXPECT_EQ(0, (uint8_t)stub_ShortEH.get().value().TxPrev);
    // No conversion needed.
    // ALOGI("Adasis HznProfSho PahIdx: %d", message.header().parent_path_index().path_number());
    // dataForHznProfShoSignal.PahIdx = (uint8_t) message.header().parent_path_index().path_number();
    EXPECT_EQ(4, stub_ShortEH.get().value().PahIdx);
    // No conversion needed.
    //  ALOGI("Adasis HznProfSho Offs: %f", message.header().offset().offset_from_start_meters());
    // dataForHznProfShoSignal.Offs = (uint16_t) message.header().offset().offset_from_start_meters();
    EXPECT_EQ(0.000000, stub_ShortEH.get().value().Offs);
    // No conversion needed.
    //  ALOGI("Adasis HznProfSho Upd: %d", message.header().update());
    // dataForHznProfShoSignal.Upd = (autosar::NoYes1) message.header().update();
    EXPECT_EQ(0, (uint8_t)stub_ShortEH.get().value().Upd);
    // No conversion needed.
    //  ALOGI("Adasis HznProfSho ProfTyp: %d", message.profile_short().profile_type());
    // dataForHznProfShoSignal.ProfTyp = (autosar::HznProfShoTyp) message.profile_short().profile_type();
    EXPECT_EQ(0, (uint8_t)stub_ShortEH.get().value().ProfTyp);
    // No conversion needed.
    // ALOGI("Adasis HznProfSho NodCtrl: %d", message.profile_short().control_point());
    // dataForHznProfShoSignal.NodCtrl = (autosar::NoYes1) message.profile_short().control_point();
    EXPECT_EQ(0, (uint8_t)stub_ShortEH.get().value().NodCtrl);

    // ALOGI("Adasis HznProfSho Dst1: %f", message.profile_short().second_profile_distance_meters());
    // dataForHznProfShoSignal.Dst1 = (uint16_t) message.profile_short().second_profile_distance_meters();

    EXPECT_EQ(0.000000, stub_ShortEH.get().value().Dst1);
    ALOGI("Finishing %s", test_info_->name());
}