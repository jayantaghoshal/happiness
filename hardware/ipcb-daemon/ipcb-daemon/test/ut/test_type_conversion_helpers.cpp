/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "util/type_conversion_helpers.h"

namespace gen_conn = v0::org::volvo::connectivity;
namespace v0_climate = v0::org::volvocars::climate;
namespace iis_utils = InfotainmentIpService::Utils;

class TypeConvHelpersFixture : public ::testing::Test
{
public:
    TypeConvHelpersFixture()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

// TODO: Tests for all 'old' the functionality...
//       (Currently there are only tests for wlan functions, thus the low coverage.)

TEST_F(TypeConvHelpersFixture, TestEcuToIpcbType)
{
    EXPECT_EQ(e_Icb_Ecu_none, iis_utils::EcuToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kNone));
    EXPECT_EQ(e_Icb_Ecu_ihu, iis_utils::EcuToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kIhu));
    EXPECT_EQ(e_Icb_Ecu_vcm, iis_utils::EcuToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm));
    EXPECT_EQ(e_Icb_Ecu_tem, iis_utils::EcuToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem));
    EXPECT_EQ(
        e_Icb_Ecu_none,
        iis_utils::EcuToIpcbType(static_cast<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, TestOnOffSettingToIpcbType)
{
    EXPECT_EQ(e_Icb_OnOffSetting_off,
              iis_utils::OnOffSettingToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOff));
    EXPECT_EQ(e_Icb_OnOffSetting_on,
              iis_utils::OnOffSettingToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOn));
    EXPECT_EQ(e_Icb_OnOffSetting_off,
              iis_utils::OnOffSettingToIpcbType(
                  static_cast<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, TestEcuToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kNone, iis_utils::EcuToCapiType(e_Icb_Ecu_none));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kIhu, iis_utils::EcuToCapiType(e_Icb_Ecu_ihu));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm, iis_utils::EcuToCapiType(e_Icb_Ecu_vcm));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem, iis_utils::EcuToCapiType(e_Icb_Ecu_tem));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kNone,
              iis_utils::EcuToCapiType(static_cast<Icb_Ecu_t>(55)));
}

TEST_F(TypeConvHelpersFixture, TestOnOffSettingToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOff,
              iis_utils::OnOffSettingToCapiType(e_Icb_OnOffSetting_off));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOn,
              iis_utils::OnOffSettingToCapiType(e_Icb_OnOffSetting_on));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOff,
              iis_utils::OnOffSettingToCapiType(static_cast<Icb_OnOffSetting_t>(55)));
}

TEST_F(TypeConvHelpersFixture, TestWlanModeToCapiType)
{
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kOff,
              iis_utils::WlanModeToCapiType(e_Icb_WLANMode_off));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kSta,
              iis_utils::WlanModeToCapiType(e_Icb_WLANMode_sta));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kAp,
              iis_utils::WlanModeToCapiType(e_Icb_WLANMode_ap));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kStaWs,
              iis_utils::WlanModeToCapiType(e_Icb_WLANMode_staWS));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kOff,
              iis_utils::WlanModeToCapiType(static_cast<Icb_WLANMode_t>(-1)));
}

TEST_F(TypeConvHelpersFixture, TestWlanConnectionStatusToCapiType)
{
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected,
              iis_utils::WlanConnectionStatusToCapiType(e_Icb_WLANConnectionStatus_disconnected));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kConnected,
              iis_utils::WlanConnectionStatusToCapiType(e_Icb_WLANConnectionStatus_connected));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected,
              iis_utils::WlanConnectionStatusToCapiType(static_cast<Icb_WLANConnectionStatus_t>(100)));
}

TEST_F(TypeConvHelpersFixture, TestWlanArrayOfSsidToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_ArrayOf_SSID ssids = Icb_ArrayOf_SSID_Create(session);
    Icb_ArrayOf_SSID_SetSize(ssids, session, 2);

    char c_str_ssid[] = "testing123";
    ASN_OctetString_AssociateText(ssids->data_array[1], c_str_ssid);

    const std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid> array_ssid =
        iis_utils::WlanArrayOfSsidToCapiType(ssids);
    ASSERT_EQ(array_ssid.size(), static_cast<std::size_t>(2));
    ASSERT_EQ(array_ssid[0].compare(""), 0);
    ASSERT_EQ(array_ssid[1].compare(c_str_ssid), 0);
}

TEST_F(TypeConvHelpersFixture, TestWlanSecurityTypeToCapiType)
{
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kOpen,
              iis_utils::WlanSecurityTypeToCapiType(e_Icb_WLANSecurityType_open));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWepOpen,
              iis_utils::WlanSecurityTypeToCapiType(e_Icb_WLANSecurityType_wEP_open));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWepSharedKey,
              iis_utils::WlanSecurityTypeToCapiType(e_Icb_WLANSecurityType_wEP_SharedKey));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpaEapTls,
              iis_utils::WlanSecurityTypeToCapiType(e_Icb_WLANSecurityType_wPA_EAP_TLS));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpaPsk,
              iis_utils::WlanSecurityTypeToCapiType(e_Icb_WLANSecurityType_wPA_PSK));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpa2Psk,
              iis_utils::WlanSecurityTypeToCapiType(e_Icb_WLANSecurityType_wPA2_PSK));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpa2EapTls,
              iis_utils::WlanSecurityTypeToCapiType(e_Icb_WLANSecurityType_wPA2_EAP_TLS));
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kOpen,
              iis_utils::WlanSecurityTypeToCapiType(static_cast<Icb_WLANConnectionStatus_t>(-1)));
}

TEST_F(TypeConvHelpersFixture, WlanChannelWidthDataTypeToCapiType)
{
    EXPECT_EQ(gen_conn::Wlan::WlanChannelWidthData::Literal::Width5MHz,
              iis_utils::WlanChannelWidthDataTypeToCapiType(e_Icb_WLANChannelWidthData_width_5MHz));
    EXPECT_EQ(gen_conn::Wlan::WlanChannelWidthData::Literal::Width10MHz,
              iis_utils::WlanChannelWidthDataTypeToCapiType(e_Icb_WLANChannelWidthData_width_10MHz));
    EXPECT_EQ(gen_conn::Wlan::WlanChannelWidthData::Literal::Width20MHz,
              iis_utils::WlanChannelWidthDataTypeToCapiType(e_Icb_WLANChannelWidthData_width_20MHz));
}

