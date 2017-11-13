#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataCollector.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataProvider.h>
#include <iostream>

#include "mock_uds_provider.h"

using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataProvider;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadResult;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;
using ::android::hardware::Void;
using ::android::hardware::Return;
using ::android::sp;

TEST(UdsCollectorOnTargetTest, TestProgramDoNotCrash) {}

TEST(UdsCollectorOnTargetTest, TestProgramCanObtainCollectorHandle) {
    auto handle = IUdsDataCollector::getService();
    EXPECT_THAT(handle, testing::NotNull());
}

TEST(UdsCollectorOnTargetTest, TestProgramCanObtainTestPointProviderHandle) {
    auto handle = IUdsDataProvider::getService("test-point");
    EXPECT_THAT(handle, testing::NotNull());
}

TEST(UdsCollectorOnTargetTest, TestPointCanRead0xFFFF) {
    auto handle = IUdsDataProvider::getService("test-point");
    auto ret = handle->readDidValue(0xFFFF, [&](const DidReadResult& result) {
        EXPECT_THAT(result.status, DidReadStatusCode::SUCCESS);
        EXPECT_THAT((std::vector<uint8_t>)result.data, testing::ElementsAre(0x00, 0x01, 0x02, 0x03));
    });

    EXPECT_TRUE(ret.isOk());
}
// TODO(krzysztof.wesolowski@volvocars.com) Fix after O-MR1 intergration
// TODO(b/32172906)
TEST(UdsCollectorOnTargetTest, DISABLED_SubscriptionResubscriptionAndUnsubscriptionOfProvider) {
    auto collector = IUdsDataCollector::getService();
    sp<IUdsDataProvider> provider = new testing::StrictMock<MockUdsProvider>();

    auto registered = collector->registerProvider(provider, {0x0000u});
    EXPECT_TRUE(registered);

    auto reregistered_different_did_same_provider = collector->registerProvider(provider, {0x0001u});
    EXPECT_FALSE(reregistered_different_did_same_provider);

    auto reregistered_same_did_same_provider = collector->registerProvider(provider, {0x0001u});
    EXPECT_FALSE(reregistered_same_did_same_provider);

    auto unregistered_provider = collector->unregisterProvider(provider);
    EXPECT_TRUE(registered);

    auto reregistered_after_unregister = collector->registerProvider(provider, {0x0000u});
    EXPECT_TRUE(reregistered_after_unregister);

    auto unregistered_provider_again = collector->unregisterProvider(provider);
    EXPECT_TRUE(registered);
}
