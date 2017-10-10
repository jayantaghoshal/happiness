#include <vcc/localconfig.h>

#include <gtest/gtest.h>
#include <stdio.h>
#include <sys/stat.h>

#include <iostream>

struct LocalConfigOnTargetTest : public ::testing::Test {
  const vcc::LocalConfigReaderInterface* lcfg_ = vcc::LocalConfigDefault();
};

TEST(LocalConfigOnTargetTest, DefaultLocalConfigFileCopiedToRWPartitionOnBoot) {
  struct stat sb;
  EXPECT_TRUE(stat("/oem_config/localconfig/localconfig.json", &sb) == 0);
}

TEST_F(LocalConfigOnTargetTest, LocalConfigTestGetInt) { lcfg_->GetInt("IIPS_LM_NofNodesRG1"); }

TEST_F(LocalConfigOnTargetTest, LocalConfigTestGetDouble) {
  EXPECT_EQ(1.5, lcfg_->GetDouble("CONN_increaseTimerValueWFA"));
}

TEST_F(LocalConfigOnTargetTest, LocalConfigTestGetString) {
  EXPECT_EQ("198.18.34.1", lcfg_->GetString("IIPS_IpAddress_LOCAL"));
}
