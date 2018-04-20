/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/volvocars/hardware/iplm/1.0/IIplm.h>
#include <vendor/volvocars/hardware/iplm/1.0/IIplmCallback.h>

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

class IplmCallbackImpl : public IIplmCallback {
  public:
    std::function<void(int resourceGroup, std::string resourceGroupStatus, std::string resourceGroupPrio)>
            onResourceGroupStatusCallback_;
    std::function<void(std::string ecuType, bool ecuStatus)> onNodeStatusCallback_;

  private:
    // Methods from ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback follow.
    Return<void> onResourceGroupStatus(ResourceGroup resourceGroup,
                                       ResourceGroupStatus resourceGroupStatus,
                                       ResourceGroupPrio resourceGroupPrio) override;
    Return<void> onNodeStatus(Ecu ecuType, bool ecuStatus) override;
};

class LscService {
  public:
    LscService();
    ~LscService();
    void RegisterLSC(std::string LSCName,
                     const std::function<void(int resourceGroup,
                                              std::string resourceGroupStatus,
                                              std::string resourceGroupPrio)>& onResourceGroupStatusCallback,
                     const std::function<void(std::string ecuType, bool ecuStatus)>& onNodeStatusCallback_);
    void UnregisterLSC(std::string LSCName);
    void ReleaseResourceGroup(const std::string& lscName, ResourceGroup _rg);
    void RequestResourceGroup(const std::string& lscName, ResourceGroup _rg, ResourceGroupPrio _prio);

  private:
    // Variables
    sp<IIplm> iplmservice;
    std::map<std::string, sp<IplmCallbackImpl>> registered_LSCs_;
};
