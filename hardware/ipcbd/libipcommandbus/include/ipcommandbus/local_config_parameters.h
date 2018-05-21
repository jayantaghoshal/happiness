/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <chrono>
#include <string>
#include <unordered_map>

#include <vcc/localconfig.h>

#include "ipcommandbus/isocket.h"

namespace Connectivity {
class LocalconfigParameters {
  public:
    LocalconfigParameters(const vcc::LocalConfigReaderInterface* lcfg = vcc::LocalConfigDefault());

    static LocalconfigParameters& getInstance();

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

    const std::string& getLocalIpAddress();
    const std::string& getBroadcastIpAddress();
    const std::string& getDIMIpAddress();
    const std::string& getVGMIpAddress();
    const std::string& getTCAMIpAddress();

    uint16_t getLocalPort();
    uint16_t getBroadcastPort();
    uint16_t getDIMPort();
    uint16_t getTCAMPort();
    uint16_t getVGMPort();
    uint32_t getTestPort();

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

    // Note: Values will be loaded from localconfig. Default values are just for safety.
    std::chrono::milliseconds defaultAckTimeout_{500};
    uint32_t defaultAckNumRetries_ = 7;
    double defaultAckMultiplier_ = 1.5;

    std::chrono::milliseconds defaultRespTimeout_ = std::chrono::milliseconds{1000};
    uint32_t defaultRespNumRetries_ = 2;
    double defaultRespMultiplier_ = 2.0;

    std::string ip_address_local_ = "198.19.101.66";
    std::string ip_address_broadcast_ = "198.19.101.95";
    std::string ip_address_dim_ = "198.18.24.1";
    std::string ip_address_tcam_ = "198.18.32.1";
    std::string ip_address_vgm_ = "198.18.32.2";

    uint16_t port_local_ = 50000;
    uint16_t port_broadcast_ = 50001;
    uint16_t port_dim_ = 50000;
    uint16_t port_tcam_ = 50000;
    uint16_t port_vgm_ = 50000;
    uint32_t port_test_ = 70000;

    std::chrono::seconds dim_keep_alive_time_{10};
    std::chrono::seconds dim_keep_alive_interval_{5};
    int dim_keep_alive_probes_{2};

    int networkControlPrio_ = 0;

    const vcc::LocalConfigReaderInterface* const lcfg_;
};

}  // namespace Connectivity
