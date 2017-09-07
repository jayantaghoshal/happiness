#ifndef VENDOR_VOLVOCARS_HARDWARE_IPCB_V1_0_IPCBCALLBACK_H
#define VENDOR_VOLVOCARS_HARDWARE_IPCB_V1_0_IPCBCALLBACK_H

#include <vendor/volvocars/hardware/ipcb/1.0/IIpcbCallback.h>
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
using ::vendor::volvocars::hardware::ipcb::V1_0::IIpcbCallback;
using ::vendor::volvocars::hardware::ipcb::V1_0::Msg;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct IpcbCallback : public IIpcbCallback {
    // Methods from ::vendor::volvocars::hardware::ipcb::V1_0::IIpcbCallback follow.
    Return<void> onMessageRcvd(const Msg& msg) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" IIpcbCallback* HIDL_FETCH_IIpcbCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace ipcb
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor

#endif  // VENDOR_VOLVOCARS_HARDWARE_IPCB_V1_0_IPCBCALLBACK_H
