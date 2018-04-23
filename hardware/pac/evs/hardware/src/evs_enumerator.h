/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <android/hardware/automotive/evs/1.0/IEvsEnumerator.h>
#include <gtest/gtest_prod.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

class EvsEnumerator : public IEvsEnumerator {
  public:
    Return<void> getCameraList(getCameraList_cb camera_list_cb) override;
    Return<sp<IEvsCamera>> openCamera(const hidl_string& camera_id) override;
    Return<void> closeCamera(const sp<IEvsCamera>& car_camera) override;
    Return<sp<IEvsDisplay>> openDisplay() override;
    Return<void> closeDisplay(const sp<IEvsDisplay>& display) override;
    Return<DisplayState> getDisplayState() override;

  private:
    wp<IEvsDisplay> active_display_;

    FRIEND_TEST(EvsEnumeratorTest, ConstructObject);

    FRIEND_TEST(EvsEnumeratorDisplayTest, CloseDisplayNullPtr);
    FRIEND_TEST(EvsEnumeratorDisplayTest, CloseDisplaySuccess);
    FRIEND_TEST(EvsEnumeratorDisplayTest, CloseDisplayUnknownDisplay);
    FRIEND_TEST(EvsEnumeratorDisplayTest, OpenDisplay);
};

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
