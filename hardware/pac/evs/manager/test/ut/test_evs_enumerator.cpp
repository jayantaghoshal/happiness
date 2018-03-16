/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <evs_enumerator.h>

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

class MockHwEnumerator : public IEvsEnumerator {
  public:
    MOCK_METHOD1(getService, sp<IEvsEnumerator>(const std::string& serviceName));
    MOCK_METHOD1(getCameraList, Return<void>(getCameraList_cb _hidl_cb));
    MOCK_METHOD1(openCamera, Return<sp<IEvsCamera>>(const hidl_string& cameraId));
    MOCK_METHOD1(closeCamera, Return<void>(const sp<IEvsCamera>& carCamera));
    MOCK_METHOD0(openDisplay, Return<sp<IEvsDisplay>>());
    MOCK_METHOD1(closeDisplay, Return<void>(const sp<IEvsDisplay>& display));
    MOCK_METHOD0(getDisplayState, Return<DisplayState>());
};

TEST(EvsEnumeratorTest, InitSuccess) {
    sp<IEvsEnumerator> mockHwEnumerator = new MockHwEnumerator();
    sp<EvsEnumerator> enumerator = new EvsEnumerator();
    EXPECT_TRUE(enumerator->Init(mockHwEnumerator));
}

TEST(EvsEnumeratorTest, InitFailure) {
    sp<EvsEnumerator> enumerator = new EvsEnumerator();
    EXPECT_FALSE(enumerator->Init(sp<IEvsEnumerator>(nullptr)));
}

TEST(EvsEnumeratorTest, MultipleInitFails) {
    sp<IEvsEnumerator> mockHwEnumerator = new MockHwEnumerator();
    sp<EvsEnumerator> enumerator = new EvsEnumerator();
    EXPECT_TRUE(enumerator->Init(mockHwEnumerator));
    EXPECT_FALSE(enumerator->Init(mockHwEnumerator));
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android
