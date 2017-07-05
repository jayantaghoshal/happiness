/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "ipcommandbus/vcc_pdu_header.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/Pdu.h"

namespace Connectivity
{
class VCCPDUHeaderTest : public testing::Test
{
  protected:
    static const std::vector<std::uint8_t> &valid_header_data()
    {
        static const std::vector<std::uint8_t> data{0x00,
                                                    0xa9,
                                                    0x09,
                                                    0x02,
                                                    0x00,
                                                    0x00,
                                                    0x00,
                                                    0x6f,
                                                    0xa9,
                                                    0x02,
                                                    0x05,
                                                    0x00,
                                                    0x02,
                                                    0x05,
                                                    0x00,
                                                    0x00};
        return data;
    }

    static std::vector<std::uint8_t> invalid_header_data_too_short_length()
    {
        static std::vector<std::uint8_t> data{0x00,
                                              0xa9,
                                              0x09,
                                              0x02,
                                              0x00,
                                              0x00,
                                              0x00,
                                              0x05,
                                              0xa9,
                                              0x02,
                                              0x05,
                                              0x00,
                                              0x02,
                                              0x05,
                                              0x00,
                                              0x00};
        return data;
    }

    static std::vector<std::uint8_t> invalid_header_data_too_long_length()
    {
        static std::vector<std::uint8_t> data{0x00,
                                              0xa9,
                                              0x09,
                                              0x02,
                                              0xff,
                                              0xff,
                                              0xff,
                                              0xff,
                                              0xa9,
                                              0x02,
                                              0x05,
                                              0x00,
                                              0x02,
                                              0x05,
                                              0x00,
                                              0x00};
        return data;
    }

