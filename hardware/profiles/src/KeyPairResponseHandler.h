/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/volvocars/hardware/profiles/1.0/IKeyPairResponseHandler.h>

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

struct KeyPairResponseHandler : public IKeyPairResponseHandler {
    Return<void> keySearchCompleted(KeyPairStatus result) override;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
