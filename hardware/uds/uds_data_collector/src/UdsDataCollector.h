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

class UdsDataCollector : public IUdsDataCollector, public IUdsDataProvider {
 public:
  virtual ~UdsDataCollector() = default;

  // IUdsDataCollector
  Return<bool> registerProvider(const sp<IUdsDataProvider>& provider,
                                const hidl_vec<uint16_t>& supported_dids) override;

  Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override;

 private:
  struct ProviderDecl {
    std::set<uint16_t> dids;
    sp<IUdsDataProvider> provider;
  };

  std::vector<ProviderDecl> providers_decls_;
  std::mutex providers_mtx_;
};

#endif  // VENDOR_VOLVOCARS_HARDWARE_UDS_V1_0_UDSDATACOLLECTOR_H