TEST_F(TypeConvHelpersFixture, TestWlanArrayOfNetworkInfoToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_ArrayOf_AvailableNetworkInformation network_info_list = Icb_ArrayOf_AvailableNetworkInformation_Create(session);
    Icb_ArrayOf_AvailableNetworkInformation_SetSize(network_info_list, session, 2);

    char ssid1[] = "ssid1";
    ASN_OctetString_AssociateText(network_info_list->data_array[1]->sSID, ssid1);
    network_info_list->data_array[1]->signalStrength = 313;
    network_info_list->data_array[1]->securityType = e_Icb_WLANSecurityType_wPA2_PSK;
    network_info_list->data_array[1]->rememberedSSID = true;

    std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation> array_network_info =
        iis_utils::WlanArrayOfNetworkInfoToCapiType(network_info_list);

    ASSERT_EQ(static_cast<std::size_t>(2), array_network_info.size());
    ASSERT_EQ(ssid1, array_network_info[1].getSsid());
    ASSERT_EQ(static_cast<uint32_t>(313), array_network_info[1].getSignalStrength());
    ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpa2Psk,
              array_network_info[1].getSecurityType());
    ASSERT_EQ(true, array_network_info[1].getRememberedSsid());
}

TEST_F(TypeConvHelpersFixture, TestInternetSourceToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kNone,
              iis_utils::InternetSourceToCapiType(Icb_InternetSource_t::e_Icb_InternetSource_none));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kBluetooth,
              iis_utils::InternetSourceToCapiType(Icb_InternetSource_t::e_Icb_InternetSource_bluetooth));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kPSim,
              iis_utils::InternetSourceToCapiType(Icb_InternetSource_t::e_Icb_InternetSource_psim));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kUsb,
              iis_utils::InternetSourceToCapiType(Icb_InternetSource_t::e_Icb_InternetSource_usb));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kNone,
              iis_utils::InternetSourceToCapiType(static_cast<Icb_InternetSource_t>(5)));
}

TEST_F(TypeConvHelpersFixture, TestInternetSourceToIpcbType)
{
    EXPECT_EQ(
        e_Icb_InternetSource_none,
        iis_utils::InternetSourceToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kNone));
    EXPECT_EQ(
        e_Icb_InternetSource_psim,
        iis_utils::InternetSourceToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kPSim));
    EXPECT_EQ(e_Icb_InternetSource_bluetooth,
              iis_utils::InternetSourceToIpcbType(
                  gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kBluetooth));
    EXPECT_EQ(
        e_Icb_InternetSource_usb,
        iis_utils::InternetSourceToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kUsb));
    EXPECT_EQ(
        e_Icb_InternetSource_none,
        iis_utils::InternetSourceToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kError));
}

#define TestGenericErrorTypeToCapiType_MACRO(expected, input_error_type)                                          \
    EXPECT_EQ(expected, iis_utils::GenericErrorTypeToCapiType(input_error_type, {true, 0, 0}));                   \
    EXPECT_EQ(expected, iis_utils::GenericErrorTypeToCapiType(input_error_type, {false, 0, 0}));                  \
    EXPECT_EQ(expected, iis_utils::GenericErrorTypeToCapiType(input_error_type, {true, 0xffffffff, 0}));          \
    EXPECT_EQ(expected, iis_utils::GenericErrorTypeToCapiType(input_error_type, {false, 0xffffffff, 0}));         \
    EXPECT_EQ(expected, iis_utils::GenericErrorTypeToCapiType(input_error_type, {true, 0, 0xffffffff}));          \
    EXPECT_EQ(expected, iis_utils::GenericErrorTypeToCapiType(input_error_type, {false, 0, 0xffffffff}));         \
    EXPECT_EQ(expected, iis_utils::GenericErrorTypeToCapiType(input_error_type, {true, 0xffffffff, 0xffffffff})); \
    EXPECT_EQ(expected, iis_utils::GenericErrorTypeToCapiType(input_error_type, {false, 0xffffffff, 0xffffffff}));

TEST_F(TypeConvHelpersFixture, TestGenericErrorTypeToCapiType)
{
    TestGenericErrorTypeToCapiType_MACRO(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK,
                                         Connectivity::TransportServices::ErrorType::OK);

    TestGenericErrorTypeToCapiType_MACRO(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR,
                                         Connectivity::TransportServices::ErrorType::REMOTE_ERROR);

    TestGenericErrorTypeToCapiType_MACRO(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::TIMEOUT,
                                         Connectivity::TransportServices::ErrorType::LOCAL_TIMEOUT);

    TestGenericErrorTypeToCapiType_MACRO(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR,
                                         Connectivity::TransportServices::ErrorType::PAYLOAD_DECODE_ERROR);

    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR,
              iis_utils::GenericErrorTypeToCapiType(static_cast<Connectivity::TransportServices::ErrorType>(55),
                                                    {false, 0, 0}));
}

TEST_F(TypeConvHelpersFixture, TestWlanModeToIpcbType)
{
    EXPECT_EQ(iis_utils::WlanModeToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kOff),
              e_Icb_WLANMode_off);
    EXPECT_EQ(iis_utils::WlanModeToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kSta),
              e_Icb_WLANMode_sta);
    EXPECT_EQ(iis_utils::WlanModeToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kAp),
              e_Icb_WLANMode_ap);
    EXPECT_EQ(iis_utils::WlanModeToIpcbType(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kStaWs),
              e_Icb_WLANMode_staWS);
    EXPECT_EQ(iis_utils::WlanModeToIpcbType(
                  static_cast<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::Literal>(100)),
              e_Icb_WLANMode_off);
}

TEST_F(TypeConvHelpersFixture, TestFixTypeToString)
{
    EXPECT_EQ(std::string("Not available"), iis_utils::FixTypeToString(e_Icb_GnssFixType_notAvailable));
    EXPECT_EQ(std::string("No fix"), iis_utils::FixTypeToString(e_Icb_GnssFixType_noFix));
    EXPECT_EQ(std::string("Fix 2D"), iis_utils::FixTypeToString(e_Icb_GnssFixType_fix2D));
    EXPECT_EQ(std::string("Fix 3D"), iis_utils::FixTypeToString(e_Icb_GnssFixType_fix3D));
    EXPECT_EQ(std::string("Startup mode"), iis_utils::FixTypeToString(e_Icb_GnssFixType_startupMode));
    EXPECT_EQ(std::string("No fix"), iis_utils::FixTypeToString(static_cast<Icb_GnssFixType_t>(55)));
}

TEST_F(TypeConvHelpersFixture, TestFixedPoint32ToDegreesD)
{
    EXPECT_GT(iis_utils::FixedPoint32ToDegreesD(INT_MAX), 179.9999999);
    EXPECT_LT(iis_utils::FixedPoint32ToDegreesD(INT_MAX), 180.0);
    EXPECT_EQ(iis_utils::FixedPoint32ToDegreesD(0), 0);
}

