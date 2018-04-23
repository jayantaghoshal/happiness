/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <evs_drm_display.h>
#include <gtest/gtest.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

TEST(EvsDrmDisplayTest, ConstructObject) {
    sp<EvsDrmDisplay> display = new EvsDrmDisplay();
    EXPECT_NE(display, nullptr);
}

TEST(EvsDrmDisplayTest, DeleteObject) {
    sp<EvsDrmDisplay> display = new EvsDrmDisplay();

    display.clear();

    EXPECT_EQ(display, (EvsDrmDisplay*)nullptr);
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
