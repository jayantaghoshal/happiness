/**
 * Unit test of the vipcom_client file.
 *
 * Production code under test:
 *   ihu_cedric/carconfig_updater/src/vipcom_client.cpp
 *
 */

#include "vipcom_client.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class CarConfigVipComTestFixture : public ::testing::Test {
  public:
    CarConfigVipCom ccVipcom;
    virtual void SetUp() {}
    virtual void TearDown() {}
};

// CarConfigVipCom::sendConfig() just delegates work to VipCom.::sendMessage(),
// so we are only covering the case available through the VipCom stub.
TEST_F(CarConfigVipComTestFixture, sendConfig_Ok) {
    std::vector<uint8_t> payload = {1, 2};
    // Expecting 2 extra bytes to be sent: FID and message control byte
    int expected = payload.size() + 2;

    ASSERT_EQ(ccVipcom.sendConfig(payload), expected);
}

// CarConfigVipCom::waitForVipAcknowledge() relies on multithreaded asynchronous
// response from VipCom library so we are only covering the available case.
TEST_F(CarConfigVipComTestFixture, waitForVipAcknowledge_NotOk_Timeout) {
    ASSERT_EQ(ccVipcom.waitForVipAcknowledge(), -1);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
