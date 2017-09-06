/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "services/rvdc_client.h"

#include "cedric/mock_cedric_localconfig.h"
#include "ipcommandbus/Message.h"
#include "mock_CommonApiAttribute.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_rvdc_proxy.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;
namespace gen_privacypolicy = v0::org::volvocars::privacypolicy;

namespace
{
auto ReturnCallSuccess =
    Return(ByMove(std::async(std::launch::deferred, []() { return CommonAPI::CallStatus::SUCCESS; })));
}

MockLocalconfig *mock_local_config;

namespace Connectivity
{
class TestRvdcClient : public Test
{
public:
    TestRvdcClient()
    {
        mock_local_config = new NiceMock<MockLocalconfig>();

        ON_CALL(*proxy_, getApprovalstatusesAttribute()).WillByDefault(ReturnRef(approvalStatusesAttribute_));
    }

    void SetUp()
    {
        createSut();
    }

protected:
    void createSut()
    {
        sut_ = std::make_unique<RvdcClient>();
        sut_->Initialize(&msgDispatcher_, common_api_main_loop_context_, proxy_);
    }

    std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
        std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");
    NiceMock<MockMessageDispatcher> msgDispatcher_;
    std::shared_ptr<NiceMock<RvdcProxyMock>> proxy_ = std::make_shared<NiceMock<RvdcProxyMock>>();
    const CommonAPI::ConnectionId_t clientId_ = "something";
    NiceMock<MockCommonApiAttribute<gen_privacypolicy::Rvdc::ApprovalStatuses>> approvalStatusesAttribute_;

    std::unique_ptr<RvdcClient> sut_;
};

TEST_F(TestRvdcClient, cbApprovalStatusRequest_WhenReceivingRequest_WillSendRequestToProxy)
{
    Pdu pdu;
    pdu.createHeader(VccIpCmd::ServiceId::DiagnosticManagement,
                     VccIpCmd::OperationId::ApprovalStatus,
                     VccIpCmd::OperationType::REQUEST,
                     VccIpCmd::DataType::UNDEFINED,
                     0);
    Message msg;
    msg.pdu = pdu;
    msg.ecu = Message::Ecu::VCM;

    // Should be called one time when getting value from CAPI + once during destructor
    EXPECT_CALL(*proxy_, getApprovalstatusesAttribute()).Times(2);

    // Simulate incoming message from cloud
    msgDispatcher_.simulateMessageReceived(msg, nullptr);
}

MATCHER(IsApprovalStatusError, "")
{
    return (arg.pdu.header.service_id == VccIpCmd::ServiceId::DiagnosticManagement &&
            arg.pdu.header.operation_id == VccIpCmd::OperationId::ApprovalStatus &&
            arg.pdu.header.operation_type == VccIpCmd::OperationType::ERROR &&
            arg.pdu.header.data_type == VccIpCmd::DataType::ENCODED);
}

TEST_F(TestRvdcClient, cbApprovalStatusReponse_WhenCapiError_WillSendErrorToCloud)
{
    const auto callStatusRetVal = CommonAPI::CallStatus::CONNECTION_FAILED;
    const gen_privacypolicy::Rvdc::ApprovalStatuses statuses;

    Pdu pdu;
    pdu.createHeader(VccIpCmd::ServiceId::DiagnosticManagement,
                     VccIpCmd::OperationId::ApprovalStatus,
                     VccIpCmd::OperationType::REQUEST,
                     VccIpCmd::DataType::UNDEFINED,
                     0);
    Message msg;
    msg.pdu = pdu;
    msg.ecu = Message::Ecu::VCM;

    // Simulate response from common API
    ON_CALL(*proxy_, getApprovalstatusesAttribute()).WillByDefault(ReturnRef(approvalStatusesAttribute_));
    ON_CALL(approvalStatusesAttribute_, getValueAsync(_, _))
        .WillByDefault(DoAll(InvokeArgument<0>(callStatusRetVal, statuses),
                             Return(ByMove(std::async(std::launch::deferred, []() { return callStatusRetVal; })))));

    EXPECT_CALL(msgDispatcher_, sendMessage(IsApprovalStatusError(), _)).Times(1);

    // Simulate incoming message from cloud
    msgDispatcher_.simulateMessageReceived(msg, nullptr);
}

MATCHER(IsApprovalStatusResponse, "")
{
    return (arg.pdu.header.service_id == VccIpCmd::ServiceId::DiagnosticManagement &&
            arg.pdu.header.operation_id == VccIpCmd::OperationId::ApprovalStatus &&
            arg.pdu.header.operation_type == VccIpCmd::OperationType::RESPONSE &&
            arg.pdu.header.data_type == VccIpCmd::DataType::ENCODED);
}

TEST_F(TestRvdcClient, cbApprovalStatusResponse_WhenReceivingResponseFromCapi_WillSendResponseToCloud)
{
    const auto callStatusRetVal = CommonAPI::CallStatus::SUCCESS;
    const gen_privacypolicy::Rvdc::ApprovalStatuses statuses;

    Pdu pdu;
    pdu.createHeader(VccIpCmd::ServiceId::DiagnosticManagement,
                     VccIpCmd::OperationId::ApprovalStatus,
                     VccIpCmd::OperationType::REQUEST,
                     VccIpCmd::DataType::UNDEFINED,
                     0);
    Message msg;
    msg.pdu = pdu;
    msg.ecu = Message::Ecu::VCM;

    // Simulate response from common API
    ON_CALL(*proxy_, getApprovalstatusesAttribute()).WillByDefault(ReturnRef(approvalStatusesAttribute_));
    ON_CALL(approvalStatusesAttribute_, getValueAsync(_, _))
        .WillByDefault(DoAll(InvokeArgument<0>(callStatusRetVal, statuses), ReturnCallSuccess));

    EXPECT_CALL(msgDispatcher_, sendMessage(IsApprovalStatusResponse(), _)).Times(1);

    // Simulate incoming message from cloud
    msgDispatcher_.simulateMessageReceived(msg, nullptr);
}

MATCHER(IsApprovalStatusNotification, "")
{
    return (arg.pdu.header.service_id == VccIpCmd::ServiceId::DiagnosticManagement &&
            arg.pdu.header.operation_id == VccIpCmd::OperationId::ApprovalStatus &&
            arg.pdu.header.operation_type == VccIpCmd::OperationType::NOTIFICATION &&
            arg.pdu.header.data_type == VccIpCmd::DataType::ENCODED);
}

TEST_F(TestRvdcClient, ApprovalStatusNotification_WhenReceivingNotificationFromCapi_WillSendNotificationToCloud)
{
    gen_privacypolicy::Rvdc::ApprovalStatuses statuses;

    EXPECT_CALL(msgDispatcher_, sendMessage(IsApprovalStatusNotification(), _)).Times(1);

    approvalStatusesAttribute_.getChangedEvent().simulateNotifyListeners(statuses);
}
}
