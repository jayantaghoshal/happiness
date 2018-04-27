/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cloudd_local_config.h"
#include <vcc/localconfig.h>

#define LOG_TAG "CloudD"
#include <cutils/log.h>

namespace {
/*
const std::string connNhEthernetGatewayIpAddressId = "CONN_NhEthernetGatewayIpAddressId";

const std::string connNhLocalEthernetMac = "CONN_NhLocalEthernetMAC";
const std::string connNhLocalMostMac = "CONN_NhLocalMostMAC";
const std::string connNhLocalApixMac = "CONN_NhLocalApixMAC";
const std::string connNhEthernetMtu = "CONN_NhEthernetMtu";

const std::string connNhCarNet = "CONN_NhCarNet";
const std::string connNhCarSubnet = "CONN_NhCarSubnet";

const std::string connNhLocalEthernetId = "CONN_NhLocalEthernetId";
const std::string connNhLocalMostId = "CONN_NhLocalMostId";
const std::string connNhLocalApixId = "CONN_NhLocalApixId";

const std::string connNhWlanInterface = "CONN_WlanInterface";
const std::string connNhCmodInterface = "CONN_CmodInterface";

const std::string connTcpKeepAliveTime = "CONN_TcpKeepAliveTime";
const std::string connTcpKeepAliveIntvl = "CONN_TcpKeepAliveIntvl";
const std::string connTcpKeepAliveProbes = "CONN_TcpKeepAliveProbes";

const std::string connNhLoggingEnabled = "Lcfg_EnableDiagnosticsHMI";*/
const std::string connCloudEntryPointUrl = "CONN_CloudEntryPointAddress";
/*
const std::string connNhTrafficLimit = "CONN_NhTrafficLimit"; */
}

