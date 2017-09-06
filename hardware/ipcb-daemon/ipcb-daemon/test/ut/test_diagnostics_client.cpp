/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "CommonAPI/CommonAPI.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <diag_message_keys.h>
#include "cedric/cedric_localconfig.h"
#include "ipcommandbus/Message.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_commonapi_runtime.hpp"
#include "mocks/mock_ip_service.h"
#include "services/diagnostics_client.h"

#include <map>
#include <memory>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

LocalconfigStub local_config_stub;

extern bool g_diagnosticsProxyAvailable;
extern std::vector<uint8_t> g_payload;

namespace Connectivity
{
class DiagnosticsClientFixture : public ::testing::Test
{
public:
    DiagnosticsClientFixture()
    {
        diagnosticsClient.p_proxy_ = nullptr;
        diagnosticsClient.call_info_.sender_ = 0;
        diagnosticsClient.call_info_.timeout_ = 0;
        diagnosticsClient.ip_bus_server_subscriptions_ = {};
        mockRuntime = CommonAPI::MockRuntime::get();
    }

    void SetUp()
    {
        logging::ConsoleLogContext::setGlobalLogLevel(logging::LogLevel::None);
        mockRuntime->available = true;
    }

    void TearDown()
    {
    }
    std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
        std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");
    Connectivity::DiagnosticsClient diagnosticsClient;
    std::shared_ptr<CommonAPI::MockRuntime> mockRuntime;

protected:
    void verifyAndClear(void)
    {
    }
};

// External variables.

TEST_F(DiagnosticsClientFixture, TestInitialize)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;

    EXPECT_CALL(diagnosticsClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(diagnosticsClient, RegisterServer(_)).WillOnce(Return(false));
    diagnosticsClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    EXPECT_CALL(diagnosticsClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(diagnosticsClient, RegisterServer(_)).WillOnce(Return(true));
    diagnosticsClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);
}

TEST_F(DiagnosticsClientFixture, TestAvailabilityCallback)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;

    EXPECT_CALL(diagnosticsClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(diagnosticsClient, RegisterServer(_));

    diagnosticsClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    diagnosticsClient.AvailabilityCallback(CommonAPI::AvailabilityStatus::NOT_AVAILABLE);

    g_diagnosticsProxyAvailable = false;
    diagnosticsClient.AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);

    g_diagnosticsProxyAvailable = true;

    diagnosticsClient.AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
}

TEST_F(DiagnosticsClientFixture, TestSubscribeOnKeysAsyncCallback)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;

    EXPECT_CALL(diagnosticsClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(diagnosticsClient, RegisterServer(_)).WillOnce(Return(true));
    diagnosticsClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::INVALID_VALUE);

    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::SUCCESS);

    g_diagnosticsProxyAvailable = false;
    diagnosticsClient.SetInvalidDataFromTEM(false);
    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::SUCCESS);
    diagnosticsClient.SetInvalidDataFromTEM(false);

    g_diagnosticsProxyAvailable = true;
    diagnosticsClient.SetInvalidDataFromTEM(false);
    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::SUCCESS);
    diagnosticsClient.SetInvalidDataFromTEM(false);

    diagnosticsClient.SetInvalidDataFromTEM(true);
    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::SUCCESS);
    diagnosticsClient.SetInvalidDataFromTEM(true);

    g_diagnosticsProxyAvailable = false;
    diagnosticsClient.SetInvalidDataFromVCM(false);
    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::SUCCESS);
    diagnosticsClient.SetInvalidDataFromVCM(false);

    g_diagnosticsProxyAvailable = true;
    diagnosticsClient.SetInvalidDataFromVCM(false);
    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::SUCCESS);
    diagnosticsClient.SetInvalidDataFromVCM(false);

    diagnosticsClient.SetInvalidDataFromVCM(true);
    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::SUCCESS);
    diagnosticsClient.SetInvalidDataFromVCM(true);

    g_diagnosticsProxyAvailable = true;
    diagnosticsClient.SetInvalidDataFromVCM(true);
    diagnosticsClient.SubscribeOnKeysAsyncCallback(CommonAPI::CallStatus::SUCCESS);
}