TEST_F(TypeConvHelpersFixture, TestFixStatusToIpcbType)
{
    EXPECT_EQ(Icb_GnssFixType_t::e_Icb_GnssFixType_noFix, iis_utils::FixStatusToIpcbType(0x00));
    EXPECT_EQ(Icb_GnssFixType_t::e_Icb_GnssFixType_noFix, iis_utils::FixStatusToIpcbType(0x01));
    EXPECT_EQ(Icb_GnssFixType_t::e_Icb_GnssFixType_fix2D, iis_utils::FixStatusToIpcbType(0x02));
    EXPECT_EQ(Icb_GnssFixType_t::e_Icb_GnssFixType_fix3D, iis_utils::FixStatusToIpcbType(0x03));
    EXPECT_EQ(Icb_GnssFixType_t::e_Icb_GnssFixType_noFix, iis_utils::FixStatusToIpcbType(0x04));
}

TEST_F(TypeConvHelpersFixture, TestAsnErrCodeToString)
{
    EXPECT_STREQ("No error", iis_utils::AsnErrCodeToString(ASN_ERROR_NO_ERROR).c_str());
    EXPECT_STREQ("Memory", iis_utils::AsnErrCodeToString(ASN_ERROR_MEMORY).c_str());
    EXPECT_STREQ("Buffer full", iis_utils::AsnErrCodeToString(ASN_ERROR_BUFFER_FULL).c_str());
    EXPECT_STREQ("Buffer starving", iis_utils::AsnErrCodeToString(ASN_ERROR_BUFFER_STARVING).c_str());
    EXPECT_STREQ("No valid choice", iis_utils::AsnErrCodeToString(ASN_ERROR_NO_VALID_CHOICE).c_str());
    EXPECT_STREQ("Parameter value out of range (of ASN spec. Bad param from CommonAPI?)",
                 iis_utils::AsnErrCodeToString(ASN_ERROR_VALUE_NOT_WITHIN_RANGE).c_str());
    EXPECT_STREQ("Size not supported", iis_utils::AsnErrCodeToString(ASN_ERROR_SIZE_NOT_SUPPORTED).c_str());
    EXPECT_STREQ("Unknown error code", iis_utils::AsnErrCodeToString(0xff).c_str());
}

TEST_F(TypeConvHelpersFixture, TestWlanConnectStatusToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected,
              iis_utils::WlanConnectStatusToCapiType(Icb_WLANConnectStatus_t::e_Icb_WLANConnectStatus_disconnected));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kConnected,
              iis_utils::WlanConnectStatusToCapiType(Icb_WLANConnectStatus_t::e_Icb_WLANConnectStatus_connected));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kWrongPassword,
              iis_utils::WlanConnectStatusToCapiType(Icb_WLANConnectStatus_t::e_Icb_WLANConnectStatus_wrongPassword));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected,
              iis_utils::WlanConnectStatusToCapiType(static_cast<Icb_WLANConnectionStatus_t>(10)));
}

TEST_F(TypeConvHelpersFixture, TestOTAGenericOkStatusToCapiType)
{
    EXPECT_EQ(gen_conn::OTATypes::GenericOkStatus::unknown,
              iis_utils::OTAGenericOkStatusToCapiType(Icb_GenericOkStatus_t::e_Icb_GenericOkStatus_unknown));
    EXPECT_EQ(gen_conn::OTATypes::GenericOkStatus::ok,
              iis_utils::OTAGenericOkStatusToCapiType(Icb_GenericOkStatus_t::e_Icb_GenericOkStatus_ok));
    EXPECT_EQ(gen_conn::OTATypes::GenericOkStatus::notOk,
              iis_utils::OTAGenericOkStatusToCapiType(Icb_GenericOkStatus_t::e_Icb_GenericOkStatus_notOk));
    EXPECT_EQ(gen_conn::OTATypes::GenericOkStatus::unknown,
              iis_utils::OTAGenericOkStatusToCapiType(static_cast<Icb_GenericOkStatus_t>(10)));
}

TEST_F(TypeConvHelpersFixture, TestOTADownloadStatusToCapiType)
{
    EXPECT_EQ(gen_conn::OTATypes::DownloadStatus::idle,
              iis_utils::OTADownloadStatusToCapiType(Icb_DownloadStatus_t::e_Icb_DownloadStatus_idle));
    EXPECT_EQ(gen_conn::OTATypes::DownloadStatus::active,
              iis_utils::OTADownloadStatusToCapiType(Icb_DownloadStatus_t::e_Icb_DownloadStatus_active));
    EXPECT_EQ(gen_conn::OTATypes::DownloadStatus::noService,
              iis_utils::OTADownloadStatusToCapiType(Icb_DownloadStatus_t::e_Icb_DownloadStatus_noService));
    EXPECT_EQ(gen_conn::OTATypes::DownloadStatus::noService,
              iis_utils::OTADownloadStatusToCapiType(static_cast<Icb_DownloadStatus_t>(10)));
}

TEST_F(TypeConvHelpersFixture, TestOTADownloadStateToCapiType)
{
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::queued,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_queued));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::requestDownloadInfo,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_requestDownloadInfo));
    EXPECT_EQ(
        gen_conn::OTATypes::DownloadState::requestDownloadInfo_Halted,
        iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_requestDownloadInfo_Halted));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::validatingConfig,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_validatingConfig));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::configValidated,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_configValidated));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::downloading,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_downloading));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::downloading_Halted,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_downloading_Halted));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::downloaded,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_downloaded));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::installing,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_installing));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::installed_Waitreset,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_installed_Waitreset));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::installed,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_installed));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorNoStorage,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_errorNoStorage));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorSoftwareNotOk,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_errorSoftwareNotOk));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorDownloadFileFailed,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_errorDownloadFileFailed));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorTimeToLiveExceeded,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_errorTimeToLiveExceeded));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorValidationFailed,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_errorValidationFailed));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorInstallationFailed,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_errorInstallationFailed));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorInstallationFailedMemoryFull,
              iis_utils::OTADownloadStateToCapiType(
                  Icb_DownloadState_t::e_Icb_DownloadState_errorInstallationFailedMemoryFull));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorNoDiagResponse,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_errorNoDiagResponse));
    EXPECT_EQ(
        gen_conn::OTATypes::DownloadState::errorDownloadInfoRequestFailed,
        iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_errorDownloadInfoRequestFailed));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::cancelling,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_cancelling));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::cancelled,
              iis_utils::OTADownloadStateToCapiType(Icb_DownloadState_t::e_Icb_DownloadState_cancelled));
    EXPECT_EQ(gen_conn::OTATypes::DownloadState::errorDownloadFileFailed,
              iis_utils::OTADownloadStateToCapiType(static_cast<Icb_DownloadState_t>(100)));
}

