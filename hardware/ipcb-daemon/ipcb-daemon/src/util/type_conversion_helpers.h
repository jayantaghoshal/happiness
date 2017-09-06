/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef TYPE_CONVERSION_HELPER_H
#define TYPE_CONVERSION_HELPER_H

#include <ipcommandbus/TransportServices.h>

#include <cstring>

extern "C" {
#include "infotainmentIpBus.h"
#include "pl/asn_base/asn_base.h"
}

namespace InfotainmentIpService
{
namespace Utils
{

/*
Convert from UTC time Icb_DateTime_t to ms since 1970-01-01 00:00:00
*/
int64_t ToMsSince1970(const Icb_DateTime_t* utc_time);

//// 'ToString' functions (mainly for logging purposes)


/**
 * Convert 'GNSS_FIX_STATUS' from PositionInfo map (EnhancedPositionServiceTypes.fidl) to ipbus FixType enum.
 *
 * Note: Although this is effectively used as an enum, it is in fact stored as uint64_t in map,
 *       and the enum constants values are only comments,
 *       i.e. not actually declared in either .fidl files or the generated header files.
 *       (Looks like the Franca maps do not yet support storage of enums...)
 *
 * @param[in] gnss_fix_status    Franca fix status 'enum'.
 */

std::string AsnErrCodeToString(ASN_Errcode code);

//// CommonAPI <-> ASN conversion functions

inline void AsnOctetStringToCapiType(ASN_OctetString p_string, ASN_BYTE* byte_data, U32& length)
{
    ASN_BYTE* data = nullptr;
    length = ASN_OctetString_Get(p_string, &data);

    if (!data)
    {
        // log_error().writeFormatted("%s - ASN_OctetString_Get returned NULL pointer in data!", __FUNCTION__);
        return;
    }

    memcpy(byte_data, data, length);
}

/*
template <class StringType>
inline StringType AsnOctetStringToCapiType(ASN_OctetString p_string)
{
    ASN_BYTE* data = nullptr;
    const U32 length = ASN_OctetString_Get(p_string, &data);

    if (!data)
    {
        // log_error().writeFormatted("%s - NULL octet string pointer!", __FUNCTION__);
        return "";
    }

    StringType ret(reinterpret_cast<char*>(data), static_cast<std::size_t>(length));
    return std::move(ret);
}
*/

inline std::string AsnUtf8StringToCapiType(Icb_UUID p_string)
{
    ASN_BYTE* data = nullptr;
    const U32 length = ASN_UTF8String_Get(p_string->uuid, &data);

    if (!data)
    {
        return "";
    }

    std::string ret(reinterpret_cast<char*>(data), static_cast<std::size_t>(length));
    return ret;
}

inline std::string AsnUtf8StringToCapiType(Icb_PartIdentifier p_string)
{
    ASN_BYTE* data = nullptr;
    const U32 length = ASN_UTF8String_Get(p_string->partID, &data);

    if (!data)
    {
        return "";
    }

    std::string ret(reinterpret_cast<char*>(data), static_cast<std::size_t>(length));
    return ret;
}

inline std::string AsnUtf8StringToCapiType(ASN_UTF8String p_string)
{
    ASN_BYTE* data = nullptr;
    const U32 length = ASN_UTF8String_Get(p_string, &data);

    if (!data)
    {
        return "";
    }

    std::string ret(reinterpret_cast<char*>(data), static_cast<std::size_t>(length));
    return ret;
}

inline ASN_OctetString CapiStringTypeToAsnOctetString(ASN_Session session, ASN_BYTE* data, U32 length)
{
    ASN_OctetString str_ptr = ASN_OctetString_Create(session);
    ASN_OctetString_Set(str_ptr, session, data, length);

    return str_ptr;
}

inline ASN_OctetString CapiStringTypeToAsnOctetString(ASN_Session session, std::string string)
{
    ASN_OctetString str_ptr = ASN_OctetString_Create(session);
    ASN_OctetString_SetText(str_ptr, session, const_cast<char*>(string.c_str()));

    return str_ptr;
}
inline ASN_UTF8String CapiStringTypeToAsnUtf8String(ASN_Session session, const std::string& string)
{
    ASN_UTF8String str_ptr = ASN_UTF8String_Create(session);
    ASN_UTF8String_SetText(str_ptr, session, const_cast<char*>(string.c_str()));

    return str_ptr;
}
// clang-format off
/*
Icb_Ecu_t
    EcuToIpcbType(const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu &ecu);

Icb_OnOffSetting_t
    OnOffSettingToIpcbType(const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting &on_off);

Icb_InternetSource_t
    InternetSourceToIpcbType(const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource &internet_source);

Icb_WLANMode WlanModeToIpcbType(const gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode mode);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu
    EcuToCapiType(const Icb_Ecu_t gateway_ecu);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_OnOffSetting
    OnOffSettingToCapiType(const Icb_OnOffSetting_t on_off);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_InternetSource
    InternetSourceToCapiType(const Icb_InternetSource_t internet_source);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors
    GenericErrorTypeToCapiType(const Connectivity::TransportServices::ErrorType error_type,
                        Icb_OpGeneric_Error_t error_info);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode WlanModeToCapiType(const Icb_WLANMode_t mode);

// Workaround for generated code.
// (For enum types we get Icb_Xxx from the generated decode function, but enum is in fact declared in Icb_Xxx_t.)
inline gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanMode WlanModeToCapiType(const Icb_WLANMode mode)
{
    return WlanModeToCapiType( static_cast<Icb_WLANMode_t>(mode) );
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus WlanConnectionStatusToCapiType(Icb_WLANConnectionStatus_t status);

inline gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus WlanConnectionStatusToCapiType(Icb_WLANConnectionStatus status)
{
    return WlanConnectionStatusToCapiType( static_cast<Icb_WLANConnectionStatus_t>(status) );
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus WlanConnectStatusToCapiType(Icb_WLANConnectStatus_t status);

inline gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus WlanConnectStatusToCapiType(Icb_WLANConnectStatus status)
{
    return WlanConnectStatusToCapiType( static_cast<Icb_WLANConnectStatus_t>(status) );
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid WlanSsidToCapiType(ASN_OctetString ssid);

std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ssid> WlanArrayOfSsidToCapiType(Icb_ArrayOf_SSID p_array_ssid);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType WlanSecurityTypeToCapiType(Icb_WLANSecurityType_t type);

inline gen_conn::IpCommandBusTypes::Infotainment_IP_bus_WlanSecurityType WlanSecurityTypeToCapiType(Icb_WLANSecurityType type)
{
    return WlanSecurityTypeToCapiType(static_cast<Icb_WLANSecurityType_t>(type));
}

gen_conn::Wlan::WlanChannelWidthData WlanChannelWidthDataTypeToCapiType(Icb_WLANChannelWidthData_t type);
static inline gen_conn::Wlan::WlanChannelWidthData WlanChannelWidthDataTypeToCapiType(Icb_WLANChannelWidthData type)
{
    return WlanChannelWidthDataTypeToCapiType(static_cast<Icb_WLANChannelWidthData_t>(type));
}

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation WlanNetworkInfoToCapiType(Icb_AvailableNetworkInformation p_network_info);

gen_conn::IpCommandBusTypes::PSIMConnectionStatus PSIMConnectStatusToCapiType(Icb_PSIMConnectionStatus_t connectionStatus);

gen_conn::IpCommandBusTypes::NetworkRegistrationStatus NetworkRegistrationStatusToCapiType(Icb_RegisterState_Choices registerState);

gen_conn::IpCommandBusTypes::WirelessTechnology WirelessTechnologyToCapiType(Icb_WirelessTechnology_t wirelessTechnology);

bool PinPukCommandToIpcbType(ASN_Session session,
                             gen_conn::IpCommandBusTypes::CarModem_PinPukCommand inCommand,
                             Icb_PinPukCommand& outCommand);

std::vector<gen_conn::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation> WlanArrayOfNetworkInfoToCapiType(Icb_ArrayOf_AvailableNetworkInformation p_array_network_info);


std::string FixTypeToString(Icb_GnssFixType_t fix_type);
*/
/*
gen_conn::OTATypes::GenericOkStatus OTAGenericOkStatusToCapiType(const Icb_GenericOkStatus status);

gen_conn::IpCommandBusTypes::GenericOkStatus IpCommandBusGenericOkStatusToCapiType(const Icb_GenericOkStatus status);

Icb_GenericOkStatus IpCommandBusGenericOkStatusToIcbType(const gen_conn::IpCommandBusTypes::GenericOkStatus status);

gen_conn::OTATypes::DownloadStatus OTADownloadStatusToCapiType(const Icb_DownloadStatus download_status);

gen_conn::OTATypes::SoftwareProduct OTASoftwareProductToCapiType(Icb_SoftwareProduct software_product);

gen_conn::OTATypes::DownloadState OTADownloadStateToCapiType(Icb_DownloadState download_state);

std::vector<gen_conn::OTATypes::SoftwareProduct> OTAArrayOfSoftwareProductToCapiType(Icb_ArrayOf_SoftwareProduct softwre_product);

gen_conn::OTATypes::UUID OTADownloadSoftwareToCapiType(Icb_UUID uuid);

std::vector<gen_conn::OTATypes::UUID> OTAArrayOfDownloadSoftwareToCapiType(Icb_ArrayOf_UUID uuid);

gen_conn::OTATypes::PartIdentifier OTADeletedSoftwareToCapiType(Icb_PartIdentifier_t part_identifier);

std::vector<gen_conn::OTATypes::PartIdentifier> OTAArrayOfDeletedSoftwareToCapiType(Icb_ArrayOf_PartIdentifier p_part_identifier);

std::vector<gen_conn::OTATypes::InstallationData> ArrayOfInstallationDataToCapiType(Icb_ArrayOf_InstallationData installation_data);

gen_conn::OTATypes::InstallationData InstallationDataToCapiType(Icb_InstallationData installation_data);

gen_conn::OTATypes::InstallCommand InstallCommandToCapiType(Icb_InstallCommand install_command);

Icb_InstallStatus_t InstallStatusToIpcbType(gen_conn::OTATypes::InstallStatus);

Icb_ArrayOf_InstalledData ArrayOfInstalledDataToIpcbType(ASN_Session session, std::vector<gen_conn::OTATypes::InstalledData> installedList);

Icb_ArrayOf_UUID ArrayOfUUIDToIpcbType(ASN_Session session, std::vector<gen_conn::OTATypes::UUID> uuidList);

Icb_SBWarning sbWarningToIpcbType(ASN_Session session, gen_conn::IpCommandBusTypes::SBWarning warning);

Icb_ArrayOf_SBWarning ArrayOfsbWarningToIpcbType(ASN_Session session, std::vector<gen_conn::IpCommandBusTypes::SBWarning> warnings);

Icb_UUID OTAUuidToIpcbType(ASN_Session session, gen_conn::OTATypes::UUID uuid);

Icb_PartIdentifier PartIdentifierToIpcbType(gen_conn::OTATypes::PartIdentifier part_id);

Icb_ArrayOf_PartIdentifier ArrayOfPartIdentifierToIpcbType(ASN_Session session, std::vector<gen_conn::OTATypes::PartIdentifier> deleted_software);

gen_conn::IpCommandBusTypes::TEM_ActivationStatus
IpCommandBusTEMActivationStatusToCapiType(const Icb_ActivationStatus status);

gen_conn::IpCommandBusTypes::TEM_RescueStatus
IpCommandBusTEMRescueStatusToCapiType(const Icb_RescueStatus status);

gen_conn::IpCommandBusTypes::TEM_VoiceStatus
IpCommandBusTEMVoiceStatusToCapiType(const Icb_VoiceStatus status);

gen_conn::IpCommandBusTypes::SourceStatus
IpCommandBusSourceStatusToCapiType(const Icb_SourceStatus status);

gen_conn::IpCommandBusTypes::TEM_MessageStatus
IpCommandBusTEMMessageStatusToCapiType(const Icb_MessageStatus status);

gen_conn::IpCommandBusTypes::TEM_ButtonStatus
IpCommandBusTEMButtonStatusToCapiType(const Icb_ButtonStatus status);

gen_conn::IpCommandBusTypes::TEM_PSAPStatus
IpCommandBusTEMPSAPStatusToCapiType(const Icb_PSAPStatus status);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_CoordinatesLongLat_WGS84
IpCommandBusCoordinatesLongLatToCapiType(const Icb_CoordinatesLongLat icb_longlat);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DateTime
IpCommandBusDateTimeToCapiType(const Icb_DateTime icb_datetime);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_GnssFixType
IpCommandBusGnssFixTypeToCapiType(const Icb_GnssFixType fix_type);

gen_conn::IpCommandBusTypes::Infotainment_IP_bus_DeadReckoningType
IpCommandBusDeadReckoningTypeToCapiType(const Icb_DeadReckoningType dr_type);

gen_conn::IpCommandBusTypes::TEM_SimpleVehiclePosition
IpCommandBusTEMSimpleVehiclePositionToCapiType(const Icb_SimpleVehiclePosition icb_position);

gen_conn::IpCommandBusTypes::SecurityStatus
SecurityStatusToCapiType(const Icb_SecurityStatus& status);

Icb_ErrorCode_t callStatusToErrorCode(const CommonAPI::CallStatus &callStatus);

uint8_t WeekdaysToByte(v0_climate::ParkingClimate::WeekdaysVec weekdays);

v0_climate::ParkingClimate::WeekdaysVec ByteToWeekdays(const uint8_t& c);

void ParkTimersToString(v0_climate::ParkingClimate::ParkingTimers parkTimers, ASN_BYTE* byte_data);

Icb_Setting ParkingTimersToIpcbType(ASN_Session session,v0_climate::ParkingClimate::ParkingTimers parkTimers);

v0_climate::ParkingClimate::ParkingTimers
SettingToCAPIType(const Icb_Setting& setting);

Icb_AudioStatus AudioStatusToIpcbType(ASN_Session session,const gen_conn::IpCommandBusTypes::AudioStatus audioStatus);
*/
Icb_DateTime TimeToIpcbType(ASN_Session session, std::time_t time);
/*
gen_conn::IpCommandBusTypes::DiagMgm_RemoteDiagInfo RemoteDiagInfoToCapiType(const Icb_RemoteDiagInfo_t& in);
gen_conn::IpCommandBusTypes::DiagMgm_DoIPMode DoIPModeToCapiType(const Icb_DoIPMode_t& in);
gen_conn::IpCommandBusTypes::DiagMgm_DoIPPhase DoIPPhaseToCapiType(const Icb_DoIPPhase_t& in);
gen_conn::IpCommandBusTypes::DiagMgm_DoIPState DoIPStateToCapiType(const Icb_OpCurrentDoIPState_Notification& in);
gen_conn::IpCommandBusTypes::DiagMgm_CurrentDoIPConn DoIPConnToCapiType(const Icb_CurrentDoIPConn& in);
Icb_UserAuthCredentials UserAuthCredentialsToIpcbType(const gen_conn::IpCommandBusTypes::DiagMgm_UserAuthCredentials& in);
Icb_UpdateDacMode UpdateDacModeToIpcbType(const gen_conn::IpCommandBusTypes::DiagMgm_UpdateDacMode& in);
Icb_UpdateDacMode WorkshopWlanDaCModeToIpcbType(const gen_conn::WorkshopWlan::DaCMode& mode);
gen_conn::WorkshopWlan::DaCMode WorkshopWlanIpcbTypeDaCModeToCapiType(const Icb_UpdateDacMode& mode);

gen_conn::WorkshopWlan::WorkshopWlanConnectionResult IpCommandBusWorkshopWlanConnectionResultToCapiType(
        Icb_WorkshopWlanConnectionResult connection_result);*/
/**
 * Convert 32-bit integer angle degree value (see VCC info.ip.bus spec (WGS84)) to floating point (double).
 *
 * @param[in] fixed_point   Fixed point 32 bit integer to convert to double precision.
 */
inline double FixedPoint32ToDegreesD(std::int32_t fixed_point)
{
  return fixed_point / static_cast<double>(0x100000000) * 360.0;
}

/**
 * Convert floating point (double) angle degree value to fixed point 32-bit integer (see VCC info.ip.bus spec (WGS84)).
 * @param[in] double   Double precision floating point value to convert to 32-bit integer.
 */
static inline S32 DegreesDToFixedPoint32 (double degrees) {
  return S32( degrees * ( (double(1<<30)) / 90.0) );
}

// clang-format on
}  // namespace Utils
}  // namespace InfotainmentIpService

#endif  // TYPE_CONVERSION_HELPER_H
