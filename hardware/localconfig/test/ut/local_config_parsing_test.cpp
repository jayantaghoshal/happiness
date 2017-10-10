#include <vcc/local_config_reader.h>
#include <vcc/localconfig.h>

#include <gmock/gmock.h>

namespace {
std::string good_json_input = R"(
{
    "foo" : 42,
    "bum" : "mystring",
    "bool1": true,
    "bool2": false,
    "double1": 22.333,
    "double2": 22,
    "strarray1": ["test1"],
    "strarray2": { "substrarray": ["test1"], "substrarray2": [1], "substrarray3": [1, 2.3, "hello"] },
    "FIREWALL":
    {
        "NAT_TABLE":
        {
            "CHAINS": [
            ],
            "RULES": [
                "rule1",
                "rule2"
            ]
        },
        "SECURITY_TABLE":
        {
            "CHAINS":[
            ],
            "RULES":[
            ]
        },
        "FILTER_TABLE":
        {
            "CHAINS":[
                "SPOOF_OUT",
                "BROADCAST_STORMS",
                "SYN_FLOOD",
                "SYN_FIN_PACKETS"
            ]
        }
    }
}
)";

std::string malformed_json_input = R"(
{ foo
)";

const char *kTestIntKey = "foo";
const char *kTestStringKey = "bum";
}

TEST(LocalConfigParsingTestMalformed, MalformedInput) {
  vcc::LocalConfigStaticContentReader underlying_reader{malformed_json_input};
  EXPECT_THROW(underlying_reader.Preload(), std::runtime_error);
}

class LocalConfigParsingTest : public ::testing::Test {
 private:
  vcc::LocalConfigStaticContentReader underlying_reader{good_json_input};

 protected:
  // only accessing through interface gives proper overload resolution
  vcc::LocalConfigReaderInterface &reader = underlying_reader;
};

TEST_F(LocalConfigParsingTest, GetInt) {
  EXPECT_THAT(reader.GetInt(kTestIntKey), 42);
  EXPECT_THROW(reader.GetInt("NOT_EXISTING"), std::runtime_error);

  int value;
  EXPECT_THAT(reader.TryGetValue(&value, kTestIntKey), true);
  EXPECT_THAT(value, 42);
  EXPECT_THAT(reader.TryGetValue(&value, "NOT_EXISTING"), false);
  EXPECT_THROW(reader.GetInt(kTestStringKey), std::runtime_error);
}

TEST_F(LocalConfigParsingTest, GetString) {
  EXPECT_EQ("mystring", reader.GetString(kTestStringKey));
  EXPECT_THROW(reader.GetString("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(reader.GetString(kTestIntKey), std::runtime_error);
}

TEST_F(LocalConfigParsingTest, GetBoolean) {
  EXPECT_TRUE(reader.GetBool("bool1"));
  EXPECT_FALSE(reader.GetBool("bool2"));
  EXPECT_THROW(reader.GetBool("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(reader.GetBool(kTestIntKey), std::runtime_error);
}

TEST_F(LocalConfigParsingTest, GetDouble) {
  EXPECT_DOUBLE_EQ(22.333, reader.GetDouble("double1"));
  EXPECT_DOUBLE_EQ(22, reader.GetDouble("double2"));
  EXPECT_THROW(reader.GetDouble("NOT_EXISTING"), std::runtime_error);
  EXPECT_THROW(reader.GetDouble(kTestStringKey), std::runtime_error);
}

TEST_F(LocalConfigParsingTest, GetArray) {
  EXPECT_EQ(std::vector<std::string>({"test1"}), reader.GetStringArray("strarray1"));
  EXPECT_EQ(std::vector<std::string>({"test1"}), reader.GetStringArray("strarray2", "substrarray"));
  EXPECT_EQ(std::vector<std::string>({"rule1", "rule2"}), reader.GetStringArray("FIREWALL", "NAT_TABLE", "RULES"));
  EXPECT_EQ(std::vector<std::string>(), reader.GetStringArray("FIREWALL", "NAT_TABLE", "CHAINS"));
  EXPECT_THROW(reader.GetStringArray("strarray2", "substrarray2"), std::runtime_error);
  EXPECT_THROW(reader.GetStringArray("strarray3", "substrarray3"), std::runtime_error);
  EXPECT_THROW(reader.GetStringArray("DOES_NOT_EXIST"), std::runtime_error);
}
