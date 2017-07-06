#include <vcc/localconfig.h>

#include <gtest/gtest.h>
#include <stdio.h>
#include <sys/stat.h>

#include <iostream>

TEST(LocalConfigTest, DefaultLocalConfigFileCopiedToRWPartitionOnBoot)
{
  struct stat sb;
  EXPECT_TRUE(stat("/oem_config/localconfig/localconfig.json", &sb) == 0);
}

TEST(LocalConfigTest, LocalConfigTestGetInt) { EXPECT_EQ(3, vcc::localconfig::GetInt("IIPS_LM_NofNodesRG1")); }

TEST(LocalConfigTest, LocalConfigTestGetDouble) { EXPECT_EQ(1.5, vcc::localconfig::GetDouble("CONN_increaseTimerValueWFA")); }

TEST(LocalConfigTest, LocalConfigTestGetString) { EXPECT_EQ("198.18.34.1", vcc::localconfig::GetString("IIPS_IpAddress_LOCAL")); }
