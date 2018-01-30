/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "iptables_config.h"

#include <memory>
#include <string>

#include <arpa/inet.h>
#include <libiptc/libiptc.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_TEE.h>

#define LOG_TAG "Netmand"
#include <cutils/log.h>

namespace vcc {
namespace netman {

// TODO (Samuel.Idowu): This class should be refactored.

bool IptablesConfig::configureSplitTraffic(const std::string &interface, const std::string &gateway_ip_address,
                                           bool add) {
    struct alignas(8) {
        struct ipt_entry entry;
        struct ipt_entry_target entry_target;
        struct xt_tee_tginfo tee_info;
    } iptc_info = {};

    auto handle_deleter = [](struct iptc_handle *handle) {
        if (handle) {
            iptc_free(handle);
        }
    };

    std::unique_ptr<struct iptc_handle, decltype(handle_deleter)> iptc_handle(iptc_init("mangle"), handle_deleter);
    if (!iptc_handle) {
        ALOGE("Unable to init iptc");
        return false;
    }

    // Calculate sizes and offsets
    std::uint32_t size_ipt_entry = sizeof(iptc_info.entry);
    std::uint32_t size_target = sizeof(iptc_info.entry_target);
    std::uint32_t size_tee_tg_info = sizeof(iptc_info.tee_info);
    std::uint32_t total_length = size_ipt_entry + size_target + size_tee_tg_info;
    iptc_info.entry.target_offset = size_ipt_entry;
    iptc_info.entry.next_offset = total_length;

    struct ipt_entry_target *entry_target;
    entry_target = reinterpret_cast<struct ipt_entry_target *>(iptc_info.entry.elems);
    entry_target->u.user.target_size = size_target + size_tee_tg_info;

    entry_target->u.user.revision = 1;

    // Set Target
    std::string target("TEE");
    strncpy(entry_target->u.user.name, target.c_str(), target.size() + 1);

    // Set Gateway
    struct xt_tee_tginfo *tee_info;
    tee_info = reinterpret_cast<struct xt_tee_tginfo *>(entry_target->data);
    std::uint32_t gateway;
    int pton_ret = inet_pton(AF_INET, gateway_ip_address.c_str(), &gateway);
    if (pton_ret == 0) {
        ALOGE("Inet_pton error");
        return false;
    }
    tee_info->gw.in.s_addr = gateway;

    strncpy(iptc_info.entry.ip.iniface, interface.c_str(), interface.size() + 1);
    memset(iptc_info.entry.ip.iniface_mask, 0xFF, interface.size() + 1);

    iptc_info.entry.ip.proto = IPPROTO_TCP;

    struct ipt_entry search_mask;
    memset(&search_mask, 0xFF, sizeof(struct ipt_entry));

    // PREROUTING
    if (add) {
        if (0 == iptc_check_entry("PREROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry),
                                  reinterpret_cast<unsigned char *>(&search_mask), iptc_handle.get())) {
            if (0 == iptc_insert_entry("PREROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry), 0,
                                       iptc_handle.get())) {
                ALOGE("Could not insert a mangle TEE rules in iptables %s", iptc_strerror(errno));
                return false;
            }
        }
    } else {
        if (1 == iptc_check_entry("PREROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry),
                                  reinterpret_cast<unsigned char *>(&search_mask), iptc_handle.get())) {
            if (0 == iptc_delete_entry("PREROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry),
                                       reinterpret_cast<unsigned char *>(&search_mask), iptc_handle.get())) {
                ALOGE("Could not delete a mangle rules in iptables %s", iptc_strerror(errno));
                return false;
            }
        } else {
            ALOGW("There was no matching rule to delete.");
        }
    }

    // POSTROUTING
    memset(iptc_info.entry.ip.iniface_mask, 0x00, interface.size());
    strncpy(iptc_info.entry.ip.outiface, interface.c_str(), interface.size() + 1);
    memset(iptc_info.entry.ip.outiface_mask, 0xFF, interface.size() + 1);

    if (add) {
        if (0 == iptc_check_entry("POSTROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry),
                                  reinterpret_cast<unsigned char *>(&search_mask), iptc_handle.get())) {
            if (0 == iptc_insert_entry("POSTROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry), 0,
                                       iptc_handle.get())) {
                ALOGE("Could not insert a mangle TEE rules in iptables %s", iptc_strerror(errno));
                return false;
            }
        }
    } else {
        if (1 == iptc_check_entry("POSTROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry),
                                  reinterpret_cast<unsigned char *>(&search_mask), iptc_handle.get())) {
            if (0 == iptc_delete_entry("POSTROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry),
                                       reinterpret_cast<unsigned char *>(&search_mask), iptc_handle.get())) {
                ALOGE("Could not delete a mangle rules in iptables %s", iptc_strerror(errno));
                return false;
            }
        } else {
            ALOGW("There was no matching rule to delete.");
        }
    }

    if (!iptc_commit(iptc_handle.get())) {
        ALOGI("Could not commit changes mangle TEE rules, errno %s\n", iptc_strerror(errno));
        return false;
    }

    return true;
}

bool IptablesConfig::isSplitTrafficSet(const std::string &interface, const std::string &gateway_ip_address) {
    struct alignas(8) {
        struct ipt_entry entry;
        struct ipt_entry_target entry_target;
        struct xt_tee_tginfo tee_info;
    } iptc_info = {};

    auto handle_deleter = [](struct iptc_handle *handle) {
        if (handle) {
            iptc_free(handle);
        }
    };

    std::unique_ptr<struct iptc_handle, decltype(handle_deleter)> iptc_handle(iptc_init("mangle"), handle_deleter);
    if (!iptc_handle) {
        ALOGE("Unable to init iptc");
        return false;
    }

    // Calculate sizes and offsets
    std::uint32_t size_ipt_entry = sizeof(iptc_info.entry);
    std::uint32_t size_target = sizeof(iptc_info.entry_target);
    std::uint32_t size_tee_tg_info = sizeof(iptc_info.tee_info);
    std::uint32_t total_length = size_ipt_entry + size_target + size_tee_tg_info;
    iptc_info.entry.target_offset = size_ipt_entry;
    iptc_info.entry.next_offset = total_length;

    struct ipt_entry_target *entry_target;
    entry_target = reinterpret_cast<struct ipt_entry_target *>(iptc_info.entry.elems);
    entry_target->u.user.target_size = size_target + size_tee_tg_info;

    entry_target->u.user.revision = 1;

    // Set Target
    std::string target("TEE");
    strncpy(entry_target->u.user.name, target.c_str(), target.size() + 1);

    // Set Gateway
    struct xt_tee_tginfo *tee_info;
    tee_info = reinterpret_cast<struct xt_tee_tginfo *>(entry_target->data);
    std::uint32_t gateway;
    int pton_ret = inet_pton(AF_INET, gateway_ip_address.c_str(), &gateway);
    if (pton_ret == 0) {
        ALOGE("Inet_pton error");
        return false;
    }
    tee_info->gw.in.s_addr = gateway;

    strncpy(iptc_info.entry.ip.iniface, interface.c_str(), interface.size() + 1);
    memset(iptc_info.entry.ip.iniface_mask, 0xFF, interface.size() + 1);

    iptc_info.entry.ip.proto = IPPROTO_TCP;

    struct ipt_entry search_mask;
    memset(&search_mask, 0xFF, sizeof(struct ipt_entry));

    // Check if PREROUTING rule exists
    if (0 == iptc_check_entry("PREROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry),
                              reinterpret_cast<unsigned char *>(&search_mask), iptc_handle.get())) {
        return false;
    }

    memset(iptc_info.entry.ip.iniface_mask, 0x00, interface.size());
    strncpy(iptc_info.entry.ip.outiface, interface.c_str(), interface.size() + 1);
    memset(iptc_info.entry.ip.outiface_mask, 0xFF, interface.size() + 1);

    // Check if POSTROUTING rule exists
    if (0 == iptc_check_entry("POSTROUTING", static_cast<struct ipt_entry *>(&iptc_info.entry),
                              reinterpret_cast<unsigned char *>(&search_mask), iptc_handle.get())) {
        return false;
    }

    return true;
}
}  // namespace netman
}  // namespace vcc
