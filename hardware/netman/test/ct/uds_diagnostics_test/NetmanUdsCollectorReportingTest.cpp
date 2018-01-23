/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollector.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollectorTestPoint.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <thread>
#include "NetmanInterfaceTestHelper.h"
using ::vendor::volvocars::hardware::uds::V1_0::IDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::IDataCollectorTestPoint;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckReport;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus;

TEST(NetmanUdsCollectorReportingTest, WhenApixIsDownShouldReportUdsApixFailed) {
    // Arrange
    // Get service
    NetmanInterfaceTestHelper netman_interface_test_helper;
    netman_interface_test_helper.BringUpNetworkInterfaces();
    auto test_collector_service = IDataCollectorTestPoint::getService();
    if (test_collector_service == nullptr) {
        FAIL();
    }
    constexpr int APIX_DTC_ID = 0x960B00;

    // Act
    // Take down APIX Link
    netman_interface_test_helper.TakeDownNetworkInterface("apix0");
    // TODO(arjun.snehaj.lal@volvocars.com) At present test is based on a constant delay of 120 msec and is fragile.
    // A cleaner solution is needed. Instead of relying on constant delay where uds is queried after netman has sent
    // a report
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    // Assert
    // Assert that APIX Link is down
    // Assert that message has been received by uds collector
    auto ret = test_collector_service->getLastDiagnosticTestResult(
            APIX_DTC_ID,
            [](const DiagnosticCheckReport& report) { EXPECT_EQ(report.status, DiagnosticCheckStatus::FAILED); });
    EXPECT_TRUE(ret.isOk());
    netman_interface_test_helper.BringUpNetworkInterfaces();
}

TEST(NetmanUdsCollectorReportingTest, WhenMostIsDownShouldReportUdsmostFailed) {
    // Arrange
    // Get service
    constexpr int MOST_DTC_ID = 0x960A00;
    NetmanInterfaceTestHelper netman_interface_test_helper;
    netman_interface_test_helper.BringUpNetworkInterfaces();
    auto test_collector_service = IDataCollectorTestPoint::getService();
    if (test_collector_service == nullptr) {
        FAIL();
    }

    // Act
    // Take down MOST Link
    netman_interface_test_helper.TakeDownNetworkInterface("meth0");

    // TODO(arjun.snehaj.lal@volvocars.com) At present test is based on a constant delay of 120 msec and is fragile.
    // A cleaner solution is needed. Instead of relying on constant delay where uds is queried after netman has sent
    // a report
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    // Assert
    // Assert that MOST Link is down
    // Assert that message has been received by uds collector

    auto ret = test_collector_service->getLastDiagnosticTestResult(
            MOST_DTC_ID,
            [](const DiagnosticCheckReport& report) { EXPECT_EQ(report.status, DiagnosticCheckStatus::FAILED); });
    EXPECT_TRUE(ret.isOk());
    netman_interface_test_helper.BringUpNetworkInterfaces();
}

TEST(NetmanUdsCollectorReportingTest, WhenTcamIsDownShouldReportUdsApixFailed) {
    // Arrange
    // Get service
    constexpr int TCAM_DTC_ID = 0x960900;
    NetmanInterfaceTestHelper netman_interface_test_helper;
    netman_interface_test_helper.BringUpNetworkInterfaces();
    auto test_collector_service = IDataCollectorTestPoint::getService();
    if (test_collector_service == nullptr) {
        FAIL();
    }

    // Act
    // Take down TCAM Link
    netman_interface_test_helper.TakeDownNetworkInterface("tcam0");

    // TODO(arjun.snehaj.lal@volvocars.com) At present test is based on a constant delay of 120 msec and is fragile.
    // A cleaner solution is needed. Instead of relying on constant delay where uds is queried after netman has sent
    // a report
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    // Assert
    // Assert that TCAM Link is down
    // Assert that message has been received by uds collector
    auto ret = test_collector_service->getLastDiagnosticTestResult(
            TCAM_DTC_ID,
            [](const DiagnosticCheckReport& report) { EXPECT_EQ(report.status, DiagnosticCheckStatus::FAILED); });
    EXPECT_TRUE(ret.isOk());
    netman_interface_test_helper.BringUpNetworkInterfaces();
}