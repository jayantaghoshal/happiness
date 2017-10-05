#include <vendor/volvocars/hardware/iplm/1.0/IIplm.h>
#include <vendor/volvocars/hardware/iplm/1.0/IIplmCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <cutils/log.h>

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vendor::volvocars::hardware::common::V1_0::Ecu;
using ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroup;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroupStatus;
using ::vendor::volvocars::hardware::iplm::V1_0::IIplm;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroupPrio;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

#define LOG_TAG "LscMocker"

class LscMocker : public IIplmCallback
{
    public:
        LscMocker();
        void RegisterLSC(std::string LSCName);
        void UnregisterLSC(std::string LSCName);
        void ReleaseResourceGroup(const hidl_string& lscName, ResourceGroup _rg);
        void RequestResourceGroup(const hidl_string& lscName, ResourceGroup _rg, ResourceGroupPrio _prio);

        // Variables
        std::function<void(ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus)> onResourceGroupStatusCallback;
        std::function<void(Ecu ecuType, bool ecuStatus)> onNodeStatusCallback;
    private:
        // Methods from ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback follow.
        Return<void> onResourceGroupStatus(ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) override;
        Return<void> onNodeStatus(Ecu ecuType, bool ecuStatus) override;
        // Variables
        sp<IIplm> iplmservice;
    };
