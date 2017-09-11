/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "libipcb_localconfig"

#include <vcc/localconfig.h>
#include <cutils/log.h>

#include "ipcommandbus/local_config_parameters.h"
#include "ipcommandbus/IpCmdTypes.h"

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
    } catch (std::runtime_error e) {
        ALOGE("Error: Could not get parameter: %s", configId.c_str());
        return false;
    }

    ALOGI("%s %d", configId.c_str(), config);
    return true;
}

bool LocalconfigParameters::ReadLocalConfig(const std::string &configId, double &config)
{
    try {
        config = localconfig::GetDouble(configId);
    } catch (std::runtime_error e) {
        ALOGE("Error: Could not get parameter: %s", configId.c_str());
        return false;
    }

    ALOGI("%s %lf", configId.c_str(), config);
    return true;
}

bool LocalconfigParameters::ReadLocalConfig(const std::string &configId, std::string &config)
{
    try {
        config = localconfig::GetString(configId);
    } catch (std::runtime_error e) {
        ALOGE("Error: Could not get parameter: %s", configId.c_str());
        return false;
    }

    ALOGI("%s %s", configId.c_str(), config.c_str());
    return true;
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

    ALOGD("Ack: timeout: %lli retries: %i multiplier: %f\n",
                               defaultAckTimeout_.count(),
                               defaultAckNumRetries_,
                               defaultAckMultiplier_);
    ALOGD("Resp: timeout: %lli retires: %i multiplier: %f\n",
                               defaultRespTimeout_.count(),
                               defaultRespNumRetries_,
                               defaultRespMultiplier_);
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
    else
    {
        ALOGE("Failed to read network control priority level from local config");
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
