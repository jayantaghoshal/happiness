/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <log/log.h>
#include <utils/StrongPointer.h>
#include <vhal_v2_0/VehicleHalManager.h>
#include <chrono>
#include <deque>
#include <thread>
#include "AdasisMessageUtilities.h"
#include "Application_dataelement.h"
#include "include/adasisFlexrayConverterImpl.h"

#define LOG_TAG "AdasisDataElementConv"

using namespace ApplicationDataElement;
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
namespace a_utilities = ::geo::vms::vms_adasis_converter::adasis_message_utilities;

adasisFlexrayConverterImpl::adasisFlexrayConverterImpl() {
#ifndef UNIT_TEST
    converter = CreateVmsAdasisConverter();
#endif
    ALOGI("Adasis message: in create converter");
    layersSubscribed = false;
    layerAvailaible = false;
    messageSubscribe = false;
}
adasisFlexrayConverterImpl::~adasisFlexrayConverterImpl() {
    converter->StopVmsAdasisConverter();
}

adasisFlexrayConverter::~adasisFlexrayConverter() = default;

#ifdef UNIT_TEST
void adasisFlexrayConverterImpl::initUnitTest(std::unique_ptr<VmsAdasisConverter> mock_converter) {
    ALOGI("mocked the vms to adasis converter for unit");
    converter = std::move(mock_converter);
    open_result = true;
}
#endif

bool adasisFlexrayConverterImpl::startConverterInstance() {
    open_result = converter->StartVmsAdasisConverter();
    ALOGI("Adasis message: in start converter");
    return open_result;
}

bool adasisFlexrayConverterImpl::getConverterStarted() {
    return open_result;
}

void adasisFlexrayConverterImpl::vmsMessageController(const VehiclePropValue& value) {
    ALOGI("Pranay :Handle VMS property: %d, Vms Message type: %d", isValidVmsMessage(value), parseMessageType(value));
    VmsPacket vms_packet;
    vms_packet.ParseFromString(parseData(value));
    ALOGI("Pranay vms packet: %s", vms_packet.ShortDebugString().c_str());
    if (isValidVmsMessage(value) && parseMessageType(value) == VmsMessageType::DATA) {
        HandleVmsVehicleProperty(value);
    }
}

std::vector<std::unique_ptr<VehiclePropValue>> adasisFlexrayConverterImpl::StartVmsSubscription() {
    std::vector<std::unique_ptr<VehiclePropValue>> subscribe_messages;
    this->startConverterInstance();
    for (const auto& layer : converter->GetLayers()) {
        subscribe_messages.push_back(createSubscribeMessage(VmsLayer(static_cast<int32_t>(layer), 0, 1)));
        ALOGI("layer subscribes: %d", static_cast<int32_t>(layer));
    }
    ALOGI("layer subscribes: %d", subscribe_messages.size());
    return subscribe_messages;
}

void adasisFlexrayConverterImpl::HandleVmsVehicleProperty(const VehiclePropValue& value) {
    ALOGI("Handle VMS property: %d, Vms Message type: %d", isValidVmsMessage(value), parseMessageType(value));
    VmsPacket vms_packet;
    vms_packet.ParseFromString(parseData(value));
    ALOGI("vms packet: %s", vms_packet.ShortDebugString().c_str());
    if (converter != nullptr) {
        AdasisMessage message;
        converter->HandleVmsPacket(vms_packet);
    }
}

void adasisFlexrayConverterImpl::stopConverterInstance() {
    converter->StopVmsAdasisConverter();
}

int sgn(double v) {  // Returns the sign.
    return ((static_cast<uint8_t>(v > 0)) - (static_cast<uint8_t>(v < 0)));
}

