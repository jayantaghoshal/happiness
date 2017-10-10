/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "libipcb_localconfig"

#include <vcc/localconfig.h>
#include <cutils/log.h>

#include "ipcommandbus/local_config_parameters.h"
#include "ipcommandbus/VccIpCmdApi.h"

using namespace vcc;

namespace Connectivity
{

LocalconfigParameters::LocalconfigParameters()
{
    Init();
}

LocalconfigParameters::~LocalconfigParameters()
{
}

bool LocalconfigParameters::ReadLocalConfig(const std::string &configId, int &config)
{
    try {
        config = localconfig::GetInt(configId);
    } catch (const std::runtime_error& e) {
        ALOGW("Could not get parameter: %s, reason: %s", configId.c_str(), e.what());
        return false;
    }
    return true;
}

bool LocalconfigParameters::ReadLocalConfig(const std::string &configId, double &config)
{
    try {
        config = localconfig::GetDouble(configId);
    } catch (const std::runtime_error& e) {
        ALOGW("Could not get parameter: %s, reason: %s", configId.c_str(), e.what());
        return false;
    }
    return true;
}

bool LocalconfigParameters::ReadLocalConfig(const std::string &configId, std::string &config)
{
    try {
        config = localconfig::GetString(configId);
    } catch (const std::runtime_error& e) {
        ALOGW("Could not get parameter: %s, reason: %s", configId.c_str(), e.what());
        return false;
    }
    return true;
}

std::chrono::milliseconds LocalconfigParameters::getTimeout(const VccIpCmd::CombinedId id)
{
    const auto it = timeout_values_.find(id);
    if (it != timeout_values_.end())
    {
        return it->second;
    }
    else
    {
        return defaultRespTimeout_;
    }
}

uint32_t LocalconfigParameters::getRetries(const VccIpCmd::CombinedId id)
{
    const auto it = retries_values_.find(id);
    if (it != retries_values_.end())
    {
        return it->second;
    }
    else
    {
        return defaultRespNumRetries_;
    }
}

void LocalconfigParameters::Init()
{
    InitTimeoutValues();
    InitNetworkPriority();
    InitNetworkConfiguration();
}

void LocalconfigParameters::InitNetworkConfiguration()
{
    ReadLocalConfig("IIPS_IpAddress_LOCAL", ip_address_local_);
    ReadLocalConfig("IIPS_IpAddress_BROADCAST", ip_address_broadcast_);
    ReadLocalConfig("IIPS_IpAddress_VCM", ip_address_vcm_);
    ReadLocalConfig("IIPS_IpAddress_TEM", ip_address_tem_);
    ReadLocalConfig("IIPS_IpAddress_DIM", ip_address_dim_);

    int temp_val = 0;
    if (ReadLocalConfig("IIPS_Port_LOCAL", temp_val))
    {
        port_local_ = static_cast<uint16_t>(temp_val);
    }

    if (ReadLocalConfig("IIPS_Port_BROADCAST", temp_val))
    {
        port_broadcast_ = static_cast<uint16_t>(temp_val);
    }

    if (ReadLocalConfig("IIPS_Port_VCM", temp_val))
    {
        port_vcm_ = static_cast<uint16_t>(temp_val);
    }

    if (ReadLocalConfig("IIPS_Port_TEM", temp_val))
    {
        port_tem_ = static_cast<uint16_t>(temp_val);
    }

    if (ReadLocalConfig("IIPS_Port_DIM", temp_val))
    {
        port_dim_ = static_cast<uint16_t>(temp_val);
    }

    if (ReadLocalConfig("CONN_DIMKeepAliveTime", temp_val))
    {
        dim_keep_alive_time_ = std::chrono::seconds{temp_val};
    }
    if (ReadLocalConfig("CONN_DIMKeepAliveIntvl", temp_val))
    {
        dim_keep_alive_interval_ = std::chrono::seconds{temp_val};
    }
    if (ReadLocalConfig("CONN_DIMKeepAliveProbes", temp_val))
    {
        dim_keep_alive_probes_ = temp_val;
    }
}

void LocalconfigParameters::InitTimeoutValues()
{
    // Read default values
    int iValue;
    double dValue;
    if (ReadLocalConfig("CONN_defaultTimeoutWFA", iValue))
    {
        defaultAckTimeout_ = std::chrono::milliseconds{iValue};
    }
    if (ReadLocalConfig("CONN_numberOfRetriesWFA", iValue))
    {
        defaultAckNumRetries_ = iValue;
    }
    if (ReadLocalConfig("CONN_increaseTimerValueWFA", dValue))
    {
        defaultAckMultiplier_ = dValue;
    }
    if (ReadLocalConfig("CONN_defaultTimeoutWFR", iValue))
    {
        defaultRespTimeout_ = std::chrono::milliseconds{iValue};
    }
    if (ReadLocalConfig("CONN_numberOfRetriesWFR", iValue))
    {
        defaultRespNumRetries_ = iValue;
    }
    if (ReadLocalConfig("CONN_increaseTimerValueWFR", dValue))
    {
        defaultRespMultiplier_ = dValue;
    }

    // Mapping from the internal enums to the parameter name in Localconfig
    std::unordered_map<VccIpCmd::CombinedId, std::string, CombinedIdHash> temporaryMap = {
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMSecurity),
         "CommonPSIM_TEM_PSIMSecurity_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMEnterCode),
         "CommonPSIM_TEM_PSIMEnterCode_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity,
                              VccIpCmd::OperationId::PSIMRegisterState),
         "CommonPSIM_TEM_PSIMRegisterState_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity,
                              VccIpCmd::OperationId::PSIMNetworkOperators),
         "CommonPSIM_TEM_PSIMNetworkOperators_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity,
                              VccIpCmd::OperationId::PSIMNetworkOperatorCommand),
         "CommonPSIM_TEM_PSIMNetworkOperatorCommand_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMState),
         "CommonPSIM_TEM_PSIMState_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMUssd),
         "CommonPSIM_TEM_PSIMUssd_TimeoutMs"},

        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMDataCounter),
         "CONN_TEM_PSIMDataCounter_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMResetDataCounter),
         "CONN_TEM_PSIMResetDataCounter_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMNetworkConfiguration),
         "CONN_TEM_PSIMNetworkConfiguration_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMConnect),
         "CONN_TEM_PSIMConnect_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMConnectionStatus),
         "CONN_TEM_PSIMConnectionStatus_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMRoamingConfiguration),
         "CONN_TEM_PSIMRoamingConfiguration_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::AccessRightPinCode),
         "CONN_TEM_AccessRightPinCode_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::CurrentInternetSource),
         "CONN_VCM_CurrentInternetSource_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetSourceCtrlBT),
         "CONN_IHU_InternetSourceCtrlBT_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::InternetSourceCtrlUSB),
         "CONN_IHU_InternetSourceCtrlUSB_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMMessage),
         "CONN_IHU_PSIMMessage_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::DiagnosticManagement,
                              VccIpCmd::OperationId::RequestUserAuthentication),
         "RD_USER_TIMEOUT"},

        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANMode),
         "WLAN_VCM_WLANMode_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPSSID),
         "WLAN_VCM_WLANAPSSID_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPPasswd),
         "WLAN_VCM_WLANAPPassword_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTAConnect),
         "WLAN_VCM_WLANSTAConnect_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTADisconnect),
         "WLAN_VCM_WLANSTADisconnect_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANAPStatusSettings),
         "WLAN_VCM_WLANAPStatusSettings_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTAStatus),
         "WLAN_VCM_WLANSTAStatus_TimeoutMs"},
        {VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANSTAForgetRememberedNetwk),
         "WLAN_VCM_WLANSTAForgetRememberedNetwk_TimeoutMs"}};

    for (auto &it : temporaryMap)
    {
        int value;
        if (ReadLocalConfig(it.second, value))
        {
            if (it.second == "RD_USER_TIMEOUT")
            {
                timeout_values_.emplace(it.first, std::chrono::minutes{value});
            }
            else
            {
                timeout_values_.emplace(it.first, std::chrono::milliseconds{value});
            }
        }
    }

    // REQPROD:338711
    retries_values_[VccIpCmd::CombinedId(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::CancelDownload)] = 0;
    // REQPROD:346425
    retries_values_[VccIpCmd::CombinedId(VccIpCmd::ServiceId::OTA, VccIpCmd::OperationId::DownloadSoftware)] = 0;
}

