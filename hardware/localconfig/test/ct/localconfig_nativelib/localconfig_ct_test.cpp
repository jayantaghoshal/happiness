/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <vcc/localconfig.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

#include <iostream>

using namespace ::testing;

struct LocalConfigOnTargetTest : public ::Test {
    const vcc::LocalConfigReaderInterface* const lcfg_ = vcc::LocalConfigDefault();
};

TEST_F(LocalConfigOnTargetTest, LocalConfigTestGetInt) { EXPECT_NO_THROW(lcfg_->GetInt("IIPS_LM_NofNodesRG1")); }

TEST_F(LocalConfigOnTargetTest, LocalConfigTestGetDouble) {
    EXPECT_THAT(lcfg_->GetDouble("TEST_KEY_DOUBLE"), DoubleEq(8.5));
}

TEST_F(LocalConfigOnTargetTest, LocalConfigTestGetString) {
    EXPECT_THAT(lcfg_->GetString("TEST_KEY_STRING"), StrEq("KEY_TYPE_STRING"));
}

TEST_F(LocalConfigOnTargetTest, LocalConfigTestGetBoolean) { EXPECT_TRUE(lcfg_->GetBool("TEST_KEY_BOOLEAN")); }
