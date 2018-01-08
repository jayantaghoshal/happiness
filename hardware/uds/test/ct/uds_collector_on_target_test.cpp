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

using ::vendor::volvocars::hardware::uds::V1_0::IDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::IDataHandler;

using ::vendor::volvocars::hardware::uds::V1_0::IDataCollectorTestPoint;
using ::vendor::volvocars::hardware::uds::V1_0::mocks::MockDataHandler;

using ::vendor::volvocars::hardware::uds::V1_0::DidReadResult;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;
using ::vendor::volvocars::hardware::uds::V1_0::DidReadStatusCode;

using ::android::hardware::Void;
using ::android::hardware::Return;
using ::android::sp;

TEST(UdsCollectorOnTargetTest, TestProgramDoNotCrash) {}

TEST(UdsCollectorOnTargetTest, TestProgramCanObtainCollectorHandle) {
    auto handle = IDataCollector::getService();
    EXPECT_THAT(handle, testing::NotNull());
}

TEST(UdsCollectorOnTargetTest, TestProgramCanObtainTestPointProviderHandle) {
    auto handle = IDataCollectorTestPoint::getService();
    EXPECT_THAT(handle, testing::NotNull());
}

TEST(UdsCollectorOnTargetTest, SubscriptionResubscriptionAndUnsubscriptionOfProvider) {
    auto collector = IDataCollector::getService();
    ASSERT_TRUE(collector != nullptr);

    sp<IDataHandler> data_handler = new testing::StrictMock<MockDataHandler>();
    auto handle_0x0000 = collector->registerDataHandler(data_handler, {0x0000u});
    EXPECT_TRUE(handle_0x0000);

    auto handle_0x0001 = collector->registerDataHandler(data_handler, {0x0001u});
    EXPECT_TRUE(handle_0x0001);

    auto reregistered_same_did_same_provider = collector->registerDataHandler(data_handler, {0x0001u});
    EXPECT_TRUE(reregistered_same_did_same_provider == 0);

    auto unregistered_provider_0x0000 = collector->unregister(handle_0x0000);
    EXPECT_TRUE(unregistered_provider_0x0000);
    auto unregistered_provider_0x0001 = collector->unregister(handle_0x0001);
    EXPECT_TRUE(unregistered_provider_0x0001);

    auto handle_0x0000_again = collector->registerDataHandler(data_handler, {0x0000u});
    EXPECT_TRUE(handle_0x0000_again);

    auto unregistered_provider_0x0000_again = collector->unregister(handle_0x0000_again);
    EXPECT_TRUE(unregistered_provider_0x0000_again);
}
