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

#include "firewall_config.h"
#include <sys/wait.h>
#include <cstdlib>
#include <vector>

namespace vcc {
namespace netman {

const std::string FirewallConfig::kDefaultIptablesRulesPath = "/mnt/iptables.rules";

bool FirewallConfig::ParseAndSave(const std::string& output_path) {
    if (output_path.empty()) {
        return false;
    }

    output_path_ = output_path;

    // create .rules file
    std::ofstream output_file;
    output_file.open(output_path.c_str(), std::ofstream::out | std::ofstream::trunc);
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

#ifdef ENABLE_TESTS
    std::string command = output_path_;
#else
    std::string command;
    if (ip_type == IP::IPv4_) {
        command = "/system/bin/iptables-restore < " + output_path_;
    } else {
        command = "/system/bin/ip6tables-restore < " + output_path_;
    }

#endif  // ENABLE_TESTS

    int command_status = system(command.c_str());

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
    for (std::string it : table.chains) file << it << std::endl;

    std::vector<std::string> chains = lcfg_->GetStringArray(primary_fw_key, table_name, "CHAINS");
    for (auto item : chains) {
        file << ":" << item << " - [0:0]" << std::endl;
    }

    std::vector<std::string> rules = lcfg_->GetStringArray(primary_fw_key, table_name, "RULES");
    for (auto item : rules) {
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
