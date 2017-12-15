/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataCollector.h>
#include <iostream>

#include "mock_uds_provider.h"

using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::IUdsDataProvider;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadResult;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;
using ::vendor::volvocars::hardware::uds::V1_0::SubscriptionHandle;

using ::android::sp;
using ::android::hardware::Void;
using ::android::hardware::Return;

struct UdsCollectorProperForwardingTest : public ::testing::Test {
  public:
    void SetUp() override {
        provider1 = new ::testing::StrictMock<MockUdsProvider>();
        provider2 = new ::testing::StrictMock<MockUdsProvider>();
        collector = IUdsDataCollector::getService();
        provider_test_point = IUdsDataProvider::getService("test-point");

        h1 = collector->registerDidProvider(provider1, {0x0001});
        h2 = collector->registerDidProvider(provider2, {0x0002});
    }

    void TearDown() override {
        auto ret1 = collector->unregister(h1);
        EXPECT_TRUE(ret1);
        auto ret2 = collector->unregister(h2);
        EXPECT_TRUE(ret2);
    }

    sp<MockUdsProvider> provider1;
    sp<MockUdsProvider> provider2;
    sp<IUdsDataProvider> provider_test_point;
    sp<IUdsDataCollector> collector;

    SubscriptionHandle h1;
    SubscriptionHandle h2;
};

TEST_F(UdsCollectorProperForwardingTest, UnknownDidReturnNotSupported) {
    auto ret = provider_test_point->readDidValue(0xFFFE, [](const DidReadResult& didread) {
        EXPECT_THAT(didread.status, DidReadStatusCode::NOT_SUPPORTED);
    });
    EXPECT_TRUE(ret.isOk());
}

TEST_F(UdsCollectorProperForwardingTest, ForwardingCallToProviderOne) {
    EXPECT_CALL(*provider1, ReadDidValue(0x0001))
            .WillOnce(testing::Return(DidReadResult{DidReadStatusCode::SUCCESS, {0x01u}}));

    auto ret = provider_test_point->readDidValue(0x0001, [](const DidReadResult& didread) {
        EXPECT_THAT(didread.status, DidReadStatusCode::SUCCESS);
        EXPECT_THAT((std::vector<uint8_t>)didread.data, ::testing::ElementsAre(0x01));
    });
    EXPECT_TRUE(ret.isOk());
}

TEST_F(UdsCollectorProperForwardingTest, ForwardingCallToProviderTwo) {
    EXPECT_CALL(*provider2, ReadDidValue(0x0002))
            .WillOnce(::testing::Return(DidReadResult{DidReadStatusCode::SUCCESS, {0x02u}}));

    auto ret = provider_test_point->readDidValue(0x0002, [](const DidReadResult& didread) {
        EXPECT_THAT(didread.status, DidReadStatusCode::SUCCESS);
        EXPECT_THAT((std::vector<uint8_t>)didread.data, ::testing::ElementsAre(0x02));
    });
    EXPECT_TRUE(ret.isOk());
}
