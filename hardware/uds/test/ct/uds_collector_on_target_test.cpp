#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataCollector.h>
#include <iostream>

using namespace ::testing;

using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataProvider;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadResult;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;

TEST(UdsCollectorOnTargetTest, TestProgramDoNotCrash) {}

TEST(UdsCollectorOnTargetTest, TestProgramCanObtainCollectorHandle) {
    auto handle = IUdsDataCollector::getService();
    EXPECT_THAT(handle, NotNull());
}

TEST(UdsCollectorOnTargetTest, TestProgramCanObtainTestPointProviderHandle) {
    auto handle = IUdsDataProvider::getService("test-point");
    EXPECT_THAT(handle, NotNull());
}

TEST(UdsCollectorOnTargetTest, TestPointCanRead0xFFFF) {
    auto handle = IUdsDataProvider::getService("test-point");
    auto ret = handle->readDidValue(0xFFFF, [&](const DidReadResult& result) {
        EXPECT_THAT(result.status, DidReadStatusCode::SUCCESS);
        EXPECT_THAT((std::vector<uint8_t>)result.data, ElementsAre(0x00, 0x01, 0x02, 0x03));
    });

    EXPECT_TRUE(ret.isOk());
}
