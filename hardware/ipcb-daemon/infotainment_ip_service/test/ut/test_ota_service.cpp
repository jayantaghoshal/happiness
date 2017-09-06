/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <functional>
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_ip_service.h"
#include "mocks/mock_ota_service_stub.h"
#include "services/ota_service.h"

namespace Connectivity
{
namespace gen_conn = v0::org::volvo::connectivity;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;

class OtaServiceFixture : public ::testing::Test
{
public:
    OtaServiceFixture()
    {
    }

    virtual void SetUp()
    {
        fflush(stderr);
        ip_service_.setDispatcher(nullptr);
        ota_service_.ota_service_stub_ = p_ota_service_stub_;
    }

    virtual void TearDown()
    {
    }

    OTAService ota_service_;

    // The mock class (IpService) is the base class of the test class (OTAService).
    // (In fact IpService is used as a utility class, _not_ a base class in the classic sense with method overriding.)
    // So the test object and mock object are the one and same.
    // Just having this reference to be able to avoid confusion of whether context is test or mock...

    NiceMock<MockIpService> &ip_service_ = ota_service_;

    std::shared_ptr<NiceMock<MockOtaStub>> p_ota_service_stub_ = std::make_shared<NiceMock<MockOtaStub>>();
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

TEST_F(OtaServiceFixture, TestInitialize)
{
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    // (Doesn't work with reference, == operator missing:
    //  EXPECT_CALL(ip_service_, RegisterClient(wlan_service_.ip_bus_client_subscriptions));
    // )
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(true));

    bool ret = ota_service_.Initialize(&dispatcher);
    ASSERT_EQ(ret, true);
}

TEST_F(OtaServiceFixture, TestInitializeFailRegister)
{
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;

    EXPECT_CALL(ip_service_, setDispatcher(&dispatcher));
    // (Doesn't work with reference, == operator missing:
    //  EXPECT_CALL(ip_service_, RegisterClient(wlan_service_.ip_bus_client_subscriptions));
    // )
    EXPECT_CALL(ip_service_, RegisterClient(_)).WillOnce(Return(false));

    bool ret = ota_service_.Initialize(&dispatcher);
    ASSERT_EQ(ret, false);
}

TEST_F(OtaServiceFixture, TestUninitialize)
{
    ota_service_.Uninitialize();
}

/*
 * OTA DownloadStatus
 */

static void CapiCallbackOtaDownloadStatus(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                          gen_conn::OTATypes::DownloadStatus _downloadStatus,
                                          std::vector<gen_conn::OTATypes::SoftwareProduct> _downloadList)
{
}

TEST_F(OtaServiceFixture, TestDownloadStatusRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::OTAStub::DownloadStatusRequestReply_t reply = CapiCallbackOtaDownloadStatus;

    EXPECT_CALL(ip_service_,
                SendGenericRequest(VccIpCmd::ServiceId::OTA,
                                   VccIpCmd::OperationId::DownloadStatus,
                                   Message::Ecu::VCM,
                                   LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                                       std::shared_ptr<OTAService::OTADownloadStatusRequestCallerData> p_data =
                                           std::dynamic_pointer_cast<OTAService::OTADownloadStatusRequestCallerData>(
                                               p_caller_data);
                                       EXPECT_NE(nullptr, p_data);
                                       EXPECT_NE(nullptr, p_data->reply_);
                                   })));

    ota_service_.DownloadStatusRequest(client, reply);
}

