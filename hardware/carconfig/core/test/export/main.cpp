/**
 * Unit test of the
 *
 * Production code under test:
 *
 *
 */

#include <cedric_carconfig_mock.h>
#include <cedric_localconfig_mock.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

using ::testing::Return;
using ::testing::_;

class CarConfigTestFixture : public ::testing::Test
{
  public:
    virtual void SetUp()
    {
        ccmock = new CarConfigMock();
    }

    virtual void TearDown() { delete ccmock; }
  protected:
    CarConfigMock *ccmock;
};

class LocalConfigTestFixture : public ::testing::Test
{
  public:
    virtual void SetUp()
    {
        lcfgmock = new LocalConfigMock();
    }

    virtual void TearDown() { delete lcfgmock; }
  protected:
    LocalConfigMock *lcfgmock;
};

TEST_F(CarConfigTestFixture, testcc1)
{
    EXPECT_CALL(*ccmock, init()).WillOnce(Return(1));

    ASSERT_TRUE(cedric::core::carconfig::init() == 1);
}

TEST_F(LocalConfigTestFixture, testlcfg1)
{
    EXPECT_CALL(*lcfgmock, init()).WillOnce(Return(true));

    ASSERT_TRUE(cedric::core::localconfig::init() == true);
}

TEST_F(LocalConfigTestFixture, testlcfg2)
{
   const int myInt = 42;
   EXPECT_CALL(*lcfgmock, getIntValue("MyParam1")).WillOnce(Return(&myInt));

   ASSERT_TRUE(*cedric::core::localconfig::getValue<int>("MyParam1") == myInt);
}

TEST_F(LocalConfigTestFixture, testlcfg3)
{
   const std::string myStr = "My String";
   EXPECT_CALL(*lcfgmock, getStringValue("MyParam2")).WillOnce(Return(&myStr));

   ASSERT_STREQ(cedric::core::localconfig::getValue<std::string>("MyParam2")->c_str(), myStr.c_str());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
