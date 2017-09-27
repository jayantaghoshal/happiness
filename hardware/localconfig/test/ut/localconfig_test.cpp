#include <vcc/local_config_reader.h>
#include <vcc/localconfig.h>

#include <gtest/gtest.h>

namespace
{
const char *kGoodTestFilePath = "/data/local/tmp/localconfig_good.json";
const char *kBadTestFilePath = "/data/local/tmp/localconfig_bad.json";
const char *kFileNotFoundTestFilePath = "file/not/found";

const char *kTestIntKey = "foo";
const char *kTestStringKey = "bum";
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

class LocalConfigParsingTest : public ::testing::Test
{
 private:
  vcc::LocalConfigFileReader file_reader{kGoodTestFilePath};

 protected:
  // only accessing through interface gives proper overload resolution
  vcc::LocalConfigReaderInterface &reader = file_reader;
};

TEST_F(LocalConfigParsingTest, GetInt)
{
  EXPECT_EQ(42, reader.GetInt(kTestIntKey));
  EXPECT_THROW(reader.GetInt("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(reader.GetInt(kTestStringKey), std::runtime_error);
}

TEST_F(LocalConfigParsingTest, GetString)
{
  EXPECT_EQ("mystring", reader.GetString(kTestStringKey));
  EXPECT_THROW(reader.GetString("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(reader.GetString(kTestIntKey), std::runtime_error);
}

TEST_F(LocalConfigParsingTest, GetBoolean)
{
  EXPECT_TRUE(reader.GetBool("bool1"));
  EXPECT_FALSE(reader.GetBool("bool2"));
  EXPECT_THROW(reader.GetBool("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(reader.GetBool(kTestIntKey), std::runtime_error);
}

TEST_F(LocalConfigParsingTest, GetDouble)
{
  EXPECT_DOUBLE_EQ(22.333, reader.GetDouble("double1"));
  EXPECT_DOUBLE_EQ(22, reader.GetDouble("double2"));
  EXPECT_THROW(reader.GetDouble("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(reader.GetDouble(kTestStringKey), std::runtime_error);
}

TEST_F(LocalConfigParsingTest, GetArray)
{
  EXPECT_EQ(std::vector<std::string>({"test1"}), reader.GetStringArray("strarray1"));
  EXPECT_EQ(std::vector<std::string>({"test1"}), reader.GetStringArray("strarray2", "substrarray"));
  EXPECT_EQ(std::vector<std::string>({"rule1", "rule2"}), reader.GetStringArray("FIREWALL", "NAT_TABLE", "RULES"));
  EXPECT_EQ(std::vector<std::string>(), reader.GetStringArray("FIREWALL", "NAT_TABLE", "CHAINS"));
  EXPECT_THROW(reader.GetStringArray("strarray2", "substrarray2"), std::runtime_error);
  EXPECT_THROW(reader.GetStringArray("strarray3", "substrarray3"), std::runtime_error);
  EXPECT_THROW(reader.GetStringArray("DOES_NOT_EXIST"), std::runtime_error);
}