// Using proposed encoding scheme for 1/m curvature from ADASIS v2 specification.
float CurvatureEncoder(float c) {
    double value = 0.0;

    if (fabs(c) < 0.00064) {
        value = 511 + round(c * 100000);
    } else if ((fabs(c) >= 0.00064) && (fabs(c) < 0.00192)) {
        value = 511 + round((c * 100000) / 2 + sgn(c) * 32);
    } else if ((fabs(c) >= 0.00192) && (fabs(c) < 0.00448)) {
        value = 511 + round((c * 100000) / 4 + sgn(c) * 80);
    } else if ((fabs(c) >= 0.00448) && (fabs(c) < 0.00960)) {
        value = 511 + round((c * 100000) / 8 + sgn(c) * 136);
    } else if ((fabs(c) >= 0.00960) && (fabs(c) < 0.01984)) {
        value = 511 + round((c * 100000) / 16 + sgn(c) * 196);
    } else if ((fabs(c) >= 0.01984) && (fabs(c) < 0.04032)) {
        value = 511 + round((c * 100000) / 32 + sgn(c) * 258);
    } else if ((fabs(c) >= 0.04032) && (fabs(c) < 0.08128)) {
        value = 511 + round((c * 100000) / 64 + sgn(c) * 321);
    } else if ((fabs(c) >= 0.08128) && (fabs(c) < 0.16192)) {
        value = 511 + round((c * 100000) / 128 + sgn(c) * 384.5);
    } else if (fabs(c) >= 0.16192) {
        value = 511 + sgn(c) * 511;
    } else {
        value = 1023;  // Unknown/Invalid
    }

    return value;
}

bool adasisFlexrayConverterImpl::sendShortHorizonMessage() {
    geo::vms::vms_adasis_converter::AdasisMessage message;
    static ApplicationDataElement::DESender<autosar::HznProfSho_info> hznProfShoSignal;
    autosar::HznProfSho2 dataForHznProfShoSignal;
    if (open_result) {
        if (converter->GetNextMessageForType(geo::vms::vms_adasis_converter::PROFILE_SHORT_TYPE, &message) ==
            geo::vms::vms_adasis_converter::VmsAdasisConverter::kSuccess) {
            // PROFILE SHORT Message
            dataForHznProfShoSignal.MsgTyp = autosar::HznMsgTyp1::ProfSho;

            // No conversion needed.
            ALOGI("Adasis HznProfSho CycCntr: %d", message.header().counter().count());
            dataForHznProfShoSignal.CycCntr = static_cast<uint8_t>(message.header().counter().count());

            // No conversion needed.
            ALOGI("Adasis HznProfSho TxPrev: %d", message.header().retransmission());
            dataForHznProfShoSignal.TxPrev = static_cast<autosar::NoYes1>(message.header().retransmission());

            // No conversion needed.
            ALOGI("Adasis HznProfSho PahIdx: %d", message.header().parent_path_index().path_number());
            dataForHznProfShoSignal.PahIdx = static_cast<uint8_t>(message.header().parent_path_index().path_number());

            // No conversion needed.
            ALOGI("Adasis HznProfSho Offs: %f", message.header().offset().offset_from_start_meters());
            dataForHznProfShoSignal.Offs = static_cast<uint16_t>(message.header().offset().offset_from_start_meters());

            // No conversion needed.
            ALOGI("Adasis HznProfSho Upd: %d", message.header().update());
            dataForHznProfShoSignal.Upd = static_cast<autosar::NoYes1>(message.header().update());

            // No conversion needed.
            ALOGI("Adasis HznProfSho ProfTyp: %d", message.profile_short().profile_type());
            dataForHznProfShoSignal.ProfTyp =
                    static_cast<autosar::HznProfShoTyp>(message.profile_short().profile_type());

            // No conversion needed.
            ALOGI("Adasis HznProfSho NodCtrl: %d", message.profile_short().control_point());
            dataForHznProfShoSignal.NodCtrl = static_cast<autosar::NoYes1>(message.profile_short().control_point());

            // Should use a Google utility function, not yet delivered.
            // ??
            ALOGI("Adasis HznProfSho Val0: %f", message.profile_short().curvature().curvature());
            // dataForHznProfShoSignal.Val0 = (uint16_t) message.profile_short().curvature().curvature();

            // Pending Google utility function.
            //---
            switch (dataForHznProfShoSignal.ProfTyp) {
                case autosar::HznProfShoTyp::Crvt: {
                    dataForHznProfShoSignal.Val0 =
                            static_cast<uint16_t>(CurvatureEncoder(message.profile_short().curvature().curvature()));
                    break;
                }
                default:
                    dataForHznProfShoSignal.Val0 = 1023;  // Unknown/Invalid
                    break;
            }

            // No conversion needed.
            ALOGI("Adasis HznProfSho Dst1: %f", message.profile_short().second_profile_distance_meters());
            dataForHznProfShoSignal.Dst1 =
                    static_cast<uint16_t>(message.profile_short().second_profile_distance_meters());

            //  Should use a Google utility function, not yet delivered.
            // ??
            ALOGI("Adasis HznProfSho Val1: %f", message.profile_short().second_curvature().curvature());
            // dataForHznProfShoSignal.Val1 = (uint16_t) message.profile_short().second_curvature().curvature();

            // Pending Google utility function.
            //---
            if (dataForHznProfShoSignal.Dst1 != 0) {
                switch (dataForHznProfShoSignal.ProfTyp) {
                    case autosar::HznProfShoTyp::Crvt: {
                        dataForHznProfShoSignal.Val1 = static_cast<uint16_t>(
                                CurvatureEncoder(message.profile_short().second_curvature().curvature()));
                        break;
                    }
                    default:
                        dataForHznProfShoSignal.Val1 = 1023;  // Unknown/Invalid
                        break;
                }
            } else {
                dataForHznProfShoSignal.Val1 = 1023;  // Unknown/Invalid
            }
            // No conversion needed.
            ALOGI("Adasis HznProfSho ProfTypQly: %d", message.profile_short().accuracy());
            dataForHznProfShoSignal.ProfTypQly =
                    static_cast<autosar::HznProfTypQly1>(message.profile_short().accuracy());

            // HznShortProfile
            std::cout << "Profile Short Profile Type: " << message.profile_short().profile_type() << std::endl;

            hznProfShoSignal.send(dataForHznProfShoSignal);
        }
    }
    return (open_result);
}

