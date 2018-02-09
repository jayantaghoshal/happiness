/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cedric/cedric_localconfig.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "ipcommandbus/local_config_parameters.h"

namespace Connectivity {

LocalconfigParameters::LocalconfigParameters() {}
LocalconfigParameters::~LocalconfigParameters() {}

bool LocalconfigParameters::ReadLocalConfig(const std::string& configId, int& config) {
    (void)configId;
    (void)config;
    return true;
}

bool LocalconfigParameters::ReadLocalConfig(const std::string& configId, std::string& config) {
    (void)configId;
    (void)config;
    return true;
}

bool LocalconfigParameters::ReadLocalConfig(const std::string& configId, double& config) {
    (void)configId;
    (void)config;
    return true;
}

uint32_t LocalconfigParameters::getRetries(const VccIpCmd::CombinedId id) {
    (void)id;
    return 2;
}

std::chrono::milliseconds LocalconfigParameters::getTimeout(const VccIpCmd::CombinedId id) {
    (void)id;
    return std::chrono::milliseconds(1000);
}

void LocalconfigParameters::InitTimeoutValues() {}

std::chrono::milliseconds LocalconfigParameters::getDefaultAckTimeout() { return std::chrono::milliseconds(500); }

uint32_t LocalconfigParameters::getDefaultAckNumRetries() { return 7; }

double LocalconfigParameters::getDefaultAckMultiplier() { return 1.5; }

double LocalconfigParameters::getDefaultRespMultiplier() { return 2.0; }

ISocket::IpPrecedence LocalconfigParameters::getNetworkControlPriority() {
    return ISocket::IpPrecedence::kNetworkControl;
}

LocalconfigParameters& LocalconfigParameters::getInstance() {
    static LocalconfigParameters instance;
    return instance;
}

}  // Connectivity
