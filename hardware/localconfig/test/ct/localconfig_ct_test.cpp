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
    EXPECT_THAT(lcfg_->GetDouble("CONN_increaseTimerValueWFA"), DoubleEq(1.5));
}

TEST_F(LocalConfigOnTargetTest, LocalConfigTestGetString) {
    EXPECT_THAT(lcfg_->GetString("IIPS_IpAddress_LOCAL"), StrEq("198.18.34.1"));
}
