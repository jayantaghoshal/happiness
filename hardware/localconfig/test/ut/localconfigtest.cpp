#include <gtest/gtest.h>
#include <localconfig.h>
#include <cstdlib>
#include <iostream>

namespace {
const char *good_test_filepath = "/data/local/tmp/localconfig_good.json";
const char *bad_test_filepath = "/data/local/tmp/localconfig_bad.json";
const char *file_not_found_test_filepath = "file/not/found";
}

TEST(LocalConfigTest, localconfig_init_good_file) {
  vcc::localconfig::initWithFilepath(good_test_filepath);
}

TEST(LocalConfigTest, localconfig_init_bad_file) {
  ASSERT_THROW(vcc::localconfig::initWithFilepath(bad_test_filepath), std::runtime_error);
}

TEST(LocalConfigTest, localconfig_init_non_existing_file) {
  ASSERT_THROW(vcc::localconfig::initWithFilepath(file_not_found_test_filepath), std::runtime_error);
}

TEST(LocalConfigTest, localconfig_get_value_int) {
  vcc::localconfig::initWithFilepath(good_test_filepath);
  EXPECT_EQ(42, vcc::localconfig::getValueInt("foo"));
  EXPECT_THROW(vcc::localconfig::getValueInt("NOT_EXISTING"), std::runtime_error);
}

TEST(LocalConfigTest, localconfig_get_value_string) {
  vcc::localconfig::initWithFilepath(good_test_filepath);
  EXPECT_STREQ("mystring", vcc::localconfig::getValueString("bum").c_str());
  EXPECT_THROW(vcc::localconfig::getValueString("NOT_EXISTING"), std::runtime_error);
}

TEST(LocalConfigTest, localconfig_get_value_boolean) {
  vcc::localconfig::initWithFilepath(good_test_filepath);
  EXPECT_TRUE(vcc::localconfig::getValueBool("bool1"));
  EXPECT_FALSE(vcc::localconfig::getValueBool("bool2"));
  EXPECT_THROW(vcc::localconfig::getValueBool("NOT_EXISTING"), std::runtime_error);
}

TEST(LocalConfigTest, localconfig_get_value_double) {
  vcc::localconfig::initWithFilepath(good_test_filepath);
  ASSERT_DOUBLE_EQ(22.333, vcc::localconfig::getValueDouble("double1"));
  ASSERT_DOUBLE_EQ(22, vcc::localconfig::getValueDouble("double2"));
  EXPECT_THROW(vcc::localconfig::getValueDouble("NOT_EXISTING"), std::runtime_error);
}
