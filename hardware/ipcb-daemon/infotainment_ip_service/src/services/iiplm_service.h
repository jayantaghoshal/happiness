#pragma once

#include <sstream>
#include "ipcommandbus/Message.h"

namespace Connectivity
{
class IIpLmService
{

  public:
    // ============================================================================
    // TODO these enums should probably NOT be defined here. Maybe in a Xidl file!?
    enum class XResourceGroup
    {
        kResourceGroup1,
        kResourceGroup2,
        kResourceGroup3,
        kResourceGroup4,
        kResourceGroup5,
        kResourceGroup6,
        kResourceGroup7,
        kResourceGroup8
    };

    enum class XResourcePrio
    {
        kPrioNormal,
        kPrioHigh
    };

    enum class XResourceGroupStatus {
        kResourceGroupStatus_Unavailable,
        kResourceGroupStatus_PartlyAvailable,
        kResourceGroupStatus_Available
    };
    // ====================================================


    virtual ~IIpLmService() = default;
    /**
     * description: Request the given Resource Group
     */
    virtual void Request(std::string _serviceName,
                         XResourceGroup _rg,
                         XResourcePrio _prio /*,
                         generated::IpLmServiceStub::RequestReply_t _reply*/) = 0;

    /**
     * description: Release the given Resource Group
     */
    virtual void Release(std::string _serviceName,
                         XResourceGroup _rg /*,
                         generated::IpLmServiceStub::ReleaseReply_t _reply*/) = 0;

    /**
     * description: Register Local Software Component with Link Manager
     */
    virtual void RegisterService(std::string _serviceName /*,
                                 generated::IpLmServiceStub::RegisterServiceReply_t _reply*/) = 0;

    /**
     * description: Unregister Local Software Component with Link Manager
     */
    virtual void UnRegisterService(std::string _serviceName /*,
                                   generated::IpLmServiceStub::UnRegisterServiceReply_t _reply*/) = 0;

    virtual bool GetEcuAvailibility(const Message::Ecu &ecu, uint8_t &resourceGroup, uint8_t &prio) = 0;
};

}  // namespace Connectivity