    static VCCPDUHeader valid_header()
    {
        VCCPDUHeader header;

        header.service_id = VccIpCmd::ServiceId::Positioning;          //0x00a9;
        header.operation_id = VccIpCmd::OperationId::GNSSPositionData; //0x0902;
        header.length = 0x0000006f;
        header.sender_handle_id = 0xa9020500;
        header.protocol_version = 0x02;
        header.operation_type = VccIpCmd::OperationType::NOTIFICATION; //0x05;
        header.data_type = VccIpCmd::DataType::ENCODED;                //0x00;
        header.process_flag_and_reserved = 0x00;

        return header;
    }
};

TEST_F(VCCPDUHeaderTest, FromData)
{
    VCCPDUHeader header = VCCPDUHeader::from_data(valid_header_data());
    VCCPDUHeader expected_header = valid_header();

    EXPECT_EQ(expected_header.service_id, header.service_id);
    EXPECT_EQ(expected_header.operation_id, header.operation_id);
    EXPECT_EQ(expected_header.length, header.length);
    EXPECT_EQ(expected_header.sender_handle_id, header.sender_handle_id);
    EXPECT_EQ(expected_header.protocol_version, header.protocol_version);
    EXPECT_EQ(expected_header.operation_type, header.operation_type);
    EXPECT_EQ(expected_header.data_type, header.data_type);
    EXPECT_EQ(expected_header.process_flag_and_reserved, header.process_flag_and_reserved);
}

TEST_F(VCCPDUHeaderTest, PduFromDataInvalidHeader)
{
    std::vector<std::uint8_t> v = invalid_header_data_too_short_length();
    bool res = Pdu().fromData(v);
    EXPECT_FALSE(res);

    v = invalid_header_data_too_long_length();
    res = Pdu().fromData(v);
    EXPECT_FALSE(res);
}

TEST_F(VCCPDUHeaderTest, FromDataTooSmall)
{
    for (std::size_t size = 0; size < valid_header_data().size(); size++)
    {
        auto data = valid_header_data();
        data.resize(size);

        EXPECT_EQ(VCCPDUHeader::from_data(data).length, VCCPDUHeader().length) << "Data size: " << size;
    }
}

TEST_F(VCCPDUHeaderTest, ToData)
{
    VCCPDUHeader header = valid_header();
    std::vector<std::uint8_t> data;

    header.to_data(data);
    EXPECT_EQ(valid_header_data(), data);
}

#define EXPECT_ENUM2STR(func, enumName, string) EXPECT_STREQ(func(enumName::string), #string)
#define EXPECT_ENUM2STR_SERVICEID(sid, string) EXPECT_STREQ(VccIpCmd::toString(sid).c_str(), string)
#define EXPECT_ENUM2STR_SERVICEID_OPERATIONID(sid, oid, string) EXPECT_STREQ(VccIpCmd::toString(sid, oid).c_str(), string)

TEST_F(VCCPDUHeaderTest, TestToString)
{
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::Undefined, "Undefined");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::Telematics, "Telematics");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::Connectivity, "Connectivity");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::WLAN, "WLAN");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, "CommonPSIMPhoneConnectivity");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::CommonPhoneTelematics, "CommonPhoneTelematics");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::CommonAll, "CommonAll");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::Positioning, "Positioning");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::DiagnosticManagement, "DiagnosticManagement");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::OTA, "OTA");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::TVM, "TVM");
    EXPECT_ENUM2STR_SERVICEID(VccIpCmd::ServiceId::LM, "LM");

    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Undefined, VccIpCmd::OperationId::Undefined, "Undefined.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::Undefined, "Telematics.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::SVTStatus, "Telematics.SVTStatus");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::TNStatus, "Telematics.TNStatus");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::TelematicSettings, "Telematics.TelematicSettings");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::PositionData, "Telematics.PositionData");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::TextMessage, "Telematics.TextMessage");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::SubscriptionActivation, "Telematics.SubscriptionActivation");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::ServiceActivation, "Telematics.ServiceActivation");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::RescueStatus, "Telematics.RescueStatus");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::IHUSystemInfo, "Telematics.IHUSystemInfo");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::UserPrivacySettings, "Telematics.UserPrivacySettings");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::SendToCar, "Telematics.SendToCar");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::SendToCarConfirmation, "Telematics.SendToCarConfirmation");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::ClimateTimers, "Telematics.ClimateTimers");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::ServiceBooking, "Telematics.ServiceBooking");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::SoHWarning, "Telematics.SoHWarning");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::SoHPacketSend, "Telematics.SoHPacketSend");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::SoHPackageUploaded, "Telematics.SoHPackageUploaded");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::GenericSettingSynch, "Telematics.GenericSettingSynch");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Telematics, VccIpCmd::OperationId::EcallSettings, "Telematics.EcallSettings");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::Undefined, "Connectivity.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMDataCounter, "Connectivity.PSIMDataCounter");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMResetDataCounter, "Connectivity.PSIMResetDataCounter");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMNetworkConfiguration, "Connectivity.PSIMNetworkConfiguration");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMConnect, "Connectivity.PSIMConnect");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMConnectionStatus, "Connectivity.PSIMConnectionStatus");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMRoamingConfiguration, "Connectivity.PSIMRoamingConfiguration");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetGateway, "Connectivity.InternetGateway");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::ConnectivityStatus, "Connectivity.ConnectivityStatus");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::ConnectWorkshopWlan, "Connectivity.ConnectWorkshopWlan");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::WorkshopWlanDetected, "Connectivity.WorkshopWlanDetected");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::AccessRightPinCode, "Connectivity.AccessRightPinCode");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, "Connectivity.FactoryDefaultRestore");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::CurrentInternetSource, "Connectivity.CurrentInternetSource");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetSourceCtrlBT, "Connectivity.InternetSourceCtrlBT");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetSourceCtrlUSB, "Connectivity.InternetSourceCtrlUSB");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMMessage, "Connectivity.PSIMMessage");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::Undefined, "WLAN.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANMode, "WLAN.WLANMode");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANFreqBandSelect, "WLAN.WLANFreqBandSelect");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANChannelFreqInfo, "WLAN.WLANChannelFreqInfo");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPSSID, "WLAN.WLANAPSSID");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPPasswd, "WLAN.WLANAPPasswd");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPConnDev, "WLAN.WLANAPConnDev");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTAConnect, "WLAN.WLANSTAConnect");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTADisconnect, "WLAN.WLANSTADisconnect");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPStatusSettings, "WLAN.WLANAPStatusSettings");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTAStatus, "WLAN.WLANSTAStatus");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTARememberedNetwks, "WLAN.WLANSTARememberedNetwks");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTAForgetRememberedNetwk, "WLAN.WLANSTAForgetRememberedNetwk");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::Undefined, "CommonPSIMPhoneConnectivity.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMSecurity, "CommonPSIMPhoneConnectivity.PSIMSecurity");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMEnterCode, "CommonPSIMPhoneConnectivity.PSIMEnterCode");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMRegisterState, "CommonPSIMPhoneConnectivity.PSIMRegisterState");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMNetworkOperators, "CommonPSIMPhoneConnectivity.PSIMNetworkOperators");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMNetworkOperatorCommand, "CommonPSIMPhoneConnectivity.PSIMNetworkOperatorCommand");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMState, "CommonPSIMPhoneConnectivity.PSIMState");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMPhoneOperationMode, "CommonPSIMPhoneConnectivity.PSIMPhoneOperationMode");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMSignalStrength, "CommonPSIMPhoneConnectivity.PSIMSignalStrength");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMUssd, "CommonPSIMPhoneConnectivity.PSIMUssd");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPhoneTelematics, VccIpCmd::OperationId::Undefined, "CommonPhoneTelematics.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPhoneTelematics, VccIpCmd::OperationId::PremiumAudio, "CommonPhoneTelematics.PremiumAudio");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonPhoneTelematics, VccIpCmd::OperationId::CallHandling, "CommonPhoneTelematics.CallHandling");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonAll, VccIpCmd::OperationId::Undefined, "CommonAll.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonAll, VccIpCmd::OperationId::TEMIdentification, "CommonAll.TEMIdentification");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonAll, VccIpCmd::OperationId::TEMRadioMode, "CommonAll.TEMRadioMode");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonAll, VccIpCmd::OperationId::TunerFrequencies, "CommonAll.TunerFrequencies");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::CommonAll, VccIpCmd::OperationId::DLCConnectedSignal, "CommonAll.DLCConnectedSignal");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Positioning, VccIpCmd::OperationId::Undefined, "Positioning.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Positioning, VccIpCmd::OperationId::DeadReckonedPosition, "Positioning.DeadReckonedPosition");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Positioning, VccIpCmd::OperationId::GNSSPositionData, "Positioning.GNSSPositionData");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::Positioning, VccIpCmd::OperationId::GNSSPositionDataAccuracy, "Positioning.GNSSPositionDataAccuracy");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::Undefined, "DiagnosticManagement.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::RemoteDiagnosticLink, "DiagnosticManagement.RemoteDiagnosticLink");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::RemoteDiagnosticInfo, "DiagnosticManagement.RemoteDiagnosticInfo");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::RequestUserAuthentication, "DiagnosticManagement.RequestUserAuthentication");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::TextFromOperatorToUser, "DiagnosticManagement.TextFromOperatorToUser");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::CurrentJ2534Session, "DiagnosticManagement.CurrentJ2534Session");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::CurrentDoIPState, "DiagnosticManagement.CurrentDoIPState");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::CurrentDoIPConnection, "DiagnosticManagement.CurrentDoIPConnection");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::TextFromUserToOperator, "DiagnosticManagement.TextFromUserToOperator");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::RequestRemoteDiagnostics, "DiagnosticManagement.RequestRemoteDiagnostics");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::AbortRemoteDiagnostics, "DiagnosticManagement.AbortRemoteDiagnostics");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::UpdateDaCMode, "DiagnosticManagement.UpdateDaCMode");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::DiagnosticManagement, VccIpCmd::OperationId::ApprovalStatus, "DiagnosticManagement.ApprovalStatus");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::Undefined, "OTA.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::DownloadStatus, "OTA.DownloadStatus");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::DownloadSoftware, "OTA.DownloadSoftware");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::DeletedSoftware, "OTA.DeletedSoftware");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::DeviceInstall, "OTA.DeviceInstall");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::CancelDownload, "OTA.CancelDownload");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::LocalStorage, "OTA.LocalStorage");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::TVM, VccIpCmd::OperationId::Undefined, "TVM.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::TVM, VccIpCmd::OperationId::TvmVehicleInfo, "TVM.TvmVehicleInfo");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::Undefined, "InfotainmentPlatform.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::DimFullAppScreenUpdate, "InfotainmentPlatform.DimFullAppScreenUpdate");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::DimItemUpdatePartial, "InfotainmentPlatform.DimItemUpdatePartial");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::IHUSteerWheelSwitch, "InfotainmentPlatform.IHUSteerWheelSwitch");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::DIMNotification, "InfotainmentPlatform.DIMNotification");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::DIMCurrentlyActiveSoundArea, "InfotainmentPlatform.DIMCurrentlyActiveSoundArea");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::DIMTbTArea, "InfotainmentPlatform.DIMTbTArea");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::IHUDIMResponse, "InfotainmentPlatform.IHUDIMResponse");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::InfotainmentPlatform, VccIpCmd::OperationId::DIMHitlist, "InfotainmentPlatform.DIMHitlist");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::LM, VccIpCmd::OperationId::Undefined, "LM.Undefined");
    EXPECT_ENUM2STR_SERVICEID_OPERATIONID(VccIpCmd::ServiceId::LM, VccIpCmd::OperationId::IpActivity, "LM.IpActivity");

    // Unknown OperationId returns only ServiceId
    EXPECT_STREQ(VccIpCmd::toString(VccIpCmd::ServiceId::LM, static_cast<VccIpCmd::OperationId>(0xffff)).c_str(), "LM");
    // Unknown ServiceId returns ServiceId=<integer>
    EXPECT_STREQ(VccIpCmd::toString(static_cast<VccIpCmd::ServiceId>(0xfffe), static_cast<VccIpCmd::OperationId>(0xffff)).c_str(), "ServiceId=65534");

    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, REQUEST);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, SETREQUEST_NORETURN);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, SETREQUEST);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, NOTIFICATION_REQUEST);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, RESPONSE);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, NOTIFICATION);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, NOTIFICATION_CYCLIC);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, ACK);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, ERROR);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::OperationType, UNDEFINED);

    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::DataType, ENCODED);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::DataType, NOT_ENCODED);
    EXPECT_ENUM2STR(VccIpCmd::toString, VccIpCmd::DataType, UNDEFINED);
}
} // namespace