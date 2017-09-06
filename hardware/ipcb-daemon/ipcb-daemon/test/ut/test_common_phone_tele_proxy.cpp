/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "CommonAPI/CommonAPI.hpp"
#include "ipcommandbus/Message.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_commonapi_runtime.hpp"
#include "mocks/mock_ip_service.h"
#include "services/common_phone_tele_proxy.h"

#include <functional>
#include <map>
#include <memory>

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::NiceMock;

extern int g_common_phone_tele_proxy_function_calls;
extern int g_common_phone_tele_proxy_available_calls;
extern bool g_common_phone_tele_proxy_available;

namespace Connectivity
{
class CommonPhoneTeleProxyFixture : public ::testing::Test
{
public:
    CommonPhoneTeleProxyFixture()
    {
        mock_runtime_ = CommonAPI::MockRuntime::get();
    }

    virtual void SetUp()
    {
        ON_CALL(common_phone_tele_proxy_, RegisterServer(_)).WillByDefault(Return(true));
    }
    virtual void TearDown()
    {
        g_common_phone_tele_proxy_function_calls = 0;
        g_common_phone_tele_proxy_available_calls = 0;
        common_phone_tele_proxy_.common_phone_tele_proxy_available_ = false;
    }

    // Access private functions
    void AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
    {
        common_phone_tele_proxy_.AvailabilityCallback(status);
    }

    void cbPremiumAudioSetRequest(Message &msg)
    {
        common_phone_tele_proxy_.cbPremiumAudioSetRequest(msg);
    }

    void SetCommonTeleProxyAvailable(bool mode)
    {
        common_phone_tele_proxy_.common_phone_tele_proxy_available_ = mode;
    }

    void SubscriptionPremiumAudioNotificationCallback(v0_conn::IpCommandBusTypes::AudioStatus audio_status)
    {
        common_phone_tele_proxy_.SubscriptionPremiumAudioNotificationCallback(audio_status);
    }
    std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
        std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");
    CommonPhoneTeleProxy common_phone_tele_proxy_;
    std::shared_ptr<CommonAPI::MockRuntime> mock_runtime_;
    NiceMock<MockMessageDispatcher> dispatcher_;
};

TEST_F(CommonPhoneTeleProxyFixture, TestInitialize)
{
    mock_runtime_->available = true;

    EXPECT_CALL(common_phone_tele_proxy_, RegisterServer(_)).WillOnce(Return(false));
    EXPECT_FALSE(common_phone_tele_proxy_.Initialize(&dispatcher_, common_api_main_loop_context_));

    EXPECT_CALL(common_phone_tele_proxy_, RegisterServer(_)).WillOnce(Return(true));
    EXPECT_TRUE(common_phone_tele_proxy_.Initialize(&dispatcher_, common_api_main_loop_context_));
}

TEST_F(CommonPhoneTeleProxyFixture, AvailabilityCallback_Status_Available_And_Proxy_Available)
{
    mock_runtime_->available = true;
    common_phone_tele_proxy_.Initialize(&dispatcher_, common_api_main_loop_context_);

    g_common_phone_tele_proxy_available = false;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::NOT_AVAILABLE);
    EXPECT_EQ(0, g_common_phone_tele_proxy_available_calls);

    g_common_phone_tele_proxy_available = true;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(1, g_common_phone_tele_proxy_available_calls);

    g_common_phone_tele_proxy_available = true;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(2, g_common_phone_tele_proxy_available_calls);
}

TEST_F(CommonPhoneTeleProxyFixture, TestcbPremiumAudioSetRequest)
{
    mock_runtime_->available = true;
    common_phone_tele_proxy_.Initialize(&dispatcher_, common_api_main_loop_context_);

    // Proxy not available
    SetCommonTeleProxyAvailable(false);
    EXPECT_CALL(common_phone_tele_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::CommonPhoneTelematics,
                                 Connectivity::VccIpCmd::OperationId::PremiumAudio,
                                 Connectivity::Message::Ecu::TEM,
                                 8));

    Message msg;
    msg.ecu = Message::Ecu::TEM;
    msg.pdu.header.sender_handle_id = 8;
    this->cbPremiumAudioSetRequest(msg);

    // Proxy available
    SetCommonTeleProxyAvailable(true);

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
    Icb_OpPremiumAudio_SetRequest req = Icb_OpPremiumAudio_SetRequest_Create(session);

    req->isRequested = true;

    // Decode OK
    EXPECT_CALL(common_phone_tele_proxy_, DecodeMessagePremiumAudioSetRequest(_, _, _, _))
        .WillOnce(DoAll(SetArgReferee<1>(req), Return(true)));
    this->cbPremiumAudioSetRequest(msg);

    // DECODE NOT OK
    msg.pdu.header.sender_handle_id = 9;
    EXPECT_CALL(common_phone_tele_proxy_, DecodeMessagePremiumAudioSetRequest(_, _, _, _)).WillOnce(Return(false));
    EXPECT_CALL(common_phone_tele_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::CommonPhoneTelematics,
                                 Connectivity::VccIpCmd::OperationId::PremiumAudio,
                                 Connectivity::Message::Ecu::TEM,
                                 9));
    this->cbPremiumAudioSetRequest(msg);
}

}  // namespace Connectivity
