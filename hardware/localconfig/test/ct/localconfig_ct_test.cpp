#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <localconfig.h>

TEST(LocalConfigTest, DefaultLocalconfigFileCopiedToRWPartitionOnBoot) {
  struct stat sb;
  EXPECT_TRUE( stat("/oem_config/localconfig/localconfig.json", &sb) == 0);
}

TEST(LocalConfigTest, LocalConfigFileIsValid) { EXPECT_EQ(42, vcc::localconfig::GetValueInt("Param1")); }
  