TEST_F(TypeConvHelpersFixture, TestInstallCommandToCapiType)
{
    EXPECT_EQ(gen_conn::OTATypes::InstallCommand::install,
              iis_utils::InstallCommandToCapiType(Icb_InstallCommand_t::e_Icb_InstallCommand_install));
    EXPECT_EQ(gen_conn::OTATypes::InstallCommand::useSoftware,
              iis_utils::InstallCommandToCapiType(Icb_InstallCommand_t::e_Icb_InstallCommand_useSoftware));
    EXPECT_EQ(gen_conn::OTATypes::InstallCommand::cancelInstall,
              iis_utils::InstallCommandToCapiType(Icb_InstallCommand_t::e_Icb_InstallCommand_cancelInstall));
    EXPECT_EQ(gen_conn::OTATypes::InstallCommand::cancelInstall,
              iis_utils::InstallCommandToCapiType(static_cast<Icb_InstallCommand_t>(100)));
}

TEST_F(TypeConvHelpersFixture, TestInstallStatusToIpcbType)
{
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_idle,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::idle));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_installing,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::installing));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_installationComplete,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::installationComplete));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_installationPendingCmd,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::installationPendingCmd));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_cancelled,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::cancelled));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_failed,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::failed));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_failedMemoryFull,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::failedMemoryFull));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_installationWaitReboot,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::installationWaitReboot));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_failedValidation,
              iis_utils::InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus::failedValidation));
    EXPECT_EQ(Icb_InstallStatus_t::e_Icb_InstallStatus_idle,
              iis_utils::InstallStatusToIpcbType(static_cast<gen_conn::OTATypes::InstallStatus::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, TestsbWarningToIpcbType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_SBWarning icb_sb_warning;
    gen_conn::IpCommandBusTypes::SBWarning sb_warning;

    sb_warning.setId(1);
    sb_warning.setMessage("Kyiv");

    icb_sb_warning = iis_utils::sbWarningToIpcbType(session, sb_warning);

    ASSERT_EQ(icb_sb_warning->id, static_cast<uint32_t>(1));
    ASSERT_EQ(iis_utils::AsnUtf8StringToCapiType(icb_sb_warning->message).compare("Kyiv"), 0);
}

TEST_F(TypeConvHelpersFixture, TestArrayOfsbWarningToIpcbType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_ArrayOf_SBWarning icb_warnings = Icb_ArrayOf_SBWarning_Create(session);

    Icb_ArrayOf_SBWarning_SetSize(icb_warnings, session, 2);

    gen_conn::IpCommandBusTypes::SBWarning sb_warning1;
    gen_conn::IpCommandBusTypes::SBWarning sb_warning2;
    std::vector<gen_conn::IpCommandBusTypes::SBWarning> sb_warnings;

    sb_warning1.setId(0);
    sb_warning1.setMessage("Lviv");

    sb_warning2.setId(1);
    sb_warning2.setMessage("Odesa");

    sb_warnings.push_back(sb_warning1);
    sb_warnings.push_back(sb_warning2);

    icb_warnings = iis_utils::ArrayOfsbWarningToIpcbType(session, sb_warnings);

    ASSERT_EQ(icb_warnings->actual_size, static_cast<std::size_t>(2));

    ASSERT_EQ(icb_warnings->data_array[0]->id, static_cast<uint32_t>(0));
    ASSERT_EQ(iis_utils::AsnUtf8StringToCapiType((icb_warnings->data_array[0]->message)).compare("Lviv"), 0);

    ASSERT_EQ(icb_warnings->data_array[1]->id, static_cast<uint32_t>(1));
    ASSERT_EQ(iis_utils::AsnUtf8StringToCapiType((icb_warnings->data_array[1]->message)).compare("Odesa"), 0);
}

TEST_F(TypeConvHelpersFixture, TestOTASoftwareProductToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_UUID icb_uuid = Icb_UUID_Create(session);
    icb_uuid->uuid = (ASN_UTF8String) "Hello";

    Icb_SoftwareProduct icb_software_product = Icb_SoftwareProduct_Create(session);
    icb_software_product->id = icb_uuid;
    icb_software_product->downloadState = Icb_DownloadState_t::e_Icb_DownloadState_cancelled;
    icb_software_product->progress = 0;

    gen_conn::OTATypes::SoftwareProduct software_product =
        iis_utils::OTASoftwareProductToCapiType(icb_software_product);

    EXPECT_EQ(software_product.getId(), iis_utils::AsnUtf8StringToCapiType(icb_software_product->id));
    EXPECT_EQ(software_product.getDownloadState(), gen_conn::OTATypes::DownloadState::cancelled);
    EXPECT_TRUE(0 == software_product.getDownloadProgress());
}

TEST_F(TypeConvHelpersFixture, TestOTAArrayOfSoftwareProductToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_ArrayOf_SoftwareProduct icb_software_products = Icb_ArrayOf_SoftwareProduct_Create(session);
    Icb_ArrayOf_SoftwareProduct_SetSize(icb_software_products, session, 2);

    Icb_UUID icb_uuid_1 = Icb_UUID_Create(session);
    icb_uuid_1->uuid = (ASN_UTF8String) "Hello";
    Icb_UUID icb_uuid_2 = Icb_UUID_Create(session);
    icb_uuid_2->uuid = (ASN_UTF8String) "World";

    Icb_SoftwareProduct icb_software_product_1 = Icb_SoftwareProduct_Create(session);
    icb_software_product_1->id = icb_uuid_1;
    Icb_SoftwareProduct icb_software_product_2 = Icb_SoftwareProduct_Create(session);
    icb_software_product_2->id = icb_uuid_2;

    icb_software_products->data_array[0] = icb_software_product_1;
    icb_software_products->data_array[1] = icb_software_product_2;
    std::vector<gen_conn::OTATypes::SoftwareProduct> software_products =
        iis_utils::OTAArrayOfSoftwareProductToCapiType(icb_software_products);

    EXPECT_EQ(software_products[0].getId(),
              iis_utils::OTASoftwareProductToCapiType(icb_software_products->data_array[0]).getId());
    EXPECT_EQ(software_products[1].getId(),
              iis_utils::OTASoftwareProductToCapiType(icb_software_products->data_array[1]).getId());
}

TEST_F(TypeConvHelpersFixture, TestOTADownloadSoftwareToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_UUID icb_uuid = Icb_UUID_Create(session);
    icb_uuid->uuid = (ASN_UTF8String) "Hello";

    gen_conn::OTATypes::UUID uuid = iis_utils::OTADownloadSoftwareToCapiType(icb_uuid);

    EXPECT_EQ(uuid, iis_utils::AsnUtf8StringToCapiType(icb_uuid));
}

TEST_F(TypeConvHelpersFixture, TestOTAArrayOfDownloadSoftwareToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_ArrayOf_UUID icb_uuids = Icb_ArrayOf_UUID_Create(session);
    Icb_ArrayOf_UUID_SetSize(icb_uuids, session, 2);

    Icb_UUID icb_uuid_1 = Icb_UUID_Create(session);
    icb_uuid_1->uuid = (ASN_UTF8String) "Hello";
    Icb_UUID icb_uuid_2 = Icb_UUID_Create(session);
    icb_uuid_2->uuid = (ASN_UTF8String) "World";

    icb_uuids->data_array[0] = icb_uuid_1;
    icb_uuids->data_array[0] = icb_uuid_2;

    std::vector<gen_conn::OTATypes::UUID> uuids = iis_utils::OTAArrayOfDownloadSoftwareToCapiType(icb_uuids);

    EXPECT_EQ(uuids[0].getUuid(), iis_utils::OTADownloadSoftwareToCapiType(icb_uuids->data_array[0]).getUuid());
    EXPECT_EQ(uuids[1].getUuid(), iis_utils::OTADownloadSoftwareToCapiType(icb_uuids->data_array[1]).getUuid());
}

TEST_F(TypeConvHelpersFixture, TestOTAUuidToIpcbType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    gen_conn::OTATypes::UUID uuid;
    uuid.setUuid("Hello");

    Icb_UUID icb_uuid = iis_utils::OTAUuidToIpcbType(session, uuid);

    EXPECT_EQ(uuid.getUuid(), iis_utils::AsnUtf8StringToCapiType(icb_uuid->uuid));
}

TEST_F(TypeConvHelpersFixture, TestOTAArrayOfDeletedSoftwareToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_ArrayOf_PartIdentifier icb_part_ids = Icb_ArrayOf_PartIdentifier_Create(session);
    Icb_ArrayOf_PartIdentifier_SetSize(icb_part_ids, session, 2);

    Icb_PartIdentifier icb_part_id_1 = Icb_PartIdentifier_Create(session);
    icb_part_id_1->partID = (ASN_UTF8String) "Hello";

    Icb_PartIdentifier icb_part_id_2 = Icb_PartIdentifier_Create(session);
    icb_part_id_2->partID = (ASN_UTF8String) "World";

    icb_part_ids->data_array[0] = icb_part_id_1;
    icb_part_ids->data_array[1] = icb_part_id_2;

    std::vector<gen_conn::OTATypes::PartIdentifier> part_ids =
        iis_utils::OTAArrayOfDeletedSoftwareToCapiType(icb_part_ids);

    EXPECT_EQ(part_ids[0].getPartID(), iis_utils::AsnUtf8StringToCapiType(icb_part_id_1->partID));
    EXPECT_EQ(part_ids[1].getPartID(), iis_utils::AsnUtf8StringToCapiType(icb_part_id_2->partID));
}

TEST_F(TypeConvHelpersFixture, TestIpCommandBusTEMActivationStatusToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kDeactivated,
              iis_utils::IpCommandBusTEMActivationStatusToCapiType(e_Icb_ActivationStatus_deactivated));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kActivatedProvisioned,
              iis_utils::IpCommandBusTEMActivationStatusToCapiType(e_Icb_ActivationStatus_activated_provisioned));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kPreActivatedUnprovisioned,
              iis_utils::IpCommandBusTEMActivationStatusToCapiType(e_Icb_ActivationStatus_pre_activated_unprovisioned));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kActivatedUnprovisioned,
              iis_utils::IpCommandBusTEMActivationStatusToCapiType(e_Icb_ActivationStatus_activated_unprovisioned));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kRemoteProvisioningOngoing,
              iis_utils::IpCommandBusTEMActivationStatusToCapiType(e_Icb_ActivationStatus_remote_provisioning_ongoing));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kProvisioningOngoing,
              iis_utils::IpCommandBusTEMActivationStatusToCapiType(e_Icb_ActivationStatus_provisioning_ongoing));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kDeactivated,
              iis_utils::IpCommandBusTEMActivationStatusToCapiType(
                  static_cast<gen_conn::IpCommandBusTypes::TEM_ActivationStatus::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, IpCommandBusTEMRescueStatusToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_RescueStatus::kDisabled,
              iis_utils::IpCommandBusTEMRescueStatusToCapiType(e_Icb_RescueStatus_disabled));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_RescueStatus::kNotActive,
              iis_utils::IpCommandBusTEMRescueStatusToCapiType(e_Icb_RescueStatus_notActive));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_RescueStatus::kServiceNotAvailable,
              iis_utils::IpCommandBusTEMRescueStatusToCapiType(e_Icb_RescueStatus_serviceNotAvailable));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_RescueStatus::kActive,
              iis_utils::IpCommandBusTEMRescueStatusToCapiType(e_Icb_RescueStatus_active));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_RescueStatus::kTerminated,
              iis_utils::IpCommandBusTEMRescueStatusToCapiType(e_Icb_RescueStatus_terminated));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_RescueStatus::kDisabled,
              iis_utils::IpCommandBusTEMRescueStatusToCapiType(
                  static_cast<gen_conn::IpCommandBusTypes::TEM_RescueStatus::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, IpCommandBusTEMVoiceStatusToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kNoConnection,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(e_Icb_VoiceStatus_noConnection));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectingCSC,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(e_Icb_VoiceStatus_connectingCSC));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectingPSAP,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(e_Icb_VoiceStatus_connectingPSAP));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectedCSC,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(e_Icb_VoiceStatus_connectedCSC));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectedPSAP,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(e_Icb_VoiceStatus_connectedPSAP));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kIncomingCall,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(e_Icb_VoiceStatus_incomingCall));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectedCall,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(e_Icb_VoiceStatus_connectedCall));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectedIncoming,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(e_Icb_VoiceStatus_connectedIncoming));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kNoConnection,
              iis_utils::IpCommandBusTEMVoiceStatusToCapiType(
                  static_cast<gen_conn::IpCommandBusTypes::TEM_VoiceStatus::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, IpCommandBusSourceStatusToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::SourceStatus::kUnidentified,
              iis_utils::IpCommandBusSourceStatusToCapiType(e_Icb_VoiceStatus_noConnection));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::SourceStatus::kTrusted,
              iis_utils::IpCommandBusSourceStatusToCapiType(e_Icb_SourceStatus_trusted));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::SourceStatus::kCallCenter,
              iis_utils::IpCommandBusSourceStatusToCapiType(e_Icb_SourceStatus_callCenter));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::SourceStatus::Kpsap,
              iis_utils::IpCommandBusSourceStatusToCapiType(e_Icb_SourceStatus_psap));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::SourceStatus::kOutgoing,
              iis_utils::IpCommandBusSourceStatusToCapiType(e_Icb_SourceStatus_outgoing));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::SourceStatus::kUnidentified,
              iis_utils::IpCommandBusSourceStatusToCapiType(
                  static_cast<gen_conn::IpCommandBusTypes::SourceStatus::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, IpCommandBusTEMButtonStatusToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kDisabled,
              iis_utils::IpCommandBusTEMButtonStatusToCapiType(e_Icb_ButtonStatus_disabled));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kOnCallPressed,
              iis_utils::IpCommandBusTEMButtonStatusToCapiType(e_Icb_ButtonStatus_onCallPressed));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kOnCallReleased,
              iis_utils::IpCommandBusTEMButtonStatusToCapiType(e_Icb_ButtonStatus_onCallReleased));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kSosPressed,
              iis_utils::IpCommandBusTEMButtonStatusToCapiType(e_Icb_ButtonStatus_sosPressed));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kSosReleased,
              iis_utils::IpCommandBusTEMButtonStatusToCapiType(e_Icb_ButtonStatus_sosReleased));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kAllReleased,
              iis_utils::IpCommandBusTEMButtonStatusToCapiType(e_Icb_ButtonStatus_allReleased));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kDisabled,
              iis_utils::IpCommandBusTEMButtonStatusToCapiType(
                  static_cast<gen_conn::IpCommandBusTypes::TEM_ButtonStatus::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, IpCommandBusTEMPSAPStatusToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_PSAPStatus::kConfirmRequired,
              iis_utils::IpCommandBusTEMPSAPStatusToCapiType(e_Icb_PSAPStatus_confirmRequired));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_PSAPStatus::kConfirmNotRequired,
              iis_utils::IpCommandBusTEMPSAPStatusToCapiType(e_Icb_PSAPStatus_confirmNotRequired));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::TEM_PSAPStatus::kConfirmRequired,
              iis_utils::IpCommandBusTEMPSAPStatusToCapiType(
                  static_cast<gen_conn::IpCommandBusTypes::TEM_PSAPStatus::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, IpCommandBusCoordinatesLongLatToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_CoordinatesLongLat icb_longlat = Icb_CoordinatesLongLat_Create(session);
    icb_longlat->latitude = 10;
    icb_longlat->longitude = 20;

    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_CoordinatesLongLat_WGS84 capi_longlat =
        iis_utils::IpCommandBusCoordinatesLongLatToCapiType(icb_longlat);

    EXPECT_EQ(capi_longlat.getLatitude(), icb_longlat->latitude);
    EXPECT_EQ(capi_longlat.getLongitude(), icb_longlat->longitude);
}

