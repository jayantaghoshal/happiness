#include "UdsDataCollector.h"

using ::vendor::volvocars::hardware::uds::V1_0::DidReadResult;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_death_recipient;

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "uds-collector"

Return<bool> UdsDataCollector::registerProvider(const sp<IUdsDataProvider>& provider,
                                                const hidl_vec<uint16_t>& supported_dids) {
  {
    std::unique_lock<std::mutex> lock(providers_mtx_);
    auto pre_existing_it = findProviderByBase(provider.get());
    if (pre_existing_it != providers_decls_.end()) {
      ALOGE("This provider is already registered");
      return false;
    }

    hidl_death_recipient* this_as_recipient = this;
    provider->linkToDeath(this_as_recipient, 0);

    providers_decls_.emplace(provider, supported_dids);
  }
  return true;
}

Return<void> UdsDataCollector::readDidValue(uint16_t did, readDidValue_cb _hidl_cb) {
  sp<IUdsDataProvider> provider = nullptr;
  {
    std::unique_lock<std::mutex> lock(providers_mtx_);
    for (const ProviderDecl& p : providers_decls_) {
      if (p.SupportsDid(did)) {
        provider = p.provider_;
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

Return<void> UdsDataCollector::unregisterProvider(const sp<IUdsDataProvider>& provider) {
  removeProvider(provider.get());
  hidl_death_recipient* this_as_recipient = this;
  provider->unlinkToDeath(this_as_recipient);
  return Void();
}

void UdsDataCollector::serviceDied(uint64_t cookie, const android::wp<IBase>& who) {
  (void)cookie;
  auto died_service = who.promote();
  removeProvider(died_service.get());
}

std::set<UdsDataCollector::ProviderDecl>::iterator UdsDataCollector::findProviderByBase(IBase* iface) {
  auto iface_matcher = [iface](const ProviderDecl& p) {
    IBase* stored_service_ptr = p.provider_.get();
    return stored_service_ptr == iface;
  };
  auto found_it = std::find_if(providers_decls_.begin(), providers_decls_.end(), iface_matcher);
  return found_it;
}

void UdsDataCollector::removeProvider(IBase* died_service_ptr) {
  std::unique_lock<std::mutex> lock(providers_mtx_);

  auto found_it = findProviderByBase(died_service_ptr);

  if (found_it != providers_decls_.end()) {
    providers_decls_.erase(found_it);
  } else {
    ALOGE("Service was not subscribed during unsubscribe attempt");
  }
}
