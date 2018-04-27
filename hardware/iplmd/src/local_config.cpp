/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "local_config.h"

#include <vcc/localconfig.h>

#define LOG_TAG "IplmD"
#include <cutils/log.h>

namespace Iplmd {
namespace LocalConfig {
using namespace vcc;

class Configuration {
  public:
    Configuration(const vcc::LocalConfigReaderInterface* lcfg = vcc::LocalConfigDefault())
        : nof_nodes_in_RG1_(3), nof_nodes_in_RG3_(3), request_monitoring_tmo_(3000), nof_LSCs_(0), lcfg_(lcfg) {}

    void init() {
        lcfg_->TryGetValue(&nof_nodes_in_RG1_, NodesInRG1_Id);
        lcfg_->TryGetValue(&nof_nodes_in_RG3_, NodesInRG3_Id);
        lcfg_->TryGetValue(&request_monitoring_tmo_, RequestMonitoringTmo_Id);
        lcfg_->TryGetValue(&nof_LSCs_, NofLSCs_Id);
    }

    int getNofNodesInRG1() { return nof_nodes_in_RG1_; }

    int getNofNodesInRG3() { return nof_nodes_in_RG3_; }

    int getRequestMonitoringTmo() { return request_monitoring_tmo_; }

    int getNofLSCs() { return nof_LSCs_; }

  private:
    int nof_nodes_in_RG1_;
    int nof_nodes_in_RG3_;
    int request_monitoring_tmo_;
    int nof_LSCs_;

    // Config ID strings
    const std::string NodesInRG1_Id = "IPCB_LM_NofNodesRG1";
    const std::string NodesInRG3_Id = "IPCB_LM_NofNodesRG3";
    const std::string RequestMonitoringTmo_Id = "IPCB_LM_RequestMonitoringTimeout";
    const std::string NofLSCs_Id = "IPCB_LM_NofLSCs";

    const vcc::LocalConfigReaderInterface* const lcfg_;
};

static Configuration config;

void loadLocalConfig() {
    config.init();
}

int getNofNodesInRG1() {
    return config.getNofNodesInRG1();
}

int getNofNodesInRG3() {
    return config.getNofNodesInRG3();
}

int getRequestMonitoringTmo() {
    return config.getRequestMonitoringTmo();
}

int getNofLocalSoftwareComponents() {
    return config.getNofLSCs();
}
}
}
