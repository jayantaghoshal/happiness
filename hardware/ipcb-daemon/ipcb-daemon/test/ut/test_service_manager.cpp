/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <util/local_config.h>
#include "cedric/mock_cedric_localconfig.h"
#include "connectivity-sd/mock_time_provider.h"
#include "mocks/MockMessageDispatcher.h"
#include "service_manager.h"

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

class ServiceManagerFixture : public ::testing::Test
{
public:
    /**
     * Fixture Constructor
     */
    ServiceManagerFixture() : sm_{timeProvider, common_api_main_loop_context_}
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

    std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
        std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");
    Connectivity::TimeProviderStub timeProvider;
    ServiceManager sm_;
    NiceMock<MockMessageDispatcher> messageDispatcher_;
};

// Original wrapper used in the old transport layer unit tests:
// This setup lead to quite long failure logs with the actual fault being a little too well disguised.
// MATCHER_P(LambdaMatcher, func, "") { bool ret=false; *result_listener << func(arg, ret); return ret; }

// Instead we here catch the faults with EXPECT_EQ, EXPECT_TRUE, etc which give a compact and clear error message upon
// failure.
// It may look weird that this wrapper always returns true, but if an expect check fails in func, the parent test will
// automatically fail as well.
MATCHER_P(LambdaMatcher, func, "")
{
    func(arg);
    return true;
}

TEST_F(ServiceManagerFixture, RegisterAllCommonApiServices)
{
    // sm_.RegisterAllCommonApiServices(&messageDispatcher_);
}
