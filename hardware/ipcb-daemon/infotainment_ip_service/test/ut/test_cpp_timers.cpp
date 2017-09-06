/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "util/timer.h"

using namespace InfotainmentIpService::Utils;

class TimerFixture : public ::testing::Test
{
public:
    /**
     * Fixture Constructor
     */
    TimerFixture()
    {
    }

    /**
     * Set up fixture
     */
    virtual void SetUp()
    {
    }

    /**
     * Tear down fixture
     */
    virtual void TearDown()
    {
    }

    SingleShotTimer sst;
    RecurrentTimer rt;
};

TEST_F(TimerFixture, SingleShotTimer)
{
    volatile int testUserData = 0;
    volatile BaseTimer::ReasonCode testResult = BaseTimer::ReasonCode::TIMER_CANCELLED;

    EXPECT_TRUE(sst.setTimeoutHandler(
        [&](BaseTimer::ReasonCode result, int userData) {
            testUserData = userData;
            testResult = result;
        },
        555));
    EXPECT_TRUE(sst.setTimeout(std::chrono::milliseconds(100)));
    EXPECT_TRUE(sst.start());

    // Test error conditions on already started timers
    EXPECT_FALSE(sst.start());
    EXPECT_FALSE(sst.setTimeoutHandler(
        [&](BaseTimer::ReasonCode result, int userData) {
            testUserData = userData;
            testResult = result;
        },
        666));

    // Test expiry
    usleep(200000);
    EXPECT_EQ(testUserData, 555);
    EXPECT_EQ(testResult, BaseTimer::ReasonCode::TIMER_EXPIRED);

    // Test error condition on already exipired timer
    EXPECT_EQ(sst.stop(), false);

    // Reset
    testUserData = 0;
    testResult = BaseTimer::ReasonCode::TIMER_EXPIRED;
    EXPECT_TRUE(sst.setTimeout(std::chrono::milliseconds(100)));
    EXPECT_TRUE(sst.start());

    // Test cancel
    usleep(10);
    EXPECT_TRUE(sst.stop());
    // Error condition on already cancelled
    EXPECT_FALSE(sst.stop());

    // Wait until full expiry
    usleep(200000);
    EXPECT_FALSE(sst.stop());
    EXPECT_EQ(testUserData, 555);
    EXPECT_EQ(testResult, BaseTimer::ReasonCode::TIMER_CANCELLED);
}

TEST_F(TimerFixture, RecurrentTimer)
{
    volatile int testUserData = 0;
    volatile BaseTimer::ReasonCode testResult = BaseTimer::ReasonCode::TIMER_CANCELLED;
    volatile bool timerFired = false;

    EXPECT_TRUE(rt.setTimeoutHandler(
        [&](BaseTimer::ReasonCode result, int userData) {
            testUserData = userData;
            testResult = result;
            timerFired = true;
        },
        555));
    EXPECT_TRUE(rt.setTimeout(std::chrono::milliseconds(100)));
    EXPECT_TRUE(rt.start());

    // Test error conditions on already started timers
    EXPECT_FALSE(rt.start());
    EXPECT_FALSE(rt.setTimeout(std::chrono::milliseconds(100)));

    // Test fire once
    timerFired = false;
    testUserData = 0;
    testResult = BaseTimer::ReasonCode::TIMER_CANCELLED;
    while (!timerFired)
    {
        usleep(1000);
    }
    EXPECT_EQ(testUserData, 555);
    EXPECT_EQ(testResult, BaseTimer::ReasonCode::TIMER_EXPIRED);

    // Test fire twice
    timerFired = false;
    testUserData = 0;
    testResult = BaseTimer::ReasonCode::TIMER_CANCELLED;
    while (!timerFired)
    {
        usleep(1000);
    }
    EXPECT_EQ(testUserData, 555);
    EXPECT_EQ(testResult, BaseTimer::ReasonCode::TIMER_EXPIRED);

    // Test cancel
    timerFired = false;
    testUserData = 0;
    testResult = BaseTimer::ReasonCode::TIMER_EXPIRED;
    usleep(100);
    EXPECT_TRUE(rt.stop());
    EXPECT_FALSE(rt.stop());

    // Final fire after cancel.
    while (!timerFired)
    {
        usleep(100000);
    }
    EXPECT_EQ(testUserData, 555);
    EXPECT_EQ(testResult, BaseTimer::ReasonCode::TIMER_CANCELLED);
}
