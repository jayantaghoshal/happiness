/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <vendor/volvocars/hardware/signals/1.0/ISignals.h>
#include <vendor/volvocars/hardware/signals/1.0/ISignalsChangedCallback.h>
#include <vendor/volvocars/hardware/signals/1.0/types.h>
#include <regex>
#include <utility>
#include <vector>

namespace vendor {
namespace volvocars {
namespace hardware {
namespace signals {
namespace V1_0 {
namespace implementation {

struct WildCardSubscription {
  public:
    WildCardSubscription(const std::string filter, const Dir dir, ::android::sp<ISignalsChangedCallback> callback);
    std::string filter;
    std::regex regexFilter;
    Dir dir;
    ::android::sp<ISignalsChangedCallback> callback;
};

class SignalsServer final : public ISignals {
  private:
    using signal_key = std::pair<std::string, Dir>;
    signal_key make_key(std::string s, Dir dir);

    std::map<signal_key, std::vector<::android::sp<ISignalsChangedCallback>>> subscriptions;
    std::vector<WildCardSubscription> wildcard_subscriptions;
    std::map<signal_key, std::string> signalStorage;
    std::vector<Result> get_all_matching(const ::android::hardware::hidl_string& filter, const Dir dir);

  public:
    ::android::hardware::Return<void> subscribe(const ::android::hardware::hidl_string&, Dir dir,
                                                const ::android::sp<ISignalsChangedCallback>& cb) override;

    ::android::hardware::Return<void> send(const ::android::hardware::hidl_string& signalname, Dir dir,
                                           const ::android::hardware::hidl_string& data) override;

    using get_cb = std::function<void(const ::android::hardware::hidl_string& data)>;
    ::android::hardware::Return<void> get(const ::android::hardware::hidl_string& signalname, Dir dir,
                                          get_cb _hidl_cb) override;

    using get_all_cb = std::function<void(const ::android::hardware::hidl_vec<Result>& data)>;
    ::android::hardware::Return<void> get_all(const ::android::hardware::hidl_string& filter, const Dir dir,
                                              get_all_cb _hidl_cb) override;
};
}
}
}
}
}
}
