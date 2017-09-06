/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Connectivity.h"
#include "cedric/mock_cedric_localconfig.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

MockLocalconfig* mock_local_config;

class ConnectivityFixture : public ::testing::Test
{
public:
    /**
     * Fixture Constructor
     */
    ConnectivityFixture()
    {
    }

    /**
     * Set up fixture
     */
    virtual void SetUp()
    {
    }

    /**
     * Tear down fixture
     */
    virtual void TearDown()
    {
    }

    Connectivity::ConnectivityX connectivity;
};

TEST_F(ConnectivityFixture, EcuToString)
{
    EXPECT_STREQ("none", connectivity.EcuToString(Icb_Ecu_t::e_Icb_Ecu_none));
    EXPECT_STREQ("ihu", connectivity.EcuToString(Icb_Ecu_t::e_Icb_Ecu_ihu));
    EXPECT_STREQ("tem", connectivity.EcuToString(Icb_Ecu_t::e_Icb_Ecu_tem));
    EXPECT_STREQ("vcm", connectivity.EcuToString(Icb_Ecu_t::e_Icb_Ecu_vcm));
    EXPECT_STREQ("undefined ecu enum", connectivity.EcuToString(static_cast<Icb_Ecu_t>(100)));
}
