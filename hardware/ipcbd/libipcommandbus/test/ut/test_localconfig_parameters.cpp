/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "ipcommandbus/local_config_parameters.h"
#include "mocks/cedric/mock_cedric_localconfig.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgPointee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::StrEq;

using namespace Connectivity;

const int testValue = 5555;
const int defaultTimeout = 5800;

MockLocalconfig *g_mock_local_config = nullptr;

class LocalconfigParametersFixture : public ::testing::Test {
  public:
    virtual void SetUp() { g_mock_local_config = new MockLocalconfig(); }

    virtual void TearDown() { delete g_mock_local_config; }

  protected:
    virtual void VerifyAndClear() {}
};

TEST_F(LocalconfigParametersFixture, construction_fault) {
    EXPECT_CALL(*g_mock_local_config, init()).WillOnce(Return(false));
    LocalconfigParameters lcfg;
}

TEST_F(LocalconfigParametersFixture, construction_init_and_read) {
    EXPECT_CALL(*g_mock_local_config, init()).WillOnce(Return(true));
    EXPECT_CALL(*g_mock_local_config, getValueInt(_)).WillRepeatedly(Return(&testValue));
    EXPECT_CALL(*g_mock_local_config, getValueInt("CONN_defaultTimeoutWFR")).WillRepeatedly(Return(&defaultTimeout));
    LocalconfigParameters lcfg;
    EXPECT_EQ(std::chrono::milliseconds(5555),
              lcfg.getTimeout(VccIpCmd::CombinedId(VccIpCmd::ServiceId::WLAN, VccIpCmd::OperationId::WLANMode)));
    EXPECT_EQ(std::chrono::milliseconds(5800),
              lcfg.getTimeout(VccIpCmd::CombinedId(VccIpCmd::ServiceId::Undefined, VccIpCmd::OperationId::Undefined)));
}
