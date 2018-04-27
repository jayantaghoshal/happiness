/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "lsc_service.h"

#undef LOG_TAG  // workaround #include <hidl/HidlTransportSupport.h>
#define LOG_TAG "iplm_ctrl.main"
#include <log/log.h>

LscService::LscService() {
    ALOGD("IIplm getService!");
    iplmservice = IIplm::getService();

    if (nullptr == iplmservice.get()) {
        ALOGE("Couldn't find interface to IplmD service");
    }
}

void LscService::RegisterLSC(
        std::string LSCName,
        const std::function<void(int resourceGroup, std::string resourceGroupStatus, std::string resourceGroupPrio)>&
                onResourceGroupStatusCallback,
        const std::function<void(std::string ecuType, bool ecuStatus)>& onNodeStatusCallback) {
    if (iplmservice != nullptr) {
        sp<IplmCallbackImpl> iplmCallbackImpl = new IplmCallbackImpl();
        iplmCallbackImpl->onResourceGroupStatusCallback_ = onResourceGroupStatusCallback;
        iplmCallbackImpl->onNodeStatusCallback_ = onNodeStatusCallback;

        ALOGD("Call register service");
        auto result = iplmservice->registerService(LSCName, iplmCallbackImpl);
        if (result.isOk()) {
            ALOGD("Register succeded, adding LSC");
            registered_LSCs_[LSCName] = iplmCallbackImpl;
        } else {
            ALOGD("Register failed, Not adding LSC!");
        }
    }
}
void LscService::UnregisterLSC(std::string LSCName) {
    if (iplmservice != nullptr) {
        ALOGD("Call unregister service");
        auto result = iplmservice->unregisterService(LSCName);
        if (result.isOk()) {
            ALOGD("Unregister succeded, removing LSC");
            registered_LSCs_.erase(LSCName);
        } else {
            ALOGD("Unregister failed!");
        }
    }
}
void LscService::ReleaseResourceGroup(const std::string& lscName, ResourceGroup _rg) {
    if (iplmservice != nullptr) {
        auto result = iplmservice->releaseResourceGroup(lscName, _rg);
        if (result.isOk()) {
            ALOGD("Release RG succeded");
        } else {
            ALOGD("Release RG failed!");
        }
    }
}
void LscService::RequestResourceGroup(const std::string& lscName, ResourceGroup _rg, ResourceGroupPrio _prio) {
    if (iplmservice != nullptr) {
        auto result = iplmservice->requestResourceGroup(lscName, _rg, _prio);
        if (result.isOk()) {
            ALOGD("Request RG succeded");
        } else {
            ALOGD("Request RG failed!");
        }
    }
}

LscService::~LscService() {
    ALOGD("~LscService");
}

// Methods from ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback follow.
Return<void> IplmCallbackImpl::onResourceGroupStatus(ResourceGroup resourceGroup,
                                                     ResourceGroupStatus resourceGroupStatus,
                                                     ResourceGroupPrio resourceGroupPrio) {
    int rg = (int)resourceGroup;

    std::string resourceGroupStatusStr = "";
    switch (resourceGroupStatus) {
        case ResourceGroupStatus::Unavailable:
            resourceGroupStatusStr = "Unavailable";
            break;
        case ResourceGroupStatus::PartlyAvailable:
            resourceGroupStatusStr = "PartlyAvailable";
            break;
        case ResourceGroupStatus::Available:
            resourceGroupStatusStr = "Available";
            break;
    }

    std::string resourceGroupPrioStr = "";
    switch (resourceGroupPrio) {
        case ResourceGroupPrio::Normal:
            resourceGroupPrioStr = "Normal";
            break;
        case ResourceGroupPrio::High:
            resourceGroupPrioStr = "High";
            break;
    }

    if (onResourceGroupStatusCallback_ != nullptr) {
        onResourceGroupStatusCallback_(rg, resourceGroupStatusStr, resourceGroupPrioStr);
    }

    return Void();
}

Return<void> IplmCallbackImpl::onNodeStatus(Ecu ecuType, bool ecuStatus) {
    std::string ecuTypeStr = "";

    switch (ecuType) {
        case Ecu::ALL:
            ecuTypeStr = "ALL";
            break;
        case Ecu::IHU:
            ecuTypeStr = "IHU";
            break;
        case Ecu::VCM:
            ecuTypeStr = "VCM";
            break;
        case Ecu::TEM:
            ecuTypeStr = "TEM";
            break;
        case Ecu::DIM:
            ecuTypeStr = "DIM";
            break;
        case Ecu::TCAM:
            ecuTypeStr = "TCAM";
            break;
        case Ecu::VGM:
            ecuTypeStr = "VGM";
            break;
    }

    if (onNodeStatusCallback_ != nullptr) {
        onNodeStatusCallback_(ecuTypeStr, ecuStatus);
    }

    return Void();
}
