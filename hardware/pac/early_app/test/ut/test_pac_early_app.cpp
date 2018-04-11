/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <pac_early_app.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

using android::hardware::automotive::evs::V1_0::vcc_implementation::PacEarlyApp;

TEST(VtsPacEarlyApp, ConstructObject) {
    sp<PacEarlyApp> pac_early_app = new PacEarlyApp();
    EXPECT_NE(pac_early_app, nullptr);
}

TEST(VtsPacEarlyApp, DeleteObject) {
    sp<PacEarlyApp> pac_early_app = new PacEarlyApp();
    pac_early_app.clear();
    EXPECT_EQ(pac_early_app, (PacEarlyApp*)nullptr);
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