bool adasisFlexrayConverterImpl::sendLongHorizonMessage() {
    // auto converter = vms_adasis_converter::CreateVmsAdasisConverter();
    geo::vms::vms_adasis_converter::AdasisMessage message;
    // bool open_result = converter->StartVmsAdasisConverter();
    static ApplicationDataElement::DESender<autosar::HznProfLong_info> hznProfLongSignal;
    autosar::HznProfLongGroup3 dataForHznProfLongSignal;
    if (open_result) {
        if (converter->GetNextMessageForType(geo::vms::vms_adasis_converter::PROFILE_LONG_TYPE, &message) ==
            geo::vms::vms_adasis_converter::VmsAdasisConverter::kSuccess) {
            // PROFILE LONG Message
            dataForHznProfLongSignal.MsgTyp = autosar::HznMsgTyp1::ProfLong;

            // No conversion needed.
            ALOGI("Adasis HznProfLong CycCntr: %d", message.header().counter().count());
            dataForHznProfLongSignal.CycCntr = static_cast<uint8_t>(message.header().counter().count());

            // No conversion needed.
            ALOGI("Adasis HznProfLong TxPrev: %d", message.header().retransmission());
            dataForHznProfLongSignal.TxPrev = static_cast<autosar::NoYes1>(message.header().retransmission());

            // No conversion needed.
            ALOGI("Adasis HznProfLong PahIdx: %d", message.header().parent_path_index().path_number());
            dataForHznProfLongSignal.PahIdx = static_cast<uint8_t>(message.header().parent_path_index().path_number());

            // No conversion needed.
            ALOGI("Adasis HznProfLong Offs: %f", message.header().offset().offset_from_start_meters());
            dataForHznProfLongSignal.Offs = static_cast<uint16_t>(message.header().offset().offset_from_start_meters());

            // No conversion needed.
            ALOGI("Adasis HznProfLong Upd: %d", message.header().update());
            dataForHznProfLongSignal.Upd = static_cast<autosar::NoYes1>(message.header().update());

            // No conversion needed.
            ALOGI("Adasis HznProfLong ProfTyp: %d", message.profile_long().profile_type());
            dataForHznProfLongSignal.ProfTyp =
                    static_cast<autosar::HznProfLongTyp>(message.profile_long().profile_type());

            // No conversion needed.
            ALOGI("Adasis HznProfLong NodCtrl: %d", message.profile_long().control_point());
            dataForHznProfLongSignal.NodCtrl = static_cast<autosar::NoYes1>(message.profile_long().control_point());

            // The value needs to be read from the specific type of message. For example,
            // message.profile_short().curvature(). I will provide a library function that returns a single value by
            // reading the right type
            // ALOGI("Adasis HznProfLong Val: %d", message.profile_long().curvature().curvature());
            // dataForHznProfLongSignal.Val = (uint32_t) message.profile_long().traffic_sign().default_sign_value();

            // With the adasis.rec test file, only speed limits with profile type 8 is supported, so the speed limit is
            // put into Val.
            // Shall be updated when adasis.rec no longer used.
            //
            // ??
            // dataForHznProfLongSignal.Val = (uint32_t)
            // message.profile_long().traffic_sign().speed_limit_value().speed_limit_value().effective_speed_limit();

            // Pending Google utility function. Supporting only profile no. 8.
            //---
            switch (dataForHznProfLongSignal.ProfTyp) {
                case static_cast<autosar::HznProfLongTyp>(8): {  // Type Traffic Sign currently undefined in SDB (!)
                    dataForHznProfLongSignal.Val =
                            message.profile_long().traffic_sign().sign_type() +
                            255 * a_utilities::GetEffectiveSpeedLimit(message.profile_long()
                                                                              .traffic_sign()
                                                                              .speed_limit_value()
                                                                              .speed_limit_value());
                    break;
                }
                default:
                    dataForHznProfLongSignal.Val = 0xffffffff;  // Unknown/Invalid
                    break;
            }

            hznProfLongSignal.send(dataForHznProfLongSignal);
        }
    }
    return (open_result);
}