TEST_F(OtaServiceFixture, TestDownloadStatusResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTADownloadStatusRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTADownloadStatusRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpDownloadStatus_Response resp = Icb_OpDownloadStatus_Response_Create(session);

    resp->downloadStatus = e_Icb_DownloadStatus_active;

    Icb_ArrayOf_SoftwareProduct_SetSize(resp->downloadList, session, 2);
    char uuid1[] = "uuid1";
    ASN_UTF8String_AssociateText(resp->downloadList->data_array[1]->id->uuid, uuid1);
    resp->downloadList->data_array[1]->downloadState = e_Icb_DownloadState_queued;
    resp->downloadList->data_array[1]->progress = 0;

    EXPECT_CALL(ip_service_,
                DecodeResponseDownloadStatus(
                    _, _, _, Icb_OpDownloadStatus_Response_Create, Icb_OpDownloadStatus_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_ = [uuid1, &b_callback_called](
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        gen_conn::OTATypes::DownloadStatus _downloadStatus,
        std::vector<gen_conn::OTATypes::SoftwareProduct> _downloadList) {
        b_callback_called = true;
        // fprintf(stderr, "b_callback_called (callback)=%u\n", b_callback_called);
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
        ASSERT_EQ(gen_conn::OTATypes::DownloadStatus::active, _downloadStatus);

        ASSERT_EQ(static_cast<std::size_t>(2), _downloadList.size());
        ASSERT_EQ(uuid1, _downloadList[1].getId().getUuid());
        ASSERT_EQ(gen_conn::OTATypes::DownloadState::queued, _downloadList[1].getDownloadState());
        ASSERT_EQ(0, _downloadList[1].getDownloadProgress());
    };

    ota_service_.cbOTADownloadStatusResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(OtaServiceFixture, TestDownloadStatusResponseErrorStat)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTADownloadStatusRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTADownloadStatusRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseDownloadStatus(_, _, _, _, _)).Times(0);

    bool b_callback_called = false;
    // fprintf(stderr, "b_callback_called (before)=%u\n", b_callback_called);
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                 gen_conn::OTATypes::DownloadStatus _downloadStatus,
                                                 std::vector<gen_conn::OTATypes::SoftwareProduct> _downloadList) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    ota_service_.cbOTADownloadStatusResponse(msg, p_caller_data);
    // fprintf(stderr, "b_callback_called (after)=%u\n", b_callback_called);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(OtaServiceFixture, TestDownloadStatusResponseDecodeError)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTADownloadStatusRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTADownloadStatusRequestCallerData>());

    EXPECT_CALL(ip_service_,
                DecodeResponseDownloadStatus(
                    _, _, _, Icb_OpDownloadStatus_Response_Create, Icb_OpDownloadStatus_Response_Decode))
        .WillOnce(Return(false));

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                 gen_conn::OTATypes::DownloadStatus _downloadStatus,
                                                 std::vector<gen_conn::OTATypes::SoftwareProduct> _downloadList) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    ota_service_.cbOTADownloadStatusResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(OtaServiceFixture, TestDownloadStatusNotification)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpDownloadStatus_Response resp = Icb_OpDownloadStatus_Response_Create(session);

    resp->downloadStatus = e_Icb_DownloadStatus_active;

    Icb_ArrayOf_SoftwareProduct_SetSize(resp->downloadList, session, 2);
    char uuid1[] = "uuid1";
    ASN_UTF8String_AssociateText(resp->downloadList->data_array[1]->id->uuid, uuid1);
    resp->downloadList->data_array[1]->downloadState = e_Icb_DownloadState_queued;
    resp->downloadList->data_array[1]->progress = 0;

    EXPECT_CALL(
        ip_service_,
        DecodeMessageDownloadStatus(_, _, Icb_OpDownloadStatus_Response_Create, Icb_OpDownloadStatus_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(resp), Return(true)));

    EXPECT_CALL(*p_ota_service_stub_,
                fireDownloadStatusNotificationEvent(
                    LambdaMatcher([](const gen_conn::OTATypes::DownloadStatus &_downloadStatus) {
                        EXPECT_EQ(gen_conn::OTATypes::DownloadStatus::active, _downloadStatus);
                    }),
                    LambdaMatcher([uuid1](const std::vector<gen_conn::OTATypes::SoftwareProduct> &_downloadList) {
                        EXPECT_EQ(static_cast<std::size_t>(2), _downloadList.size());
                        EXPECT_EQ(uuid1, _downloadList[1].getId().getUuid());
                        EXPECT_EQ(gen_conn::OTATypes::DownloadState::queued, _downloadList[1].getDownloadState());
                        EXPECT_EQ(0, _downloadList[1].getDownloadProgress());
                    })));

    ota_service_.cbOTADownloadStatusNotification(msg);
}

