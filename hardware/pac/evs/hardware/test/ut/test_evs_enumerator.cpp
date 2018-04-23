/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <evs_drm_display.h>
#include <evs_enumerator.h>

#include <gtest/gtest.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

class EvsEnumeratorDisplayTest : public ::testing::Test {
  public:
    void SetUp() override { enumerator_ = new EvsEnumerator(); }

    void TearDown() override { enumerator_.clear(); }

    sp<EvsEnumerator> enumerator_;
};

TEST(EvsEnumeratorTest, ConstructObject) {
    sp<EvsEnumerator> enumerator = new EvsEnumerator();
    EXPECT_NE(enumerator, nullptr);
    EXPECT_EQ(enumerator->active_display_, nullptr);
}

TEST(EvsEnumeratorTest, DeleteObject) {
    sp<EvsEnumerator> enumerator = new EvsEnumerator();

    enumerator.clear();

    EXPECT_EQ(enumerator, (EvsEnumerator*)nullptr);
}

TEST_F(EvsEnumeratorDisplayTest, OpenDisplay) {
    // Open new display and verify that its the new active display
    sp<IEvsDisplay> display = enumerator_->openDisplay();
    EXPECT_NE(display, nullptr);
    EXPECT_EQ(enumerator_->active_display_, display);

    // Open a new display again and verify that the new one is now the active display
    sp<IEvsDisplay> second_display = enumerator_->openDisplay();
    EXPECT_NE(second_display, nullptr);
    EXPECT_NE(display, second_display);
    EXPECT_EQ(enumerator_->active_display_, second_display);
}

TEST_F(EvsEnumeratorDisplayTest, CloseDisplayNullPtr) {
    sp<IEvsDisplay> display = new EvsDrmDisplay();
    enumerator_->active_display_ = display;

    // Verify that the active display is not released for a NULL pointer
    enumerator_->closeDisplay(nullptr);
    EXPECT_EQ(enumerator_->active_display_, display);
}

TEST_F(EvsEnumeratorDisplayTest, CloseDisplayUnknownDisplay) {
    sp<IEvsDisplay> display = new EvsDrmDisplay();
    enumerator_->active_display_ = display;

    // Verify that the active display is not released for an unknown display
    sp<IEvsDisplay> unknown_display = new EvsDrmDisplay();
    enumerator_->closeDisplay(unknown_display);
    EXPECT_EQ(enumerator_->active_display_, display);
}

TEST_F(EvsEnumeratorDisplayTest, CloseDisplaySuccess) {
    sp<IEvsDisplay> display = new EvsDrmDisplay();
    enumerator_->active_display_ = display;

    // Verify that the active display is released if the active display is specified
    enumerator_->closeDisplay(display);
    EXPECT_EQ(enumerator_->active_display_, nullptr);
}

TEST_F(EvsEnumeratorDisplayTest, GetDisplayStateNoActiveDisplay) {
    // Verify that NOT_OPEN is returned if there is no active display
    EXPECT_EQ(enumerator_->getDisplayState(), DisplayState::NOT_OPEN);
}

TEST_F(EvsEnumeratorDisplayTest, GetDisplayStateOK) {
    sp<IEvsDisplay> display = enumerator_->openDisplay();
    // EvsDrmDisplay is currently hardcoded to return DEAD
    EXPECT_EQ(enumerator_->getDisplayState(), DisplayState::DEAD);
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