bool adasisFlexrayConverterImpl::sendHorizonMetaData() {
    // auto converter = vms_adasis_converter::CreateVmsAdasisConverter();
    geo::vms::vms_adasis_converter::AdasisMessage message;
    // bool open_result = converter->StartVmsAdasisConverter();
    static ApplicationDataElement::DESender<autosar::HznData_info> hznDataSignal;
    autosar::HznDataGroup2 dataForHznDataSignal;
    if (open_result) {
        if (converter->GetNextMessageForType(geo::vms::vms_adasis_converter::METADATA_TYPE, &message) ==
            geo::vms::vms_adasis_converter::VmsAdasisConverter::kSuccess) {
            // META-DATA Message
            dataForHznDataSignal.MsgTyp = autosar::HznMsgTyp1::Data;

            // No conversion needed.
            ALOGI("Adasis metadatamessage count: %d", message.header().counter().count());
            dataForHznDataSignal.CycCntr = static_cast<uint8_t>(message.header().counter().count());

            // No conversion needed.
            ALOGI("Adasis metadatamessage CtryCode: %d", message.metadata().country_code());
            dataForHznDataSignal.CtryCod = static_cast<uint32_t>(message.metadata().country_code());

            // Attribute Region code not supported by Google GAS, according to GAS FSD v0.5.
            ALOGI("Adasis metadatamessage RegnCod: %d", message.metadata().region_code());
            dataForHznDataSignal.RegnCod = static_cast<uint32_t>(message.metadata().region_code());

            // No conversion needed.
            ALOGI("Adasis metadatamessage DrvgSide: %d", message.metadata().is_right_side_drive());
            dataForHznDataSignal.DrvgSide = static_cast<autosar::DrvgSide1>(message.metadata().is_right_side_drive());

            // No conversion needed.
            ALOGI("Adasis metadatamessage SpdUnit: %d", message.metadata().is_speed_in_mph());
            dataForHznDataSignal.SpdUnit = static_cast<autosar::SpdUnit1>(message.metadata().is_speed_in_mph());

            // No conversion needed.
            ALOGI("Adasis metadatamessage HdTxVers: %d", message.metadata().protocol_version().major());
            dataForHznDataSignal.HdTxVers = static_cast<uint8_t>(message.metadata().protocol_version().major());

            // No conversion needed.
            ALOGI("Adasis metadatamessage TxVers: %d", message.metadata().protocol_version().minor());
            dataForHznDataSignal.TxVers = static_cast<uint8_t>(message.metadata().protocol_version().minor());

            // No conversion needed.
            ALOGI("Adasis metadatamessage ChdTxVers: %d", message.metadata().protocol_version().patch());
            dataForHznDataSignal.ChdTxVers = static_cast<uint8_t>(message.metadata().protocol_version().patch());

            //  VCC shall determine how to set the HwVers parameter in terms of variants (HW & SW) of the IHU.
            ALOGI("Adasis metadatamessage HwVers: %d", message.metadata().provider_version().major());
            dataForHznDataSignal.HwVers = static_cast<uint16_t>(message.metadata().provider_version().major());

            // No conversion needed.
            ALOGI("Adasis metadatamessage MapSrc: %d", message.metadata().map_provider());
            dataForHznDataSignal.MapSrc = static_cast<autosar::HznMapSrc2>(message.metadata().map_provider());

            // Attribute Map version year not supported by Google GAS, according to GAS FSD v0.5.
            ALOGI("Adasis metadatamessage YrVersOfMap: %d", message.metadata().map_version_year());
            dataForHznDataSignal.YrVersOfMap = static_cast<uint8_t>(message.metadata().map_version_year());

            // Attribute Map version quarter not supported by Google GAS, according to GAS FSD v0.5.
            ALOGI("Adasis metadatamessage PartOfYrVersOfMap: %d", message.metadata().map_version_quarter());
            dataForHznDataSignal.PartOfYrVersOfMap = static_cast<uint8_t>(message.metadata().map_version_quarter());

            // HznMetaData
            std::cout << "Hzn Meta Data: " << message.metadata().country_code() << std::endl;

            hznDataSignal.send(dataForHznDataSignal);
        }
    }
    return (open_result);
}

