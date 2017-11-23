#ifndef VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H
#define VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H

#include <hidl/Status.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataCollector.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataProvider.h>
#include <cstdint>
#include <mutex>
#include <set>
#include <vector>

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::SubscriptionHandle;
using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataProvider;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckReport;
using ::vendor::volvocars::hardware::uds::V1_0::Did;
using ::vendor::volvocars::hardware::uds::V1_0::DtcId;

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using ::android::wp;

class UdsDataCollector : public IUdsDataCollector,
                         public IUdsDataProvider,
                         private ::android::hardware::hidl_death_recipient {
  public:
    virtual ~UdsDataCollector() = default;

    // IUdsDataCollector
    Return<SubscriptionHandle> registerDidProvider(const sp<IUdsDataProvider>& provider,
                                                   const hidl_vec<uint16_t>& supported_dids) override;
    Return<uint64_t> provideStaticDidValue(uint16_t did, const ::android::hardware::hidl_vec<uint8_t>& value) override;

    Return<bool> unregister(SubscriptionHandle handle) override;

    // IUdsDataProvider
    Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override;

    // hidl_death_recipient
    void serviceDied(uint64_t cookie, const wp<IBase>& who) override;

    Return<bool> reportDiagnosticTestResult(DtcId id, const DiagnosticCheckReport& report) override;

  private:
    struct DynamicProvider {
        DynamicProvider(sp<IUdsDataProvider> provider, std::vector<Did> dids)
            : provider_(provider), dids_(dids.begin(), dids.end()) {}

        bool SupportsDid(Did did) const { return dids_.find(did) != dids_.end(); }

        const sp<IUdsDataProvider> provider_;
        const std::set<Did> dids_;
    };

    struct StaticProvider {
        StaticProvider(Did did, std::vector<uint8_t> value) : did(did), value(value) {}

        const Did did;
        const std::vector<uint8_t> value;
    };

    std::mutex providers_mtx_;
    typedef std::map<SubscriptionHandle, DynamicProvider> DynamicMap;
    typedef std::map<SubscriptionHandle, StaticProvider> StaticMap;

    DynamicMap dynamic_providers_;
    StaticMap static_providers_;

    uint64_t providers_handle_gen_ = 1000;

    StaticMap::const_iterator findStaticByDid(Did did) const;
    DynamicMap::const_iterator findDynamicByDid(Did did) const;
    bool PreexistingHandlerExists(const android::hardware::hidl_vec<Did>& supported_dids);
};

#endif  // VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H
