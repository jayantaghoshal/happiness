/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_common_phone_tele_service_stub.h"
#include "mocks/mock_ip_service.h"
#include "services/common_phone_tele_service.h"

namespace v0_conn = v0::org::volvo::connectivity;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

namespace Connectivity
{
class CommonPhoneTelematicsServiceFixture : public ::testing::Test
{
public:
    CommonPhoneTelematicsServiceFixture()
    {
    }

    virtual void SetUp()
    {
        ip_service_.setDispatcher(nullptr);
        common_phone_telematics_service_.common_phone_telematics_service_stub_ =
            p_common_phone_telematics_service_stub_;
    }

    virtual void TearDown()
    {
    }

    CommonPhoneTelematicsService common_phone_telematics_service_;

    NiceMock<MockIpService> &ip_service_ = common_phone_telematics_service_;
    std::shared_ptr<NiceMock<MockCommonPhoneTelematicsStub>> p_common_phone_telematics_service_stub_ =
        std::make_shared<NiceMock<MockCommonPhoneTelematicsStub>>();
};

MATCHER_P(LambdaMatcher, func, "")
{
    func(arg);
    return true;
}

TEST_F(CommonPhoneTelematicsServiceFixture, TestInitialize)
{
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(true));

    bool ret = common_phone_telematics_service_.Initialize(&dispatcher);

    ASSERT_EQ(ret, true);
}

TEST_F(CommonPhoneTelematicsServiceFixture, TestInitializeFailRegister)
{
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(false));

    bool ret = common_phone_telematics_service_.Initialize(&dispatcher);

    ASSERT_EQ(ret, false);
}

void CapiCallbackCallHandlingSetRequest(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error)
{
}

TEST_F(CommonPhoneTelematicsServiceFixture, TestCallHandlingSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    const v0_conn::IpCommandBusTypes::AssistCallAction action =
        v0_conn::IpCommandBusTypes::AssistCallAction::kAcceptCall;

    const v0_conn::CommonPhoneTelematicsStub::CallHandlingSetRequestReply_t reply = CapiCallbackCallHandlingSetRequest;

    EXPECT_CALL(ip_service_,
                SendSetRequestOpCallHandling(
                    _,
                    Icb_OpCallHandling_SetRequest_Encode,
                    Icb_OpCallHandling_SetRequest_EncodedSize,
                    VccIpCmd::ServiceId::CommonPhoneTelematics,
                    VccIpCmd::OperationId::CallHandling,
                    Message::Ecu::TEM,
                    LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                        std::shared_ptr<CommonPhoneTelematicsService::CallHandlingSetRequestCallerData> p_data =
                            std::dynamic_pointer_cast<CommonPhoneTelematicsService::CallHandlingSetRequestCallerData>(
                                p_caller_data);
                        EXPECT_NE(nullptr, p_data);
                        EXPECT_NE(nullptr, p_data->reply_);
                    })));

    common_phone_telematics_service_.CallHandlingSetRequest(client, action, 1, reply);
}

TEST_F(CommonPhoneTelematicsServiceFixture, TestCallHandlingResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<CommonPhoneTelematicsService::CallHandlingSetRequestCallerData> p_caller_data(
        std::make_shared<CommonPhoneTelematicsService::CallHandlingSetRequestCallerData>());

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error) {
        b_callback_called = true;
        ASSERT_EQ(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    common_phone_telematics_service_.cbCommonPhoneTelematicsCallHandlingResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CommonPhoneTelematicsServiceFixture, TestCallHandlingResponseErrorStat)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<CommonPhoneTelematicsService::CallHandlingSetRequestCallerData> p_caller_data(
        std::make_shared<CommonPhoneTelematicsService::CallHandlingSetRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error) {
        b_callback_called = true;
        ASSERT_EQ(v0_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    common_phone_telematics_service_.cbCommonPhoneTelematicsCallHandlingResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}
}  // namespace Connectivity
