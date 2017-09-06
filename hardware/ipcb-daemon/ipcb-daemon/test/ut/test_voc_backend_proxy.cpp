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
#include "services/voc_backend_proxy.h"

#include <functional>
#include <map>
#include <memory>

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::NiceMock;

extern int g_voc_backend_function_calls;
extern int g_voc_backend_available_calls;
extern bool g_voc_backend_proxy_available;

namespace Connectivity
{
class VOCBackendProxyFixture : public ::testing::Test
{
public:
    VOCBackendProxyFixture()
    {
        mock_runtime_ = CommonAPI::MockRuntime::get();
    }

    virtual void SetUp()
    {
        ON_CALL(voc_backend_proxy_, RegisterServer(_)).WillByDefault(Return(true));
    }
    virtual void TearDown()
    {
        g_voc_backend_function_calls = 0;
        g_voc_backend_available_calls = 0;
        voc_backend_proxy_.voc_backend_proxy_available_ = false;
    }

    // Access private functions
    void AvailabilityCallback(const CommonAPI::AvailabilityStatus &status)
    {
        voc_backend_proxy_.AvailabilityCallback(status);
    }

    void cbTextMessageSetRequest(Message &msg)
    {
        voc_backend_proxy_.cbTextMessageSetRequest(msg);
    }

    void SetVOCBackendProxyAvailable(bool mode)
    {
        voc_backend_proxy_.voc_backend_proxy_available_ = mode;
    }

    VOCBackendProxy voc_backend_proxy_;
    std::shared_ptr<CommonAPI::MockRuntime> mock_runtime_;
    NiceMock<MockMessageDispatcher> dispatcher_;
    std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
        std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");
};

TEST_F(VOCBackendProxyFixture, TestInitialize)
{
    mock_runtime_->available = true;

    EXPECT_CALL(voc_backend_proxy_, RegisterServer(_)).WillOnce(Return(false));
    EXPECT_FALSE(voc_backend_proxy_.Initialize(&dispatcher_, common_api_main_loop_context_));

    EXPECT_CALL(voc_backend_proxy_, RegisterServer(_)).WillOnce(Return(true));
    EXPECT_TRUE(voc_backend_proxy_.Initialize(&dispatcher_, common_api_main_loop_context_));
}

TEST_F(VOCBackendProxyFixture, AvailabilityCallback_Status_Available_And_Proxy_Available)
{
    mock_runtime_->available = true;
    voc_backend_proxy_.Initialize(&dispatcher_, common_api_main_loop_context_);

    g_voc_backend_proxy_available = false;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::NOT_AVAILABLE);
    EXPECT_EQ(0, g_voc_backend_available_calls);

    g_voc_backend_proxy_available = true;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(1, g_voc_backend_available_calls);

    g_voc_backend_proxy_available = true;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(2, g_voc_backend_available_calls);
}

TEST_F(VOCBackendProxyFixture, TestcbTextMessageSetRequest)
{
    mock_runtime_->available = true;
    voc_backend_proxy_.Initialize(&dispatcher_, common_api_main_loop_context_);

    // Proxy not available
    SetVOCBackendProxyAvailable(false);
    EXPECT_CALL(voc_backend_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::Telematics,
                                 Connectivity::VccIpCmd::OperationId::TextMessage,
                                 Connectivity::Message::Ecu::TEM,
                                 8));

    Message msg;
    msg.ecu = Message::Ecu::TEM;
    msg.pdu.header.sender_handle_id = 8;
    this->cbTextMessageSetRequest(msg);

    // Proxy available
    SetVOCBackendProxyAvailable(true);

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
    Icb_OpTextMessage_SetRequest req = Icb_OpTextMessage_SetRequest_Create(session);

    req->sourceStatus = e_Icb_SourceStatus_unidentified;
    req->source = InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, "");
    req->text = InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, "");

    // Decode OK
    EXPECT_CALL(voc_backend_proxy_, DecodeMessageTextMessageSetRequest(_, _, _, _))
        .WillOnce(DoAll(SetArgReferee<1>(req), Return(true)));
    this->cbTextMessageSetRequest(msg);

    // DECODE NOT OK
    msg.pdu.header.sender_handle_id = 9;
    EXPECT_CALL(voc_backend_proxy_, DecodeMessageTextMessageSetRequest(_, _, _, _)).WillOnce(Return(false));
    EXPECT_CALL(voc_backend_proxy_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::Telematics,
                                 Connectivity::VccIpCmd::OperationId::TextMessage,
                                 Connectivity::Message::Ecu::TEM,
                                 9));
    this->cbTextMessageSetRequest(msg);
}

}  // namespace Connectivity
