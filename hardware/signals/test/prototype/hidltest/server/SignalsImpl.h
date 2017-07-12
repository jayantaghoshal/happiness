#ifndef VCC_VCCTEST_V1_0_SIGNALS_H
#define VCC_VCCTEST_V1_0_SIGNALS_H

#include <vcc/vcctest/1.0/ISignals.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include "vcc/vcctest/1.0/ISignalsCallback.h"

namespace vcc {
namespace vcctest {
namespace V1_0 {
namespace implementation {

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vcc::vcctest::V1_0::ISignals;
using ::vcc::vcctest::V1_0::ISignalsCallback;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;


struct SignalsImpl : public ISignals {
    SignalsImpl();
    // Methods from ::vcc::vcctest::V1_0::ISignals follow.
    Return<uint32_t> set(const hidl_string& propValue) override;
    Return<void> subscribe(const sp<ISignalsCallback>& cb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    sp<ISignalsCallback> _cb;
};

extern "C" ISignals* HIDL_FETCH_ISignals(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace vcctest
}  // namespace vcc

#endif  // VCC_VCCTEST_V1_0_SIGNALS_H