void LocalconfigParameters::InitNetworkPriority()
{
    int priority;
    if (ReadLocalConfig("CONN_IpNetworkPrioNetworkControl", priority))
    {
        if (priority >= 0 && priority < 8)
        {
            networkControlPrio_ = priority;
        }
        else
        {
            ALOGW("Network control prioity level out of range, setting to default");
        }
    }
}

std::chrono::milliseconds LocalconfigParameters::getDefaultAckTimeout()
{
    return defaultAckTimeout_;
}
uint32_t LocalconfigParameters::getDefaultAckNumRetries()
{
    return defaultAckNumRetries_;
}
double LocalconfigParameters::getDefaultAckMultiplier()
{
    return defaultAckMultiplier_;
}

double LocalconfigParameters::getDefaultRespMultiplier()
{
    return defaultRespMultiplier_;
}

const std::string &LocalconfigParameters::getLocalIpAddress()
{
    return ip_address_local_;
}

const std::string &LocalconfigParameters::getBroadcastIpAddress()
{
    return ip_address_broadcast_;
}

ISocket::IpPrecedence LocalconfigParameters::getNetworkControlPriority()
{
    return static_cast<ISocket::IpPrecedence>(networkControlPrio_);
}

const std::string &LocalconfigParameters::getVCMIpAddress()
{
    return ip_address_vcm_;
}

const std::string &LocalconfigParameters::getTEMIpAddress()
{
    return ip_address_tem_;
}

const std::string &LocalconfigParameters::getDIMIpAddress()
{
    return ip_address_dim_;
}

uint16_t LocalconfigParameters::getLocalPort()
{
    return port_local_;
}

uint16_t LocalconfigParameters::getLocalBroadcastPort()
{
    return port_broadcast_;
}

uint16_t LocalconfigParameters::getVCMPort()
{
    return port_vcm_;
}

uint16_t LocalconfigParameters::getTEMPort()
{
    return port_tem_;
}

uint16_t LocalconfigParameters::getDIMPort()
{
    return port_dim_;
}

LocalconfigParameters &LocalconfigParameters::getInstance()
{
    static LocalconfigParameters instance;
    return instance;
}

std::chrono::seconds LocalconfigParameters::getDIMKeepAliveTime()
{
    return dim_keep_alive_time_;
}

std::chrono::seconds LocalconfigParameters::getDIMKeepAliveInterval()
{
    return dim_keep_alive_interval_;
}

int LocalconfigParameters::getDIMKeepAliveProbes()
{
    return dim_keep_alive_probes_;
}

}  // Connectivity
