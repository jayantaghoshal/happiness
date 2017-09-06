/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef LOCALCONFIG_PARAMETERS_H
#define LOCALCONFIG_PARAMETERS_H

#include <string>
#include <unordered_map>
#include <chrono>

#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/isocket.h"

namespace Connectivity
{
class LocalconfigParameters
{
public:
    LocalconfigParameters();
    ~LocalconfigParameters();

    static LocalconfigParameters &getInstance();

    // ------------
    // Ack timeouts
    // ------------
    std::chrono::milliseconds getDefaultAckTimeout();
    uint32_t getDefaultAckNumRetries();
    double getDefaultAckMultiplier();

    // -----------------
    // Response timeouts
    // -----------------
    double getDefaultRespMultiplier();
    std::chrono::milliseconds getTimeout(VccIpCmd::CombinedId id);
    uint32_t getRetries(VccIpCmd::CombinedId id);

    const std::string &getLocalIpAddress();
    const std::string &getBroadcastIpAddress();
    const std::string &getVCMIpAddress();
    const std::string &getTEMIpAddress();
    const std::string &getDIMIpAddress();

    uint16_t getLocalPort();
    uint16_t getLocalBroadcastPort();
    uint16_t getVCMPort();
    uint16_t getTEMPort();
    uint16_t getDIMPort();

    std::chrono::seconds getDIMKeepAliveTime();
    std::chrono::seconds getDIMKeepAliveInterval();
    int getDIMKeepAliveProbes();

    // ----------------
    // Network priority
    // ----------------
    ISocket::IpPrecedence getNetworkControlPriority();

private:
    void Init();
    void InitTimeoutValues();
    void InitNetworkPriority();
    void InitNetworkConfiguration();

    // Local config accessors
    bool ReadLocalConfig(const std::string &configId, int &config);
    bool ReadLocalConfig(const std::string &configId, double &config);
    bool ReadLocalConfig(const std::string &configId, std::string &config);
    // --------------
    // Timeout values
    // --------------
    struct CombinedIdHash
    {
        std::uint32_t operator()(const VccIpCmd::CombinedId &id) const
        {
            return (static_cast<std::uint16_t>(id.serviceId) << 16) | static_cast<std::uint16_t>(id.operationId);
        }
    };

    std::unordered_map<VccIpCmd::CombinedId, std::chrono::milliseconds, CombinedIdHash> timeout_values_;
    std::unordered_map<VccIpCmd::CombinedId, uint32_t, CombinedIdHash> retries_values_;

    // Note: Values will be loaded from localconfig. Default values are just for safety.
    std::chrono::milliseconds defaultAckTimeout_{500};
    uint32_t defaultAckNumRetries_ = 7;
    double defaultAckMultiplier_ = 1.5;

    std::chrono::milliseconds defaultRespTimeout_ = std::chrono::milliseconds{1000};
    uint32_t defaultRespNumRetries_ = 2;
    double defaultRespMultiplier_ = 2.0;

    std::string ip_address_local_ = "198.18.34.1";
    std::string ip_address_broadcast_ = "198.18.255.255";
    std::string ip_address_vcm_ = "198.18.32.1";
    std::string ip_address_tem_ = "198.18.48.17";
    std::string ip_address_dim_ = "198.18.24.1";

    uint16_t port_local_ = 50000;
    uint16_t port_broadcast_ = 50000;
    uint16_t port_vcm_ = 50000;
    uint16_t port_tem_ = 50000;
    uint16_t port_dim_ = 50000;

    std::chrono::seconds dim_keep_alive_time_{10};
    std::chrono::seconds dim_keep_alive_interval_{5};
    int dim_keep_alive_probes_{2};

    int networkControlPrio_ = 0;
};

}  // Connectivity

#endif  // LOCALCONFIG_PARAMETERS_H
