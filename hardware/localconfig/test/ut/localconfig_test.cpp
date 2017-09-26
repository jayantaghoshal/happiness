#include <vcc/local_config_reader.h>
#include <vcc/localconfig.h>

#include <gtest/gtest.h>

namespace
{
const char *kGoodTestFilePath = "/data/local/tmp/localconfig_good.json";
const char *kBadTestFilePath = "/data/local/tmp/localconfig_bad.json";
const char *kFileNotFoundTestFilePath = "file/not/found";

}  // namespace

TEST(LocalConfigTest, TestInitWithGoodFile)
{
  vcc::LocalConfigFileReader reader(kGoodTestFilePath);
  EXPECT_NO_THROW(reader.Preload());
}

TEST(LocalConfigTest, TestInitFailsWithBadFile)
{
  vcc::LocalConfigFileReader reader(kBadTestFilePath);
  EXPECT_THROW(reader.Preload(), std::runtime_error);
}

TEST(LocalConfigTest, TestInitFailsWithNonExistingFile)
{
  vcc::LocalConfigFileReader reader(kFileNotFoundTestFilePath);
  EXPECT_THROW(reader.Preload(), std::runtime_error);
}
