#include "IpcbCallback.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace ipcb {
namespace V1_0 {
namespace implementation {

// Methods from ::vendor::volvocars::hardware::ipcb::V1_0::IIpcbCallback follow.
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
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
