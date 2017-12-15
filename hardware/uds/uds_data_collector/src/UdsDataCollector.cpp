/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "UdsDataCollector.h"
#include <mutex>

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "uds-collector"

using ::vendor::volvocars::hardware::uds::V1_0::DidReadResult;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_death_recipient;

Return<void> UdsDataCollector::readDidValue(Did did, readDidValue_cb _hidl_cb) {
    sp<IUdsDataProvider> provider = nullptr;
    // Locked part of the function, requires atomic access to providers, important for performance
    {
        std::lock_guard<std::mutex> lock(providers_mtx_);

        auto sit = this->findStaticByDid(did);

        if (sit != static_providers_.end()) {
            DidReadResult result;
            result.status = DidReadStatusCode::SUCCESS;
            result.data = sit->second.value;
            _hidl_cb(result);
            return Return<void>();
        }

        auto dit = this->findDynamicByDid(did);
        if (dit != dynamic_providers_.end()) {
            provider = dit->second.provider_;
            // will do actual call when out of lock
        }
    }

    if (provider != nullptr) {
        return provider->readDidValue(did, _hidl_cb);
    }

    DidReadResult result;
    if (did == 0xFFFF) {
        result.status = DidReadStatusCode::SUCCESS;
        result.data = std::vector<uint8_t>{0x00, 0x01, 0x02, 0x03};
    } else {
        result.status = DidReadStatusCode::NOT_SUPPORTED;
    }
    _hidl_cb(result);
    return Return<void>();
}

Return<uint64_t> UdsDataCollector::provideStaticDidValue(uint16_t did, const hidl_vec<uint8_t>& value) {
    std::lock_guard<std::mutex> lock(providers_mtx_);
    bool preexisting_did_handler = PreexistingHandlerExists({did});

    if (not preexisting_did_handler) {
        providers_handle_gen_++;
        static_providers_.emplace(providers_handle_gen_, StaticProvider(did, value));
        return providers_handle_gen_;
    }
    return 0ul;
}

// TODO(krzysztof.wesolowski@volvocars.com) either map key or reuse, forward to Delphi Diagnostics
Return<bool> UdsDataCollector::reportDiagnosticTestResult(DtcId id, const DiagnosticCheckReport& report) {
    (void)report;
    (void)id;
    return true;
}

bool UdsDataCollector::PreexistingHandlerExists(const hidl_vec<Did>& supported_dids) {
    // This is O(n^2) but it should be fine with amount of actual dids/handlers
    // its also optional, but without this - happy debugging...
    for (Did ndid : std::vector<Did>(supported_dids)) {
        auto sit = this->findStaticByDid(ndid);
        if (sit != static_providers_.end()) {
            ALOGE("Provider for DID %u already registered as static at handle %lu", ndid, sit->first);
            return true;
        }
    }
    for (Did ndid : std::vector<Did>(supported_dids)) {
        auto sit = this->findDynamicByDid(ndid);
        if (sit != dynamic_providers_.end()) {
            ALOGE("Provider for DID %u already registered as dynamic at handle %lu", ndid, sit->first);
            return true;
        }
    }
    return false;
}

Return<SubscriptionHandle> UdsDataCollector::registerDidProvider(const sp<IUdsDataProvider>& provider,
                                                                 const hidl_vec<Did>& supported_dids) {
    std::lock_guard<std::mutex> lock(providers_mtx_);
    bool preexisting_did_handler = PreexistingHandlerExists(supported_dids);

    if (not preexisting_did_handler) {
        providers_handle_gen_++;
        hidl_death_recipient* this_as_recipient = this;
        provider->linkToDeath(this_as_recipient, providers_handle_gen_);
        dynamic_providers_.emplace(providers_handle_gen_, DynamicProvider(provider, supported_dids));
        return providers_handle_gen_;
    }
    return 0ul;
}

Return<bool> UdsDataCollector::unregister(SubscriptionHandle handle) {
    std::lock_guard<std::mutex> lock(providers_mtx_);

    auto dit = dynamic_providers_.find(handle);
    if (dit != dynamic_providers_.end()) {
        hidl_death_recipient* this_as_recipient = this;
        dit->second.provider_->unlinkToDeath(this_as_recipient);
        dynamic_providers_.erase(dit);
        return true;
    }

    auto sit = static_providers_.find(handle);
    if (sit != static_providers_.end()) {
        static_providers_.erase(sit);
        return true;
    }

    ALOGE("Handle %lu was not registered before call to %s", handle, __FUNCTION__);
    return false;
}

void UdsDataCollector::serviceDied(uint64_t cookie, const android::wp<IBase>& who) {
    (void)who;
    auto it = dynamic_providers_.find(cookie);
    if (it != dynamic_providers_.end()) {
        dynamic_providers_.erase(it);
    } else {
        ALOGE("Handle %lu was not registered before call to %s", cookie, __FUNCTION__);
    }
}

UdsDataCollector::StaticMap::const_iterator UdsDataCollector::findStaticByDid(Did did) const {
    auto matcher = [did](const StaticMap::value_type& p) { return did == p.second.did; };
    auto found_it = std::find_if(static_providers_.begin(), static_providers_.end(), matcher);
    return found_it;
}

UdsDataCollector::DynamicMap::const_iterator UdsDataCollector::findDynamicByDid(Did did) const {
    auto matcher = [did](const DynamicMap::value_type& v) { return v.second.SupportsDid(did); };
    auto found_it = std::find_if(dynamic_providers_.begin(), dynamic_providers_.end(), matcher);
    return found_it;
}
