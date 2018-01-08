#include "DataCollector.h"

#include <mutex>

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "uds-collector"

using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_death_recipient;

Return<void> DataCollector::readDidValue(Did did, readDidValue_cb _hidl_cb) {
    sp<uds_v1_0::IDataHandler> provider = nullptr;
    // Locked part of the function, requires atomic access to providers, important for performance
    {
        std::lock_guard<std::mutex> lock(providers_mtx_);

        auto sit = this->findStaticByDid(did);

        if (sit != static_providers_.end()) {
            uds_v1_0::DidReadResult result;
            result.status = uds_v1_0::DidReadStatusCode::SUCCESS;
            result.data = sit->second.value;
            _hidl_cb(result);
            return Return<void>();
        }

        auto dit = this->findHandlerByDid(did);
        if (dit != dynamic_providers_.end()) {
            provider = dit->second.data_handler_;
            // will do actual call when out of lock
        }
    }

    if (provider != nullptr) {
        return provider->readDidValue(did, _hidl_cb);
    }

    _hidl_cb(uds_v1_0::DidReadResult{uds_v1_0::DidReadStatusCode::NOT_SUPPORTED, {}});
    return Return<void>();
}

Return<DidWriteStatusCode> DataCollector::writeDidValue(uint16_t did, const hidl_vec<uint8_t>& data) {
    sp<uds_v1_0::IDataHandler> provider = nullptr;
    // Locked part of the function, requires atomic access to providers, important for performance
    {
        std::lock_guard<std::mutex> lock(providers_mtx_);

        auto dit = this->findHandlerByDid(did);
        if (dit != dynamic_providers_.end()) {
            provider = dit->second.data_handler_;
            // will do actual call when out of lock
        }
    }

    if (provider != nullptr) {
        return provider->writeDidValue(did, data);
    } else {
        return DidWriteStatusCode::NOT_SUPPORTED;
    }
}
Return<uint64_t> DataCollector::provideStaticDidValue(Did did, const hidl_vec<uint8_t>& value) {
    std::lock_guard<std::mutex> lock(providers_mtx_);
    bool preexisting_did_handler = PreexistingDidHandler({did});

    if (not preexisting_did_handler) {
        handle_gen_++;
        static_providers_.emplace(handle_gen_, StaticProvider(did, value));
        return handle_gen_;
    }
    return 0ul;
}

Return<bool> DataCollector::reportDiagnosticTestResult(DtcId id, const uds_v1_0::DiagnosticCheckReport& report) {
    {
        std::lock_guard<std::mutex> lock(dtc_cache_mtx_);
        dtc_cache_[id] = report;
    }

    // TODO(krzysztof.wesolowski@volvocars.com) either map key or reuse, forward to Delphi Diagnostics
    return true;
}

Return<void> DataCollector::getLastDiagnosticTestResult(DtcId id, getLastDiagnosticTestResult_cb _hidl_cb) {
    uds_v1_0::DiagnosticCheckReport report;
    {
        std::lock_guard<std::mutex> lock(dtc_cache_mtx_);
        auto it = dtc_cache_.find(id);
        if (it != dtc_cache_.end()) {
            report = it->second;
        } else {
            report.status = uds_v1_0::DiagnosticCheckStatus::NEVER_REPORTED;
            report.checks_since_last_report = 0;
        }
    }
    _hidl_cb(report);
    return Return<void>();
}

Return<uint64_t> DataCollector::registerRoutineHandler(const ::android::sp<uds_v1_0::IRoutineControlHandler>& handler,
                                                       const hidl_vec<uint16_t>& supported_routine_ids) {
    std::lock_guard<std::mutex> lock(providers_mtx_);
    bool preexisting_handler = PreexistingRoutineHandler(supported_routine_ids);

    if (not preexisting_handler) {
        handle_gen_++;
        hidl_death_recipient* this_as_recipient = this;
        handler->linkToDeath(this_as_recipient, handle_gen_);
        routine_handlers_.emplace(handle_gen_, RoutineHandler(handler, supported_routine_ids));
        return handle_gen_;
    }
    return Return<SubscriptionHandle>(::android::hardware::Status::EX_ILLEGAL_STATE);
}

template <typename RoutineExecutionXxxRequest>
sp<uds_v1_0::IRoutineControlHandler> DataCollector::FindRoutineHandler(const RoutineExecutionXxxRequest& request) {
    sp<uds_v1_0::IRoutineControlHandler> handler = nullptr;
    {
        std::lock_guard<std::mutex> lock(providers_mtx_);
        auto hit = findByRoutineId(request.rid);
        if (hit != routine_handlers_.end()) {
            handler = hit->second.handler_;
        }
    }
    return handler;
}

Return<void> DataCollector::startAsynchronousRoutine(const uds_v1_0::RoutineExecutionStartRequest& request,
                                                     startAsynchronousRoutine_cb _hidl_cb) {
    sp<uds_v1_0::IRoutineControlHandler> handler = FindRoutineHandler(request);

    if (handler != nullptr) {
        auto result = handler->startAsynchronousRoutine(request, _hidl_cb);
        return result;
    }

    _hidl_cb(uds_v1_0::RoutineExecutionStartResponse{uds_v1_0::RoutineControlStatusCode::NOT_SUPPORTED, {}});
    return ::android::hardware::Void();
}