TEST_F(DiagnosticsClientFixture, TestMessageToDiagAsyncCallback)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;

    EXPECT_CALL(diagnosticsClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(diagnosticsClient, RegisterServer(_)).WillOnce(Return(true));
    diagnosticsClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    diagnosticsClient.MessageToDiagAsyncCallback(CommonAPI::CallStatus::INVALID_VALUE);

    diagnosticsClient.MessageToDiagAsyncCallback(CommonAPI::CallStatus::SUCCESS);
}

TEST_F(DiagnosticsClientFixture, TestSetInvalidData)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;

    EXPECT_CALL(diagnosticsClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(diagnosticsClient, RegisterServer(_)).WillOnce(Return(true));
    diagnosticsClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    diagnosticsClient.SetInvalidData(Connectivity::Message::Ecu::TEM, true);
    diagnosticsClient.SetInvalidData(Connectivity::Message::Ecu::VCM, false);
    diagnosticsClient.SetInvalidData(Connectivity::Message::Ecu::UNKNOWN, true);
}

TEST_F(DiagnosticsClientFixture, MessageToApp)
{
    NiceMock<Connectivity::MockMessageDispatcher> messageDispatcherStub;

    EXPECT_CALL(diagnosticsClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(diagnosticsClient, RegisterServer(_)).WillOnce(Return(false));
    diagnosticsClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    std::vector<uint8_t> dummy_payload(6);
    diagnosticsClient.MessageToApp(MK_CD_DID_IP_LINK_MANAGER_ACTIVE_SERVICE_REQ, dummy_payload);

    EXPECT_CALL(diagnosticsClient, setDispatcher(&messageDispatcherStub)).Times(1);
    EXPECT_CALL(diagnosticsClient, RegisterServer(_)).WillOnce(Return(false));
    diagnosticsClient.Initialize(&messageDispatcherStub, common_api_main_loop_context_);

    diagnosticsClient.MessageToApp(MK_CD_DID_IP_LINK_MANAGER_ACTIVE_SERVICE_REQ, dummy_payload);
    std::vector<uint8_t> expected_payload = {
        static_cast<uint8_t>(diagnosticsClient.MessageEcuToDiagnosticsEcu(Connectivity::Message::Ecu::VCM)),
        static_cast<uint8_t>(diagnosticsClient.MessageEcuToDiagnosticsEcu(Connectivity::Message::Ecu::TEM))

        };
    EXPECT_EQ(g_payload, expected_payload);
}

TEST_F(DiagnosticsClientFixture, MessageEcuToDiagnosticsEcu)
{
    EXPECT_EQ(diagnosticsClient.MessageEcuToDiagnosticsEcu(Message::Ecu::UNKNOWN), DiagnosticsClient::Ecu::NONE);
    EXPECT_EQ(diagnosticsClient.MessageEcuToDiagnosticsEcu(Message::Ecu::ALL), DiagnosticsClient::Ecu::NONE);
    EXPECT_EQ(diagnosticsClient.MessageEcuToDiagnosticsEcu(Message::Ecu::IHU), DiagnosticsClient::Ecu::NONE);
    EXPECT_EQ(diagnosticsClient.MessageEcuToDiagnosticsEcu(Message::Ecu::IHU), DiagnosticsClient::Ecu::NONE);
    EXPECT_EQ(diagnosticsClient.MessageEcuToDiagnosticsEcu(Message::Ecu::TEM), DiagnosticsClient::Ecu::TEM);
    EXPECT_EQ(diagnosticsClient.MessageEcuToDiagnosticsEcu(Message::Ecu::VCM), DiagnosticsClient::Ecu::VCM);
}

}  // namespace Connectivity