TEST_F(TypeConvHelpersFixture, IpCommandBusDateTimeToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_DateTime icb_datetime = Icb_DateTime_Create(session);
    icb_datetime->year = 2016;
    icb_datetime->month = 10;
    icb_datetime->day = 21;
    icb_datetime->hour = 11;
    icb_datetime->minute = 29;
    icb_datetime->second = 55;

    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DateTime capi_datetime =
        iis_utils::IpCommandBusDateTimeToCapiType(icb_datetime);

    EXPECT_EQ(capi_datetime.getYear(), icb_datetime->year);
    EXPECT_EQ(capi_datetime.getMonth(), icb_datetime->month);
    EXPECT_EQ(capi_datetime.getDay(), icb_datetime->day);
    EXPECT_EQ(capi_datetime.getHour(), icb_datetime->hour);
    EXPECT_EQ(capi_datetime.getMinute(), icb_datetime->minute);
    EXPECT_EQ(capi_datetime.getSecond(), icb_datetime->second);
}

TEST_F(TypeConvHelpersFixture, IpCommandBusGnssFixTypeToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_notAvailable,
              iis_utils::IpCommandBusGnssFixTypeToCapiType(e_Icb_GnssFixType_notAvailable));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_noFix,
              iis_utils::IpCommandBusGnssFixTypeToCapiType(e_Icb_GnssFixType_noFix));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_fix2D,
              iis_utils::IpCommandBusGnssFixTypeToCapiType(e_Icb_GnssFixType_fix2D));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_fix3D,
              iis_utils::IpCommandBusGnssFixTypeToCapiType(e_Icb_GnssFixType_fix3D));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_startupMode,
              iis_utils::IpCommandBusGnssFixTypeToCapiType(e_Icb_GnssFixType_startupMode));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_notAvailable,
              iis_utils::IpCommandBusGnssFixTypeToCapiType(
                  static_cast<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, IpCommandBusDeadReckoningTypeToCapiType)
{
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::kNoDr,
              iis_utils::IpCommandBusDeadReckoningTypeToCapiType(e_Icb_DeadReckoningType_noDr));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::kDrNoMapMatch,
              iis_utils::IpCommandBusDeadReckoningTypeToCapiType(e_Icb_DeadReckoningType_drNoMapMatch));
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::kDrMapMatched,
              iis_utils::IpCommandBusDeadReckoningTypeToCapiType(e_Icb_DeadReckoningType_drMapMatched));

    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::kNoDr,
              iis_utils::IpCommandBusDeadReckoningTypeToCapiType(
                  static_cast<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, IpCommandBusTEMSimpleVehiclePositionToCapiType_No_Valid_Data)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_SimpleVehiclePosition icb_position = Icb_SimpleVehiclePosition_Create(session);
    icb_position->choice = e_Icb_SimpleVehiclePosition_noValidData;

    gen_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition capi_position =
        iis_utils::IpCommandBusTEMSimpleVehiclePositionToCapiType(icb_position);

    EXPECT_EQ(capi_position.isType<std::string>(), true);
    EXPECT_EQ(capi_position.get<std::string>(), "");
}

