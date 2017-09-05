#ifndef ANDROID_VENDOR_VOLVOCARS_IPCB_V1_0_IPCBCALLBACK_H
#define ANDROID_VENDOR_VOLVOCARS_IPCB_V1_0_IPCBCALLBACK_H

#include <android/vendor/volvocars/ipcb/1.0/IIpcbCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace vendor {
namespace volvocars {
namespace ipcb {
namespace V1_0 {
namespace implementation {

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::vendor::volvocars::ipcb::V1_0::IIpcbCallback;
using ::android::vendor::volvocars::ipcb::V1_0::Msg;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct IpcbCallback : public IIpcbCallback {
    // Methods from ::android::vendor::volvocars::ipcb::V1_0::IIpcbCallback follow.
    Return<void> onMessageRcvd(const Msg& msg) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

extern "C" IIpcbCallback* HIDL_FETCH_IIpcbCallback(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace ipcb
}  // namespace volvocars
}  // namespace vendor
}  // namespace android

#endif  // ANDROID_VENDOR_VOLVOCARS_IPCB_V1_0_IPCBCALLBACK_H