bool adasisFlexrayConverterImpl::sendStubMessage() {
    // auto converter = vms_adasis_converter::CreateVmsAdasisConverter();
    geo::vms::vms_adasis_converter::AdasisMessage message;
    // bool open_result = converter->StartVmsAdasisConverter();
    static ApplicationDataElement::DESender<autosar::HznEdge_info> hznEdgeSignal;
    autosar::HznEdgeGroup2 dataForHznEdgeSignal;
    if (open_result) {
        if (converter->GetNextMessageForType(geo::vms::vms_adasis_converter::STUB_TYPE, &message) ==
            geo::vms::vms_adasis_converter::VmsAdasisConverter::kSuccess) {
            // STUB Message
            dataForHznEdgeSignal.MsgTyp = autosar::HznMsgTyp1::Edge;

            // No conversion needed.
            ALOGI("Adasis StubMessage CycCntr: %d", message.header().counter().count());
            dataForHznEdgeSignal.CycCntr = static_cast<uint8_t>(message.header().counter().count());

            // No conversion needed.
            ALOGI("Adasis StubMessage TxPrev: %d", message.header().retransmission());
            dataForHznEdgeSignal.TxPrev = static_cast<autosar::NoYes1>(message.header().retransmission());

            // No conversion needed.
            ALOGI("Adasis StubMessage PahIdx: %d", message.header().parent_path_index().path_number());
            dataForHznEdgeSignal.PahIdx = static_cast<uint8_t>(message.header().parent_path_index().path_number());

            // No conversion needed.
            ALOGI("Adasis StubMessage Offs: %f", message.header().offset().offset_from_start_meters());
            dataForHznEdgeSignal.Offs = static_cast<uint16_t>(message.header().offset().offset_from_start_meters());

            // No conversion needed.
            ALOGI("Adasis StubMessage Upd: %d", message.header().update());
            dataForHznEdgeSignal.Upd = static_cast<autosar::NoYes1>(message.header().update());

            // No conversion needed.
            ALOGI("Adasis StubMessage PahIdxNew: %d", message.stub().sub_path_index().path_number());
            dataForHznEdgeSignal.PahIdxNew = static_cast<uint8_t>(message.stub().sub_path_index().path_number());

            // Conversion to ADASISv2 format by  the VIP.
            ALOGI("Adasis StubMessage TurnAg: %d", message.stub().turn_angle().angle_degrees());
            dataForHznEdgeSignal.TurnAg = static_cast<double>(message.stub().turn_angle().angle_degrees());

            // Conversion to ADASISv2 format by  the VIP.
            ALOGI("Adasis StubMessage RelProblty: %u", message.stub().probability().percentage());
            dataForHznEdgeSignal.RelProblty = static_cast<double>(message.stub().probability().percentage());

            // No conversion needed.
            ALOGI("Adasis StubMessage TypOfWay: %d", message.stub().road_attributes().form_of_way());
            dataForHznEdgeSignal.TypOfWay =
                    static_cast<autosar::HznTypOfWay1>(message.stub().road_attributes().form_of_way());

            // Conversion by Google provided utility function GetNumberOfLanesInDrivingDirection.
            ALOGI("Adasis StubMessage NrOfLaneInDrvgDir: %d",
                  message.stub().road_attributes().number_of_lanes_driving_dir());
            dataForHznEdgeSignal.NrOfLaneInDrvgDir = static_cast<autosar::HznNrOfLaneInDrvgDir1>(
                    a_utilities::GetNumberOfLanesInDrivingDirection(message.stub().road_attributes()));

            // Conversion by Google provided utility function GetNumberOfLanesInOppositeDirection.
            ALOGI("Adasis StubMessage NrOfLaneInSecDir: %d",
                  message.stub().road_attributes().number_of_lanes_oppo_dir());
            dataForHznEdgeSignal.NrOfLaneInSecDir = static_cast<autosar::HznNrOfLaneInSecDir1>(
                    a_utilities::GetNumberOfLanesInDrivingDirection(message.stub().road_attributes()));

            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
            ALOGI("Adasis StubMessage IntscnCmplx: %d", message.stub().road_attributes().is_complex_intersection());
            dataForHznEdgeSignal.IntscnCmplx =
                    static_cast<autosar::HznIntscnCmplx1>(a_utilities::GetBoolOrUnknownFromQualifiedBool(
                            message.stub().road_attributes().is_complex_intersection()));

            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool
            ALOGI("Adasis StubMessage RiOfWay: %d", message.stub().right_of_way());
            dataForHznEdgeSignal.RiOfWay = static_cast<autosar::HznRiOfWay1>(
                    a_utilities::GetBoolOrUnknownFromQualifiedBool(message.stub().right_of_way()));

            // Conversion by Google provided utility function GetFunctionalRoadClass.
            //  Shall be confirmed in Google and Active Safety how we want RoadClass to be defined.
            // ADASIS v2: "Functional Road Class indicates relative importance of the road in the routing network and
            // lower values indicate higher priority. This information is usually map-provider specific. Regular values
            // are 1-6; value 0 is unknown, value 7 indicates N/A. "
            ALOGI("Adasis StubMessage RoadClass: %d", message.stub().road_attributes().road_class());
            dataForHznEdgeSignal.RoadClass =
                    static_cast<uint8_t>(a_utilities::GetFunctionalRoadClass(message.stub().road_attributes()));

            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
            ALOGI("Adasis StubMessage PartOfCourseCalcd: %d",
                  message.stub().road_attributes().is_on_calculated_route());
            dataForHznEdgeSignal.PartOfCourseCalcd =
                    static_cast<autosar::HznPartOfCourseCalcd1>(a_utilities::GetBoolOrUnknownFromQualifiedBool(
                            message.stub().road_attributes().is_on_calculated_route()));

            // No conversion needed.
            ALOGI("Adasis StubMessage LstEdgeAtOffs: %d", message.stub().last_stub_at_offset());
            dataForHznEdgeSignal.LstEdgeAtOffs = static_cast<autosar::NoYes1>(message.stub().last_stub_at_offset());

            // HznStubMessage
            std::cout << "Hzn Stub angle in degrees: " << message.stub().turn_angle().angle_degrees() << std::endl;

            hznEdgeSignal.send(dataForHznEdgeSignal);
        }
    }
    return (open_result);
}

