/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/volvocars/hardware/profiles/1.0/IProfileChangedHandler.h>
#include <vendor/volvocars/hardware/profiles/1.0/types.h>

namespace vendor {
namespace volvocars {
namespace hardware {
namespace profiles {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct ProfileChangedHandler : public IProfileChangedHandler {
    Return<void> profileChanged(const hidl_string& androidUserId, ProfileIdentifier profileId) override;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
