/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <cutils/log.h>
#include <vcc/localconfig.h>

#include "ipcommandbus/IpCmdTypes.h"
#include "ipcommandbus/local_config_parameters.h"

#define LOG_TAG "libipcb_localconfig"

using namespace vcc;

namespace Connectivity
{
LocalconfigParameters::LocalconfigParameters(const vcc::LocalConfigReaderInterface *lcfg) : lcfg_(lcfg)
{
    Init();
}

void LocalconfigParameters::Init()
{
    InitTimeoutValues();
    InitNetworkPriority();
    InitNetworkConfiguration();
}

void LocalconfigParameters::InitNetworkConfiguration()
{
    lcfg_->TryGetValue(&ip_address_local_, "IIPS_IpAddress_LOCAL");
    lcfg_->TryGetValue(&ip_address_broadcast_, "IIPS_IpAddress_BROADCAST");
    lcfg_->TryGetValue(&ip_address_vcm_, "IIPS_IpAddress_VCM");
    lcfg_->TryGetValue(&ip_address_tem_, "IIPS_IpAddress_TEM");
    lcfg_->TryGetValue(&ip_address_dim_, "IIPS_IpAddress_DIM");
    lcfg_->TryGetValue(&ip_address_tcam_, "IIPS_IpAddress_TCAM");
    lcfg_->TryGetValue(&ip_address_vgm_, "IIPS_IpAddress_VGM");

    lcfg_->TryGetValue(&port_local_, "IIPS_Port_LOCAL");
    lcfg_->TryGetValue(&port_broadcast_, "IIPS_Port_BROADCAST");

    lcfg_->TryGetValue(&port_vcm_, "IIPS_Port_VCM");
    lcfg_->TryGetValue(&port_tem_, "IIPS_Port_TEM");
    lcfg_->TryGetValue(&port_dim_, "IIPS_Port_DIM");
    lcfg_->TryGetValue(&port_tcam_, "IIPS_Port_TCAM");
    lcfg_->TryGetValue(&port_vgm_, "IIPS_Port_VGM");

    lcfg_->TryGetValue(&dim_keep_alive_time_, "CONN_DIMKeepAliveTime");
    lcfg_->TryGetValue(&dim_keep_alive_interval_, "CONN_DIMKeepAliveIntvl");
    lcfg_->TryGetValue(&dim_keep_alive_probes_, "CONN_DIMKeepAliveProbes");
}

void LocalconfigParameters::InitTimeoutValues()
{
    lcfg_->TryGetValue(&defaultAckTimeout_, "CONN_defaultTimeoutWFA");
    lcfg_->TryGetValue(&defaultAckNumRetries_, "CONN_numberOfRetriesWFA");
    lcfg_->TryGetValue(&defaultAckMultiplier_, "CONN_increaseTimerValueWFA");

    lcfg_->TryGetValue(&defaultRespTimeout_, "CONN_defaultTimeoutWFR");
    lcfg_->TryGetValue(&defaultRespNumRetries_, "CONN_numberOfRetriesWFR");
    lcfg_->TryGetValue(&defaultRespMultiplier_, "CONN_increaseTimerValueWFR");

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
    if (lcfg_->TryGetValue(&priority, "CONN_IpNetworkPrioNetworkControl"))
    {
        if (priority >= 0 && priority < 8)
        {
            networkControlPrio_ = priority;
        }
        else
        {
            ALOGW("Network control prrioity level out of range, leaving to default");
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

const std::string &LocalconfigParameters::getTCAMIpAddress()
{
    return ip_address_tcam_;
}

const std::string &LocalconfigParameters::getVGMIpAddress()
{
    return ip_address_vgm_;
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
uint16_t LocalconfigParameters::getTCAMPort()
{
    return port_tcam_;
}

uint16_t LocalconfigParameters::getVGMPort()
{
    return port_vgm_;
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