bool adasisFlexrayConverterImpl::sendSegmentMessage() {
    // auto converter = vms_adasis_converter::CreateVmsAdasisConverter();
    geo::vms::vms_adasis_converter::AdasisMessage message;
    // bool open_result = converter->StartVmsAdasisConverter();
    static ApplicationDataElement::DESender<autosar::HznSeg_info> hznSegSignal;
    autosar::HznSegGroup2 dataForHznSegSignal;
    if (open_result) {
        if (converter->GetNextMessageForType(geo::vms::vms_adasis_converter::SEGMENT_TYPE, &message) ==
            geo::vms::vms_adasis_converter::VmsAdasisConverter::kSuccess) {
            // SEGMENT Message
            dataForHznSegSignal.MsgTyp = autosar::HznMsgTyp1::Seg;

            // No conversion needed.
            ALOGI("Adasis segmentmessage count: %d", message.header().counter().count());
            dataForHznSegSignal.CycCntr = static_cast<uint8_t>(message.header().counter().count());

            // No conversion needed.
            ALOGI("Adasis segmentmessage txprev: %d", message.header().retransmission());
            dataForHznSegSignal.TxPrev = static_cast<autosar::NoYes1>(message.header().retransmission());

            // No conversion needed.
            ALOGI("Adasis segmentmessage PahIdx: %d", message.header().parent_path_index().path_number());
            dataForHznSegSignal.PahIdx = static_cast<uint8_t>(message.header().parent_path_index().path_number());

            // No conversion needed.
            ALOGI("Adasis segmentmessage offset: %f", message.header().offset().offset_from_start_meters());
            dataForHznSegSignal.Offs = static_cast<uint16_t>(message.header().offset().offset_from_start_meters());

            // No conversion needed.
            ALOGI("Adasis segmentmessage updatet: %d", message.header().update());
            dataForHznSegSignal.Upd = static_cast<autosar::NoYes1>(message.header().update());

            // Conversion by Google provided utility function GetFunctionalRoadClass.
            // Shall be confirmed in Google and Active Safety how we want RoadClass to be defined.
            // ADASIS v2: "Functional Road Class indicates relative importance of the road in the routing network and
            // lower values indicate higher priority. This information is usually map-provider specific. Regular values
            // are 1-6; value 0 is unknown, value 7 indicates N/A. "
            ALOGI("Adasis segmentmessage RoadClass: %d", message.segment().road_attributes().road_class());
            dataForHznSegSignal.RoadClass =
                    static_cast<uint8_t>(a_utilities::GetFunctionalRoadClass(message.segment().road_attributes()));

            // No conversion needed.
            ALOGI("Adasis segmentmessage typeofway: %d", message.segment().road_attributes().form_of_way());
            dataForHznSegSignal.TypOfWay =
                    static_cast<autosar::HznTypOfWay1>(message.segment().road_attributes().form_of_way());

            // Conversion by Google provided utility function GetEffectiveSpeedLimit.
            // Confirm with Google how to support value "Unlimited" (currently not assigned).
            ALOGI("Adasis segmentmessage SpeedLimitEfc: %d",
                  message.segment().road_attributes().speed_limit().effective_speed_limit());
            dataForHznSegSignal.SpdLimEfc = static_cast<autosar::HznSpdLimEfc1>(
                    a_utilities::GetEffectiveSpeedLimit(message.segment().road_attributes().speed_limit()));

            // Conversion by Google provided utility function GetEffectiveSpeedLimitType.
            // Not all values (yet?) supported by Google.
            ALOGI("Adasis segmentmessage SpeedLimitTypeEfc: %d",
                  message.segment().road_attributes().speed_limit().type());
            dataForHznSegSignal.SpdLimTypEfc = static_cast<autosar::HznSpdLimTypEfc1>(
                    a_utilities::GetEffectiveSpeedLimitType(message.segment().road_attributes().speed_limit()));

            // Conversion by Google provided utility function GetNumberOfLanesInDrivingDirection.
            ALOGI("Adasis segmentmessage NrOfLaneInDrvgDir: %d",
                  message.segment().road_attributes().number_of_lanes_driving_dir());
            dataForHznSegSignal.NrOfLaneInDrvgDir = static_cast<autosar::HznNrOfLaneInDrvgDir1>(
                    a_utilities::GetNumberOfLanesInDrivingDirection(message.segment().road_attributes()));

            // Conversion by Google provided utility function GetNumberOfLanesInOppositeDirection.
            ALOGI("Adasis segmentmessage NrOfLaneInSecDir: %d",
                  message.segment().road_attributes().number_of_lanes_oppo_dir());
            dataForHznSegSignal.NrOfLaneInSecDir = static_cast<autosar::HznNrOfLaneInSecDir1>(
                    a_utilities::GetNumberOfLanesInOppositeDirection(message.segment().road_attributes()));

            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
            ALOGI("Adasis segmentmessage Tnl: %d", message.segment().road_attributes().is_tunnel());
            dataForHznSegSignal.Tnl = static_cast<autosar::HznTnl1>(
                    a_utilities::GetBoolOrUnknownFromQualifiedBool(message.segment().road_attributes().is_tunnel()));

            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
            ALOGI("Adasis segmentmessage Brdg: %d", message.segment().road_attributes().is_bridge());
            dataForHznSegSignal.Brdg = static_cast<autosar::HznBrdg1>(
                    a_utilities::GetBoolOrUnknownFromQualifiedBool(message.segment().road_attributes().is_bridge()));

            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
            ALOGI("Adasis segmentmessage RoadMpl: %d", message.segment().road_attributes().is_divided_road());
            dataForHznSegSignal.RoadMpl =
                    static_cast<autosar::HznRoadMpl1>(a_utilities::GetBoolOrUnknownFromQualifiedBool(
                            message.segment().road_attributes().is_divided_road()));

            // Attrribute Built up areas not supported by Google GAS, according to GAS FSD v0.5.
            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool
            ALOGI("Adasis segmentmessage RoadLiAr: %d", message.segment().road_attributes().is_built_up_area());
            dataForHznSegSignal.RoadLiAr =
                    static_cast<autosar::HznRoadLiAr1>(a_utilities::GetBoolOrUnknownFromQualifiedBool(
                            message.segment().road_attributes().is_built_up_area()));

            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
            ALOGI("Adasis segmentmessage IntscnCmplx: %d",
                  message.segment().road_attributes().is_complex_intersection());
            dataForHznSegSignal.IntscnCmplx =
                    static_cast<autosar::HznIntscnCmplx1>(a_utilities::GetBoolOrUnknownFromQualifiedBool(
                            message.segment().road_attributes().is_complex_intersection()));

            // Conversion to ADASISv2 format by the VIP.
            ALOGI("Adasis segmentmessage RelProblty: %u", message.segment().probability().percentage());
            dataForHznSegSignal.RelProblty = static_cast<double>(message.segment().probability().percentage());

            // Conversion by Google provided utility function GetBoolOrUnknownFromQualifiedBool.
            ALOGI("Adasis segmentmessage PartOfCourseCalcd: %d",
                  message.segment().road_attributes().is_on_calculated_route());
            dataForHznSegSignal.PartOfCourseCalcd =
                    static_cast<autosar::HznPartOfCourseCalcd1>(a_utilities::GetBoolOrUnknownFromQualifiedBool(
                            message.segment().road_attributes().is_on_calculated_route()));

            hznSegSignal.send(dataForHznSegSignal);
        }
    }
    return (open_result);
}