TEST_F(OtaServiceFixture, TestDownloadStatusNotificationDecodeError)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;

    EXPECT_CALL(
        ip_service_,
        DecodeMessageDownloadStatus(_, _, Icb_OpDownloadStatus_Response_Create, Icb_OpDownloadStatus_Response_Decode))
        .WillOnce(Return(false));

    EXPECT_CALL(*p_ota_service_stub_, fireDownloadStatusNotificationEvent(_, _)).Times(0);

    ota_service_.cbOTADownloadStatusNotification(msg);
}

/*
 * OTA DownloadSoftwareSetRequest
 */

void CapiCallbackDownloadSoftware(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error)
{
}

TEST_F(OtaServiceFixture, TestDownloadSoftwareSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::OTAStub::DownloadSoftwareSetRequestReply_t reply = CapiCallbackDownloadSoftware;

    const gen_conn::OTATypes::UUID uuid1("uuid1");
    const gen_conn::OTATypes::UUID uuid2("uuid2");
    std::vector<v0_conn::OTATypes::UUID> downloadList;
    downloadList.push_back(uuid1);
    downloadList.push_back(uuid2);

    EXPECT_CALL(ip_service_,
                SendSetRequestDownloadSoftware(
                    _,
                    Icb_OpDownloadSoftware_SetRequest_Encode,
                    Icb_OpDownloadSoftware_SetRequest_EncodedSize,
                    VccIpCmd::ServiceId::OTA,
                    VccIpCmd::OperationId::DownloadSoftware,
                    Message::Ecu::VCM,
                    LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                        std::shared_ptr<OTAService::OTADownloadSoftwareRequestCallerData> p_data =
                            std::dynamic_pointer_cast<OTAService::OTADownloadSoftwareRequestCallerData>(p_caller_data);
                        EXPECT_NE(nullptr, p_data);
                        EXPECT_NE(nullptr, p_data->reply_);
                    })));

    ota_service_.DownloadSoftwareSetRequest(client, downloadList, reply);
}

TEST_F(OtaServiceFixture, TestDownloadSoftwareResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTADownloadSoftwareRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTADownloadSoftwareRequestCallerData>());

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    ota_service_.cbOTADownloadSoftwareResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(OtaServiceFixture, TestDownloadSoftwareResponseErrorStat)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTADownloadSoftwareRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTADownloadSoftwareRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    ota_service_.cbOTADownloadSoftwareResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}

/*
 * OTA DeletedSoftwareSetRequest
 */

void CapiCallbackDeletedSoftware(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error)
{
}

TEST_F(OtaServiceFixture, TestDeletedSoftwareSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::OTAStub::DeletedSoftwareSetRequestReply_t reply = CapiCallbackDeletedSoftware;

    const gen_conn::OTATypes::PartIdentifier partid1("partid1");
    const gen_conn::OTATypes::PartIdentifier partid2("partid2");
    std::vector<v0_conn::OTATypes::PartIdentifier> partList;
    partList.push_back(partid1);
    partList.push_back(partid2);

    EXPECT_CALL(ip_service_,
                SendSetRequestDeletedSoftware(
                    _,
                    Icb_OpDeletedSoftware_SetRequest_Encode,
                    Icb_OpDeletedSoftware_SetRequest_EncodedSize,
                    VccIpCmd::ServiceId::OTA,
                    VccIpCmd::OperationId::DeletedSoftware,
                    Message::Ecu::VCM,
                    LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                        std::shared_ptr<OTAService::OTADeletedSoftwareRequestCallerData> p_data =
                            std::dynamic_pointer_cast<OTAService::OTADeletedSoftwareRequestCallerData>(p_caller_data);
                        EXPECT_NE(nullptr, p_data);
                        EXPECT_NE(nullptr, p_data->reply_);
                    })));

    ota_service_.DeletedSoftwareSetRequest(client, partList, reply);
}

