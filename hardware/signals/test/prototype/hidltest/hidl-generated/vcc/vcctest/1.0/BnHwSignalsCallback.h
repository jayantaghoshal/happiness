#ifndef HIDL_GENERATED_VCC_VCCTEST_V1_0_BNHWSIGNALSCALLBACK_H
#define HIDL_GENERATED_VCC_VCCTEST_V1_0_BNHWSIGNALSCALLBACK_H

#include <vcc/vcctest/1.0/IHwSignalsCallback.h>

namespace vcc {
namespace vcctest {
namespace V1_0 {

struct BnHwSignalsCallback : public ::android::hidl::base::V1_0::BnHwBase {
    explicit BnHwSignalsCallback(const ::android::sp<ISignalsCallback> &_hidl_impl);
    explicit BnHwSignalsCallback(const ::android::sp<ISignalsCallback> &_hidl_impl, const std::string& HidlInstrumentor_package, const std::string& HidlInstrumentor_interface);

    ::android::status_t onTransact(
            uint32_t _hidl_code,
            const ::android::hardware::Parcel &_hidl_data,
            ::android::hardware::Parcel *_hidl_reply,
            uint32_t _hidl_flags = 0,
            TransactCallback _hidl_cb = nullptr) override;

    ::android::sp<ISignalsCallback> getImpl() { return _hidl_mImpl; };
private:
    // Methods from ISignalsCallback follow.

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    ::android::hardware::Return<void> ping();
    using getDebugInfo_cb = ::android::hidl::base::V1_0::IBase::getDebugInfo_cb;
    ::android::hardware::Return<void> getDebugInfo(getDebugInfo_cb _hidl_cb);

    ::android::sp<ISignalsCallback> _hidl_mImpl;
};

}  // namespace V1_0
}  // namespace vcctest
}  // namespace vcc

#endif  // HIDL_GENERATED_VCC_VCCTEST_V1_0_BNHWSIGNALSCALLBACK_H
