/*
 * ===========================================================================
 * [2017]- Volvo Car Corporation
 * All Rights Reserved
 * LEGAL
 * NOTICE:  All information (including intellectual and technical concepts) contained herein is, and remains the
 * property of or licensed to Volvo Car Corporation. This information is proprietary to Volvo Car Corporation and may be
 * covered by patents or patent applications. This information is protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from Volvo Car Corporation.
 */

#ifndef _NETUTILS_H_
#define _NETUTILS_H_

#include <string>
#include <vector>

#include <vcc/localconfig.h>
#include "interface_configuration.h"
#include "netlink_event_handler.h"

namespace vcc {
namespace netman {

void LoadInterfaceConfiguration(std::vector<InterfaceConfiguration> *interface_configurations,
                                const vcc::LocalConfigReaderInterface *lcfg);

void PrintInterfaceConfiguration(const std::string &context, const InterfaceConfiguration &conf);

void MoveNetworkInterfaceToNamespace(const std::string &network_interface_name, const std::string &ns,
                                     const std::string &new_name = "");

void SetupInterface(const std::vector<InterfaceConfiguration> &interface_configurations);

bool SetupInterface(const char *interface_name, const std::vector<uint8_t> &mac_address, const char *ip_addr,
                    const char *netmask, const char *broadcast_addr, const uint32_t mtu);

void BringInterfaceUp(const std::string &interface_name, const std::string &ns);

bool BringInterfaceUp(const char *interface_name);

bool TakeInterfaceDown(const char *interface_name);

}  // namespace netman
}  // namespace vcc

#endif  // _NETUTILS_H_
