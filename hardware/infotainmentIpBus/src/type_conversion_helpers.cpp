#define LOG_TAG "Infotainment_IP_Service.tcon"

#include "type_conversion_helpers.h"
#include <cassert>
//#include <cmath>
//#include <string>
#include <cutils/log.h>
#include "date.h"

namespace InfotainmentIpBus
{
namespace Utils
{
//using namespace Connectivity;
using namespace std::chrono;

int64_t ToMsSince1970(const Icb_DateTime_t* utc_time)
{
    // create the epoch as a time_point
    static const date::year_month_day ymd1970(date::year(1970), date::month(1), date::day(1));
    static const std::chrono::system_clock::time_point tp1970((date::sys_days(ymd1970)));

    // create received time as a time_point
    const date::year_month_day ymd(date::year(utc_time->year), date::month(utc_time->month), date::day(utc_time->day));
    const std::chrono::system_clock::time_point tp(date::sys_days(ymd) + hours(utc_time->hour) + minutes(utc_time->minute) + seconds(utc_time->second));

    return duration_cast<milliseconds>(tp - tp1970).count();
}

std::string FixTypeToString(Icb_GnssFixType_t fix_type)
{
    switch (fix_type)
    {
        case e_Icb_GnssFixType_notAvailable:
        {
            return "Not available";
        }
        break;
        case e_Icb_GnssFixType_noFix:
        {
            return "No fix";
        }
        break;
        case e_Icb_GnssFixType_fix2D:
        {
            return "Fix 2D";
        }
        break;
        case e_Icb_GnssFixType_fix3D:
        {
            return "Fix 3D";
        }
        break;
        case e_Icb_GnssFixType_startupMode:
        {
            return "Startup mode";
        }
        break;
        default:
            assert(false);
            return "No fix";
    }
}

Icb_GnssFixType_t FixStatusToIpcbType(const uint64_t gnss_fix_status)

{
    //  key = GNSS_FIX_STATUS, value = value of type 'UInt16', that represents an
    //  enum(NO_FIX(0x01),TIME_FIX(0x01),2D_FIX(0x02),3D_FIX(0x03), ... )

    const uint64_t no_fix = 0x01;
    const uint64_t d2_fix = 0x02;
    const uint64_t d3_fix = 0x03;

    switch (gnss_fix_status)
    {
        case no_fix:
        {
            return e_Icb_GnssFixType_noFix;
        }
        break;

        case d2_fix:
        {
            return e_Icb_GnssFixType_fix2D;
        }
        break;

        case d3_fix:
        {
            return e_Icb_GnssFixType_fix3D;
        }
        break;

        default:
        {
            // NOTE!! DON'T assert here!
            //        The remote endpoint application (Enhanced Position server) is developer by external location team.
            //        and the value provided by CommonAPI is a regular uint64 (not an enum, and no constants declared)
            //        so easy to do mistakes...
            //        (We don't want to get 'ip service crashes' support issues for this...)
            ALOGE("Out of range fix status (fix type) received from CommonAPI!");
            return e_Icb_GnssFixType_noFix;
        }
    }
}

std::string AsnErrCodeToString(ASN_Errcode code)
{
    switch (code)
    {
        case ASN_ERROR_NO_ERROR:
        {
            return "No error";
        }
        break;
        case ASN_ERROR_MEMORY:
        {
            return "Memory";
        }
        break;
        case ASN_ERROR_BUFFER_FULL:
        {
            return "Buffer full";
        }
        break;
        case ASN_ERROR_BUFFER_STARVING:
        {
            return "Buffer starving";
        }
        break;
        case ASN_ERROR_NO_VALID_CHOICE:
        {
            return "No valid choice";
        }
        break;
        case ASN_ERROR_VALUE_NOT_WITHIN_RANGE:
        {
            return "Parameter value out of range (of ASN spec. Bad param from CommonAPI?)";
        }
        break;
        case ASN_ERROR_SIZE_NOT_SUPPORTED:
        {
            return "Size not supported";
        }
        break;
        default:
            return "Unknown error code";
    }
}
/*
Icb_Ecu_t EcuToIpcbType(const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu& ecu)
{
    switch (ecu.value_)
    {
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kIhu:
        {
            return e_Icb_Ecu_ihu;
        }
        break;
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kNone:
        {
            return e_Icb_Ecu_none;
        }
        break;
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem:
        {
            return e_Icb_Ecu_tem;
        }
        break;
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm:
        {
            return e_Icb_Ecu_vcm;
        }
        break;
        default:
        {
            assert(false);
            return e_Icb_Ecu_none;
        }
    }
}

Icb_OnOffSetting_t OnOffSettingToIpcbType(const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting& on_off)
{
    switch (on_off.value_)
    {
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOn:
        {
            return e_Icb_OnOffSetting_on;
        }
        break;

        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOff:
        {
            return e_Icb_OnOffSetting_off;
        }
        break;

        default:
        {
            assert(false);
            return e_Icb_OnOffSetting_off;
        }
    }
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu EcuToCapiType(const Icb_Ecu_t gateway_ecu)
{
    switch (gateway_ecu)
    {
        case e_Icb_Ecu_none:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kNone);
        }
        break;
        case e_Icb_Ecu_ihu:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kIhu);
        }
        break;
        case e_Icb_Ecu_vcm:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm);
        }
        break;
        case e_Icb_Ecu_tem:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem);
        }
        break;

        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kNone);
        }
    }
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting OnOffSettingToCapiType(const Icb_OnOffSetting_t on_off)
{
    switch (on_off)
    {
        case e_Icb_OnOffSetting_off:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOff);
        }
        break;
        case e_Icb_OnOffSetting_on:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOn);
        }
        break;

        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting::kOff);
        }
    }
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode WlanModeToCapiType(const Icb_WLANMode_t mode)
{
    switch (mode)
    {
        case e_Icb_WLANMode_off:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kOff);
        }
        break;

        case e_Icb_WLANMode_sta:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kSta);
        }
        break;

        case e_Icb_WLANMode_ap:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kAp);
        }
        break;

        case e_Icb_WLANMode_staWS:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kStaWs);
        }
        break;

        default:
        {
            assert(false);
        }
    }

    return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode(
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kOff);
}

Icb_WLANMode WlanModeToIpcbType(const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode mode)
{
    switch (mode)
    {
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kOff:
        {
            return static_cast<Icb_WLANMode>(e_Icb_WLANMode_off);
        }
        break;

        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kSta:
        {
            return static_cast<Icb_WLANMode>(e_Icb_WLANMode_sta);
        }
        break;

        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kAp:
        {
            return static_cast<Icb_WLANMode>(e_Icb_WLANMode_ap);
        }
        break;

        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode::kStaWs:
        {
            return static_cast<Icb_WLANMode>(e_Icb_WLANMode_staWS);
        }
        break;

        default:
        {
            assert(false);
        }
    }

    return static_cast<Icb_WLANMode>(e_Icb_WLANMode_off);
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus WlanConnectionStatusToCapiType(
    Icb_WLANConnectionStatus_t status)
{
    switch (status)
    {
        case e_Icb_WLANConnectionStatus_disconnected:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected);
        }
        break;

        case e_Icb_WLANConnectionStatus_connected:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kConnected);
        }
        break;

        default:
        {
            assert(false);
        }
    }

    return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected;
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus WlanConnectStatusToCapiType(
    Icb_WLANConnectStatus_t status)
{
    switch (status)
    {
        case e_Icb_WLANConnectStatus_disconnected:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected);
        }
        break;

        case e_Icb_WLANConnectStatus_connected:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kConnected);
        }
        break;

        case e_Icb_WLANConnectStatus_wrongPassword:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kWrongPassword);
        }
        break;

        default:
        {
            assert(false);
        }
    }

    return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus::kDisconnected;
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid WlanSsidToCapiType(ASN_OctetString ssid)
{
    return std::move(AsnOctetStringToCapiType<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid>(ssid));
}

std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid> WlanArrayOfSsidToCapiType(
    Icb_ArrayOf_SSID p_array_ssid)
{
    std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid> vec;

    vec.reserve(p_array_ssid->actual_size);

    for (U32 i = 0; i < p_array_ssid->actual_size; ++i)
    {
        vec.push_back(WlanSsidToCapiType(p_array_ssid->data_array[i]));
    }

    return std::move(vec);
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType WlanSecurityTypeToCapiType(
    Icb_WLANSecurityType_t type)
{
    switch (type)
    {
        case e_Icb_WLANSecurityType_open:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kOpen);
        }
        break;

        case e_Icb_WLANSecurityType_wEP_open:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWepOpen);
        }
        break;

        case e_Icb_WLANSecurityType_wEP_SharedKey:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWepSharedKey);
        }
        break;

        case e_Icb_WLANSecurityType_wPA_EAP_TLS:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpaEapTls);
        }
        break;

        case e_Icb_WLANSecurityType_wPA_PSK:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpaPsk);
        }
        break;

        case e_Icb_WLANSecurityType_wPA2_PSK:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpa2Psk);
        }
        break;

        case e_Icb_WLANSecurityType_wPA2_EAP_TLS:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kWpa2EapTls);
        }
        break;

        default:
        {
            assert(false);
        }
    }

    return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType::kOpen;
}

gen_conn::Wlan::WlanChannelWidthData WlanChannelWidthDataTypeToCapiType(Icb_WLANChannelWidthData_t type)
{
    switch (type)
    {
        case e_Icb_WLANChannelWidthData_width_5MHz:
            return gen_conn::Wlan::WlanChannelWidthData::Literal::Width5MHz;
        case e_Icb_WLANChannelWidthData_width_10MHz:
            return gen_conn::Wlan::WlanChannelWidthData::Literal::Width10MHz;
        case e_Icb_WLANChannelWidthData_width_20MHz:
            return gen_conn::Wlan::WlanChannelWidthData::Literal::Width20MHz;
    }

    assert(false);

    return gen_conn::Wlan::WlanChannelWidthData::Literal::Unknown;
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation WlanNetworkInfoToCapiType(
    Icb_AvailableNetworkInformation p_network_info)
{
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation network_info;

    network_info.setSsid(WlanSsidToCapiType(p_network_info->sSID));
    network_info.setSignalStrength(static_cast<int32_t>(p_network_info->signalStrength));
    network_info.setSecurityType(WlanSecurityTypeToCapiType(p_network_info->securityType));
    network_info.setRememberedSsid(static_cast<bool>(p_network_info->rememberedSSID));

    return std::move(network_info);
}

std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation>
WlanArrayOfNetworkInfoToCapiType(Icb_ArrayOf_AvailableNetworkInformation p_array_network_info)
{
    std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation> vec;

    // There is a bug in the asn.1 compiler that leaves the max_size variable undefined.
    // If that bug should ever get fixed, re-enable this assert.
    // assert(p_array_network_info->actual_size <= p_array_network_info->max_size);
    vec.reserve(p_array_network_info->actual_size);

    for (U32 i = 0; i < p_array_network_info->actual_size; ++i)
    {
        vec.push_back(WlanNetworkInfoToCapiType(p_array_network_info->data_array[i]));
    }

    return std::move(vec);
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource InternetSourceToCapiType(
    const Icb_InternetSource_t internet_source)
{
    switch (internet_source)
    {
        case e_Icb_InternetSource_none:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kNone);
        }
        break;
        case e_Icb_InternetSource_psim:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kPSim);
        }
        break;
        case e_Icb_InternetSource_bluetooth:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kBluetooth);
        }
        break;
        case e_Icb_InternetSource_usb:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kUsb);
        }
        break;

        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kNone);
        }
    }
}

Icb_InternetSource_t InternetSourceToIpcbType(
    const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource& internet_source)
{
    switch (internet_source.value_)
    {
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kNone:
        {
            return e_Icb_InternetSource_none;
        }
        break;
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kPSim:
        {
            return e_Icb_InternetSource_psim;
        }
        break;
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kBluetooth:
        {
            return e_Icb_InternetSource_bluetooth;
        }
        break;
        case gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource::kUsb:
        {
            return e_Icb_InternetSource_usb;
        }
        break;

        default:
        {
            assert(false);
            return e_Icb_InternetSource_none;
        }
    }
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors GenericErrorTypeToCapiType(
    const Connectivity::TransportServices::ErrorType error_type, Icb_OpGeneric_Error_t error_info)
{
    switch (error_type)
    {
        case Connectivity::TransportServices::ErrorType::OK:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK);
        }
        case Connectivity::TransportServices::ErrorType::REMOTE_ERROR:
        {
            // TODO(johan, 160413) Handle error info too, can be more specific
            (void)error_info;
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR);
        }
        case Connectivity::TransportServices::ErrorType::LOCAL_TIMEOUT:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::TIMEOUT);
        }
        case Connectivity::TransportServices::ErrorType::PAYLOAD_DECODE_ERROR:  // (For use by service specific layer.)
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR);
        }
    }
}

gen_conn::OTATypes::GenericOkStatus OTAGenericOkStatusToCapiType(const Icb_GenericOkStatus status)
{
    switch (status)
    {
        case e_Icb_GenericOkStatus_unknown:
        {
            return gen_conn::OTATypes::GenericOkStatus(gen_conn::OTATypes::GenericOkStatus::unknown);
        }
        case e_Icb_GenericOkStatus_ok:
        {
            return gen_conn::OTATypes::GenericOkStatus(gen_conn::OTATypes::GenericOkStatus::ok);
        }
        case e_Icb_GenericOkStatus_notOk:
        {
            return gen_conn::OTATypes::GenericOkStatus(gen_conn::OTATypes::GenericOkStatus::notOk);
        }
        default:
        {
            assert(false);
            return gen_conn::OTATypes::GenericOkStatus(gen_conn::OTATypes::GenericOkStatus::unknown);
        }
    }
}

gen_conn::IpCommandBusTypes::GenericOkStatus IpCommandBusGenericOkStatusToCapiType(const Icb_GenericOkStatus status)
{
    switch (status)
    {
        case e_Icb_GenericOkStatus_unknown:
        {
            return gen_conn::IpCommandBusTypes::GenericOkStatus(gen_conn::IpCommandBusTypes::GenericOkStatus::unknown);
        }
        case e_Icb_GenericOkStatus_ok:
        {
            return gen_conn::IpCommandBusTypes::GenericOkStatus(gen_conn::IpCommandBusTypes::GenericOkStatus::ok);
        }
        case e_Icb_GenericOkStatus_notOk:
        {
            return gen_conn::IpCommandBusTypes::GenericOkStatus(gen_conn::IpCommandBusTypes::GenericOkStatus::notOk);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::GenericOkStatus(gen_conn::IpCommandBusTypes::GenericOkStatus::unknown);
        }
    }
}

Icb_GenericOkStatus IpCommandBusGenericOkStatusToIcbType(const gen_conn::IpCommandBusTypes::GenericOkStatus status)
{
    switch (status)
    {
        case gen_conn::IpCommandBusTypes::GenericOkStatus::unknown:
        {
            return e_Icb_GenericOkStatus_unknown;
        }
        case gen_conn::IpCommandBusTypes::GenericOkStatus::ok:
        {
            return e_Icb_GenericOkStatus_ok;
        }
        case gen_conn::IpCommandBusTypes::GenericOkStatus::notOk:
        {
            return e_Icb_GenericOkStatus_notOk;
        }
        default:
        {
            assert(false);
            return e_Icb_GenericOkStatus_unknown;
        }
    }
}

gen_conn::OTATypes::DownloadStatus OTADownloadStatusToCapiType(const Icb_DownloadStatus download_status)
{
    switch (download_status)
    {
        case e_Icb_DownloadStatus_idle:
        {
            return gen_conn::OTATypes::DownloadStatus(gen_conn::OTATypes::DownloadStatus::idle);
        }
        case e_Icb_DownloadStatus_active:
        {
            return gen_conn::OTATypes::DownloadStatus(gen_conn::OTATypes::DownloadStatus::active);
        }
        case e_Icb_DownloadStatus_noService:
        {
            return gen_conn::OTATypes::DownloadStatus(gen_conn::OTATypes::DownloadStatus::noService);
        }
        default:
        {
            assert(false);
            return gen_conn::OTATypes::DownloadStatus(gen_conn::OTATypes::DownloadStatus::noService);
        }
    }
}

gen_conn::OTATypes::DownloadState OTADownloadStateToCapiType(Icb_DownloadState download_state)
{
    switch (download_state)
    {
        case e_Icb_DownloadState_queued:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::queued);
        }
        case e_Icb_DownloadState_requestDownloadInfo:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::requestDownloadInfo);
        }
        case e_Icb_DownloadState_requestDownloadInfo_Halted:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::requestDownloadInfo_Halted);
        }
        case e_Icb_DownloadState_validatingConfig:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::validatingConfig);
        }
        case e_Icb_DownloadState_configValidated:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::configValidated);
        }
        case e_Icb_DownloadState_downloading:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::downloading);
        }
        case e_Icb_DownloadState_downloading_Halted:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::downloading_Halted);
        }
        case e_Icb_DownloadState_downloaded:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::downloaded);
        }
        case e_Icb_DownloadState_installing:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::installing);
        }
        case e_Icb_DownloadState_installed_Waitreset:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::installed_Waitreset);
        }
        case e_Icb_DownloadState_installed:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::installed);
        }
        case e_Icb_DownloadState_errorNoStorage:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorNoStorage);
        }
        case e_Icb_DownloadState_errorSoftwareNotOk:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorSoftwareNotOk);
        }
        case e_Icb_DownloadState_errorDownloadFileFailed:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorDownloadFileFailed);
        }
        case e_Icb_DownloadState_errorTimeToLiveExceeded:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorTimeToLiveExceeded);
        }
        case e_Icb_DownloadState_errorValidationFailed:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorValidationFailed);
        }
        case e_Icb_DownloadState_errorInstallationFailed:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorInstallationFailed);
        }
        case e_Icb_DownloadState_errorInstallationFailedMemoryFull:
        {
            return gen_conn::OTATypes::DownloadState(
                gen_conn::OTATypes::DownloadState::errorInstallationFailedMemoryFull);
        }
        case e_Icb_DownloadState_errorNoDiagResponse:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorNoDiagResponse);
        }
        case e_Icb_DownloadState_errorDownloadInfoRequestFailed:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorDownloadInfoRequestFailed);
        }
        case e_Icb_DownloadState_cancelling:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::cancelling);
        }
        case e_Icb_DownloadState_cancelled:
        {
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::cancelled);
        }
        default:
        {
            assert(false);
            return gen_conn::OTATypes::DownloadState(gen_conn::OTATypes::DownloadState::errorDownloadFileFailed);
        }
    }
}

gen_conn::OTATypes::SoftwareProduct OTASoftwareProductToCapiType(Icb_SoftwareProduct p_software_product)
{
    gen_conn::OTATypes::SoftwareProduct software_product;

    software_product.setId(AsnUtf8StringToCapiType(p_software_product->id));
    software_product.setDownloadState(OTADownloadStateToCapiType(p_software_product->downloadState));
    software_product.setDownloadProgress(static_cast<int16_t>(p_software_product->progress));

    return std::move(software_product);
}

std::vector<gen_conn::OTATypes::SoftwareProduct> OTAArrayOfSoftwareProductToCapiType(
    Icb_ArrayOf_SoftwareProduct p_software_product)
{
    std::vector<gen_conn::OTATypes::SoftwareProduct> vec;

    vec.reserve(p_software_product->actual_size);

    for (U32 i = 0; i < p_software_product->actual_size; ++i)
    {
        vec.push_back(OTASoftwareProductToCapiType(p_software_product->data_array[i]));
    }

    return std::move(vec);
}

gen_conn::OTATypes::UUID OTADownloadSoftwareToCapiType(Icb_UUID p_uuid)
{
    gen_conn::OTATypes::UUID uuid;

    uuid.setUuid(AsnUtf8StringToCapiType(p_uuid));

    return std::move(uuid);
}

std::vector<gen_conn::OTATypes::UUID> OTAArrayOfDownloadSoftwareToCapiType(Icb_ArrayOf_UUID p_uuid)
{
    std::vector<gen_conn::OTATypes::UUID> vec;

    vec.reserve(p_uuid->actual_size);

    for (U32 i = 0; i < p_uuid->actual_size; ++i)
    {
        vec.push_back(OTADownloadSoftwareToCapiType(p_uuid->data_array[i]));
    }

    return std::move(vec);
}

Icb_ArrayOf_UUID ArrayOfUUIDToIpcbType(ASN_Session session, std::vector<gen_conn::OTATypes::UUID> uuidList)
{
    Icb_ArrayOf_UUID output = Icb_ArrayOf_UUID_Create(session);
    Icb_ArrayOf_UUID_SetSize(output, session, uuidList.size());

    for (U32 i = 0; i < output->actual_size; i++)
    {
        output->data_array[i]->uuid = CapiStringTypeToAsnOctetString(session, uuidList[i].getUuid());
    }

    return std::move(output);
}

Icb_UUID OTAUuidToIpcbType(ASN_Session session, gen_conn::OTATypes::UUID uuid)
{
    Icb_UUID icb_uuid = Icb_UUID_Create(session);

    icb_uuid->uuid = CapiStringTypeToAsnOctetString(session, uuid.getUuid());

    return std::move(icb_uuid);
}

gen_conn::OTATypes::PartIdentifier OTADeletedSoftwareToCapiType(Icb_PartIdentifier p_part_id)
{
    gen_conn::OTATypes::PartIdentifier part_id;

    part_id.setPartID(AsnUtf8StringToCapiType(p_part_id));

    return std::move(part_id);
}

std::vector<gen_conn::OTATypes::PartIdentifier> OTAArrayOfDeletedSoftwareToCapiType(
    Icb_ArrayOf_PartIdentifier p_part_id)
{
    std::vector<gen_conn::OTATypes::PartIdentifier> vec;

    vec.reserve(p_part_id->actual_size);

    for (U32 i = 0; i < p_part_id->actual_size; ++i)
    {
        vec.push_back(OTADeletedSoftwareToCapiType(p_part_id->data_array[i]));
    }

    return std::move(vec);
}

std::vector<gen_conn::OTATypes::InstallationData> ArrayOfInstallationDataToCapiType(
    Icb_ArrayOf_InstallationData installation_data)
{
    std::vector<gen_conn::OTATypes::InstallationData> output;

    output.reserve(installation_data->actual_size);

    for (U32 i = 0; i < installation_data->actual_size; i++)
    {
        output.push_back(InstallationDataToCapiType(installation_data->data_array[i]));
    }

    return std::move(output);
}

gen_conn::OTATypes::InstallationData InstallationDataToCapiType(Icb_InstallationData installation_data)
{
    gen_conn::OTATypes::PartIdentifier partId(
        AsnOctetStringToCapiType<std::string>(installation_data->identifier->partID));
    gen_conn::OTATypes::Uri uri(AsnOctetStringToCapiType<std::string>(installation_data->name->uri));
    gen_conn::OTATypes::InstallationData output(partId, uri, installation_data->rebootNeeded);

    return std::move(output);
}

gen_conn::OTATypes::InstallCommand InstallCommandToCapiType(Icb_InstallCommand install_command)
{
    switch (install_command)
    {
        case Icb_InstallCommand_t::e_Icb_InstallCommand_install:
        {
            return gen_conn::OTATypes::InstallCommand(gen_conn::OTATypes::InstallCommand::install);
        }
        case Icb_InstallCommand_t::e_Icb_InstallCommand_useSoftware:
        {
            return gen_conn::OTATypes::InstallCommand(gen_conn::OTATypes::InstallCommand::useSoftware);
        }
        case Icb_InstallCommand_t::e_Icb_InstallCommand_cancelInstall:
        {
            return gen_conn::OTATypes::InstallCommand(gen_conn::OTATypes::InstallCommand::cancelInstall);
        }
        default:
            assert(false);
            return gen_conn::OTATypes::InstallCommand(gen_conn::OTATypes::InstallCommand::cancelInstall);
    }
}

Icb_InstallStatus_t InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus status)
{
    switch (status)
    {
        case gen_conn::OTATypes::InstallStatus::idle:
        {
            return e_Icb_InstallStatus_idle;
        }
        case gen_conn::OTATypes::InstallStatus::installing:
        {
            return e_Icb_InstallStatus_installing;
        }
        case gen_conn::OTATypes::InstallStatus::installationComplete:
        {
            return e_Icb_InstallStatus_installationComplete;
        }
        case gen_conn::OTATypes::InstallStatus::installationPendingCmd:
        {
            return e_Icb_InstallStatus_installationPendingCmd;
        }
        case gen_conn::OTATypes::InstallStatus::cancelled:
        {
            return e_Icb_InstallStatus_cancelled;
        }
        case gen_conn::OTATypes::InstallStatus::failed:
        {
            return e_Icb_InstallStatus_failed;
        }
        case gen_conn::OTATypes::InstallStatus::failedMemoryFull:
        {
            return e_Icb_InstallStatus_failedMemoryFull;
        }
        case gen_conn::OTATypes::InstallStatus::installationWaitReboot:
        {
            return e_Icb_InstallStatus_installationWaitReboot;
        }
        case gen_conn::OTATypes::InstallStatus::failedValidation:
        {
            return e_Icb_InstallStatus_failedValidation;
        }
        default:
            return e_Icb_InstallStatus_idle;
    }
}

Icb_ArrayOf_InstalledData ArrayOfInstalledDataToIpcbType(ASN_Session session,
                                                         std::vector<gen_conn::OTATypes::InstalledData> installedList)
{
    Icb_ArrayOf_InstalledData output = Icb_ArrayOf_InstalledData_Create(session);
    Icb_ArrayOf_InstalledData_SetSize(output, session, installedList.size());

    for (U32 i = 0; i < output->actual_size; i++)
    {
        output->data_array[i]->identifier->partID =
            (CapiStringTypeToAsnOctetString(session, installedList[i].getIdentifier().getPartID()));
        output->data_array[i]->fileStatus = static_cast<Icb_FileInstallationStatus>(installedList[i].getFileStatus());
    }

    return std::move(output);
}

Icb_PartIdentifier PartIdentifierToIpcbType(ASN_Session session, gen_conn::OTATypes::PartIdentifier part_id)
{
    Icb_PartIdentifier part_identifier = Icb_PartIdentifier_Create(session);

    part_identifier->partID = CapiStringTypeToAsnOctetString(session, part_id.getPartID());

    return part_identifier;
}

Icb_ArrayOf_PartIdentifier ArrayOfPartIdentifierToIpcbType(
    ASN_Session session, std::vector<gen_conn::OTATypes::PartIdentifier> deleted_software_list)
{
    Icb_ArrayOf_PartIdentifier part_id_list = Icb_ArrayOf_PartIdentifier_Create(session);
    Icb_ArrayOf_PartIdentifier_SetSize(part_id_list, session, deleted_software_list.size());

    for (U32 i = 0; i < part_id_list->actual_size; i++)
    {
        part_id_list->data_array[i]->partID =
            CapiStringTypeToAsnOctetString(session, deleted_software_list[i].getPartID());
    }

    return std::move(part_id_list);
}

Icb_SBWarning sbWarningToIpcbType(ASN_Session session, gen_conn::IpCommandBusTypes::SBWarning warning)
{
    Icb_SBWarning icb_warning = Icb_SBWarning_Create(session);

    icb_warning->id = warning.getId();
    icb_warning->message = CapiStringTypeToAsnOctetString(session, warning.getMessage());

    return icb_warning;
}

Icb_ArrayOf_SBWarning ArrayOfsbWarningToIpcbType(ASN_Session session,
                                                 std::vector<gen_conn::IpCommandBusTypes::SBWarning> warnings)
{
    Icb_ArrayOf_SBWarning icb_array_of_sb_warning = Icb_ArrayOf_SBWarning_Create(session);

    Icb_ArrayOf_SBWarning_SetSize(icb_array_of_sb_warning, session, warnings.size());

    for (U32 i = 0; i < icb_array_of_sb_warning->actual_size; i++)
    {
        icb_array_of_sb_warning->data_array[i]->id = warnings[i].getId();
        icb_array_of_sb_warning->data_array[i]->message =
            InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, warnings[i].getMessage());
    }

    return std::move(icb_array_of_sb_warning);
}

gen_conn::IpCommandBusTypes::TEM_ActivationStatus IpCommandBusTEMActivationStatusToCapiType(
    const Icb_ActivationStatus status)
{
    switch (status)
    {
        case e_Icb_ActivationStatus_deactivated:
        {
            return gen_conn::IpCommandBusTypes::TEM_ActivationStatus(
                gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kDeactivated);
        }
        case e_Icb_ActivationStatus_activated_provisioned:
        {
            return gen_conn::IpCommandBusTypes::TEM_ActivationStatus(
                gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kActivatedProvisioned);
        }
        case e_Icb_ActivationStatus_pre_activated_unprovisioned:
        {
            return gen_conn::IpCommandBusTypes::TEM_ActivationStatus(
                gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kPreActivatedUnprovisioned);
        }
        case e_Icb_ActivationStatus_activated_unprovisioned:
        {
            return gen_conn::IpCommandBusTypes::TEM_ActivationStatus(
                gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kActivatedUnprovisioned);
        }
        case e_Icb_ActivationStatus_remote_provisioning_ongoing:
        {
            return gen_conn::IpCommandBusTypes::TEM_ActivationStatus(
                gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kRemoteProvisioningOngoing);
        }
        case e_Icb_ActivationStatus_provisioning_ongoing:
        {
            return gen_conn::IpCommandBusTypes::TEM_ActivationStatus(
                gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kProvisioningOngoing);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::TEM_ActivationStatus(
                gen_conn::IpCommandBusTypes::TEM_ActivationStatus::kDeactivated);
        }
    }
}

gen_conn::IpCommandBusTypes::TEM_RescueStatus IpCommandBusTEMRescueStatusToCapiType(const Icb_RescueStatus status)
{
    switch (status)
    {
        case e_Icb_RescueStatus_disabled:
        {
            return gen_conn::IpCommandBusTypes::TEM_RescueStatus(
                gen_conn::IpCommandBusTypes::TEM_RescueStatus::kDisabled);
        }
        case e_Icb_RescueStatus_notActive:
        {
            return gen_conn::IpCommandBusTypes::TEM_RescueStatus(
                gen_conn::IpCommandBusTypes::TEM_RescueStatus::kNotActive);
        }
        case e_Icb_RescueStatus_serviceNotAvailable:
        {
            return gen_conn::IpCommandBusTypes::TEM_RescueStatus(
                gen_conn::IpCommandBusTypes::TEM_RescueStatus::kServiceNotAvailable);
        }
        case e_Icb_RescueStatus_active:
        {
            return gen_conn::IpCommandBusTypes::TEM_RescueStatus(
                gen_conn::IpCommandBusTypes::TEM_RescueStatus::kActive);
        }
        case e_Icb_RescueStatus_terminated:
        {
            return gen_conn::IpCommandBusTypes::TEM_RescueStatus(
                gen_conn::IpCommandBusTypes::TEM_RescueStatus::kTerminated);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::TEM_RescueStatus(
                gen_conn::IpCommandBusTypes::TEM_RescueStatus::kDisabled);
        }
    }
}

gen_conn::IpCommandBusTypes::TEM_VoiceStatus IpCommandBusTEMVoiceStatusToCapiType(const Icb_VoiceStatus status)
{
    switch (status)
    {
        case e_Icb_VoiceStatus_noConnection:
        {
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kNoConnection);
        }
        case e_Icb_VoiceStatus_connectingCSC:
        {
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectingCSC);
        }
        case e_Icb_VoiceStatus_connectingPSAP:
        {
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectingPSAP);
        }
        case e_Icb_VoiceStatus_connectedCSC:
        {
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectedCSC);
        }
        case e_Icb_VoiceStatus_connectedPSAP:
        {
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectedPSAP);
        }
        case e_Icb_VoiceStatus_incomingCall:
        {
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kIncomingCall);
        }

        case e_Icb_VoiceStatus_connectedCall:
        {
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectedCall);
        }
        case e_Icb_VoiceStatus_connectedIncoming:
        {
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kConnectedIncoming);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::TEM_VoiceStatus(
                gen_conn::IpCommandBusTypes::TEM_VoiceStatus::kNoConnection);
        }
    }
}

gen_conn::IpCommandBusTypes::SourceStatus IpCommandBusSourceStatusToCapiType(const Icb_SourceStatus status)
{
    switch (status)
    {
        case e_Icb_SourceStatus_unidentified:
        {
            return gen_conn::IpCommandBusTypes::SourceStatus(gen_conn::IpCommandBusTypes::SourceStatus::kUnidentified);
        }
        case e_Icb_SourceStatus_trusted:
        {
            return gen_conn::IpCommandBusTypes::SourceStatus(gen_conn::IpCommandBusTypes::SourceStatus::kTrusted);
        }
        case e_Icb_SourceStatus_callCenter:
        {
            return gen_conn::IpCommandBusTypes::SourceStatus(gen_conn::IpCommandBusTypes::SourceStatus::kCallCenter);
        }
        case e_Icb_SourceStatus_psap:
        {
            return gen_conn::IpCommandBusTypes::SourceStatus(gen_conn::IpCommandBusTypes::SourceStatus::Kpsap);
        }
        case e_Icb_SourceStatus_outgoing:
        {
            return gen_conn::IpCommandBusTypes::SourceStatus(gen_conn::IpCommandBusTypes::SourceStatus::kOutgoing);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::SourceStatus(gen_conn::IpCommandBusTypes::SourceStatus::kUnidentified);
        }
    }
}

gen_conn::IpCommandBusTypes::TEM_MessageStatus IpCommandBusTEMMessageStatusToCapiType(const Icb_MessageStatus status)
{
    switch (status)
    {
        case e_Icb_MessageStatus_notSent:
        {
            return gen_conn::IpCommandBusTypes::TEM_MessageStatus(
                gen_conn::IpCommandBusTypes::TEM_MessageStatus::kNotSent);
        }
        case e_Icb_MessageStatus_sending:
        {
            return gen_conn::IpCommandBusTypes::TEM_MessageStatus(
                gen_conn::IpCommandBusTypes::TEM_MessageStatus::kSending);
        }
        case e_Icb_MessageStatus_sent:
        {
            return gen_conn::IpCommandBusTypes::TEM_MessageStatus(
                gen_conn::IpCommandBusTypes::TEM_MessageStatus::kSent);
        }
        case e_Icb_MessageStatus_ackUndefined:
        {
            return gen_conn::IpCommandBusTypes::TEM_MessageStatus(
                gen_conn::IpCommandBusTypes::TEM_MessageStatus::kAckUndefined);
        }
        case e_Icb_MessageStatus_ackDefined:
        {
            return gen_conn::IpCommandBusTypes::TEM_MessageStatus(
                gen_conn::IpCommandBusTypes::TEM_MessageStatus::kAckDefined);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::TEM_MessageStatus(
                gen_conn::IpCommandBusTypes::TEM_MessageStatus::kNotSent);
        }
    }
}

gen_conn::IpCommandBusTypes::TEM_ButtonStatus IpCommandBusTEMButtonStatusToCapiType(const Icb_ButtonStatus status)
{
    switch (status)
    {
        case e_Icb_ButtonStatus_disabled:
        {
            return gen_conn::IpCommandBusTypes::TEM_ButtonStatus(
                gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kDisabled);
        }
        case e_Icb_ButtonStatus_onCallPressed:
        {
            return gen_conn::IpCommandBusTypes::TEM_ButtonStatus(
                gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kOnCallPressed);
        }
        case e_Icb_ButtonStatus_onCallReleased:
        {
            return gen_conn::IpCommandBusTypes::TEM_ButtonStatus(
                gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kOnCallReleased);
        }
        case e_Icb_ButtonStatus_sosPressed:
        {
            return gen_conn::IpCommandBusTypes::TEM_ButtonStatus(
                gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kSosPressed);
        }
        case e_Icb_ButtonStatus_sosReleased:
        {
            return gen_conn::IpCommandBusTypes::TEM_ButtonStatus(
                gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kSosReleased);
        }
        case e_Icb_ButtonStatus_allReleased:
        {
            return gen_conn::IpCommandBusTypes::TEM_ButtonStatus(
                gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kAllReleased);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::TEM_ButtonStatus(
                gen_conn::IpCommandBusTypes::TEM_ButtonStatus::kDisabled);
        }
    }
}

gen_conn::IpCommandBusTypes::TEM_PSAPStatus IpCommandBusTEMPSAPStatusToCapiType(const Icb_PSAPStatus status)
{
    switch (status)
    {
        case e_Icb_PSAPStatus_confirmRequired:
        {
            return gen_conn::IpCommandBusTypes::TEM_PSAPStatus(
                gen_conn::IpCommandBusTypes::TEM_PSAPStatus::kConfirmRequired);
        }
        case e_Icb_PSAPStatus_confirmNotRequired:
        {
            return gen_conn::IpCommandBusTypes::TEM_PSAPStatus(
                gen_conn::IpCommandBusTypes::TEM_PSAPStatus::kConfirmNotRequired);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::TEM_PSAPStatus(
                gen_conn::IpCommandBusTypes::TEM_PSAPStatus::kConfirmRequired);  //?
        }
    }
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_CoordinatesLongLat_WGS84 IpCommandBusCoordinatesLongLatToCapiType(
    const Icb_CoordinatesLongLat icb_longlat)
{
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_CoordinatesLongLat_WGS84 capi_longlat;
    capi_longlat.setLatitude(icb_longlat->latitude);
    capi_longlat.setLongitude(icb_longlat->longitude);

    return capi_longlat;
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DateTime IpCommandBusDateTimeToCapiType(
    const Icb_DateTime icb_datetime)
{
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DateTime capi_datetime;
    capi_datetime.setYear(icb_datetime->year);
    capi_datetime.setMonth(icb_datetime->month);
    capi_datetime.setDay(icb_datetime->day);
    capi_datetime.setHour(icb_datetime->hour);
    capi_datetime.setMinute(icb_datetime->minute);
    capi_datetime.setSecond(icb_datetime->second);

    return capi_datetime;
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType IpCommandBusGnssFixTypeToCapiType(
    const Icb_GnssFixType fix_type)
{
    switch (fix_type)
    {
        case e_Icb_GnssFixType_notAvailable:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_notAvailable);
        }
        case e_Icb_GnssFixType_noFix:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_noFix);
        }
        case e_Icb_GnssFixType_fix2D:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_fix2D);
        }
        case e_Icb_GnssFixType_fix3D:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_fix3D);
        }
        case e_Icb_GnssFixType_startupMode:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_startupMode);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType::kGnssFixType_notAvailable);
        }
    }
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType IpCommandBusDeadReckoningTypeToCapiType(
    const Icb_DeadReckoningType dr_type)
{
    switch (dr_type)
    {
        case e_Icb_DeadReckoningType_noDr:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::kNoDr);
        }
        case e_Icb_DeadReckoningType_drNoMapMatch:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::kDrNoMapMatch);
        }
        case e_Icb_DeadReckoningType_drMapMatched:
        {
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::kDrMapMatched);
        }
        default:
        {
            assert(false);
            return gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType(
                gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType::kNoDr);
        }
    }
}

gen_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition IpCommandBusTEMSimpleVehiclePositionToCapiType(
    const Icb_SimpleVehiclePosition icb_position)
{
    std::string no_valid_data = "";
    gen_conn::IpCommandBusTypes::TEM_WGS84SimplePositionData capi_wgs84;

    Icb_WGS84SimplePositionData icb_wgs84 = icb_position->wgs84;
    if (e_Icb_SimpleVehiclePosition_wgs84 == icb_position->choice)
    {
        capi_wgs84.setLongLat(IpCommandBusCoordinatesLongLatToCapiType(icb_wgs84->longLat));
        capi_wgs84.setFixTime(IpCommandBusDateTimeToCapiType(icb_wgs84->fixTime));
        capi_wgs84.setFixType(IpCommandBusGnssFixTypeToCapiType(icb_wgs84->fixType));
        capi_wgs84.setDrType(IpCommandBusDeadReckoningTypeToCapiType(icb_wgs84->drType));

        gen_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition capi_position(capi_wgs84);
        return capi_position;
    }
    else
    {
        gen_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition capi_position(no_valid_data);
        return capi_position;
    }
}

gen_conn::IpCommandBusTypes::PSIMConnectionStatus PSIMConnectStatusToCapiType(
    Icb_PSIMConnectionStatus_t PSIMConnectionStatus)
{
    gen_conn::IpCommandBusTypes::PSIMConnectionStatus connectionStatus =
        gen_conn::IpCommandBusTypes::PSIMConnectionStatus::kProhibited;
    switch (PSIMConnectionStatus)
    {
        case e_Icb_PSIMConnectionStatus_connectedHome:
            connectionStatus = gen_conn::IpCommandBusTypes::PSIMConnectionStatus(
                gen_conn::IpCommandBusTypes::PSIMConnectionStatus::kConnectedHome);
            break;
        case e_Icb_PSIMConnectionStatus_connectedRoaming:
            connectionStatus = gen_conn::IpCommandBusTypes::PSIMConnectionStatus(
                gen_conn::IpCommandBusTypes::PSIMConnectionStatus::kConnectedRoaming);
            break;
        case e_Icb_PSIMConnectionStatus_connecting:
            connectionStatus = gen_conn::IpCommandBusTypes::PSIMConnectionStatus(
                gen_conn::IpCommandBusTypes::PSIMConnectionStatus::kConnecting);
            break;
        case e_Icb_PSIMConnectionStatus_disconnected:
            connectionStatus = gen_conn::IpCommandBusTypes::PSIMConnectionStatus(
                gen_conn::IpCommandBusTypes::PSIMConnectionStatus::kDisconnected);
            break;
        case e_Icb_PSIMConnectionStatus_prohibited:
            connectionStatus = gen_conn::IpCommandBusTypes::PSIMConnectionStatus(
                gen_conn::IpCommandBusTypes::PSIMConnectionStatus::kProhibited);
            break;
    }

    return connectionStatus;
}

gen_conn::IpCommandBusTypes::WirelessTechnology WirelessTechnologyToCapiType(
    Icb_WirelessTechnology_t wirelessTechnology)
{
    gen_conn::IpCommandBusTypes::WirelessTechnology technology =
        gen_conn::IpCommandBusTypes::WirelessTechnology::kUnknown;
    switch (wirelessTechnology)
    {
        case e_Icb_WirelessTechnology_unknown:
            technology = gen_conn::IpCommandBusTypes::WirelessTechnology(
                gen_conn::IpCommandBusTypes::WirelessTechnology::kUnknown);
            break;
        case e_Icb_WirelessTechnology_gprs:
            technology =
                gen_conn::IpCommandBusTypes::WirelessTechnology(gen_conn::IpCommandBusTypes::WirelessTechnology::kGprs);
            break;
        case e_Icb_WirelessTechnology_edge:
            technology =
                gen_conn::IpCommandBusTypes::WirelessTechnology(gen_conn::IpCommandBusTypes::WirelessTechnology::kEdge);
            break;
        case e_Icb_WirelessTechnology_umts:
            technology =
                gen_conn::IpCommandBusTypes::WirelessTechnology(gen_conn::IpCommandBusTypes::WirelessTechnology::kUmts);
            break;
        case e_Icb_WirelessTechnology_hsdpa:
            technology = gen_conn::IpCommandBusTypes::WirelessTechnology(
                gen_conn::IpCommandBusTypes::WirelessTechnology::kHsdpa);
            break;
        case e_Icb_WirelessTechnology_hsupa:
            technology = gen_conn::IpCommandBusTypes::WirelessTechnology(
                gen_conn::IpCommandBusTypes::WirelessTechnology::kHsupa);
            break;
        case e_Icb_WirelessTechnology_lte:
            technology =
                gen_conn::IpCommandBusTypes::WirelessTechnology(gen_conn::IpCommandBusTypes::WirelessTechnology::kLte);
            break;
    }

    return technology;
}

gen_conn::IpCommandBusTypes::NetworkRegistrationStatus NetworkRegistrationStatusToCapiType(
    Icb_RegisterState_Choices registerState)
{
    switch (registerState)
    {
        case e_Icb_RegisterState_registered:
            return gen_conn::IpCommandBusTypes::NetworkRegistrationStatus(
                gen_conn::IpCommandBusTypes::NetworkRegistrationStatus::kRegistered);
            break;
        case e_Icb_RegisterState_regRoaming:
            return gen_conn::IpCommandBusTypes::NetworkRegistrationStatus(
                gen_conn::IpCommandBusTypes::NetworkRegistrationStatus::kRegistered_Roaming);
            break;
        case e_Icb_RegisterState_notRegSearching:
            return gen_conn::IpCommandBusTypes::NetworkRegistrationStatus(
                gen_conn::IpCommandBusTypes::NetworkRegistrationStatus::kNotRegistedSearching);
            break;
        case e_Icb_RegisterState_notRegNotSearching:
            return gen_conn::IpCommandBusTypes::NetworkRegistrationStatus(
                gen_conn::IpCommandBusTypes::NetworkRegistrationStatus::kNotRegistedNotSearching);
            break;
        case e_Icb_RegisterState_regDenied:
            return gen_conn::IpCommandBusTypes::NetworkRegistrationStatus(
                gen_conn::IpCommandBusTypes::NetworkRegistrationStatus::kRegistrationDenied);
            break;
    }

    return gen_conn::IpCommandBusTypes::NetworkRegistrationStatus(
        gen_conn::IpCommandBusTypes::NetworkRegistrationStatus::kRegistrationDenied);
}

gen_conn::IpCommandBusTypes::SecurityStatus SecurityStatusToCapiType(const Icb_SecurityStatus& status)
{
    switch (status)
    {
        case e_Icb_SecurityStatus_idle:
            return gen_conn::IpCommandBusTypes::SecurityStatus(gen_conn::IpCommandBusTypes::SecurityStatus::kIdle);
            break;
        case e_Icb_SecurityStatus_standby:
            return gen_conn::IpCommandBusTypes::SecurityStatus(gen_conn::IpCommandBusTypes::SecurityStatus::kStandby);
            break;
        case e_Icb_SecurityStatus_active:
            return gen_conn::IpCommandBusTypes::SecurityStatus(gen_conn::IpCommandBusTypes::SecurityStatus::kActive);
            break;
        case e_Icb_SecurityStatus_activeStandalone:
            return gen_conn::IpCommandBusTypes::SecurityStatus(
                gen_conn::IpCommandBusTypes::SecurityStatus::kActiveStandalone);
            break;
    }

    return gen_conn::IpCommandBusTypes::SecurityStatus(gen_conn::IpCommandBusTypes::SecurityStatus::kIdle);
}

Icb_ErrorCode_t callStatusToErrorCode(const CommonAPI::CallStatus& callStatus)
{
    switch (callStatus)
    {
        case CommonAPI::CallStatus::OUT_OF_MEMORY:
            return Icb_ErrorCode_t::e_Icb_ErrorCode_busy;
        case CommonAPI::CallStatus::NOT_AVAILABLE:
            return Icb_ErrorCode_t::e_Icb_ErrorCode_busy;
        case CommonAPI::CallStatus::CONNECTION_FAILED:
            return Icb_ErrorCode_t::e_Icb_ErrorCode_timeout;
        case CommonAPI::CallStatus::REMOTE_ERROR:
            return Icb_ErrorCode_t::e_Icb_ErrorCode_applicationError;
        case CommonAPI::CallStatus::UNKNOWN:
            return Icb_ErrorCode_t::e_Icb_ErrorCode_applicationError;
        case CommonAPI::CallStatus::INVALID_VALUE:
            return Icb_ErrorCode_t::e_Icb_ErrorCode_parameterInvalid;
        case CommonAPI::CallStatus::SUBSCRIPTION_REFUSED:
            return Icb_ErrorCode_t::e_Icb_ErrorCode_applicationError;
        default:
            return Icb_ErrorCode_t::e_Icb_ErrorCode_notOk;
    }
}

uint8_t WeekdaysToByte(v0_climate::ParkingClimate::WeekdaysVec weekdays)
{
    uint8_t ret = 0;

    for (auto weekday : weekdays)
    {
        ret |= 1 << static_cast<int32_t>(weekday);
    }

    return ret;
}

v0_climate::ParkingClimate::WeekdaysVec ByteToWeekdays(const uint8_t& c)
{
    // Helper function
    v0_climate::ParkingClimate::WeekdaysVec weekdays;

    for (unsigned int i = 0; i < 8; ++i)
    {
        if (c & (1 << static_cast<int32_t>(i)))
        {
            weekdays.push_back(static_cast<v0_climate::ParkingClimate::WeekdayEnum::Literal>(i));
        }
    }

    return weekdays;
}

void ParkTimersToString(v0_climate::ParkingClimate::ParkingTimers parkTimers, ASN_BYTE* byte_data)
{
    // Helper function
    uint16_t position = 0;
    for (auto timer : parkTimers)
    {
        byte_data[position] = timer.getTimerId();
        position++;
        byte_data[position] = timer.getActive() == v0_climate::ParkingClimate::TimerStateEnum::OFF ? 1 : 2;
        position++;
        byte_data[position] = timer.getTimerType();
        position++;
        byte_data[position] = WeekdaysToByte(timer.getWeekdays());
        position++;

        v0_climate::ParkingClimate::DateTimeStruct date = timer.getDateTime();

        byte_data[position] = date.getYear() - 2000;
        position++;  // IPCB year starts at 2000
        byte_data[position] = date.getMonth();
        position++;
        byte_data[position] = date.getDay();
        position++;
        byte_data[position] = date.getHour();
        position++;
        byte_data[position] = date.getMinute();
        position++;
        byte_data[position] = date.getSecond();
        position++;
    }
}

Icb_Setting ParkingTimersToIpcbType(ASN_Session session, v0_climate::ParkingClimate::ParkingTimers parkTimers)
{
    Icb_Setting setting = Icb_Setting_Create(session);
    setting->id = 0x02;
    setting->sType = Icb_SettingType_t::e_Icb_SettingType_setting;
    setting->length = 321;  // We are only suppose to send out resp/notification, which length is 321

    ASN_BYTE byte_data[321] = {0};

    // Add error byte first in data
    // byte_data[0] = 0; //TODO: currently no error will be reported.

    // Add timers after error byte
    ParkTimersToString(parkTimers, &byte_data[1]);

    ASN_OctetString value = CapiStringTypeToAsnOctetString(session, byte_data, 321);
    setting->value = value;

    return setting;
}

v0_climate::ParkingClimate::ParkingTimers SettingToCAPIType(const Icb_Setting& setting)
{
    std::vector<v0_climate::ParkingClimate::ParkingTimerStruct> timers;

    ASN_BYTE byte_data[1024] = {0};
    uint32_t length;
    AsnOctetStringToCapiType(setting->value, &byte_data[0], length);

    if (length % 10)
    {
        // Not evenly divided by 10 -> not correct size
        log_error().writeFormatted("SettingToCAPIType: %d not evenly divisible by 10!", length);
        length = (length / 10) * 10;  // Remove excess bytes
    }

    for (uint32_t position = 0; position < length;)
    {
        // Check if year is 0. If so, timer is most likely not defined in data and parsing should be terminated
        // since the rest of the timers should also be undefined
        if (0 == byte_data[position + 4])
        {
            break;
        }

        v0_climate::ParkingClimate::ParkingTimerStruct timer;

        timer.setTimerId(byte_data[position]);
        position++;

        if (byte_data[position] == 1)
        {
            timer.setActive(v0_climate::ParkingClimate::TimerStateEnum::OFF);
        }
        else if (byte_data[position] == 2)
        {
            timer.setActive(v0_climate::ParkingClimate::TimerStateEnum::ON);
        }
        else
        {
            // TODO error handling
            log_error().writeFormatted(
                "SettingToCAPIType: Invalid TimerState at position %d = %d", position, byte_data[position]);
        }
        position++;

        timer.setTimerType(static_cast<v0_climate::ParkingClimate::TimerTypeEnum::Literal>(byte_data[position]));
        position++;

        timer.setWeekdays(ByteToWeekdays(byte_data[position]));
        position++;

        v0_climate::ParkingClimate::DateTimeStruct date(byte_data[position] + 2000,  // year (IPCB year starts at 2000)
                                                        byte_data[position + 1],     // month
                                                        byte_data[position + 2],     // day
                                                        byte_data[position + 3],     // hour
                                                        byte_data[position + 4],     // minute
                                                        byte_data[position + 5]);    // second

        timer.setDateTime(date);
        position += 6;

        // timer ready, add to the timers vector
        timers.push_back(timer);
    }

    return timers;
}

Icb_AudioStatus AudioStatusToIpcbType(ASN_Session session, const gen_conn::IpCommandBusTypes::AudioStatus audioStatus)
{
    Icb_AudioStatus ret = Icb_AudioStatus_Create(session);

    ret->micStatus = IpCommandBusGenericOkStatusToIcbType(audioStatus.getMicStatus());
    ret->speakerStatus = IpCommandBusGenericOkStatusToIcbType(audioStatus.getSpeakerStatus());
    return ret;
}

bool PinPukCommandToIpcbType(ASN_Session session,
                             gen_conn::IpCommandBusTypes::CarModem_PinPukCommand inCommand,
                             Icb_PinPukCommand& outCommand)
{
    outCommand = Icb_PinPukCommand_Create(session);
    assert(outCommand);

    gen_conn::IpCommandBusTypes::PinPukCommand_Choice c = inCommand.getChoice();
    gen_conn::IpCommandBusTypes::PinPukCommand_data data = inCommand.getData();

    switch (c)
    {
        case gen_conn::IpCommandBusTypes::PinPukCommand_Choice::kEnterPin:
        {
            outCommand->choice = e_Icb_PinPukCommand_enterPin;

            std::string s(data.get<std::string>());
            outCommand->enterPin = CapiStringTypeToAsnOctetString(session, s);
        }
        break;

        case gen_conn::IpCommandBusTypes::PinPukCommand_Choice::kEnterPuk:
        {
            outCommand->choice = e_Icb_PinPukCommand_enterPuk;

            Icb_PinPukCodes ppc = Icb_PinPukCodes_Create(session);
            assert(ppc);

            gen_conn::IpCommandBusTypes::PinPukCodes p(data.get<gen_conn::IpCommandBusTypes::PinPukCodes>());
            ppc->newpin = CapiStringTypeToAsnOctetString(session, p.getNewpin());
            ppc->puk = CapiStringTypeToAsnOctetString(session, p.getPuk());
            outCommand->enterPuk = ppc;
        }
        break;

        case gen_conn::IpCommandBusTypes::PinPukCommand_Choice::kChangePin:
        {
            outCommand->choice = e_Icb_PinPukCommand_changePin;

            Icb_PinPinCodes ppc = Icb_PinPinCodes_Create(session);
            assert(ppc);

            gen_conn::IpCommandBusTypes::PinPinCodes p(data.get<gen_conn::IpCommandBusTypes::PinPinCodes>());
            ppc->newpin = CapiStringTypeToAsnOctetString(session, p.getNewpin());
            ppc->oldpin = CapiStringTypeToAsnOctetString(session, p.getOldpin());
            outCommand->changePin = ppc;
        }
        break;
        default:
        {
            // Error!
            log_error() << "Unexpected Choice, Failed to convert CarModem_PinPukCommand";
            return false;
        }
        break;
    }
    return true;
}
*/
Icb_DateTime TimeToIpcbType(ASN_Session session, std::time_t time)
{
    std::tm* pTime;
    pTime = gmtime(&time);

    Icb_DateTime ret = Icb_DateTime_Create(session);
    assert(ret);
    ret->year = static_cast<U32>((pTime->tm_year) + 1900);
    ret->month = static_cast<U32>((pTime->tm_mon) + 1);
    ret->day = static_cast<U32>(pTime->tm_mday);
    ret->hour = static_cast<U32>(pTime->tm_hour);
    ret->minute = static_cast<U32>(pTime->tm_min);
    ret->second = static_cast<U32>(pTime->tm_sec);

    return ret;
}
/*
gen_conn::IpCommandBusTypes::DiagMgm_RemoteDiagInfo RemoteDiagInfoToCapiType(const Icb_RemoteDiagInfo_t& in)
{
    using out = gen_conn::IpCommandBusTypes::DiagMgm_RemoteDiagInfo;
    switch (in)
    {
        case Icb_RemoteDiagInfo_t::e_Icb_RemoteDiagInfo_closed:
            return out::closed;
        case Icb_RemoteDiagInfo_t::e_Icb_RemoteDiagInfo_started:
            return out::started;
        case Icb_RemoteDiagInfo_t::e_Icb_RemoteDiagInfo_failed:
        default:
            return out::failed;
    }
}
gen_conn::IpCommandBusTypes::DiagMgm_DoIPMode DoIPModeToCapiType(const Icb_DoIPMode_t& in)
{
    using out = gen_conn::IpCommandBusTypes::DiagMgm_DoIPMode;
    switch (in)
    {
        case Icb_DoIPMode_t::e_Icb_DoIPMode_local:
            return out::local;
        case Icb_DoIPMode_t::e_Icb_DoIPMode_remote:
            return out::remote;
        case Icb_DoIPMode_t::e_Icb_DoIPMode_none:
        default:
            return out::none;
    }
}
gen_conn::IpCommandBusTypes::DiagMgm_DoIPPhase DoIPPhaseToCapiType(const Icb_DoIPPhase_t& in)
{
    using out = gen_conn::IpCommandBusTypes::DiagMgm_DoIPPhase;
    switch (in)
    {
        case Icb_DoIPPhase_t::e_Icb_DoIPPhase_activation:
            return out::activation;
        case Icb_DoIPPhase_t::e_Icb_DoIPPhase_announcement:
            return out::announcement;
        case Icb_DoIPPhase_t::e_Icb_DoIPPhase_session:
            return out::session;
        case Icb_DoIPPhase_t::e_Icb_DoIPPhase_none:
        default:
            return out::none;
    }
}
gen_conn::IpCommandBusTypes::DiagMgm_DoIPState DoIPStateToCapiType(const Icb_OpCurrentDoIPState_Notification& in)
{
    gen_conn::IpCommandBusTypes::DiagMgm_DoIPState retval(
        in->doIPState,
        DoIPModeToCapiType(static_cast<Icb_DoIPMode_t>(in->doIPMode)),
        DoIPPhaseToCapiType(static_cast<Icb_DoIPPhase_t>(in->doIPPhase)));
    return retval;
}

gen_conn::IpCommandBusTypes::DiagMgm_CurrentDoIPConn DoIPConnToCapiType(const Icb_CurrentDoIPConn& in)
{
    using out = gen_conn::IpCommandBusTypes::DiagMgm_CurrentDoIPConn;
    switch (in)
    {
        case Icb_CurrentDoIPConn_t::e_Icb_CurrentDoIPConn_ethernetlan:
            return out::ethernetlan;
        case Icb_CurrentDoIPConn_t::e_Icb_CurrentDoIPConn_ethernetp2p:
            return out::ethernetp2p;
        case Icb_CurrentDoIPConn_t::e_Icb_CurrentDoIPConn_phone:
            return out::phone;
        case Icb_CurrentDoIPConn_t::e_Icb_CurrentDoIPConn_wlan:
            return out::wlan;
        case Icb_CurrentDoIPConn_t::e_Icb_CurrentDoIPConn_none:
            return out::none;
        default:
            return out::none;
    }
}

Icb_UserAuthCredentials UserAuthCredentialsToIpcbType(
    const gen_conn::IpCommandBusTypes::DiagMgm_UserAuthCredentials& in)
{
    switch (in)
    {
        case gen_conn::IpCommandBusTypes::DiagMgm_UserAuthCredentials::accept:
            return Icb_UserAuthCredentials_t::e_Icb_UserAuthCredentials_accept;
        case gen_conn::IpCommandBusTypes::DiagMgm_UserAuthCredentials::reject:
        default:
            return Icb_UserAuthCredentials_t::e_Icb_UserAuthCredentials_reject;
    }
}

Icb_UpdateDacMode UpdateDacModeToIpcbType(const gen_conn::IpCommandBusTypes::DiagMgm_UpdateDacMode& in)
{
    switch (in)
    {
        case gen_conn::IpCommandBusTypes::DiagMgm_UpdateDacMode::automatic:
            return Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_automatic;
        case gen_conn::IpCommandBusTypes::DiagMgm_UpdateDacMode::semiautomatic:
            return Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_semiautomatic;
        case gen_conn::IpCommandBusTypes::DiagMgm_UpdateDacMode::manual:
        default:
            log_warning() << __FUNCTION__ << "Unhandled DacMode, returing default value...";
            return Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_manual;
    }
}

Icb_UpdateDacMode WorkshopWlanDaCModeToIpcbType(const gen_conn::WorkshopWlan::DaCMode& mode)
{
    switch (mode)
    {
        case gen_conn::WorkshopWlan::DaCMode::AUTOMATIC:
            return Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_automatic;
        case gen_conn::WorkshopWlan::DaCMode::SEMI_AUTOMATIC:
            return Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_semiautomatic;
        case gen_conn::WorkshopWlan::DaCMode::MANUAL:
            return Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_manual;
        default:
            log_warning() << __FUNCTION__ << "Unhandled DacMode, returing default value...";
            return Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_manual;
    }
}

gen_conn::WorkshopWlan::DaCMode WorkshopWlanIpcbTypeDaCModeToCapiType(const Icb_UpdateDacMode& mode)
{
    switch (mode)
    {
        case Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_automatic:
            return gen_conn::WorkshopWlan::DaCMode::AUTOMATIC;
        case Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_semiautomatic:
            return gen_conn::WorkshopWlan::DaCMode::SEMI_AUTOMATIC;
        case Icb_UpdateDacMode_t::e_Icb_UpdateDacMode_manual:
            return gen_conn::WorkshopWlan::DaCMode::MANUAL;
        default:
            log_warning() << __FUNCTION__ << "Unhandled DacMode, returing default value...";
            return gen_conn::WorkshopWlan::DaCMode::MANUAL;
    }
}

gen_conn::WorkshopWlan::WorkshopWlanConnectionResult IpCommandBusWorkshopWlanConnectionResultToCapiType(
    Icb_WorkshopWlanConnectionResult connection_result)
{
    switch (connection_result)
    {
        case e_Icb_WorkshopWlanConnectionResult_connected:
        {
            return gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::CONNECTED;
        }
        case e_Icb_WorkshopWlanConnectionResult_failed_no_aps:
        {
            return gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_NO_APS;
        }
        case e_Icb_WorkshopWlanConnectionResult_failed_authentication:
        {
            return gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_AUTHENTICATION;
        }
        case e_Icb_WorkshopWlanConnectionResult_failed_timeout:
        {
            return gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_TIMEOUT;
        }
        case e_Icb_WorkshopWlanConnectionResult_failed_general:
        {
            return gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_GENERAL;
        }
        default:
        {
            log_warning() << __FUNCTION__ << "Unhandled DacMode, returing default value...";
            return gen_conn::WorkshopWlan::WorkshopWlanConnectionResult::FAILED_GENERAL;
        }
    }
}
*/
}  // namespace Utils
}  // namespace InfotainmentIpBus
