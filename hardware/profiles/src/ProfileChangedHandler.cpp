#include "ProfileChangedHandler.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace profiles {
namespace V1_0 {
namespace implementation {

Return<void> ProfileChangedHandler::profileChanged(const hidl_string& androidUserId, ProfileIdentifier profileId) {
    (void)androidUserId;
    (void)profileId;
    // TODO implement
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
