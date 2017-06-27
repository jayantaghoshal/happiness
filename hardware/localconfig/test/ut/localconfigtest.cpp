#include <localconfig.h>
#include <localconfig_test.h>

#include <gtest/gtest.h>

namespace {
const char *kGoodTestFilePath = "/data/local/tmp/localconfig_good.json";
const char *kBadTestFilePath = "/data/local/tmp/localconfig_bad.json";
const char *kFileNotFoundTestFilePath = "file/not/found";

const char *kTestIntKey = "foo";
const char *kTestStringKey = "bum";
}  // namespace

TEST(LocalConfigTest, DefaultConfigFileInitializedAutomatically) {
  EXPECT_EQ(42, vcc::localconfig::GetValueInt("Param1"));
}

TEST(LocalConfigTest, TestInitWithGoodFile) { EXPECT_NO_THROW(vcc::localconfig::TestInit(kGoodTestFilePath)); }

TEST(LocalConfigTest, TestInitFailsWithBadFile) {
  EXPECT_THROW(vcc::localconfig::TestInit(kBadTestFilePath), std::runtime_error);
}

TEST(LocalConfigTest, TestInitFailsWithNonExistingFile) {
  EXPECT_THROW(vcc::localconfig::TestInit(kFileNotFoundTestFilePath), std::runtime_error);
}

TEST(LocalConfigTest, GetValueInt) {
  vcc::localconfig::TestInit(kGoodTestFilePath);
  EXPECT_EQ(42, vcc::localconfig::GetValueInt(kTestIntKey));
  EXPECT_THROW(vcc::localconfig::GetValueInt("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(vcc::localconfig::GetValueInt(kTestStringKey), std::runtime_error);
}

TEST(LocalConfigTest, GetValueString) {
  vcc::localconfig::TestInit(kGoodTestFilePath);
  EXPECT_EQ("mystring", vcc::localconfig::GetValueString(kTestStringKey));
  EXPECT_THROW(vcc::localconfig::GetValueString("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(vcc::localconfig::GetValueString(kTestIntKey), std::runtime_error);
}

TEST(LocalConfigTest, GetValueBoolean) {
  vcc::localconfig::TestInit(kGoodTestFilePath);
  EXPECT_TRUE(vcc::localconfig::GetValueBool("bool1"));
  EXPECT_FALSE(vcc::localconfig::GetValueBool("bool2"));
  EXPECT_THROW(vcc::localconfig::GetValueBool("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(vcc::localconfig::GetValueBool(kTestIntKey), std::runtime_error);
}

TEST(LocalConfigTest, GetValueDouble) {
  vcc::localconfig::TestInit(kGoodTestFilePath);
  EXPECT_DOUBLE_EQ(22.333, vcc::localconfig::GetValueDouble("double1"));
  EXPECT_DOUBLE_EQ(22, vcc::localconfig::GetValueDouble("double2"));
  EXPECT_THROW(vcc::localconfig::GetValueDouble("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(vcc::localconfig::GetValueDouble(kTestStringKey), std::runtime_error);
}
