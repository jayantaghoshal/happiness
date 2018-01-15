/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "IDispatcher.h"
#include "time_provider.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>

using ::testing::_;
using namespace tarmac::timeprovider;
using namespace tarmac::eventloop;

class SystemTimerTest : public ::testing::Test {
  public:
    std::shared_ptr<IDispatcher> dispatcher = IDispatcher::CreateDispatcher();
    std::shared_ptr<TimeProvider> tp = std::shared_ptr<TimeProvider>(new TimeProvider(dispatcher));
};

TEST_F(SystemTimerTest, test_single_shot) {
    std::atomic<int> callcount{0};
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()> functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp->steady_clock_now();
    };

    std::unique_ptr<TimerSubscriptionHandle> subsHandle =
            tp->add_single_shot_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);

    std::this_thread::sleep_for(std::chrono::milliseconds(900));
    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(1, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(950)), calledAt);
}

TEST_F(SystemTimerTest, test_single_shot_2) {
    std::atomic<int> callcount{0};
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()> functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp->steady_clock_now();
    };

    auto subsHandle = tp->add_single_shot_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    EXPECT_EQ(1, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(950)), calledAt);
}

TEST_F(SystemTimerTest, test_single_shot_3) {
    std::atomic<int> callcount{0};
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()> functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp->steady_clock_now();
    };

    auto subsHandle = tp->add_single_shot_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(15000));
    EXPECT_EQ(1, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(950)), calledAt);
}

TEST_F(SystemTimerTest, test_periodic) {
    std::atomic<int> callcount{0};
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()> functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp->steady_clock_now();
    };

    auto subsHandle = tp->add_periodic_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10 + 500));
    EXPECT_EQ(10, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(1000 * 10 - 100)), calledAt);
}

TEST_F(SystemTimerTest, test_singleshot_unsubscribe) {
    std::atomic<int> callcount{0};
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()> functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp->steady_clock_now();
    };

    auto subsHandle = tp->add_single_shot_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_EQ(0, callcount);
    subsHandle.reset(nullptr);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_EQ(0, callcount);
}

TEST_F(SystemTimerTest, test_periodic_unsubscribe) {
    std::atomic<int> callcount{0};
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()> functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp->steady_clock_now();
    };

    auto subsHandle = tp->add_periodic_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 5 + 500));
    EXPECT_EQ(5, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(1000 * 5 - 100)), calledAt);

    subsHandle.reset(nullptr);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 5 + 500));
    EXPECT_EQ(5, callcount);
}