TEST_F(TypeConvHelpersFixture, IpCommandBusTEMSimpleVehiclePositionToCapiType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_SimpleVehiclePosition icb_position = Icb_SimpleVehiclePosition_Create(session);
    icb_position->choice = e_Icb_SimpleVehiclePosition_wgs84;

    Icb_WGS84SimplePositionData icb_wgs84 = Icb_WGS84SimplePositionData_Create(session);
    icb_wgs84->drType = e_Icb_DeadReckoningType_noDr;
    icb_position->wgs84 = icb_wgs84;

    gen_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition capi_position =
        iis_utils::IpCommandBusTEMSimpleVehiclePositionToCapiType(icb_position);

    EXPECT_EQ(capi_position.isType<gen_conn::IpCommandBusTypes::TEM_WGS84SimplePositionData>(), true);
    gen_conn::IpCommandBusTypes::TEM_WGS84SimplePositionData capi_wgs84 =
        capi_position.get<gen_conn::IpCommandBusTypes::TEM_WGS84SimplePositionData>();
    EXPECT_EQ(capi_wgs84.getDrType(), iis_utils::IpCommandBusDeadReckoningTypeToCapiType(icb_wgs84->drType));
}

TEST_F(TypeConvHelpersFixture, TestPinPukCommandToIpcbType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    gen_conn::IpCommandBusTypes::PinPukCommand_data pincode = std::string("1234");
    gen_conn::IpCommandBusTypes::CarModem_PinPukCommand command(
        gen_conn::IpCommandBusTypes::PinPukCommand_Choice::kEnterPin, pincode);

    Icb_PinPukCommand icb_command;
    bool res = iis_utils::PinPukCommandToIpcbType(session, command, icb_command);
    EXPECT_EQ(res, true);
    EXPECT_NE(icb_command, nullptr);

    EXPECT_EQ(Icb_PinPukCommand_Choices::e_Icb_PinPukCommand_enterPin, icb_command->choice);
    EXPECT_EQ(pincode.get<std::string>(), iis_utils::AsnOctetStringToCapiType<std::string>(icb_command->enterPin));

    gen_conn::IpCommandBusTypes::PinPukCodes ppc;
    ppc.setNewpin("1234");
    ppc.setPuk("12345678");
    gen_conn::IpCommandBusTypes::PinPukCommand_data pinpukcode = ppc;
    gen_conn::IpCommandBusTypes::CarModem_PinPukCommand command2(
        gen_conn::IpCommandBusTypes::PinPukCommand_Choice::kEnterPuk, pinpukcode);

    res = iis_utils::PinPukCommandToIpcbType(session, command2, icb_command);
    EXPECT_EQ(res, true);
    EXPECT_NE(icb_command, nullptr);

    EXPECT_EQ(Icb_PinPukCommand_Choices::e_Icb_PinPukCommand_enterPuk, icb_command->choice);
    EXPECT_EQ(ppc.getNewpin(), iis_utils::AsnOctetStringToCapiType<std::string>(icb_command->enterPuk->newpin));
    EXPECT_EQ(ppc.getPuk(), iis_utils::AsnOctetStringToCapiType<std::string>(icb_command->enterPuk->puk));

    gen_conn::IpCommandBusTypes::PinPinCodes pipic;
    pipic.setNewpin("5678");
    pipic.setOldpin("1234");
    gen_conn::IpCommandBusTypes::PinPukCommand_data pinpincode = pipic;
    gen_conn::IpCommandBusTypes::CarModem_PinPukCommand command3(
        gen_conn::IpCommandBusTypes::PinPukCommand_Choice::kChangePin, pinpincode);

    res = iis_utils::PinPukCommandToIpcbType(session, command3, icb_command);
    EXPECT_EQ(res, true);
    EXPECT_NE(icb_command, nullptr);

    EXPECT_EQ(Icb_PinPukCommand_Choices::e_Icb_PinPukCommand_changePin, icb_command->choice);
    EXPECT_EQ(pipic.getNewpin(), iis_utils::AsnOctetStringToCapiType<std::string>(icb_command->changePin->newpin));
    EXPECT_EQ(pipic.getOldpin(), iis_utils::AsnOctetStringToCapiType<std::string>(icb_command->changePin->oldpin));
}

