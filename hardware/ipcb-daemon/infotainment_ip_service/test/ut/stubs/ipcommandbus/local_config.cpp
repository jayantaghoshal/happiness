/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "ipcommandbus/local_config.h"
#include <map>
#include "cedric/cedric_localconfig.h"
#include "ipcommandbus/VccIpCmdApi.h"

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_lmt_log_context, "MT00", "Localconfig Message Timeouts")

const std::chrono::milliseconds defaultResponseTimeout{5800};

Configuration::Configuration()
{
    InitTimeoutValues();
}

bool Configuration::ReadLocalConfig(const std::string& configId, int& config)
{
    const int* p = cedric::core::localconfig::getValue<int>(configId);
    if (!p)
    {
        log_error().writeFormatted("Error: Could not get parameter: %s", configId.c_str());
        return false;
    }

    config = *p;
    log_info().writeFormatted("%s %i", configId.c_str(), config);

    return true;
}

std::chrono::milliseconds Configuration::getTimeout(VccIpCmd::OperationId opId)
{
    const auto it = timeout_values_.find(opId);
    if (it != timeout_values_.end())
    {
        return it->second;
    }
    else
    {
        // NOTE: Arbitrary default timeout. We may want to change this number later
        return defaultResponseTimeout;
    }
}

uint32_t Configuration::getRetries(VccIpCmd::OperationId opId)
{
    const auto it = retries_values_.find(opId);
    if (it != retries_values_.end())
    {
        log_debug().writeFormatted("0 numOfRetries here");
        return it->second;
    }
    else
    {
        return defaultRespNumRetries_;
    }
}

void Configuration::InitTimeoutValues()
{
    if (!cedric::core::localconfig::init())
    {
        log_error().writeFormatted("Error: LocalMessageTimeouts - could not initialise Localconfig");
        return;
    }

    // Mapping from the internal enums to the parameter name in Localconfig
    std::map<VccIpCmd::OperationId, std::string> temporaryMap = {
        {VccIpCmd::OperationId::PSIMSecurity, "CommonPSIM_TEM_PSIMSecurity_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMEnterCode, "CommonPSIM_TEM_PSIMEnterCode_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMRegisterState, "CommonPSIM_TEM_PSIMRegisterState_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMNetworkOperators, "CommonPSIM_TEM_PSIMNetworkOperators_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMNetworkOperatorCommand, "CommonPSIM_TEM_PSIMNetworkOperatorCommand_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMState, "CommonPSIM_TEM_PSIMState_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMUssd, "CommonPSIM_TEM_PSIMUssd_TimeoutMs"},

        {VccIpCmd::OperationId::PSIMDataCounter, "CONN_TEM_PSIMDataCounter_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMResetDataCounter, "CONN_TEM_PSIMResetDataCounter_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMNetworkConfiguration, "CONN_TEM_PSIMNetworkConfiguration_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMConnect, "CONN_TEM_PSIMConnect_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMConnectionStatus, "CONN_TEM_PSIMConnectionStatus_TimeoutMs"},
        {VccIpCmd::OperationId::PSIMRoamingConfiguration, "CONN_TEM_PSIMRoamingConfiguration_TimeoutMs"},
        {VccIpCmd::OperationId::AccessRightPinCode, "CONN_TEM_AccessRightPinCode_TimeoutMs"},
        {VccIpCmd::OperationId::CurrentInternetSource, "CONN_VCM_CurrentInternetSource_TimeoutMs"},
        {VccIpCmd::OperationId::InternetSourceCtrlBT, "CONN_IHU_InternetSourceCtrlBT_TimeoutMs"},
        {VccIpCmd::OperationId::InternetSourceCtrlUSB, "CONN_IHU_InternetSourceCtrlUSB_TimeoutMs"},
        {VccIpCmd::OperationId::PSimMessage, "CONN_IHU_PSIMMessage_TimeoutMs"},
        {VccIpCmd::OperationId::RequestUserAuthentication, "RD_USER_TIMEOUT"},

        {VccIpCmd::OperationId::WLANMode, "WLAN_VCM_WLANMode_TimeoutMs"},
        {VccIpCmd::OperationId::WLANAPSSID, "WLAN_VCM_WLANAPSSID_TimeoutMs"},
        {VccIpCmd::OperationId::WLANAPPasswd, "WLAN_VCM_WLANAPPassword_TimeoutMs"},
        {VccIpCmd::OperationId::WLANSTAConnect, "WLAN_VCM_WLANSTAConnect_TimeoutMs"},
        {VccIpCmd::OperationId::WLANSTADisconnect, "WLAN_VCM_WLANSTADisconnect_TimeoutMs"},
        {VccIpCmd::OperationId::WLANAPStatusSettings, "WLAN_VCM_WLANAPStatusSettings_TimeoutMs"},
        {VccIpCmd::OperationId::WLANSTAStatus, "WLAN_VCM_WLANSTAStatus_TimeoutMs"},
        {VccIpCmd::OperationId::WLANSTAForgetRememberedNetwk, "WLAN_VCM_WLANSTAForgetRememberedNetwk_TimeoutMs"}};

    for (auto it : temporaryMap)
    {
        int value;
        if (ReadLocalConfig(it.second, value))
        {
            if (it.second == "RD_USER_TIMEOUT")
            {
                // REQPROD:408216/MAIN;0 LCFG Remote Diagnostics
                // TODO: Ugly hack
                timeout_values_.emplace(it.first, std::chrono::minutes{value});
            }
            else
            {
                timeout_values_.emplace(it.first, std::chrono::milliseconds{value});
            };
        }
    }

    retries_values_[VccIpCmd::OperationId::CancelDownload] = 0;
    retries_values_[VccIpCmd::OperationId::DownloadSoftware] = 0;
}

std::chrono::milliseconds Configuration::getDefaultAckTimeout()
{
    return defaultAckTimeout_;
}
uint32_t Configuration::getDefaultAckNumRetries()
{
    return defaultAckNumRetries_;
}
double Configuration::getDefaultAckMultiplier()
{
    return defaultAckMultiplier_;
}

double Configuration::getDefaultRespMultiplier()
{
    return defaultRespMultiplier_;
}

}  // Connectivity
