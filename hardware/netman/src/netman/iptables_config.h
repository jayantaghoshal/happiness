/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <string>

namespace vcc {
namespace netman {

class IptablesConfig {
  public:
    bool configureSplitTraffic(const std::string &interface, const std::string &gateway_ip_address, bool add);

    bool isSplitTrafficSet(const std::string &interface, const std::string &gateway_ip_address);
};

}  // namespace netman
}  // namespace vcc