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

class EvsEnumeratorInitializedTest : public ::testing::Test {
  public:
    void SetUp() override { enumerator_ = new EvsEnumerator(); }

    void TearDown() override { enumerator_.clear(); }

    sp<EvsEnumerator> enumerator_;
};

class EvsEnumeratorDisplayTest : public EvsEnumeratorInitializedTest {
  public:
    void SetUp() override {
        EvsEnumeratorInitializedTest::SetUp();
        display_ = enumerator_->openDisplay();
    }

    void TearDown() override {
        display_.clear();
        EvsEnumeratorInitializedTest::TearDown();
    }

    sp<IEvsDisplay> display_;
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

TEST_F(EvsEnumeratorInitializedTest, OpenDisplaySuccess) {
    // Open new display and verify that its the new active display
    sp<IEvsDisplay> display = enumerator_->openDisplay();
    EXPECT_NE(display, nullptr);
    EXPECT_EQ(enumerator_->active_display_, display);
}

TEST_F(EvsEnumeratorDisplayTest, OpenDisplayExclusive) {
    // Verify that there is an active display
    EXPECT_EQ(enumerator_->active_display_, display_);

    // Verify that nullptr is returned when openDisplay is called when there already is an active display
    sp<IEvsDisplay> display = enumerator_->openDisplay();
    EXPECT_EQ(display, nullptr);
    EXPECT_EQ(enumerator_->active_display_, display_);
}

TEST_F(EvsEnumeratorDisplayTest, OpenNewDisplayAfterClose) {
    // Verify that there is an active display
    EXPECT_EQ(enumerator_->active_display_, display_);

    // Close the active display
    enumerator_->closeDisplay(display_);
    EXPECT_EQ(enumerator_->active_display_, nullptr);

    // Verify that its possible to open a new display after closing the old one
    sp<IEvsDisplay> display = enumerator_->openDisplay();
    EXPECT_NE(display, nullptr);
    EXPECT_EQ(enumerator_->active_display_, display);
}

TEST_F(EvsEnumeratorInitializedTest, CloseDisplayNullPtr) {
    sp<IEvsDisplay> display = new EvsDrmDisplay();
    enumerator_->active_display_ = display;

    // Verify that the active display is not released for a NULL pointer
    enumerator_->closeDisplay(nullptr);
    EXPECT_EQ(enumerator_->active_display_, display);
}

TEST_F(EvsEnumeratorInitializedTest, CloseDisplayUnknownDisplay) {
    sp<IEvsDisplay> display = new EvsDrmDisplay();
    enumerator_->active_display_ = display;

    // Verify that the active display is not released for an unknown display
    sp<IEvsDisplay> unknown_display = new EvsDrmDisplay();
    enumerator_->closeDisplay(unknown_display);
    EXPECT_EQ(enumerator_->active_display_, display);
}

TEST_F(EvsEnumeratorInitializedTest, CloseDisplaySuccess) {
    sp<IEvsDisplay> display = new EvsDrmDisplay();
    enumerator_->active_display_ = display;

    // Verify that the active display is released if the active display is specified
    enumerator_->closeDisplay(display);
    EXPECT_EQ(enumerator_->active_display_, nullptr);
}

TEST_F(EvsEnumeratorInitializedTest, GetDisplayStateNoActiveDisplay) {
    // Verify that NOT_OPEN is returned if there is no active display
    EXPECT_EQ(enumerator_->getDisplayState(), DisplayState::NOT_OPEN);
}

TEST_F(EvsEnumeratorInitializedTest, GetDisplayStateOK) {
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
