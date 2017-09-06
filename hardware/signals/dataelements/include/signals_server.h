#pragma once
#include "ihu/signals/1.0/ISignals.h"
#include "ihu/signals/1.0/ISignalsChangedCallback.h"
#include "ihu/signals/1.0/types.h"
#include <vector>
#include <utility>

namespace ihu {
namespace signals {
namespace V1_0 {
class SignalsServer final : public ihu::signals::V1_0::ISignals
{    
private:
    using signal_key = std::pair<std::string, Dir>;
    signal_key make_key(std::string s, Dir dir);

    std::map<signal_key, std::vector<::android::sp<ISignalsChangedCallback>>> subscriptions;
    std::map<signal_key, std::string> signalStorage;

public:
    ::android::hardware::Return<void> subscribe(
        const ::android::hardware::hidl_string& signalName, 
        Dir dir, 
        const ::android::sp<ISignalsChangedCallback>& cb) override;

    ::android::hardware::Return<void> send(
        const ::android::hardware::hidl_string& signalname, 
        Dir dir, 
        const ::android::hardware::hidl_string& data) override;

    using get_cb = std::function<void(const ::android::hardware::hidl_string& data, Dir dir)>;
    ::android::hardware::Return<void> get(
        const ::android::hardware::hidl_string& signalname, 
        get_cb _hidl_cb) override;

};
}
}
}