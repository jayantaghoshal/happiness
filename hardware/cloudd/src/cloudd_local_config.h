#pragma once

#include <vcc/localconfig.h>

#include <tuple>

namespace Connectivity {
/**
 * @brief Network handler class which implements the reading IHU local configuration parameters
 */
class ClouddLocalConfig {
  public:
    //    using address_netmask_collection_ = std::tuple<std::string, std::string>;
    /*
     * Tuple describing the three parts of a traffic limitation entry:
     * 1) String containing a regular expression to match an interface name
     * 2) String containing the number to use in iptables when marking a packet to be picked by a qdisc.
     * 3) String containing the tc 'rate' and 'ceil' (and also 'prio') parameters for the defined qdisc class.
     */

    //    using TrafficLimitEntry = std::tuple<std::string, std::string, std::string>;

    ClouddLocalConfig(const vcc::LocalConfigReaderInterface *lcfg = vcc::LocalConfigDefault());

    /**
     * @brief Initialise the local configuration.
     */
    bool Init();

    /**
     * @brief Get Local Ethernet MAC
     * @return String with MAC for Local Ethernet Interface.
     */
    // std::string GetLocalEthernetMac();

    /**
     * @brief Get the ip address of the ethernet gateway.
     * @return String with an ip address on the format "192.168.0.1".
     */
    // std::string GetEthernetGatewayIpAddress();

    /**
     * @brief Get the ip address of the local device.
     * @return String with an ip address on the format "192.168.0.1".
     */
    // std::string GetDnsServer();

    /**
     * @brief GetLocalEthernetMtu
     * @return Mtu for Ethernet
     */
    // uint32_t GetLocalEthernetMtu();

    /**
     * @brief GetWlanInterface this will be set if Wlan is located outside IHU
     * @return wlan interface
     */
    // std::string GetWlanInterface();

    /**
     * @brief GetCmodInterface this will be set if car modem is located outside IHU
     * @return car modem interface
     */
    // std::string GetCmodInterface();

    /**
     * @brief Get the ip address of the local device.
     * @return String with an ip address on the format "192.168.0.1".
     */
    // const std::vector<address_netmask_collection_> GetLocalEthernetConfiguration();

    /**
     * @brief Get the ip address of the local MOST device.
     * @return String with an ip address on the format "192.168.0.1".
     */
    // const std::vector<address_netmask_collection_> GetLocalMostConfiguration();

    /**
     * @brief Get MOST MAC
     * @return String with MAC for MOST.
     */
    // std::string GetLocalMostMac();

    /**
     * @brief Get the ip address of the local APIX device.
     * @return String with an ip address on the format "192.168.0.1".
     */
    // const std::vector<address_netmask_collection_> GetLocalApixConfiguration();

    /**
     * @brief Get APIX MAC
     * @return String with MAC for APIX.
     */
    // std::string GetLocalApixMac();

    /**
     * @brief Get the configured TCP keep alive timer
     * @return Value of the TCP keep alive timer.
     */
    // uint32_t GetTcpKeepAliveTime();

    /**
     * @brief Get the configured no of retries for TCP keep alive timer.
     * @return Value for the time interval between retries.
     */
    // uint32_t GetTcpKeepAliveIntvl();

    /**
     * @brief Get the configured frequency for TCP keep alive timer.
     * @return Value of the number of retries.
     */
    // uint32_t GetTcpKeepAliveProbes();

    /**
     * @brief Get Car Net IP.
     * @return Car Net string
     */
    // std::string GetCarNet();

    /**
     * @brief Get Car Subnet mask.
     * @return Car Subnet as string
     */
    // std::string GetCarSubnet();

    /**
     * @brief Get whether the logging interface is enabled.
     * @return Bool.
     */
    // bool GetLoggingEnabled();

    /**
     * @brief Get Traffic limitations for network interfaces
     * @return Vector of TrafficLimitEntry elements.
     */
    // std::vector<TrafficLimitEntry> GetTrafficLimits();

    /**
     * @brief Connected_Car_Specification_V, section 7.1.1 volvo.cloud.EntryPoint
     */
    const std::string GetCloudEntryPointAddress();

  private:
    // bool ReadLocalConfig(const std::string& configId, std::string& config);
    // bool ReadLocalConfig(const std::string& configId, int& config);
    // bool ReadLocalConfig(const std::string& configId, bool& config);
    // bool ReadLocalConfig(std::string configId, std::vector<address_netmask_collection_>& collection);
    // bool ReadLocalConfig(std::string configId, std::vector<TrafficLimitEntry>& container);

  private:
    /*    uint32_t ethernetMtu_ = 1500;
        std::string localEthernetMac_ = "02:00:00:02:12:01";
        std::string localApixMac_ = "02:00:00:00:12:01";
        std::string localMostMac_ = "02:00:00:01:12:01";
        std::string wlanInterface_ = "vcm0";
        std::string cmodInterface_ = "vcm0";
        uint32_t tcp_keepalive_time_ = 10;
        uint32_t tcp_keepalive_intvl_ = 5;
        uint32_t tcp_keepalive_probes_ = 2;
        std::string carNet_ = "198.18.0.0";
        std::string carSubnet_ = "255.255.0.0";
        bool loggingEnabled_ = false; */
    std::string cloud_entry_point_address_ = "cnep.test.volvocars.com";

    /*
    std::string ethernetGatewayIpAddress_ = "198.18.32.1";

    std::vector<address_netmask_collection_> localEthernet_ = {std::make_tuple("198.18.34.1", "255.255.240.0")};
    std::vector<address_netmask_collection_> localMost_ = {std::make_tuple("198.18.2.1", "255.255.240.0")};
    std::vector<address_netmask_collection_> localApix_ = {std::make_tuple("198.18.18.1", "255.255.240.0")};

    std::vector<TrafficLimitEntry> trafficLimits_;
*/
    const vcc::LocalConfigReaderInterface *const lcfg_;
};

}  // namespace Connectivity
