/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "cedric/mock_cedric_localconfig.h"
#include "connectivity_service.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

using namespace Connectivity;

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

    ConnectivityService cs;
};

TEST_F(ConnectivityFixture, Initialize)
{
}