TEST_F(OtaServiceFixture, TestDeletedSoftwareResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTADeletedSoftwareRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTADeletedSoftwareRequestCallerData>());

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
    };

    ota_service_.cbOTADeletedSoftwareResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(OtaServiceFixture, TestDeletedSoftwareResponseErrorStat)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTADeletedSoftwareRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTADeletedSoftwareRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    ota_service_.cbOTADeletedSoftwareResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}

/*
 * OTA CancelDownloadSetRequest
 */
void CapiCallbackCancelDownload(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                gen_conn::OTATypes::GenericOkStatus _status)
{
}

TEST_F(OtaServiceFixture, TestCancelDownloadSetRequest)
{
    const std::shared_ptr<CommonAPI::ClientId> client;
    gen_conn::OTAStub::CancelDownloadSetRequestReply_t reply = CapiCallbackCancelDownload;

    const gen_conn::OTATypes::UUID uuid1("uuid1");

    EXPECT_CALL(ip_service_,
                SendSetRequestCancelDownload(
                    _,
                    Icb_OpCancelDownload_SetRequest_Encode,
                    Icb_OpCancelDownload_SetRequest_EncodedSize,
                    VccIpCmd::ServiceId::OTA,
                    VccIpCmd::OperationId::CancelDownload,
                    Message::Ecu::VCM,
                    LambdaMatcher([reply](std::shared_ptr<MessageDispatcher::CallerData> p_caller_data) {
                        std::shared_ptr<OTAService::OTACancelDownloadRequestCallerData> p_data =
                            std::dynamic_pointer_cast<OTAService::OTACancelDownloadRequestCallerData>(p_caller_data);
                        EXPECT_NE(nullptr, p_data);
                        EXPECT_NE(nullptr, p_data->reply_);
                    })));

    ota_service_.CancelDownloadSetRequest(client, uuid1, reply);
}

TEST_F(OtaServiceFixture, TestCancelDownloadResponse)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTACancelDownloadRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTACancelDownloadRequestCallerData>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpCancelDownload_Response resp = Icb_OpCancelDownload_Response_Create(session);
    resp->status = e_Icb_GenericOkStatus_ok;

    EXPECT_CALL(ip_service_,
                DecodeResponseCancelDownload(
                    _, _, _, Icb_OpCancelDownload_Response_Create, Icb_OpCancelDownload_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                 gen_conn::OTATypes::GenericOkStatus _status) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, _error);
        ASSERT_EQ(gen_conn::OTATypes::GenericOkStatus::ok, _status);
    };

    ota_service_.cbOTACancelDownloadResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(OtaServiceFixture, TestCancelDownloadResponseErrorStat)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTACancelDownloadRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTACancelDownloadRequestCallerData>());
    p_caller_data->errorType = TransportServices::ErrorType::REMOTE_ERROR;

    EXPECT_CALL(ip_service_, DecodeResponseCancelDownload(_, _, _, _, _)).Times(0);

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                 gen_conn::OTATypes::GenericOkStatus _status) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::REMOTE_ERROR, _error);
    };

    ota_service_.cbOTACancelDownloadResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(OtaServiceFixture, TestCancelDownloadResponseDecodeError)
{
    const std::shared_ptr<CommonAPI::ClientId> client;

    Message msg;
    std::shared_ptr<OTAService::OTACancelDownloadRequestCallerData> p_caller_data(
        std::make_shared<OTAService::OTACancelDownloadRequestCallerData>());

    EXPECT_CALL(ip_service_,
                DecodeResponseCancelDownload(
                    _, _, _, Icb_OpCancelDownload_Response_Create, Icb_OpCancelDownload_Response_Decode))
        .WillOnce(Return(false));

    bool b_callback_called = false;
    p_caller_data->reply_ = [&b_callback_called](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                 gen_conn::OTATypes::GenericOkStatus _status) {
        b_callback_called = true;
        ASSERT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::DECODE_ERROR, _error);
    };

    ota_service_.cbOTACancelDownloadResponse(msg, p_caller_data);

    ASSERT_EQ(b_callback_called, true);
}
}
