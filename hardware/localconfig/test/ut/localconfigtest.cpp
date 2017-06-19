#include <gtest/gtest.h>
#include <localconfig.h>
#include <cstdlib>
#include <iostream>

namespace {
const char *good_test_filepath = "/data/local/tmp/localconfig_good.json";
const char *bad_test_filepath = "/data/local/tmp/localconfig_bad.json";
const char *file_not_found_test_filepath = "file/not/found";
}

TEST(LocalConfigTest, localconfig_init) {
  EXPECT_TRUE(vcc::localconfig::initWithFilepath(good_test_filepath));
  EXPECT_FALSE(vcc::localconfig::initWithFilepath(bad_test_filepath));
  EXPECT_FALSE(vcc::localconfig::initWithFilepath(file_not_found_test_filepath));
}

TEST(LocalConfigTest, localconfig_get_value_int) {
  ASSERT_TRUE(vcc::localconfig::initWithFilepath(good_test_filepath));
  EXPECT_EQ(42, *vcc::localconfig::getValue<int>("foo"));
  EXPECT_EQ(nullptr, vcc::localconfig::getValue<int>("bar"));
}

TEST(LocalConfigTest, localconfig_get_value_string) {
  ASSERT_TRUE(vcc::localconfig::initWithFilepath(good_test_filepath));
  EXPECT_STREQ("mystring", vcc::localconfig::getValue<std::string>("bum")->c_str());
}

TEST(LocalConfigTest, localconfig_get_value_boolean) {
  ASSERT_TRUE(vcc::localconfig::initWithFilepath(good_test_filepath));
  EXPECT_TRUE(*vcc::localconfig::getValue<bool>("bool1"));
  EXPECT_FALSE(*vcc::localconfig::getValue<bool>("bool2"));
  EXPECT_EQ(nullptr, vcc::localconfig::getValue<bool>("bar"));
}

TEST(LocalConfigTest, localconfig_get_value_double) {
  ASSERT_TRUE(vcc::localconfig::initWithFilepath(good_test_filepath));
  ASSERT_DOUBLE_EQ(22.333, *vcc::localconfig::getValue<double>("double1"));
  ASSERT_DOUBLE_EQ(22, *vcc::localconfig::getValue<double>("double2"));
  ASSERT_EQ(nullptr, vcc::localconfig::getValue<double>("bar"));
}

TEST(LocalConfigTest, localconfig_get_vector) {
  ASSERT_TRUE(vcc::localconfig::initWithFilepath(good_test_filepath));
  const std::vector<int> v = vcc::localconfig::getVector<int>("baz");
  ASSERT_EQ(3UL, v.size());
  ASSERT_EQ(42, v[1]);
}

TEST(LocalConfigTest, localconfig_get_nested_vector) {
  ASSERT_TRUE(vcc::localconfig::initWithFilepath(good_test_filepath));
  const std::vector<std::string> v = vcc::localconfig::getVector<std::string>("a", "b", "c");
  ASSERT_EQ(2UL, v.size());
  ASSERT_STREQ("d", v[0].c_str());

  const std::vector<std::string> v2 = vcc::localconfig::getVector<std::string>("aa", "bb", "cc");
  ASSERT_EQ(0UL, v2.size());
}

TEST(LocalConfigTest, localconfig_get_array) {
  ASSERT_TRUE(vcc::localconfig::initWithFilepath(good_test_filepath));
  auto array = vcc::localconfig::getArray<std::tuple<std::string, int>>("bar", "Name", "Age");

  std::string name;
  int age;

  std::tie(name, age) = array[0];

  EXPECT_EQ("John Doe", name);
  EXPECT_EQ(30, age);

  std::tie(name, age) = array[1];

  EXPECT_EQ("Jane Doe", name);
  EXPECT_EQ(55, age);
}
