/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "router.h"

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <thread>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#undef LOG_TAG
#define LOG_TAG "VSomeIP_Routerd"
#include <cutils/log.h>

namespace {

namespace pt = boost::property_tree;

std::string GetInterface(const std::string& unicast_address) {
    struct ifaddrs* addrs;
    std::string result;

    if (-1 == getifaddrs(&addrs)) {
        ALOGE("Could not retrieve list of network interfaces");
        return result;
    }

    for (struct ifaddrs* iap = addrs; iap != nullptr; iap = iap->ifa_next) {
        if (nullptr != iap->ifa_addr && IFF_UP == (iap->ifa_flags & IFF_UP) && iap->ifa_addr->sa_family == AF_INET) {
            auto sa = reinterpret_cast<struct sockaddr_in*>(iap->ifa_addr);
            char addr[16];  // len('xxx.xxx.xxx.xxx' + '\0')
            if (nullptr ==
                inet_ntop(iap->ifa_addr->sa_family, reinterpret_cast<void*>(&(sa->sin_addr)), addr, sizeof(addr))) {
                break;
            }

            if (0 == unicast_address.compare(addr)) {
                result = std::string(iap->ifa_name);
                break;
            }
        }
    }
    freeifaddrs(addrs);
    return result;
}

bool UpdateMulticastRoute(const std::string& action, const std::string& maddr, const std::string& if_name) {
    if (maddr.empty() || if_name.empty()) {
        ALOGE("invalid multicast addr '%s' or interface_name '%s'", maddr.c_str(), if_name.c_str());
        return false;
    }

    std::string cmd = std::string("/vendor/bin/ip route ") + action + " multicast " + maddr + " dev " + if_name;

    auto command_status = std::system(cmd.c_str());  // NOLINT

    if ((command_status < 0) || !WIFEXITED(command_status) || WEXITSTATUS(command_status) != EXIT_SUCCESS) {
        ALOGE("failed to set multicast route with error (%d). Service Discovery may not be functional.",
              WEXITSTATUS(command_status));
        return WEXITSTATUS(command_status) == ENOENT;
    }

    return true;
}

boost::optional<std::pair<std::string, std::string>> ReadMulticastConfiguration(const std::string& config_file_path) {
    pt::ptree root;
    pt::read_json(config_file_path, root);

    const auto& nodes = root.get_child("service-discovery", pt::ptree());
    const auto& node =
            std::find_if(nodes.begin(), nodes.end(), [](const auto& node) { return node.first == "multicast"; });
    if (node == nodes.end()) {
        return boost::none;
    }
    return std::make_pair(node->second.data(), GetInterface(root.get<std::string>("unicast", std::string())));
}

}  // namespace

namespace vcc {
namespace someip {
namespace router {

Router::Router(const std::string& vsomeip_app_name, const char* environment_value)
    : vsomeip_appl_(vsomeip::runtime::get()->create_application(vsomeip_app_name)) {
    // TODO (Abhi): Need to setup multicast route. Should Netman do it? or this router should do it?

    if (nullptr == vsomeip_appl_) {
        throw std::runtime_error(std::string("Failed to create_application ") + vsomeip_app_name);
    }

    // Initialize service application
    if (!vsomeip_appl_->init()) {
        throw std::runtime_error(std::string("Couldn't initialize ") + vsomeip_app_name);
    }

    const char* environment_variable = "VSOMEIP_CONFIGURATION";

    setenv(environment_variable, environment_value, 1 /*override*/);

    std::string maddr;
    std::string if_name;
    if (const auto& optional = ReadMulticastConfiguration(environment_value)) {
        maddr = (*optional).first;
        if_name = (*optional).second;
        if (!UpdateMulticastRoute("add", maddr, if_name)) {
            throw std::runtime_error(std::string("Failed to update mutlicast route"));
        }
    }
}

Router::~Router() {
    Stop();
}

void Router::Start() {
    vsomeip_appl_->start();
}
void Router::Stop() {
    vsomeip_appl_->stop();
}

}  // namespace router
}  // namespace someip
}  // namespace vcc