TEST_F(TypeConvHelpersFixture, TimeToIpcbType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_DateTime icb_datetime = Icb_DateTime_Create(session);
    icb_datetime->year = 2017;
    icb_datetime->month = 1;
    icb_datetime->day = 1;
    icb_datetime->hour = 17;
    icb_datetime->minute = 29;
    icb_datetime->second = 49;

    std::time_t time = 1483291789;

    Icb_DateTime result = iis_utils::TimeToIpcbType(session, time);

    EXPECT_EQ(result->year, icb_datetime->year);
    EXPECT_EQ(result->month, icb_datetime->month);
    EXPECT_EQ(result->day, icb_datetime->day);
    EXPECT_EQ(result->hour, icb_datetime->hour);
    EXPECT_EQ(result->minute, icb_datetime->minute);
    EXPECT_EQ(result->second, icb_datetime->second);
}

TEST_F(TypeConvHelpersFixture, WeekdaysToByte)
{
    v0_climate::ParkingClimate::WeekdaysVec weekdays = {};
    uint8_t result = iis_utils::WeekdaysToByte(weekdays);
    EXPECT_EQ(result, 0b00);

    weekdays = {v0_climate::ParkingClimate::WeekdayEnum::MONDAY};
    result = iis_utils::WeekdaysToByte(weekdays);
    EXPECT_EQ(result, 0b01);

    weekdays = {v0_climate::ParkingClimate::WeekdayEnum::MONDAY, v0_climate::ParkingClimate::WeekdayEnum::TUESDAY};
    result = iis_utils::WeekdaysToByte(weekdays);
    EXPECT_EQ(result, 0b11);

    weekdays = {v0_climate::ParkingClimate::WeekdayEnum::MONDAY, v0_climate::ParkingClimate::WeekdayEnum::WEDNESDAY};
    result = iis_utils::WeekdaysToByte(weekdays);
    EXPECT_EQ(result, 0b101);

    weekdays = {v0_climate::ParkingClimate::WeekdayEnum::SUNDAY,
                v0_climate::ParkingClimate::WeekdayEnum::WEDNESDAY,
                v0_climate::ParkingClimate::WeekdayEnum::TUESDAY};
    result = iis_utils::WeekdaysToByte(weekdays);
    EXPECT_EQ(result, 0b01000110);
}

TEST_F(TypeConvHelpersFixture, ByteToWeekdays)
{
    v0_climate::ParkingClimate::WeekdaysVec result = {v0_climate::ParkingClimate::WeekdayEnum::MONDAY};
    EXPECT_EQ(result, iis_utils::ByteToWeekdays(0b01));

    result = {v0_climate::ParkingClimate::WeekdayEnum::MONDAY, v0_climate::ParkingClimate::WeekdayEnum::TUESDAY};
    EXPECT_EQ(result, iis_utils::ByteToWeekdays(0b11));

    result = {v0_climate::ParkingClimate::WeekdayEnum::TUESDAY,
              v0_climate::ParkingClimate::WeekdayEnum::WEDNESDAY,
              v0_climate::ParkingClimate::WeekdayEnum::SUNDAY};
    EXPECT_EQ(result, iis_utils::ByteToWeekdays(0b01000110));
}

