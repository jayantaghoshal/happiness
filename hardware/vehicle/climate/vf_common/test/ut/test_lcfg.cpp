/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "local_config_helper.h"

#include "cedric/cedric_localconfig_mock.h"

#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using namespace testing;
using namespace std::chrono_literals;

class TestLcfgHelper : public ::testing::Test
{
public:
    TestLcfgHelper()
    {
    }

    testing::NiceMock<LocalConfigMock> localConfig;
};

TEST_F(TestLcfgHelper, Bool_WhenReading_WillReturnValue)
{
    const bool xyzParamValue = true;
    ON_CALL(localConfig, getBoolValue("xyz")).WillByDefault(Return(&xyzParamValue));
    EXPECT_EQ(xyzParamValue, util::readLocalConfig<bool>("xyz"));
}

TEST_F(TestLcfgHelper, Int_WhenReading_WillReturnValue)
{
    const int xyzParamValue = 100;
    ON_CALL(localConfig, getIntValue("xyz")).WillByDefault(Return(&xyzParamValue));
    EXPECT_EQ(xyzParamValue, util::readLocalConfig<int>("xyz"));
}

TEST_F(TestLcfgHelper, Double_WhenReading_WillReturnValue)
{
    const double xyzParamValue = 100;
    ON_CALL(localConfig, getDoubleValue("xyz")).WillByDefault(Return(&xyzParamValue));
    EXPECT_EQ(xyzParamValue, util::readLocalConfig<double>("xyz"));
}

TEST_F(TestLcfgHelper, ChronoSeconds_WhenReading_WillReturnValue)
{
    const auto lcfgValue = 100;
    ON_CALL(localConfig, getIntValue("xyz")).WillByDefault(Return(&lcfgValue));
    EXPECT_EQ(100s, util::readLocalConfig<std::chrono::seconds>("xyz"));
}

TEST_F(TestLcfgHelper, ChronoMinutes_WhenReading_WillReturnValue)
{
    const auto lcfgValue = 100;
    ON_CALL(localConfig, getIntValue("xyz")).WillByDefault(Return(&lcfgValue));
    EXPECT_EQ(100min, util::readLocalConfig<std::chrono::minutes>("xyz"));
}

TEST_F(TestLcfgHelper, Exception_WhenParameterNameIsIncorrect_WillThrowOutOfRange)
{
    EXPECT_THROW(util::readLocalConfig<bool>("NonexistantParameter"), std::out_of_range);
}

TEST_F(TestLcfgHelper, Bool_WhenReturningNullptr_WillThrowOutOfRange)
{
    ON_CALL(localConfig, getBoolValue("xyz")).WillByDefault(Return(nullptr));
    EXPECT_THROW(util::readLocalConfig<bool>("xyz"), std::out_of_range);
}

TEST_F(TestLcfgHelper, Int_WhenReturningNullptr_WillThrowOutOfRange)
{
    ON_CALL(localConfig, getIntValue("xyz")).WillByDefault(Return(nullptr));
    EXPECT_THROW(util::readLocalConfig<int>("xyz"), std::out_of_range);
}

TEST_F(TestLcfgHelper, Double_WhenReturningNullptr_WillThrowOutOfRange)
{
    ON_CALL(localConfig, getDoubleValue("xyz")).WillByDefault(Return(nullptr));
    EXPECT_THROW(util::readLocalConfig<double>("xyz"), std::out_of_range);
}
