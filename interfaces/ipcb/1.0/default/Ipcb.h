#ifndef VENDOR_VOLVOCARS_HARDWARE_IPCB_V1_0_IPCB_H
#define VENDOR_VOLVOCARS_HARDWARE_IPCB_V1_0_IPCB_H

#include <vendor/volvocars/hardware/ipcb/1.0/IIpcb.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace volvocars {
namespace hardware {
namespace ipcb {
namespace V1_0 {
namespace implementation {

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vendor::volvocars::hardware::ipcb::V1_0::IIpcb;
using ::vendor::volvocars::hardware::ipcb::V1_0::IIpcbCallback;
using ::vendor::volvocars::hardware::ipcb::V1_0::Msg;
using ::vendor::volvocars::hardware::ipcb::V1_0::Status;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Ipcb : public IIpcb {
    // Methods from ::vendor::volvocars::hardware::ipcb::V1_0::IIpcb follow.
    Return<Status> subscribe(uint16_t serviceID, uint16_t operationID, const sp<IIpcbCallback>& callbackHandler) override;
    Return<Status> unsubscribe(uint16_t serviceID, uint16_t operationID, const sp<IIpcbCallback>& callbackHandler) override;
    Return<void> sendMessage(const Msg& msg) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" IIpcb* HIDL_FETCH_IIpcb(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace ipcb
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor

#endif  // VENDOR_VOLVOCARS_HARDWARE_IPCB_V1_0_IPCB_H
