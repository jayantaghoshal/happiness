#ifndef VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H
#define VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H

#include <hidl/Status.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataCollector.h>
#include <cstdint>
#include <set>
#include <vector>

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataProvider;
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
  Return<bool> registerProvider(const sp<IUdsDataProvider>& provider,
                                const hidl_vec<uint16_t>& supported_dids) override;

  Return<void> unregisterProvider(const sp<IUdsDataProvider>& provider) override;

  // IUdsDataProvider
  Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override;

  // hidl_death_recipient
  void serviceDied(uint64_t cookie, const wp<IBase>& who) override;

 private:
  struct ProviderDecl {
    ProviderDecl(sp<IUdsDataProvider> provider, std::vector<uint16_t> dids)
        : provider_(provider), dids_(dids.begin(), dids.end()) {}

    bool SupportsDid(uint16_t did) const { return dids_.find(did) != dids_.end(); }

    const sp<IUdsDataProvider> provider_;
    const std::set<uint16_t> dids_;

    bool operator<(const ProviderDecl& rhs) const { return this->provider_.get() < rhs.provider_.get(); }
  };

  std::set<ProviderDecl> providers_decls_;
  std::mutex providers_mtx_;

  std::set<ProviderDecl>::iterator findProviderByBase(IBase*);
  void removeProvider(IBase*);
};

#endif  // VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H
