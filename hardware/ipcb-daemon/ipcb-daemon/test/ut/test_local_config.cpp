/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <util/local_config.h>
#include "cedric/mock_cedric_localconfig.h"

#include <memory>
#include <string>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

using namespace InfotainmentIpService::Utils;

MockLocalconfig* mock_local_config;

class LocalConfigFixture : public ::testing::Test
{
public:
    /**
     * Fixture Constructor
     */
    LocalConfigFixture()
    {
        mock_local_config_ = std::make_shared<MockLocalconfig>();
    }

    /**
     * Set up fixture
     */
    virtual void SetUp()
    {
        mock_local_config = mock_local_config_.get();

        EXPECT_CALL(*mock_local_config_, init()).WillOnce(Return(false));
        loadLocalConfig();

        EXPECT_CALL(*mock_local_config_, init()).WillOnce(Return(true));
        EXPECT_CALL(*mock_local_config_, getValueInt("IIPS_LM_NofNodesRG1")).WillOnce(Return(&rg_1_));
        EXPECT_CALL(*mock_local_config_, getValueInt("IIPS_LM_NofNodesRG3")).WillOnce(Return(&rg_3_));
        EXPECT_CALL(*mock_local_config_, getValueInt("IIPS_LM_RequestMonitoringTimeout"))
            .WillOnce(Return(&mon_timeout_));
        EXPECT_CALL(*mock_local_config_, getValueInt("IIPS_LM_NofLSCs")).WillOnce(Return(&noof_lscs_));

        loadLocalConfig();
    }

    /**
     * Tear down fixture
     */
    virtual void TearDown()
    {
        mock_local_config = nullptr;
    }

    const int rg_1_ = 1;
    const int rg_3_ = 3;
    const int mon_timeout_ = 5;
    const int noof_lscs_ = 8;

    std::shared_ptr<MockLocalconfig> mock_local_config_;
};

TEST_F(LocalConfigFixture, loadLocalConfig)
{
    EXPECT_CALL(*mock_local_config_, init()).WillOnce(Return(true));
    EXPECT_CALL(*mock_local_config_, getValueInt("IIPS_LM_NofNodesRG1")).WillOnce(Return(&rg_1_));
    EXPECT_CALL(*mock_local_config_, getValueInt("IIPS_LM_NofNodesRG3")).WillOnce(Return(&rg_3_));
    EXPECT_CALL(*mock_local_config_, getValueInt("IIPS_LM_RequestMonitoringTimeout")).WillOnce(Return(nullptr));
    EXPECT_CALL(*mock_local_config_, getValueInt("IIPS_LM_NofLSCs")).WillOnce(Return(nullptr));

    loadLocalConfig();
}

TEST_F(LocalConfigFixture, getRequestMonitoringTmo)
{
    EXPECT_EQ(getRequestMonitoringTmo(), mon_timeout_);
}

TEST_F(LocalConfigFixture, getNofLocalSoftwareComponents)
{
    EXPECT_EQ(getNofLocalSoftwareComponents(), noof_lscs_);
}