TEST_F(TypeConvHelpersFixture, ParkTimersToString)
{
    v0_climate::ParkingClimate::DateTimeStruct dateTime(2017, 1, 1, 17, 29, 49);
    v0_climate::ParkingClimate::WeekdaysVec weekdays = {v0_climate::ParkingClimate::WeekdayEnum::MONDAY};

    v0_climate::ParkingClimate::ParkingTimers pt = {
        v0_climate::ParkingClimate::ParkingTimerStruct(1,
                                                       v0_climate::ParkingClimate::TimerTypeEnum::DAY,
                                                       v0_climate::ParkingClimate::TimerStateEnum::ON,
                                                       dateTime,
                                                       weekdays)};
    ASN_BYTE result[10] = {0};
    iis_utils::ParkTimersToString(pt, &result[0]);

    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 1);
    EXPECT_EQ(result[3], 1);
    EXPECT_EQ(result[4], 17);
    EXPECT_EQ(result[5], 1);
    EXPECT_EQ(result[6], 1);
    EXPECT_EQ(result[7], 17);
    EXPECT_EQ(result[8], 29);
    EXPECT_EQ(result[9], 49);
}

TEST_F(TypeConvHelpersFixture, ParkingTimersToIpcbType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    v0_climate::ParkingClimate::DateTimeStruct dateTime(17, 1, 1, 17, 29, 49);
    v0_climate::ParkingClimate::WeekdaysVec weekdays = {v0_climate::ParkingClimate::WeekdayEnum::MONDAY};

    v0_climate::ParkingClimate::ParkingTimers pt = {
        v0_climate::ParkingClimate::ParkingTimerStruct(1,
                                                       v0_climate::ParkingClimate::TimerTypeEnum::DAY,
                                                       v0_climate::ParkingClimate::TimerStateEnum::ON,
                                                       dateTime,
                                                       weekdays)};

    Icb_Setting setting = iis_utils::ParkingTimersToIpcbType(session, pt);

    uint id = 0x02;
    EXPECT_EQ(setting->id, id);
    EXPECT_EQ(setting->sType, Icb_SettingType_t::e_Icb_SettingType_setting);
    uint length = 321;
    EXPECT_EQ(setting->length, length);
}

TEST_F(TypeConvHelpersFixture, SettingToCAPIType)
{
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_Setting setting = Icb_Setting_Create(session);

    v0_climate::ParkingClimate::DateTimeStruct dateTime(2017, 1, 1, 17, 29, 49);
    v0_climate::ParkingClimate::WeekdaysVec weekdays = {v0_climate::ParkingClimate::WeekdayEnum::MONDAY};

    v0_climate::ParkingClimate::ParkingTimers pt = {
        v0_climate::ParkingClimate::ParkingTimerStruct(1,
                                                       v0_climate::ParkingClimate::TimerTypeEnum::DAY,
                                                       v0_climate::ParkingClimate::TimerStateEnum::ON,
                                                       dateTime,
                                                       weekdays)};

    ASN_BYTE data[320] = {0};
    iis_utils::ParkTimersToString(pt, &data[0]);
    ASN_OctetString value = iis_utils::CapiStringTypeToAsnOctetString(session, data, 320);
    setting->value = value;

    v0_climate::ParkingClimate::ParkingTimers result = iis_utils::SettingToCAPIType(setting);

    EXPECT_EQ(result.size(), pt.size());
    EXPECT_EQ(result[0], pt[0]);
}

TEST_F(TypeConvHelpersFixture, IpCommandBusWorkshopWlanConnectionResultToCapiType)
{
    EXPECT_EQ(
        gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::CONNECTED,
        iis_utils::IpCommandBusWorkshopWlanConnectionResultToCapiType(e_Icb_WorkshopWlanConnectionResult_connected));
    EXPECT_EQ(gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_AUTHENTICATION,
              iis_utils::IpCommandBusWorkshopWlanConnectionResultToCapiType(
                  e_Icb_WorkshopWlanConnectionResult_failed_authentication));
    EXPECT_EQ(gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_GENERAL,
              iis_utils::IpCommandBusWorkshopWlanConnectionResultToCapiType(
                  e_Icb_WorkshopWlanConnectionResult_failed_general));
    EXPECT_EQ(gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_NO_APS,
              iis_utils::IpCommandBusWorkshopWlanConnectionResultToCapiType(
                  e_Icb_WorkshopWlanConnectionResult_failed_no_aps));
    EXPECT_EQ(gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_TIMEOUT,
              iis_utils::IpCommandBusWorkshopWlanConnectionResultToCapiType(
                  e_Icb_WorkshopWlanConnectionResult_failed_timeout));
    EXPECT_EQ(gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_GENERAL,
              iis_utils::IpCommandBusWorkshopWlanConnectionResultToCapiType(
                  static_cast<gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::Literal>(100)));
}

TEST_F(TypeConvHelpersFixture, WorkshopWlanDaCModeToIpcbType)
{
    EXPECT_EQ(e_Icb_UpdateDacMode_automatic,
              iis_utils::WorkshopWlanDaCModeToIpcbType(gen_conn::WorkshopWlan::DaCMode::AUTOMATIC));
    EXPECT_EQ(e_Icb_UpdateDacMode_semiautomatic,
              iis_utils::WorkshopWlanDaCModeToIpcbType(gen_conn::WorkshopWlan::DaCMode::SEMI_AUTOMATIC));
    EXPECT_EQ(e_Icb_UpdateDacMode_manual,
              iis_utils::WorkshopWlanDaCModeToIpcbType(gen_conn::WorkshopWlan::DaCMode::MANUAL));
}

TEST_F(TypeConvHelpersFixture, WorkshopWlanIpcbTypeDaCModeToCapiType)
{
    EXPECT_EQ(gen_conn::WorkshopWlan::DaCMode::AUTOMATIC,
              iis_utils::WorkshopWlanIpcbTypeDaCModeToCapiType(e_Icb_UpdateDacMode_automatic));
    EXPECT_EQ(gen_conn::WorkshopWlan::DaCMode::SEMI_AUTOMATIC,
              iis_utils::WorkshopWlanIpcbTypeDaCModeToCapiType(e_Icb_UpdateDacMode_semiautomatic));
    EXPECT_EQ(gen_conn::WorkshopWlan::DaCMode::MANUAL,
              iis_utils::WorkshopWlanIpcbTypeDaCModeToCapiType(e_Icb_UpdateDacMode_manual));
}
