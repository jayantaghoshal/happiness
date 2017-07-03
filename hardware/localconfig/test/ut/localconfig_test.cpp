#include <vcc/localconfig.h>
#include <vcc/localconfig_test.h>

#include <gtest/gtest.h>

namespace {
const char *kGoodTestFilePath = "/data/local/tmp/localconfig_good.json";
const char *kBadTestFilePath = "/data/local/tmp/localconfig_bad.json";
const char *kFileNotFoundTestFilePath = "file/not/found";

const char *kTestIntKey = "foo";
const char *kTestStringKey = "bum";
}  // namespace

TEST(LocalConfigTest, TestInitWithGoodFile) { EXPECT_NO_THROW(vcc::localconfig::TestInit(kGoodTestFilePath)); }

TEST(LocalConfigTest, TestInitFailsWithBadFile) {
  EXPECT_THROW(vcc::localconfig::TestInit(kBadTestFilePath), std::runtime_error);
}

TEST(LocalConfigTest, TestInitFailsWithNonExistingFile) {
  EXPECT_THROW(vcc::localconfig::TestInit(kFileNotFoundTestFilePath), std::runtime_error);
}

TEST(LocalConfigTest, GetInt) {
  vcc::localconfig::TestInit(kGoodTestFilePath);
  EXPECT_EQ(42, vcc::localconfig::GetInt(kTestIntKey));
  EXPECT_THROW(vcc::localconfig::GetInt("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(vcc::localconfig::GetInt(kTestStringKey), std::runtime_error);
}

TEST(LocalConfigTest, GetString) {
  vcc::localconfig::TestInit(kGoodTestFilePath);
  EXPECT_EQ("mystring", vcc::localconfig::GetString(kTestStringKey));
  EXPECT_THROW(vcc::localconfig::GetString("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(vcc::localconfig::GetString(kTestIntKey), std::runtime_error);
}

TEST(LocalConfigTest, GetBoolean) {
  vcc::localconfig::TestInit(kGoodTestFilePath);
  EXPECT_TRUE(vcc::localconfig::GetBool("bool1"));
  EXPECT_FALSE(vcc::localconfig::GetBool("bool2"));
  EXPECT_THROW(vcc::localconfig::GetBool("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(vcc::localconfig::GetBool(kTestIntKey), std::runtime_error);
}

TEST(LocalConfigTest, GetDouble) {
  vcc::localconfig::TestInit(kGoodTestFilePath);
  EXPECT_DOUBLE_EQ(22.333, vcc::localconfig::GetDouble("double1"));
  EXPECT_DOUBLE_EQ(22, vcc::localconfig::GetDouble("double2"));
  EXPECT_THROW(vcc::localconfig::GetDouble("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(vcc::localconfig::GetDouble(kTestStringKey), std::runtime_error);
}
