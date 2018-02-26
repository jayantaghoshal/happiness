/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include <gtest/gtest.h>

#include "dispatcher.h"
#include <glib.h>
#include <thread>

using namespace ::testing;

namespace common
{

class DispatcherTest : public Test
{
public:
    DispatcherTest()
        : Test()
        , _sut()
    {
    }

    static void SetUpTestCase()
    {
        mainLoop    = g_main_loop_new(NULL, FALSE);
        mainContext = g_main_loop_get_context(mainLoop);
    }

    static void TearDownTestCase()
    {
        g_main_loop_unref(mainLoop);
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

protected:
    static GMainLoop*    mainLoop;
    static GMainContext* mainContext;
    Dispatcher           _sut;
};

GMainLoop*    DispatcherTest::mainLoop;
GMainContext* DispatcherTest::mainContext;

TEST_F(DispatcherTest, Start_WillCallbackAfterRequestedTime_WhenTimeoutNonZero)
{

    Dispatcher Dispatcher;
    bool       flag = false;

    auto dur       = std::chrono::milliseconds(500);
    auto tolerance = std::chrono::milliseconds(10);

    Dispatcher.Start(dur, [&flag]() { flag = true; });
    EXPECT_FALSE(flag);

    std::this_thread::sleep_for(dur - tolerance);
    g_main_context_iteration(mainContext, FALSE);

    EXPECT_FALSE(flag);

    std::this_thread::sleep_for(2 * tolerance);
    g_main_context_iteration(mainContext, FALSE);

    EXPECT_TRUE(flag);
}

TEST_F(DispatcherTest, Start_WillCallbackImmediately_WhenTimeoutZero)
{

    Dispatcher Dispatcher;
    bool       flag = false;

    auto dur = std::chrono::milliseconds(0);

    Dispatcher.Start(dur, [&flag]() { flag = true; });
    EXPECT_FALSE(flag);

    g_main_context_iteration(mainContext, FALSE);

    EXPECT_TRUE(flag);
}

TEST_F(DispatcherTest, Restart_WillCallback_WhenRequested)
{

    Dispatcher Dispatcher;
    int        count = 0;

    auto dur = std::chrono::milliseconds(0);

    Dispatcher.Start(dur, [&count]() { count++; });
    EXPECT_EQ(0, count);

    g_main_context_iteration(mainContext, FALSE);

    EXPECT_EQ(1, count);

    Dispatcher.Restart(dur);

    g_main_context_iteration(mainContext, FALSE);

    EXPECT_EQ(2, count);
}

TEST_F(DispatcherTest, Cancel_WillCancel)
{

    Dispatcher Dispatcher;
    bool       flag = false;

    auto dur       = std::chrono::milliseconds(500);
    auto tolerance = std::chrono::milliseconds(10);

    Dispatcher.Start(dur, [&flag]() { flag = true; });
    EXPECT_FALSE(flag);

    std::this_thread::sleep_for(dur - tolerance);
    g_main_context_iteration(mainContext, FALSE);

    EXPECT_FALSE(flag);

    Dispatcher.Cancel();

    std::this_thread::sleep_for(2 * tolerance);
    g_main_context_iteration(mainContext, FALSE);

    EXPECT_FALSE(flag);
}
}
