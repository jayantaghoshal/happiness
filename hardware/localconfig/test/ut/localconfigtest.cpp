#include <gtest/gtest.h>
#include <localconfig.h>
#include <localconfig_test.h>
#include <cstdlib>
#include <iostream>

namespace
{
const char *good_test_filepath = "/data/local/tmp/localconfig_good.json";
const char *bad_test_filepath = "/data/local/tmp/localconfig_bad.json";
const char *file_not_found_test_filepath = "file/not/found";
}

// Verify that localconfig is intitialized automatically.
TEST(LocalConfigTest, localconfig_default_file) { EXPECT_EQ(42, vcc::localconfig::getValueInt("Param1")); }

// Verify that localconfig testInit is working with a "good" file.
TEST(LocalConfigTest, localconfig_init_good_file) { vcc::localconfig::testInit(good_test_filepath); }

// Verify that localconfig testInit handling a "bad" file.
TEST(LocalConfigTest, localconfig_init_bad_file)
{
  ASSERT_THROW(vcc::localconfig::testInit(bad_test_filepath), std::runtime_error);
}

// Verify that localconfig testInit handling a non existing file.
TEST(LocalConfigTest, localconfig_init_non_existing_file)
{
  ASSERT_THROW(vcc::localconfig::testInit(file_not_found_test_filepath), std::runtime_error);
}

// Verify getValueInt().
TEST(LocalConfigTest, localconfig_get_value_int)
{
  vcc::localconfig::testInit(good_test_filepath);
  EXPECT_EQ(42, vcc::localconfig::getValueInt("foo"));
  EXPECT_THROW(vcc::localconfig::getValueInt("NOT_EXISTING"), std::runtime_error);
}

// Verify getValueString().
TEST(LocalConfigTest, localconfig_get_value_string)
{
  vcc::localconfig::testInit(good_test_filepath);
  EXPECT_STREQ("mystring", vcc::localconfig::getValueString("bum").c_str());
  EXPECT_THROW(vcc::localconfig::getValueString("NOT_EXISTING"), std::runtime_error);
}

// Verify getValueBoolean().
TEST(LocalConfigTest, localconfig_get_value_boolean)
{
  vcc::localconfig::testInit(good_test_filepath);
  EXPECT_TRUE(vcc::localconfig::getValueBool("bool1"));
  EXPECT_FALSE(vcc::localconfig::getValueBool("bool2"));
  EXPECT_THROW(vcc::localconfig::getValueBool("NOT_EXISTING"), std::runtime_error);
}

// Verify getValueDouble().
TEST(LocalConfigTest, localconfig_get_value_double)
{
  vcc::localconfig::testInit(good_test_filepath);
  ASSERT_DOUBLE_EQ(22.333, vcc::localconfig::getValueDouble("double1"));
  ASSERT_DOUBLE_EQ(22, vcc::localconfig::getValueDouble("double2"));
  EXPECT_THROW(vcc::localconfig::getValueDouble("NOT_EXISTING"), std::runtime_error);
}