#ifndef HIDL_GENERATED_VCC_VCCTEST_V1_0_BPHWSIGNALS_H
#define HIDL_GENERATED_VCC_VCCTEST_V1_0_BPHWSIGNALS_H

#include <hidl/HidlTransportSupport.h>

#include <vcc/vcctest/1.0/IHwSignals.h>

namespace vcc {
namespace vcctest {
namespace V1_0 {

struct BpHwSignals : public ::android::hardware::BpInterface<ISignals>, public ::android::hardware::details::HidlInstrumentor {
    explicit BpHwSignals(const ::android::sp<::android::hardware::IBinder> &_hidl_impl);

    virtual bool isRemote() const override { return true; }

    // Methods from ISignals follow.
    ::android::hardware::Return<uint32_t> set(const ::android::hardware::hidl_string& propValue) override;
    ::android::hardware::Return<void> subscribe(const ::android::sp<ISignalsCallback>& cb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    ::android::hardware::Return<void> interfaceChain(interfaceChain_cb _hidl_cb) override;
    ::android::hardware::Return<void> debug(const ::android::hardware::hidl_handle& fd, const ::android::hardware::hidl_vec<::android::hardware::hidl_string>& options) override;
    ::android::hardware::Return<void> interfaceDescriptor(interfaceDescriptor_cb _hidl_cb) override;
    ::android::hardware::Return<void> getHashChain(getHashChain_cb _hidl_cb) override;
    ::android::hardware::Return<void> setHALInstrumentation() override;
    ::android::hardware::Return<bool> linkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient, uint64_t cookie) override;
    ::android::hardware::Return<void> ping() override;
    ::android::hardware::Return<void> getDebugInfo(getDebugInfo_cb _hidl_cb) override;
    ::android::hardware::Return<void> notifySyspropsChanged() override;
    ::android::hardware::Return<bool> unlinkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient) override;

private:
    std::mutex _hidl_mMutex;
    std::vector<::android::sp<::android::hardware::hidl_binder_death_recipient>> _hidl_mDeathRecipients;
};

}  // namespace V1_0
}  // namespace vcctest
}  // namespace vcc

#endif  // HIDL_GENERATED_VCC_VCCTEST_V1_0_BPHWSIGNALS_H
