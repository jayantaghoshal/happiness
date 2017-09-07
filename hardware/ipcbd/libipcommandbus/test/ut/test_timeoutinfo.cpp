/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "ipcommandbus/TimeoutInfo.h"
#include "stubs/cedric/cedric_localconfig.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgPointee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::StrEq;

using namespace Connectivity;

LocalconfigStub local_config_stub;

class TimeoutInfoFixture : public ::testing::Test
{
};

TEST_F(TimeoutInfoFixture, construction)
{
    TimeoutInfo ti_constr_default;
    EXPECT_EQ(ti_constr_default.getTimeoutValue(), std::chrono::milliseconds(500U));

    TimeoutInfo ti_constr_full(std::chrono::milliseconds(800), 8, 8.0);
    EXPECT_EQ(ti_constr_full.getTimeoutValue(), std::chrono::milliseconds(800U));
}

TEST_F(TimeoutInfoFixture, increase_timeout)
{
    TimeoutInfo ti(std::chrono::milliseconds(800), 2, 2.0);
    EXPECT_EQ(ti.getTimeoutValue(), std::chrono::milliseconds(800U));

    EXPECT_TRUE(ti.increaseTimeout());
    EXPECT_EQ(ti.getTimeoutValue(), std::chrono::milliseconds(1600U));

    EXPECT_TRUE(ti.increaseTimeout());
    EXPECT_EQ(ti.getTimeoutValue(), std::chrono::milliseconds(3200U));

    EXPECT_FALSE(ti.increaseTimeout());
}

TEST_F(TimeoutInfoFixture, reset)
{
    TimeoutInfo ti(std::chrono::milliseconds(800), 2, 2.0);
    EXPECT_EQ(ti.getTimeoutValue(), std::chrono::milliseconds(800U));

    EXPECT_TRUE(ti.increaseTimeout());
    EXPECT_EQ(ti.getTimeoutValue(), std::chrono::milliseconds(1600U));

    ti.reset();

    EXPECT_EQ(ti.getTimeoutValue(), std::chrono::milliseconds(800U));
}
