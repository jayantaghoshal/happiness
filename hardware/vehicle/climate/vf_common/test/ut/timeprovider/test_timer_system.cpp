/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dispatcher.h"
#include "time_provider.h"

#include <atomic>
#include <chrono>
#include <functional>
#include <glib.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

using ::testing::_;

class SystemTimerTest : public ::testing::Test
{
public:
    SystemTimerTest()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // To initialize glib mainloop
    }

    TimeProvider tp;
};

TEST_F(SystemTimerTest, testDispatcher)
{
    // Note dispatcher mainloop runs inside TimeProviderSystem

    std::atomic<int>                      callcount{ 0 };
    std::chrono::steady_clock::time_point calledAt;
    std::function<void(void)>             functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp.steady_clock_now();
    };

    Dispatcher dispatcher;
    dispatcher.Start(std::chrono::milliseconds(1000), functToCall);
    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(900));
    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    EXPECT_EQ(1, callcount);
}

TEST_F(SystemTimerTest, test_single_shot)
{
    // 2.3.2.6. Function - activationCheck

    std::atomic<int>                      callcount{ 0 };
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()>                 functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp.steady_clock_now();
    };

    std::unique_ptr<TimerSubscriptionHandle> subsHandle
        = std::move(tp.add_single_shot_timer(std::chrono::milliseconds(1000), functToCall));

    EXPECT_EQ(0, callcount);

    std::this_thread::sleep_for(std::chrono::milliseconds(900));
    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(1, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(950)), calledAt);
}

TEST_F(SystemTimerTest, test_single_shot_2)
{
    // 2.3.2.6. Function - activationCheck

    std::atomic<int>                      callcount{ 0 };
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()>                 functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp.steady_clock_now();
    };

    auto subsHandle = tp.add_single_shot_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    EXPECT_EQ(1, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(950)), calledAt);
}

TEST_F(SystemTimerTest, test_single_shot_3)
{
    // 2.3.2.6. Function - activationCheck

    std::atomic<int>                      callcount{ 0 };
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()>                 functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp.steady_clock_now();
    };

    auto subsHandle = tp.add_single_shot_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(15000));
    EXPECT_EQ(1, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(950)), calledAt);
}

TEST_F(SystemTimerTest, test_periodic)
{
    // 2.3.2.6. Function - activationCheck

    std::atomic<int>                      callcount{ 0 };
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()>                 functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp.steady_clock_now();
    };

    auto subsHandle = tp.add_periodic_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10 + 500));
    EXPECT_EQ(10, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(1000 * 10 - 100)), calledAt);
}

TEST_F(SystemTimerTest, test_singleshot_unsubscribe)
{
    // 2.3.2.6. Function - activationCheck

    std::atomic<int>                      callcount{ 0 };
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()>                 functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp.steady_clock_now();
    };

    auto subsHandle = tp.add_single_shot_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_EQ(0, callcount);
    subsHandle.reset(nullptr);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_EQ(0, callcount);
}

TEST_F(SystemTimerTest, test_periodic_unsubscribe)
{
    // 2.3.2.6. Function - activationCheck

    std::atomic<int>                      callcount{ 0 };
    std::chrono::steady_clock::time_point calledAt;
    std::function<void()>                 functToCall = [&callcount, &calledAt, this]() {
        callcount++;
        calledAt = tp.steady_clock_now();
    };

    auto subsHandle = tp.add_periodic_timer(std::chrono::milliseconds(1000), functToCall);

    EXPECT_EQ(0, callcount);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 5 + 500));
    EXPECT_EQ(5, callcount);
    EXPECT_LT(std::chrono::steady_clock::time_point(std::chrono::milliseconds(1000 * 5 - 100)), calledAt);

    subsHandle.reset(nullptr);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 5 + 500));
    EXPECT_EQ(5, callcount);
}