bool adasisFlexrayConverterImpl::sendPositionMessage() {
    geo::vms::vms_adasis_converter::AdasisMessage message;
    static ApplicationDataElement::DESender<autosar::HznPosn_info> hznPosnSignal;
    autosar::HznPosnGroup3 dataForHznPosnSignal;
    ALOGD("Adasis messages being populated with data values to be sent on FlexRay %d", open_result);

    if (open_result) {
        geo::vms::vms_adasis_converter::VmsAdasisConverter::StatusCode t =
                converter->GetNextMessageForType(geo::vms::vms_adasis_converter::POSITION_TYPE, &message);
        ALOGI("Adasis message: %s", message.ShortDebugString().c_str());
        if (t == geo::vms::vms_adasis_converter::VmsAdasisConverter::kSuccess) {
            // POSITION Message
            dataForHznPosnSignal.MsgTyp = autosar::HznMsgTyp1::Posn;

            // No conversion needed.
            ALOGI("Adasis message count: %d", message.header().counter().count());
            dataForHznPosnSignal.CycCntr = static_cast<uint8_t>(message.header().counter().count());

            // No conversion needed.
            ALOGI("Adasis message path index: %d", message.header().parent_path_index().path_number());
            dataForHznPosnSignal.PahIdx = static_cast<uint8_t>(message.header().parent_path_index().path_number());

            // No converstion needed.
            ALOGI("Adasis message offset: %f", message.header().offset().offset_from_start_meters());
            dataForHznPosnSignal.Offs = static_cast<uint16_t>(message.header().offset().offset_from_start_meters());

            // No conversion needed.
            ALOGI("Adasis message position index: %d", message.position().position_index());
            dataForHznPosnSignal.PosnIdx = static_cast<uint8_t>(message.position().position_index());

            //  As mentioned in the proto; should be followed up what this time means.
            ALOGI("Adasis message time: %lld",
                  message.position().time_of_position_calculation().milliseconds_past_unix_epoch());
            dataForHznPosnSignal.PosnTiDif = static_cast<double>(
                    message.position().time_of_position_calculation().milliseconds_past_unix_epoch());

            // Conversion to ADASISv2 format by the VIP.
            ALOGI("Adasis message speed: %f", message.position().speed_meters_per_second());
            dataForHznPosnSignal.Spd = static_cast<double>(message.position().speed_meters_per_second());

            // Conversion to ADASISv2 format by the VIP.
            ALOGI("Adasis message angle: %d", message.position().relative_heading().angle_degrees());
            dataForHznPosnSignal.RelDir = static_cast<double>(message.position().relative_heading().angle_degrees());

            // Conversion to ADASISv2 format by the VIP.
            ALOGI("Adasis message position index: %d", message.position().position_index());
            dataForHznPosnSignal.PosnProblty = static_cast<double>(message.position().probability().percentage());

            // No conversion needed.
            ALOGI("Adasis message percentage confidence: %d", message.position().confidence_rank());
            dataForHznPosnSignal.PosnQly = static_cast<uint8_t>(message.position().confidence_rank());

            // No conversion needed.
            ALOGI("Adasis message current lane: %d", message.position().current_lane());
            dataForHznPosnSignal.LanePrsnt = static_cast<autosar::HznLanePrsnt2>(message.position().current_lane());

            hznPosnSignal.send(dataForHznPosnSignal);

        } else {
            ALOGI("Adasis message No next position message");
        }
    }
    return (open_result);
}