Return<void> DataCollector::stopAsynchronousRoutine(const uds_v1_0::RoutineExecutionStopRequest& request,
                                                    stopAsynchronousRoutine_cb _hidl_cb) {
    sp<uds_v1_0::IRoutineControlHandler> handler = FindRoutineHandler(request);

    if (handler != nullptr) {
        auto result = handler->stopAsynchronousRoutine(request, _hidl_cb);
        return result;
    }

    _hidl_cb(uds_v1_0::RoutineExecutionStopResponse{uds_v1_0::RoutineControlStatusCode::NOT_SUPPORTED, {}});
    return ::android::hardware::Void();
}

Return<void> DataCollector::getAsynchronousRoutineResults(const uds_v1_0::RoutineExecutionResultsRequest& request,
                                                          getAsynchronousRoutineResults_cb _hidl_cb) {
    sp<uds_v1_0::IRoutineControlHandler> handler = FindRoutineHandler(request);

    if (handler != nullptr) {
        auto result = handler->getAsynchronousRoutineResults(request, _hidl_cb);
        return result;
    }

    _hidl_cb(uds_v1_0::RoutineExecutionResultsResponse{uds_v1_0::RoutineControlStatusCode::NOT_SUPPORTED, {}});
    return ::android::hardware::Void();
}

bool DataCollector::PreexistingDidHandler(const hidl_vec<Did>& supported_dids) {
    // This is O(n*m) but it should be fine with amount of actual dids/handlers
    // its also optional, but without this - happy debugging...
    for (Did ndid : std::vector<Did>(supported_dids)) {
        auto it = this->findStaticByDid(ndid);
        if (it != static_providers_.end()) {
            ALOGE("Provider for DID %u already registered as static at handle %lu", ndid, it->first);
            return true;
        }
    }
    for (Did ndid : std::vector<Did>(supported_dids)) {
        auto it = this->findHandlerByDid(ndid);
        if (it != dynamic_providers_.end()) {
            ALOGE("Provider for DID %u already registered as dynamic at handle %lu", ndid, it->first);
            return true;
        }
    }
    return false;
}

Return<SubscriptionHandle> DataCollector::registerDataHandler(const sp<uds_v1_0::IDataHandler>& handler,
                                                              const hidl_vec<Did>& supported_dids) {
    std::lock_guard<std::mutex> lock(providers_mtx_);
    bool preexisting_did_handler = PreexistingDidHandler(supported_dids);

    if (not preexisting_did_handler) {
        handle_gen_++;
        hidl_death_recipient* this_as_recipient = this;
        handler->linkToDeath(this_as_recipient, handle_gen_);
        dynamic_providers_.emplace(handle_gen_, DynamicProvider(handler, supported_dids));
        return handle_gen_;
    }
    return Return<SubscriptionHandle>(0);
}

Return<bool> DataCollector::unregister(SubscriptionHandle handle) {
    std::lock_guard<std::mutex> lock(providers_mtx_);

    auto dit = dynamic_providers_.find(handle);
    if (dit != dynamic_providers_.end()) {
        hidl_death_recipient* this_as_recipient = this;
        dit->second.data_handler_->unlinkToDeath(this_as_recipient);
        dynamic_providers_.erase(dit);
        return true;
    }

    auto sit = static_providers_.find(handle);
    if (sit != static_providers_.end()) {
        static_providers_.erase(sit);
        return true;
    }

    auto rit = routine_handlers_.find(handle);
    if (rit != routine_handlers_.end()) {
        routine_handlers_.erase(rit);
        return true;
    }

    ALOGE("Handle %lu was not registered before call to %s", handle, __FUNCTION__);
    return false;
}

void DataCollector::serviceDied(uint64_t cookie, const android::wp<IBase>& who) {
    (void)who;
    auto it = dynamic_providers_.find(cookie);
    if (it != dynamic_providers_.end()) {
        dynamic_providers_.erase(it);
    } else {
        ALOGE("Handle %lu was not registered before call to %s", cookie, __FUNCTION__);
    }
}

DataCollector::StaticMap::const_iterator DataCollector::findStaticByDid(Did did) const {
    auto matcher = [did](const StaticMap::value_type& p) { return did == p.second.did; };
    auto found_it = std::find_if(static_providers_.begin(), static_providers_.end(), matcher);
    return found_it;
}

DataCollector::DynamicMap::const_iterator DataCollector::findHandlerByDid(Did did) const {
    auto matcher = [did](const DynamicMap::value_type& v) { return v.second.SupportsDid(did); };
    auto found_it = std::find_if(dynamic_providers_.begin(), dynamic_providers_.end(), matcher);
    return found_it;
}

DataCollector::RoutineMap::const_iterator DataCollector::findByRoutineId(RoutineId rid) const {
    auto matcher = [rid](const RoutineMap::value_type& v) { return v.second.SupportsRoutineId(rid); };
    auto found_it = std::find_if(routine_handlers_.begin(), routine_handlers_.end(), matcher);
    return found_it;
}

bool DataCollector::PreexistingRoutineHandler(const hidl_vec<RoutineId>& supported_rids) {
    for (auto routine_id : std::vector<RoutineId>(supported_rids)) {
        auto routine_handler_it = this->findByRoutineId(routine_id);
        if (routine_handler_it != routine_handlers_.end()) {
            ALOGE("Handler for Routine ID %u already registered at handle %lu", routine_id, routine_handler_it->first);
            return true;
        }
    }
    return false;
}
