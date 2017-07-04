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

TEST(LocalConfigTest, LocalConfigFileIsValid) { EXPECT_EQ(42, vcc::localconfig::GetInt("Param1")); }
