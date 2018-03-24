/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "router.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/wait.h>

#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include <boost/optional.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <cutils/properties.h>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_Routerd"
#include <cutils/log.h>

using namespace vcc::remotectrl::router;
namespace pt = boost::property_tree;

namespace {

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
    ALOGD("%s: action = %s", __FUNCTION__, action.c_str());

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

int main(int argc, char* argv[]) {
    using namespace std::chrono_literals;

    if (argc < 2) {
        // Only expected to happen during development and not in production image
        ALOGE("Expects vsomeip configuration file location as argument");
        std::this_thread::sleep_for(1s);
        return EXIT_FAILURE;
    }

    const char* environment_variable = "VSOMEIP_CONFIGURATION";
    const char* environment_value = argv[1];
    ALOGI("%s %s", environment_variable, environment_value);

    setenv(environment_variable, environment_value, 1 /*override*/);

    std::string maddr;
    std::string if_name;
    if (const auto& optional = ReadMulticastConfiguration(environment_value)) {
        maddr = (*optional).first;
        if_name = (*optional).second;
        if (!UpdateMulticastRoute("add", maddr, if_name)) {
            std::this_thread::sleep_for(1s);
            return EXIT_FAILURE;
        }
    }

    try {
        Router router;
        property_set("remotectrl_routerd.startup_completed", "1");
        router.Start();
    } catch (const std::exception& e) {
        ALOGE("%s", e.what());
        UpdateMulticastRoute("delete", maddr, if_name);
        std::this_thread::sleep_for(1s);
        return EXIT_FAILURE;
    }

    UpdateMulticastRoute("delete", maddr, if_name);
    return EXIT_SUCCESS;
}
