#define LOG_TAG "Iplmd.lco0"
#include "local_config.h"
#include <cutils/log.h>

#include <vcc/localconfig.h>

namespace Iplmd
{
namespace LocalConfig
{
using namespace vcc;

class Configuration
{
public:
    void init()
    {
        ReadLocalConfig(NodesInRG1_Id, nof_nodes_in_RG1_);
        ReadLocalConfig(NodesInRG3_Id, nof_nodes_in_RG3_);
        ReadLocalConfig(RequestMonitoringTmo_Id, request_monitoring_tmo_);
        ReadLocalConfig(NofLSCs_Id, nof_LSCs_);
    }

    int getNofNodesInRG1()
    {
        return nof_nodes_in_RG1_;
    }

    int getNofNodesInRG3()
    {
        return nof_nodes_in_RG3_;
    }

    int getRequestMonitoringTmo()
    {
        return request_monitoring_tmo_;
    }

    int getNofLSCs()
    {
        return nof_LSCs_;
    }

    std::string getLocalIpAddress()
    {
        return ip_address_local_;
    }
    std::string getBroadcastIpAddress()
    {
        return ip_address_broadcast_;
    }
    std::string getVCMIpAddress()
    {
        return ip_address_vcm_;
    }
    std::string getTEMIpAddress()
    {
        return ip_address_tem_;
    }

    Configuration() : nof_nodes_in_RG1_(3), nof_nodes_in_RG3_(3), request_monitoring_tmo_(3000), nof_LSCs_(0)
    {
    }

private:
    bool ReadLocalConfig(std::string configId, int& config)
    {
        try {
            config = localconfig::GetInt(configId);
        } catch (std::runtime_error e) {
            ALOGE("Error: Could not get parameter: %s", configId.c_str());
            return false;
        }
        ALOGI("%s %i", configId.c_str(), config);
        return true;
    }

    bool ReadLocalConfig(std::string configId, std::string& config)
    {
        try {
            config = localconfig::GetString(configId);
        } catch (std::runtime_error e) {
            ALOGE("Error: Could not get parameter: %s", configId.c_str());
            return false;
        }
        ALOGI("%s %s", configId.c_str(), config.c_str());
        return true;
    }

    int nof_nodes_in_RG1_;
    int nof_nodes_in_RG3_;
    int request_monitoring_tmo_;
    int nof_LSCs_;
    std::string ip_address_local_;
    std::string ip_address_broadcast_;
    std::string ip_address_vcm_;
    std::string ip_address_tem_;

    // Config ID strings
    const std::string NodesInRG1_Id = "IIPS_LM_NofNodesRG1";
    const std::string NodesInRG3_Id = "IIPS_LM_NofNodesRG3";
    const std::string RequestMonitoringTmo_Id = "IIPS_LM_RequestMonitoringTimeout";
    const std::string NofLSCs_Id = "IIPS_LM_NofLSCs";
};

static Configuration config;

void loadLocalConfig()
{
    config.init();
}

int getNofNodesInRG1()
{
    return config.getNofNodesInRG1();
}

int getNofNodesInRG3()
{
    return config.getNofNodesInRG3();
}

int getRequestMonitoringTmo()
{
    return config.getRequestMonitoringTmo();
}

int getNofLocalSoftwareComponents()
{
    return config.getNofLSCs();
}
}
}
