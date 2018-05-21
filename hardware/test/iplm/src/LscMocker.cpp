/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "LscMocker.h"

#include <chrono>
#include <thread>

#undef LOG_TAG
#define LOG_TAG "LscMocker"
#include <cutils/log.h>

LscMocker::LscMocker() {
    ALOGD("IIplm getService!");
    unsigned int attempts = 0;
    while (iplmservice == nullptr && attempts < 5) {
        attempts++;
        iplmservice = IIplm::tryGetService();
        if (iplmservice == nullptr) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }
    }
}

void LscMocker::RegisterLSC(const std::string& LSCName) {
    if (iplmservice != nullptr) iplmservice->registerService(LSCName, this);
}
void LscMocker::UnregisterLSC(const std::string& LSCName) {
    if (iplmservice != nullptr) iplmservice->unregisterService(LSCName);
}
void LscMocker::ReleaseResourceGroup(const hidl_string& lscName, ResourceGroup _rg) {
    if (iplmservice != nullptr) iplmservice->releaseResourceGroup(lscName, _rg);
}
void LscMocker::RequestResourceGroup(const hidl_string& lscName, ResourceGroup _rg, ResourceGroupPrio _prio) {
    if (iplmservice != nullptr) iplmservice->requestResourceGroup(lscName, _rg, _prio);
}

// Methods from ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback follow.
Return<void> LscMocker::onResourceGroupStatus(ResourceGroup resourceGroup,
                                              ResourceGroupStatus resourceGroupStatus,
                                              ResourceGroupPrio resourceGroupPrio) {
    (void)resourceGroupPrio;
    if (onResourceGroupStatusCallback != nullptr) onResourceGroupStatusCallback(resourceGroup, resourceGroupStatus);
    return Void();
}
Return<void> LscMocker::onNodeStatus(Ecu ecuType, bool ecuStatus) {
    if (onNodeStatusCallback != nullptr) onNodeStatusCallback(ecuType, ecuStatus);
    return Void();
}
