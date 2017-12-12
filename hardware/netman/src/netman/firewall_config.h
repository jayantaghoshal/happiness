/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef FIREWALL_CONFIG_H_
#define FIREWALL_CONFIG_H_

#include <forward_list>
#include <fstream>
#include <string>
#include "vcc/localconfig.h"

namespace vcc {
namespace netman {

class FirewallConfig final {
  public:
    enum class IP { IPv4_, IPv6_ };
    static const char* const kDefaultIptablesRulesPath;

    /**
     * @brief Constructor
     */
    explicit FirewallConfig(const vcc::LocalConfigReaderInterface* lcfg) : lcfg_(lcfg) {}
    ~FirewallConfig() = default;

    /**
     * @brief Convert iptables part of LocalConfig to iptables.rules file
     * @param output_path: path to save .rules file
     * @return true if successful, false on error
     */
    bool ParseAndSave(const std::string& output_path);

    /**
     * @brief Run iptables-restore to apply rules
     * @param ip_type to select between ipv4 and ipv6 restore
     * @return true if successful, false on error
     */
    bool ApplyRules(const IP ip_type);

  private:
    /**
     * @brief iptables-style rules structure implementation
     */
    struct ruletable {
        std::string top_comment;
        std::string name;
        std::forward_list<std::string> chains;
        std::forward_list<std::string> rules;
        std::string final;
    };
    /**
     * @brief Fill the .rules file with rules, etc.
     * @param table: data for specific table of .rules file
     * @param table_name: Expected name of the table in LocalConfig
     * @param file: Instance of .rules file
     * @return void
     */
    void ParseRules(const ruletable& table, const std::string& table_name, std::ofstream& file);

    /**
     * @brief Initialize tables structures with default data
     * @return void
     */
    void InitTables();

    /**
     * @brief path of output .rules file
     */
    std::string output_path_;

    /**
     * @brief nat table data
     */
    ruletable nat_table_;

    /**
     * @brief security table data
     */
    ruletable security_;

    /**
     * @brief raw table data
     */
    ruletable raw_;

    /**
     * @brief mangle table data
     */
    ruletable mangle_;

    /**
     * @brief filter table data
     */
    ruletable filter_;

    const vcc::LocalConfigReaderInterface* const lcfg_;
};

}  // namespace netman
}  // namespace vcc

#endif  // FIREWALL_CONFIG_H_
