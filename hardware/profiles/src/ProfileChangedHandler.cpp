/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

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
    // TODO(erik.elmeke@volvocars.com) implement
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
