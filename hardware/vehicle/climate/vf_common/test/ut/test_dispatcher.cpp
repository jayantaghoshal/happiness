/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>

#include <glib.h>
#include <thread>
#include "legacydispatcher.h"

using namespace ::testing;

namespace common {

class LegacyDispatcherTest : public Test {
  public:
    LegacyDispatcherTest() : Test(), _sut() {}

    static void SetUpTestCase() {
        mainLoop = g_main_loop_new(NULL, FALSE);
        mainContext = g_main_loop_get_context(mainLoop);
    }

    static void TearDownTestCase() { g_main_loop_unref(mainLoop); }

    virtual void SetUp() {}

    virtual void TearDown() {}

  protected:
    static GMainLoop* mainLoop;
    static GMainContext* mainContext;
    LegacyDispatcher _sut;
};

GMainLoop* LegacyDispatcherTest::mainLoop;
GMainContext* LegacyDispatcherTest::mainContext;

TEST_F(LegacyDispatcherTest, Start_WillCallbackAfterRequestedTime_WhenTimeoutNonZero) {
    LegacyDispatcherLegacyDispatcher;
    bool flag = false;

    auto dur = std::chrono::milliseconds(500);
    auto tolerance = std::chrono::milliseconds(10);

    LegacyDispatcher.Start(dur, [&flag]() { flag = true; });
    EXPECT_FALSE(flag);

    std::this_thread::sleep_for(dur - tolerance);
    g_main_context_iteration(mainContext, FALSE);

    EXPECT_FALSE(flag);

    std::this_thread::sleep_for(2 * tolerance);
    g_main_context_iteration(mainContext, FALSE);

    EXPECT_TRUE(flag);
}

TEST_F(LegacyDispatcherTest, Start_WillCallbackImmediately_WhenTimeoutZero) {
    LegacyDispatcherLegacyDispatcher;
    bool flag = false;

    auto dur = std::chrono::milliseconds(0);

    LegacyDispatcher.Start(dur, [&flag]() { flag = true; });
    EXPECT_FALSE(flag);

    g_main_context_iteration(mainContext, FALSE);

    EXPECT_TRUE(flag);
}

TEST_F(LegacyDispatcherTest, Restart_WillCallback_WhenRequested) {
    LegacyDispatcherLegacyDispatcher;
    int count = 0;

    auto dur = std::chrono::milliseconds(0);

    LegacyDispatcher.Start(dur, [&count]() { count++; });
    EXPECT_EQ(0, count);

    g_main_context_iteration(mainContext, FALSE);

    EXPECT_EQ(1, count);

    LegacyDispatcher.Restart(dur);

    g_main_context_iteration(mainContext, FALSE);

    EXPECT_EQ(2, count);
}

TEST_F(LegacyDispatcherTest, Cancel_WillCancel) {
    LegacyDispatcherLegacyDispatcher;
    bool flag = false;

    auto dur = std::chrono::milliseconds(500);
    auto tolerance = std::chrono::milliseconds(10);

    LegacyDispatcher.Start(dur, [&flag]() { flag = true; });
    EXPECT_FALSE(flag);

    std::this_thread::sleep_for(dur - tolerance);
    g_main_context_iteration(mainContext, FALSE);

    EXPECT_FALSE(flag);

    LegacyDispatcher.Cancel();

    std::this_thread::sleep_for(2 * tolerance);
    g_main_context_iteration(mainContext, FALSE);

    EXPECT_FALSE(flag);
}
}
