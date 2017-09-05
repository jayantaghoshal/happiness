#include "IpcbCallback.h"

namespace android {
namespace vendor {
namespace volvocars {
namespace ipcb {
namespace V1_0 {
namespace implementation {

// Methods from ::android::vendor::volvocars::ipcb::V1_0::IIpcbCallback follow.
Return<void> IpcbCallback::onMessageRcvd(const Msg& msg) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IIpcbCallback* HIDL_FETCH_IIpcbCallback(const char* /* name */) {
    return new IpcbCallback();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace ipcb
}  // namespace volvocars
}  // namespace vendor
}  // namespace android
