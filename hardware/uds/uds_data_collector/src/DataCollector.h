#ifndef VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H
#define VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H

#include <hidl/Status.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollectorTestPoint.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataHandler.h>
#include <cstdint>
#include <mutex>
#include <set>
#include <vector>

namespace uds_v1_0 = ::vendor::volvocars::hardware::uds::V1_0;

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;

using uds_v1_0::Did;
using uds_v1_0::RoutineId;
using uds_v1_0::DtcId;
using uds_v1_0::SubscriptionHandle;
using uds_v1_0::DidWriteStatusCode;

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using ::android::wp;

constexpr SubscriptionHandle InvalidSubscriptionHandle = 0;

class DataCollector : public uds_v1_0::IDataCollector,
                      public uds_v1_0::IDataCollectorTestPoint,
                      private ::android::hardware::hidl_death_recipient {
  public:
    virtual ~DataCollector() = default;

    // IDataCollector
    Return<SubscriptionHandle> registerDataHandler(const sp<uds_v1_0::IDataHandler>& handler,
                                                   const hidl_vec<Did>& supported_dids) override;
    Return<uint64_t> provideStaticDidValue(Did did, const hidl_vec<uint8_t>& value) override;

    Return<uint64_t> registerRoutineHandler(const ::android::sp<uds_v1_0::IRoutineControlHandler>& handler,
                                            const hidl_vec<uint16_t>& supported_routine_ids) override;

    Return<bool> unregister(SubscriptionHandle handle) override;

    Return<bool> reportDiagnosticTestResult(DtcId id, const uds_v1_0::DiagnosticCheckReport& report) override;

    // IDataCollectorTestPoint
    Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override;
    Return<DidWriteStatusCode> writeDidValue(uint16_t did, const hidl_vec<uint8_t>& data) override;

    Return<void> startAsynchronousRoutine(const uds_v1_0::RoutineExecutionStartRequest& request,
                                          startAsynchronousRoutine_cb _hidl_cb) override;
    Return<void> stopAsynchronousRoutine(const uds_v1_0::RoutineExecutionStopRequest& request,
                                         stopAsynchronousRoutine_cb _hidl_cb) override;
    Return<void> getAsynchronousRoutineResults(const uds_v1_0::RoutineExecutionResultsRequest& request,
                                               getAsynchronousRoutineResults_cb _hidl_cb) override;

    Return<void> getLastDiagnosticTestResult(uint32_t id, getLastDiagnosticTestResult_cb _hidl_cb) override;

    // hidl_death_recipient
    void serviceDied(uint64_t cookie, const wp<IBase>& who) override;

  private:
    struct DynamicProvider {
        DynamicProvider(sp<uds_v1_0::IDataHandler> provider, std::vector<Did> dids)
            : data_handler_(provider), dids_(dids.begin(), dids.end()) {}

        bool SupportsDid(Did did) const { return dids_.find(did) != dids_.end(); }

        const sp<uds_v1_0::IDataHandler> data_handler_;
        const std::set<Did> dids_;
    };

    struct StaticProvider {
        StaticProvider(Did did, std::vector<uint8_t> value) : did(did), value(value) {}

        const Did did;
        const std::vector<uint8_t> value;
    };

    struct RoutineHandler {
        RoutineHandler(sp<uds_v1_0::IRoutineControlHandler> handler, std::vector<uds_v1_0::RoutineId> rids)
            : handler_(handler), rids_(rids.begin(), rids.end()) {}

        bool SupportsRoutineId(RoutineId did) const { return rids_.find(did) != rids_.end(); }

        const sp<uds_v1_0::IRoutineControlHandler> handler_;
        const std::set<RoutineId> rids_;
    };

    std::mutex providers_mtx_;
    typedef std::map<SubscriptionHandle, DynamicProvider> DynamicMap;
    typedef std::map<SubscriptionHandle, StaticProvider> StaticMap;
    typedef std::map<SubscriptionHandle, RoutineHandler> RoutineMap;

    DynamicMap dynamic_providers_;
    StaticMap static_providers_;
    RoutineMap routine_handlers_;
    uint64_t handle_gen_ = 1000;

    std::mutex dtc_cache_mtx_;
    std::map<DtcId, uds_v1_0::DiagnosticCheckReport> dtc_cache_;

    StaticMap::const_iterator findStaticByDid(Did did) const;
    DynamicMap::const_iterator findHandlerByDid(Did did) const;

    RoutineMap::const_iterator findByRoutineId(RoutineId rid) const;
    template <typename RoutineExecutionXxxRequest>
    sp<uds_v1_0::IRoutineControlHandler> FindRoutineHandler(const RoutineExecutionXxxRequest& request);

    bool PreexistingDidHandler(const hidl_vec<Did>& supported_dids);
    bool PreexistingRoutineHandler(const hidl_vec<RoutineId>& supported_rids);
};

#endif  // VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H