namespace Connectivity {

ClouddLocalConfig::ClouddLocalConfig(const vcc::LocalConfigReaderInterface* lcfg) : lcfg_(lcfg) { Init(); }
/*
bool ClouddLocalConfig::ReadLocalConfig(const std::string &configId, std::string &config)
{
    const std::string *p = cedric::core::localconfig::getValue<std::string>(configId);
    if (!p)
    {
        log_error().writeFormatted("Error: Could not get parameter: %s", configId.c_str());
        return false;
    }

    config = *p;
    log_info().writeFormatted("%s %s", configId.c_str(), config.c_str());

    return true;
}

bool ClouddLocalConfig::ReadLocalConfig(const std::string &configId, int &config)
{
    const int *p = cedric::core::localconfig::getValue<int>(configId);
    if (!p)
    {
        log_error().writeFormatted("Error: Could not get parameter: %s", configId.c_str());
        return false;
    }

    config = *p;
    log_info().writeFormatted("%s %u", configId.c_str(), config);

    return true;
}

bool ClouddLocalConfig::ReadLocalConfig(const std::string &configId, bool &config)
{
    const bool *p = cedric::core::localconfig::getValue<bool>(configId);
    if (!p)
    {
        log_error().writeFormatted("Error: Could not get parameter: %s", configId.c_str());
        return false;
    }

    config = *p;
    log_info().writeFormatted("%s %u", configId.c_str(), config);

    return true;
}

bool ClouddLocalConfig::ReadLocalConfig(std::string configId, std::vector<address_netmask_collection_> &collection)
{
    auto result = cedric::core::localconfig::getArray<std::tuple<std::string, std::string>>(
        std::move(configId), "address", "netmask");

    if (result.empty())
        return false;

    collection.swap(result);

    return true;
}

bool ClouddLocalConfig::ReadLocalConfig(std::string configId, std::vector<TrafficLimitEntry> &collection)
{
    auto result =
        cedric::core::localconfig::getArray<TrafficLimitEntry>(std::move(configId), "iface_regex", "mark", "limit");

    if (result.empty())
        return false;

    collection.swap(result);

    return true;
}*/

bool ClouddLocalConfig::Init() {
    /*
        lcfg_->TryGetValue(&localEthernetMac_, connNhLocalEthernetMac);
        lcfg_->TryGetValue(&localMostMac_, connNhLocalMostMac);
        lcfg_->TryGetValue(&localApixMac_, connNhLocalApixMac);
        lcfg_->TryGetValue(&ethernetGatewayIpAddress_, connNhEthernetGatewayIpAddressId);

        if (!ReadLocalConfig(connNhLocalEthernetId, localEthernet_))
            result = false;

        if (!ReadLocalConfig(connNhLocalMostId, localMost_))
            result = false;

        if (!ReadLocalConfig(connNhLocalApixId, localApix_))
            result = false;

        if (!ReadLocalConfig(connNhEthernetMtu, reinterpret_cast<int &>(ethernetMtu_)))
            result = false;

        if (!ReadLocalConfig(connNhWlanInterface, wlanInterface_))
            result = false;

        if (!ReadLocalConfig(connNhCmodInterface, cmodInterface_))
            result = false;

        if (!ReadLocalConfig(connTcpKeepAliveTime, reinterpret_cast<int &>(tcp_keepalive_time_)))
            result = false;

        if (!ReadLocalConfig(connTcpKeepAliveIntvl, reinterpret_cast<int &>(tcp_keepalive_intvl_)))
            result = false;

        if (!ReadLocalConfig(connTcpKeepAliveProbes, reinterpret_cast<int &>(tcp_keepalive_probes_)))
            result = false;

        if (!ReadLocalConfig(connNhCarNet, carNet_))
            result = false;

        if (!ReadLocalConfig(connNhCarSubnet, carSubnet_))
            result = false;

        if (!ReadLocalConfig(connNhLoggingEnabled, loggingEnabled_))
            result = false;
    */
    lcfg_->TryGetValue(&cloud_entry_point_address_, connCloudEntryPointUrl);
    /*
        if (!ReadLocalConfig(connNhTrafficLimit, trafficLimits_))
            result = false;
    */
    return true;
}
/*
std::string ClouddLocalConfig::GetLocalEthernetMac() { return localEthernetMac_; }

std::string ClouddLocalConfig::GetEthernetGatewayIpAddress() { return ethernetGatewayIpAddress_; }

std::string ClouddLocalConfig::GetDnsServer() { return ethernetGatewayIpAddress_; }

uint32_t ClouddLocalConfig::GetLocalEthernetMtu() { return ethernetMtu_; }

const std::vector<ClouddLocalConfig::address_netmask_collection_> ClouddLocalConfig::GetLocalEthernetConfiguration() {
    return localEthernet_;
}

std::string ClouddLocalConfig::GetLocalMostMac() { return localMostMac_; }

const std::vector<ClouddLocalConfig::address_netmask_collection_> ClouddLocalConfig::GetLocalMostConfiguration() {
    return localMost_;
}

const std::vector<ClouddLocalConfig::address_netmask_collection_> ClouddLocalConfig::GetLocalApixConfiguration() {
    return localApix_;
}

std::string ClouddLocalConfig::GetLocalApixMac() { return localApixMac_; }

std::string ClouddLocalConfig::GetWlanInterface() { return wlanInterface_; }

std::string ClouddLocalConfig::GetCmodInterface() { return cmodInterface_; }

uint32_t ClouddLocalConfig::GetTcpKeepAliveTime() { return tcp_keepalive_time_; }

uint32_t ClouddLocalConfig::GetTcpKeepAliveIntvl() { return tcp_keepalive_intvl_; }

uint32_t ClouddLocalConfig::GetTcpKeepAliveProbes() { return tcp_keepalive_probes_; }
*/
const std::string ClouddLocalConfig::GetCloudEntryPointAddress() { return cloud_entry_point_address_; }
/*
std::string ClouddLocalConfig::GetCarNet() { return carNet_; }

std::string ClouddLocalConfig::GetCarSubnet() { return carSubnet_; }

bool ClouddLocalConfig::GetLoggingEnabled() { return loggingEnabled_; }

std::vector<ClouddLocalConfig::TrafficLimitEntry> ClouddLocalConfig::GetTrafficLimits() { return trafficLimits_; }
*/
}  // namespace NetworkHandler
