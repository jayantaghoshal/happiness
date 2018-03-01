/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "firewall_config.h"
#include <sys/wait.h>
#include <cstdlib>
#include <vector>

namespace vcc {
namespace netman {

const char* const FirewallConfig::kDefaultIptablesRulesPath = "/mnt/iptables.rules";

const char* const FirewallConfig::kIptablesRestoreExecutablePath = "/vendor/bin/iptables-restore";

const char* const FirewallConfig::kIp6tablesRestoreExecutablePath = "/vendor/bin/ip6tables-restore";

bool FirewallConfig::ParseAndSave(const std::string& output_path) {
    if (output_path.empty()) {
        return false;
    }

    output_path_ = output_path;

    // create .rules file
    std::ofstream output_file(output_path);
    if (!output_file.is_open()) {
        output_path_.clear();
        return false;
    }

    InitTables();
    ParseRules(nat_table_, "NAT_TABLE", output_file);
    ParseRules(filter_, "FILTER_TABLE", output_file);

    output_file.close();
    return true;
}

bool FirewallConfig::ApplyRules(const IP ip_type) {
    if (output_path_.empty()) {
        return false;
    }

    std::string command;
    if (ip_type == IP::IPv4_) {
        command = std::string(FirewallConfig::kIptablesRestoreExecutablePath) + " < " + output_path_;
    } else {
        command = std::string(FirewallConfig::kIp6tablesRestoreExecutablePath) + " < " + output_path_;
    }

    // TODO (Abhijeet Shirolikar): system calls involves command processor and so is vunerable to injection attacks
    // Refactor code below to use exec family function together with fork and pipe
    int command_status = std::system(command.c_str());  // NOLINT

    if (command_status < 0) return false;

    if (!WIFEXITED(command_status) || WEXITSTATUS(command_status) != EXIT_SUCCESS) {
        return false;
    }

    return true;
}

void FirewallConfig::ParseRules(const ruletable& table, const std::string& table_name, std::ofstream& file) {
    const std::string primary_fw_key = "CONN_NhFirewall";

    file << table.top_comment << std::endl;
    file << table.name << std::endl;
    for (const std::string& str : table.chains) {
        file << str << std::endl;
    }

    std::vector<std::string> chains = lcfg_->GetStringArray(primary_fw_key, table_name, "CHAINS");
    for (const auto& item : chains) {
        file << ":" << item << " - [0:0]" << std::endl;
    }

    std::vector<std::string> rules = lcfg_->GetStringArray(primary_fw_key, table_name, "RULES");
    for (const auto& item : rules) {
        file << item << std::endl;
    }
    file << table.final << std::endl;
}

void FirewallConfig::InitTables() {
    nat_table_.top_comment = "# Generated by LCFG on ...";
    nat_table_.name = "*nat";
    nat_table_.chains = {":PREROUTING ACCEPT [0:0]", ":INPUT ACCEPT [0:0]", ":OUTPUT ACCEPT [0:0]",
                         ":POSTROUTING ACCEPT [0:0]"};
    nat_table_.chains = {};
    nat_table_.rules = {};
    nat_table_.final = "COMMIT\n# Completed on ...";

    filter_.top_comment = "# Generated by LCFG on ...";
    filter_.name = "*filter";
    filter_.chains = {":INPUT ACCEPT [0:0]", ":FORWARD ACCEPT [0:0]", ":OUTPUT ACCEPT [0:0]"};
    filter_.chains = {};
    filter_.rules = {};
    filter_.final = "COMMIT\n# Completed on ...";
}

}  // namespace netman
}  // namespace vcc
