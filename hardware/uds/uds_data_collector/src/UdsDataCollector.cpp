#include "UdsDataCollector.h"

using ::vendor::volvocars::hardware::uds::V1_0::DidReadResult;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;

Return<bool> UdsDataCollector::registerProvider(const sp<IUdsDataProvider>& provider,
                                                const hidl_vec<uint16_t>& supported_dids) {
  ProviderDecl p;
  p.provider = provider;
  p.dids.insert(supported_dids.begin(), supported_dids.end());
  {
    std::unique_lock<std::mutex> lock(providers_mtx_);
    providers_decls_.push_back(p);
  }
  return true;
}

Return<void> UdsDataCollector::readDidValue(uint16_t did, readDidValue_cb _hidl_cb) {
  sp<IUdsDataProvider> provider = nullptr;
  {
    std::unique_lock<std::mutex> lock(providers_mtx_);
    for (ProviderDecl& p : providers_decls_) {
      bool is_did_supported = p.dids.find(did) != p.dids.end();
      if (is_did_supported) {
        provider = p.provider;
      }
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
