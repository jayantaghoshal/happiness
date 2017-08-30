#include "Ipcb.h"

namespace android {
namespace vendor {
namespace volvocars {
namespace ipcb {
namespace V1_0 {
namespace implementation {

// Methods from ::android::vendor::volvocars::ipcb::V1_0::IIpcb follow.
Return<Status> Ipcb::subscribe(uint16_t serviceID, uint16_t operationID, const sp<IIpcbCallback>& callbackHandler) {
    // TODO implement
    return ::android::vendor::volvocars::ipcb::V1_0::Status {};
}

Return<Status> Ipcb::unsubscribe(uint16_t serviceID, uint16_t operationID, const sp<IIpcbCallback>& callbackHandler) {
    // TODO implement
    return ::android::vendor::volvocars::ipcb::V1_0::Status {};
}

Return<void> Ipcb::sendMessage(const Msg& msg) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

IIpcb* HIDL_FETCH_IIpcb(const char* /* name */) {
    return new Ipcb();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace ipcb
}  // namespace volvocars
}  // namespace vendor
}  // namespace android
