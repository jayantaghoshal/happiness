/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <string>
#include <vector>

#include <vcc/localconfig.h>
#include "interface_configuration.h"
#include "uevent_handler.h"

namespace vcc {
namespace netman {

void LoadInterfaceConfiguration(std::vector<InterfaceConfiguration>* interface_configurations,
                                const vcc::LocalConfigReaderInterface* lcfg);

void PrintInterfaceConfiguration(const std::string& context, const InterfaceConfiguration& conf);

void MoveNetworkInterfaceToNamespace(const std::string& current_name, const std::string& ns,
                                     const std::string& new_name = "");

bool SetupInterface(const char* interface_name, const std::vector<uint8_t>& mac_address, const char* ip_addr,
                    const char* netmask, const char* broadcast_addr, uint32_t mtu);

bool SetupVLan(const InterfaceConfiguration& interface_configuration);

bool BringInterfaceUp(const char* interface_name);

bool TakeInterfaceDown(const char* interface_name);

}  // namespace netman
}  // namespace vcc
