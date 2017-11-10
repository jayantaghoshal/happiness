#include "LscMocker.h"

#define LOG_TAG "LscMocker"

LscMocker::LscMocker() {
    ALOGD("IIplm getService!");
    iplmservice = IIplm::getService();

    if (nullptr == iplmservice.get()) {
        ALOGE("Couldn't find interface to IplmD service");
    }
}

LscMocker::~LscMocker() { ALOGD("~LscMocker"); }

void LscMocker::RegisterLSC(std::string LSCName) {
    if (iplmservice != nullptr) iplmservice->registerService(LSCName, this);
}
void LscMocker::UnregisterLSC(std::string LSCName) {
    if (iplmservice != nullptr) iplmservice->unregisterService(LSCName);
}
void LscMocker::ReleaseResourceGroup(const hidl_string& lscName, ResourceGroup _rg) {
    if (iplmservice != nullptr) iplmservice->releaseResourceGroup(lscName, _rg);
}
void LscMocker::RequestResourceGroup(const hidl_string& lscName, ResourceGroup _rg, ResourceGroupPrio _prio) {
    if (iplmservice != nullptr) iplmservice->requestResourceGroup(lscName, _rg, _prio);
}

// Methods from ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback follow.
Return<void> LscMocker::onResourceGroupStatus(ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) {
    if (onResourceGroupStatusCallback != nullptr) onResourceGroupStatusCallback(resourceGroup, resourceGroupStatus);
    return Void();
}
Return<void> LscMocker::onNodeStatus(Ecu ecuType, bool ecuStatus) {
    if (onNodeStatusCallback != nullptr) onNodeStatusCallback(ecuType, ecuStatus);
    return Void();
}