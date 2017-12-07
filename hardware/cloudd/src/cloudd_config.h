#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDD_CONFIG_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDD_CONFIG_H

#include <string>
#include <vector>

namespace Connectivity {
/**
 * @brief Abstract class which implements the interface to read local configuration parameters
 */
class ClouddConfig {
  public:
    using address_netmask_collection_ = std::tuple<std::string, std::string>;
    /*
     * Tuple describing the three parts of a traffic limitation entry:
     * 1) String containing a regular expression to match an interface name
     * 2) String containing the number to use in iptables when marking a packet to be picked by a qdisc.
     * 3) String containing the tc 'rate' and 'ceil' (and also 'prio') parameters for the defined qdisc class.
     */
    using TrafficLimitEntry = std::tuple<std::string, std::string, std::string>;

    /**
     * @brief Get Local Ethernet MAC
     * @return String with MAC for Local Ethernet Interface.
     */
    virtual std::string GetLocalEthernetMac() = 0;

    /**
     * @brief Get the ip address of the ethernet gateway.
     * @return String with an ip address on the format "192.168.0.1".
     */
    virtual std::string GetEthernetGatewayIpAddress() = 0;

    /**
     * @brief Get the ip address of the local device.
     * @return String with an ip address on the format "192.168.0.1".
     */
    virtual std::string GetDnsServer() = 0;

    /**
     * @brief GetLocalEthernetMtu
     * @return MTU size for ethernet device.
     */
    virtual uint32_t GetLocalEthernetMtu() = 0;

    /**
     * @brief Get the ip address of the local device.
     * @return a vector of tuple for address and netmasks for ethernet interface
     */
    virtual const std::vector<address_netmask_collection_> GetLocalEthernetConfiguration() = 0;

    /**
     * @brief Get the ip address of the local MOST device.
     * @return a vector of tuple for address and netmasks for most interface
     */
    virtual const std::vector<address_netmask_collection_> GetLocalMostConfiguration() = 0;

    /**
     * @brief Get MOST MAC
     * @return String with MAC for MOST.
     */
    virtual std::string GetLocalMostMac() = 0;

    /**
     * @brief Get the ip address of the local APIX device.
     * @return a vector of tuple for address and netmasks for apix interface
     */
    virtual const std::vector<address_netmask_collection_> GetLocalApixConfiguration() = 0;

    /**
     * @brief Get APIX MAC
     * @return String with MAC for APIX.
     */
    virtual std::string GetLocalApixMac() = 0;

    /**
     * @brief Get Wlan interface.
     * @return wlan interface
     */
    virtual std::string GetWlanInterface() = 0;

    /**
     * @brief Get car modem interface.
     * @return car modem interface
     */
    virtual std::string GetCmodInterface() = 0;

    /**
     * @brief Get Car Net IP.
     * @return Car Net string
     */
    virtual std::string GetCarNet() = 0;

    /**
     * @brief Get Car Subnet mask.
     * @return Car Subnet as string
     */
    virtual std::string GetCarSubnet() = 0;

    /**
     * @brief Get whether the logging interface is enabled.
     * @return Bool.
     */
    virtual bool GetLoggingEnabled() = 0;

    /**
     * @brief Get Traffic limitations for network interfaces
     * @return Vector of TrafficLimitEntry elements.
     */
    virtual std::vector<TrafficLimitEntry> GetTrafficLimits() = 0;
};

}  // namespace Connectivity

#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDD_CONFIG_H
