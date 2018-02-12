/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "lscservice.h"

#include <cutils/log.h>

LscService::LscService() {
    ALOGD("IIplm getService!");
    iplmservice = IIplm::getService();

    if (nullptr == iplmservice.get()) {
        ALOGE("Couldn't find interface to IplmD service");
    }
}

void LscService::RegisterLSC(std::string LSCName) {
    if (iplmservice != nullptr) iplmservice->registerService(LSCName, this);
}
void LscService::UnregisterLSC(std::string LSCName) {
    if (iplmservice != nullptr) iplmservice->unregisterService(LSCName);
}
void LscService::ReleaseResourceGroup(const hidl_string& lscName, ResourceGroup _rg) {
    if (iplmservice != nullptr) iplmservice->releaseResourceGroup(lscName, _rg);
}
void LscService::RequestResourceGroup(const hidl_string& lscName, ResourceGroup _rg, ResourceGroupPrio _prio) {
    if (iplmservice != nullptr) iplmservice->requestResourceGroup(lscName, _rg, _prio);
}

LscService::~LscService() { ALOGD("~LscService"); }
// Methods from ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback follow.
Return<void> LscService::onResourceGroupStatus(ResourceGroup resourceGroup,
                                               ResourceGroupStatus resourceGroupStatus,
                                               ResourceGroupPrio resourceGroupPrio) {
    if (onResourceGroupStatusCallback != nullptr) onResourceGroupStatusCallback(resourceGroup, resourceGroupStatus);
    return Void();
}
Return<void> LscService::onNodeStatus(Ecu ecuType, bool ecuStatus) {
    if (onNodeStatusCallback != nullptr) onNodeStatusCallback(ecuType, ecuStatus);
    return Void();
}
