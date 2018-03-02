/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <evs_enumerator.h>
#include <gtest/gtest.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

TEST(VtsHalPacEvsHardware, ConstructObject) {
    sp<EvsEnumerator> enumerator = new EvsEnumerator();
    EXPECT_NE(enumerator, nullptr);
}

TEST(VtsHalPacEvsHardware, DeleteObject) {
    sp<EvsEnumerator> enumerator = new EvsEnumerator();

    enumerator.clear();

    EXPECT_EQ(enumerator, (EvsEnumerator*)nullptr);
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
