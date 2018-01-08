/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollectorTestPoint.h>
#include <iostream>

#include "mock_data_handler.h"

using namespace ::vendor::volvocars::hardware::uds::V1_0;
using namespace ::vendor::volvocars::hardware::uds::V1_0::mocks;

using ::android::sp;
using ::android::hardware::Void;
using ::android::hardware::Return;

struct UdsCollectorProperForwardingTest : public ::testing::Test {
  public:
    void SetUp() override {
        provider1 = new ::testing::StrictMock<MockDataHandler>();
        provider2 = new ::testing::StrictMock<MockDataHandler>();
        collector = IDataCollector::getService();
        test_point = IDataCollectorTestPoint::getService();

        ASSERT_THAT(collector, ::testing::NotNull());
        ASSERT_THAT(test_point, ::testing::NotNull());

        h1 = collector->registerDataHandler(provider1, {0x0001});
        h2 = collector->registerDataHandler(provider2, {0x0002});
    }

    void TearDown() override {
        auto ret1 = collector->unregister(h1);
        EXPECT_TRUE(ret1);
        auto ret2 = collector->unregister(h2);
        EXPECT_TRUE(ret2);
    }

    sp<MockDataHandler> provider1;
    sp<MockDataHandler> provider2;
    sp<IDataCollectorTestPoint> test_point;
    sp<IDataCollector> collector;

    SubscriptionHandle h1;
    SubscriptionHandle h2;
};

TEST_F(UdsCollectorProperForwardingTest, UnknownDidReturnNotSupported) {
    auto ret = test_point->readDidValue(0xFFFE, [](const DidReadResult& didread) {
        EXPECT_THAT(didread.status, DidReadStatusCode::NOT_SUPPORTED);
    });
    EXPECT_TRUE(ret.isOk());
}

TEST_F(UdsCollectorProperForwardingTest, ForwardingCallToProviderOne) {
    EXPECT_CALL(*provider1, ReadDidValue(0x0001))
            .WillOnce(testing::Return(DidReadResult{DidReadStatusCode::SUCCESS, {0x01u}}));

    auto ret = test_point->readDidValue(0x0001, [](const DidReadResult& didread) {
        EXPECT_THAT(didread.status, DidReadStatusCode::SUCCESS);
        EXPECT_THAT((std::vector<uint8_t>)didread.data, ::testing::ElementsAre(0x01));
    });
    EXPECT_TRUE(ret.isOk());

    std::vector<uint8_t> sample_data{0xA5};

    EXPECT_CALL(*provider1, WriteDidValue(0x0001, sample_data)).WillOnce(testing::Return(DidWriteStatusCode::SUCCESS));

    auto ret2 = test_point->writeDidValue(0x0001, sample_data);

    ASSERT_TRUE(ret2.isOk());
    EXPECT_TRUE(ret2 == DidWriteStatusCode::SUCCESS);
}

TEST_F(UdsCollectorProperForwardingTest, ForwardingCallToProviderTwo) {
    EXPECT_CALL(*provider2, ReadDidValue(0x0002))
            .WillOnce(::testing::Return(DidReadResult{DidReadStatusCode::SUCCESS, {0x02u}}));

    auto ret = test_point->readDidValue(0x0002, [](const DidReadResult& didread) {
        EXPECT_THAT(didread.status, DidReadStatusCode::SUCCESS);
        EXPECT_THAT((std::vector<uint8_t>)didread.data, ::testing::ElementsAre(0x02));
    });
    EXPECT_TRUE(ret.isOk());
}
