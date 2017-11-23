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

TEST(UdsCollectorOnTargetTest, SubscriptionResubscriptionAndUnsubscriptionOfProvider) {
    auto collector = IUdsDataCollector::getService();
    sp<IUdsDataProvider> provider = new testing::StrictMock<MockUdsProvider>();
    auto handle_0x0000 = collector->registerDidProvider(provider, {0x0000u});
    EXPECT_TRUE(handle_0x0000);

    auto handle_0x0001 = collector->registerDidProvider(provider, {0x0001u});
    EXPECT_TRUE(handle_0x0001);

    auto reregistered_same_did_same_provider = collector->registerDidProvider(provider, {0x0001u});
    EXPECT_FALSE(reregistered_same_did_same_provider);

    auto unregistered_provider_0x0000 = collector->unregister(handle_0x0000);
    EXPECT_TRUE(unregistered_provider_0x0000);
    auto unregistered_provider_0x0001 = collector->unregister(handle_0x0001);
    EXPECT_TRUE(unregistered_provider_0x0001);

    auto handle_0x0000_again = collector->registerDidProvider(provider, {0x0000u});
    EXPECT_TRUE(handle_0x0000_again);

    auto unregistered_provider_0x0000_again = collector->unregister(handle_0x0000_again);
    EXPECT_TRUE(unregistered_provider_0x0000_again);
}
