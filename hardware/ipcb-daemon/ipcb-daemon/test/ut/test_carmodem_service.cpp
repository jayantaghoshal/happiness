/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <services/car_modem_service.h>
#include <services/car_modem_service_stub.h>
#include <services/icar_modem_service.h>
#include <memory>
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_car_modem_service_stub.h"
#include "mocks/mock_commonapi_runtime.hpp"
#include "mocks/mock_notify_system_ui.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::SetArgPointee;

namespace gen_conn = ::v0::org::volvo::connectivity;

namespace Connectivity
{
class CarModemServiceFixture : public ::testing::Test
{
public:
    CarModemServiceFixture() : carModemService_(std::weak_ptr<CarModemServiceStub>(p_car_modem_stub_))
    {
        p_car_modem_stub_->setListeners(&carModemService_, nullptr);
        g_mockNotifySystemUI = &mockNotifySysUI_;
    }

    ~CarModemServiceFixture()
    {
    }

    std::shared_ptr<NiceMock<MockCarModemStub>> p_car_modem_stub_ = std::make_shared<NiceMock<MockCarModemStub>>();
    CarModemService carModemService_;
    NiceMock<MockMessageDispatcher> messageDispatcherMock_;
    NiceMock<MockIpService> &ip_service_ = carModemService_;
    NiceMock<MockNotifySystemUI> mockNotifySysUI_;
};

MATCHER_P(LambdaMatcher, func, "")
{
    func(arg);
    return true;
}

TEST_F(CarModemServiceFixture, Initialize)
{
    EXPECT_CALL(ip_service_, RegisterClient(_)).Times(1).WillOnce(Return(false));
    EXPECT_CALL(ip_service_, RegisterServer(_)).Times(1).WillOnce(Return(false));
    EXPECT_EQ(carModemService_.Initialize(&messageDispatcherMock_), false);

    EXPECT_CALL(ip_service_, RegisterClient(_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(ip_service_, RegisterServer(_)).Times(1).WillOnce(Return(false));
    EXPECT_EQ(carModemService_.Initialize(&messageDispatcherMock_), false);

    EXPECT_CALL(ip_service_, RegisterClient(_)).Times(1).WillOnce(Return(false));
    EXPECT_CALL(ip_service_, RegisterServer(_)).Times(1).WillOnce(Return(true));
    EXPECT_EQ(carModemService_.Initialize(&messageDispatcherMock_), false);

    EXPECT_CALL(ip_service_, RegisterClient(_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(ip_service_, RegisterServer(_)).Times(1).WillOnce(Return(true));
    EXPECT_EQ(carModemService_.Initialize(&messageDispatcherMock_), true);

    carModemService_.Uninitialize();
}

TEST_F(CarModemServiceFixture, GetCarModemConnectionStatus)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors capture_error =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK;
    gen_conn::IpCommandBusTypes::CarModem_ConnectionStatus capture_outConnectionStatus;

    carModemService_.Initialize(&messageDispatcherMock_);

    EXPECT_CALL(
        carModemService_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMConnectionStatus, Message::Ecu::TEM, _))
        .Times(1);

    carModemService_.GetCarModemConnectionStatus(
        clientIdPtr,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::CarModem_ConnectionStatus _outConnectionStatus) {
            capture_error = _error;
            capture_outConnectionStatus = _outConnectionStatus;
        });
}

TEST_F(CarModemServiceFixture, GetCarModemRegistrationStatus)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors capture_error =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK;
    gen_conn::IpCommandBusTypes::CarModem_RegistrationStatus capture_outRegistrationStatus;

    carModemService_.Initialize(&messageDispatcherMock_);

    EXPECT_CALL(carModemService_,
                SendGenericRequest(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity,
                                   VccIpCmd::OperationId::PSIMRegisterState,
                                   Message::Ecu::TEM,
                                   _))
        .Times(1);
    carModemService_.GetCarModemRegistrationStatus(
        clientIdPtr,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::CarModem_RegistrationStatus _outRegistrationStatus) {
            capture_error = _error;
            capture_outRegistrationStatus = _outRegistrationStatus;
        });
}

TEST_F(CarModemServiceFixture, GetCarModemPSIMStatus)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors capture_error =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK;
    gen_conn::IpCommandBusTypes::CarModem_SIMCardStatus capture_outSIMCardStatus;

    carModemService_.Initialize(&messageDispatcherMock_);
    EXPECT_CALL(
        carModemService_,
        SendGenericRequest(
            VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity, VccIpCmd::OperationId::PSIMState, Message::Ecu::TEM, _))
        .Times(1);

    carModemService_.GetCarModemPSIMStatus(clientIdPtr,
                                           [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                               gen_conn::IpCommandBusTypes::CarModem_SIMCardStatus _outSIMCardStatus) {
                                               capture_error = _error;
                                               capture_outSIMCardStatus = _outSIMCardStatus;
                                           });
}

TEST_F(CarModemServiceFixture, GetCarModemPSIMDataCounter)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors capture_error =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK;
    gen_conn::IpCommandBusTypes::CarModem_SIMDataCounter capture_outSIMDataCounter;

    carModemService_.Initialize(&messageDispatcherMock_);
    EXPECT_CALL(carModemService_,
                SendGenericRequest(
                    VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMDataCounter, Message::Ecu::TEM, _))
        .Times(1);

    carModemService_.GetCarModemPSIMDataCounter(
        clientIdPtr,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::CarModem_SIMDataCounter _outSIMDataCounter) {
            capture_error = _error;
            capture_outSIMDataCounter = _outSIMDataCounter;
        });
}

TEST_F(CarModemServiceFixture, ResetCarModemPSIMDataCounter)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors capture_error =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK;
    gen_conn::IpCommandBusTypes::CarModem_ResetSIMDataCounter capture_outSIMResetDataCounter;

    carModemService_.Initialize(&messageDispatcherMock_);
    EXPECT_CALL(carModemService_,
                SendSetRequestOpPSIMResetDataCounter(_,
                                                     _,
                                                     _,
                                                     VccIpCmd::ServiceId::Connectivity,
                                                     VccIpCmd::OperationId::PSIMResetDataCounter,
                                                     Message::Ecu::TEM,
                                                     _))
        .Times(1);

    carModemService_.ResetCarModemPSIMDataCounter(
        clientIdPtr,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::CarModem_ResetSIMDataCounter _outSIMResetDataCounter) {
            capture_error = _error;
            capture_outSIMResetDataCounter = _outSIMResetDataCounter;
        });
}

TEST_F(CarModemServiceFixture, cbCarModemSignalStrengthChangedNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMSignalStrength_Notification notif = Icb_OpPSIMSignalStrength_Notification_Create(session);
    notif->rssi = 10;

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMSignalStrengthNotification(
                    _, _, Icb_OpPSIMSignalStrength_Notification_Create, Icb_OpPSIMSignalStrength_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notif), Return(true)));

    EXPECT_CALL(*p_car_modem_stub_, fireCarModemSignalStrengthChangedNotificationEvent(10));
    carModemService_.cbCarModemSignalStrengthChangedNotification(msg);

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMSignalStrengthNotification(
                    _, _, Icb_OpPSIMSignalStrength_Notification_Create, Icb_OpPSIMSignalStrength_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notif), Return(false)));

    EXPECT_CALL(*p_car_modem_stub_, fireCarModemSignalStrengthChangedNotificationEvent(_)).Times(0);
    carModemService_.cbCarModemSignalStrengthChangedNotification(msg);
}

TEST_F(CarModemServiceFixture, cbCarModemPSIMStateNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMState_Response notify = Icb_OpPSIMState_Response_Create(session);
    notify->exists__optional__simID = true;
    char simId[] = "2014234567889";
    ASN_OctetString_AssociateText(notify->simID, simId);
    notify->status->choice = e_Icb_SimStatus_readyForSettings;
    notify->status->pinRequired = e_Icb_PinTrials_unknown;
    notify->status->pinBlocked = e_Icb_PukTrials_unknown;

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMState(_, _, Icb_OpPSIMState_Response_Create, Icb_OpPSIMState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notify), Return(true)));

    EXPECT_CALL(
        *p_car_modem_stub_,
        fireCarModemPSIMStateNotificationEvent(LambdaMatcher(
            [](const ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_SIMCardStatus &_outSIMCardStatus) {
                EXPECT_EQ(gen_conn::IpCommandBusTypes::PSIMCardStatus::Literal::kReadyForSettings,
                          _outSIMCardStatus.getSimCardStatus());
                EXPECT_EQ("2014234567889", _outSIMCardStatus.getImeiNumber());
                EXPECT_EQ(gen_conn::IpCommandBusTypes::PSIMRemainingPINTrials::Literal::kUnknown,
                          _outSIMCardStatus.getRemainingPINTrials());
                EXPECT_EQ(gen_conn::IpCommandBusTypes::PSIMRemainingPUKTrials::Literal::kUnknown,
                          _outSIMCardStatus.getRemainingPUKTrials());
            })));
    carModemService_.cbCarModemPSIMStateNotification(msg);

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMState(_, _, Icb_OpPSIMState_Response_Create, Icb_OpPSIMState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notify), Return(false)));

    EXPECT_CALL(*p_car_modem_stub_, fireCarModemPSIMStateNotificationEvent(_)).Times(0);
    carModemService_.cbCarModemPSIMStateNotification(msg);
}

TEST_F(CarModemServiceFixture, cbCarModemConnectionStatusNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMConnectionStatus_Notification notify = Icb_OpPSIMConnectionStatus_Notification_Create(session);
    notify->status = e_Icb_PSIMConnectionStatus_connectedHome;
    notify->technology = e_Icb_WirelessTechnology_edge;

    EXPECT_CALL(
        ip_service_,
        DecodeMessagePSIMConnectionStatusNotification(
            _, _, Icb_OpPSIMConnectionStatus_Notification_Create, Icb_OpPSIMConnectionStatus_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notify), Return(true)));

    EXPECT_CALL(
        *p_car_modem_stub_,
        fireCarModemConnectionStatusNotificationEvent(LambdaMatcher([](
            const ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_ConnectionStatus &_outConnectionStatus) {
            EXPECT_EQ(gen_conn::IpCommandBusTypes::PSIMConnectionStatus::Literal::kConnectedHome,
                      _outConnectionStatus.getConnectionStatus());
            EXPECT_EQ(gen_conn::IpCommandBusTypes::WirelessTechnology::Literal::kEdge,
                      _outConnectionStatus.getWirelessTechnology());
        })));

    carModemService_.cbCarModemConnectionStatusNotification(msg);

    EXPECT_CALL(
        ip_service_,
        DecodeMessagePSIMConnectionStatusNotification(
            _, _, Icb_OpPSIMConnectionStatus_Notification_Create, Icb_OpPSIMConnectionStatus_Notification_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notify), Return(false)));

    EXPECT_CALL(*p_car_modem_stub_, fireCarModemConnectionStatusNotificationEvent(_)).Times(0);

    carModemService_.cbCarModemConnectionStatusNotification(msg);
}

TEST_F(CarModemServiceFixture, cbCarModemPSIMStatusResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMStatusRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMStatusRespCallback>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMState_Response resp = Icb_OpPSIMState_Response_Create(session);
    resp->exists__optional__simID = true;
    char simId[] = "2014234567889";
    ASN_OctetString_AssociateText(resp->simID, simId);
    resp->status->choice = e_Icb_SimStatus_readyForSettings;
    resp->status->pinRequired = e_Icb_PinTrials_unknown;
    resp->status->pinBlocked = e_Icb_PukTrials_unknown;

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMState(_, _, _, Icb_OpPSIMState_Response_Create, Icb_OpPSIMState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_SimStatus_t _outSimStatus,
        Icb_IMSINumber _outSimId,
        std::shared_ptr<CarModemService::CarModemPSIMStatusRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
        ASSERT_EQ(_outSimStatus.choice, e_Icb_SimStatus_readyForSettings);
        ASSERT_EQ(_outSimStatus.pinRequired, e_Icb_PinTrials_unknown);
        ASSERT_EQ(_outSimStatus.pinBlocked, e_Icb_PukTrials_unknown);
    };

    carModemService_.cbCarModemPSIMStatusResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMState(_, _, _, Icb_OpPSIMState_Response_Create, Icb_OpPSIMState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(false)));

    b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_SimStatus_t _outSimStatus,
        Icb_IMSINumber _outSimId,
        std::shared_ptr<CarModemService::CarModemPSIMStatusRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
    };

    carModemService_.cbCarModemPSIMStatusResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, cbCarModemConnectionStatusResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemConnectionStatusRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemConnectionStatusRespCallback>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMConnectionStatus_Response resp = Icb_OpPSIMConnectionStatus_Response_Create(session);
    resp->status = e_Icb_PSIMConnectionStatus_connectedHome;
    resp->technology = e_Icb_WirelessTechnology_edge;

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMConnectionStatus(
                    _, _, _, Icb_OpPSIMConnectionStatus_Response_Create, Icb_OpPSIMConnectionStatus_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_PSIMConnectionStatus_t _outConnectionStatus,
        Icb_WirelessTechnology_t _outWirelessTechnology,
        std::shared_ptr<CarModemService::CarModemConnectionStatusRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
        ASSERT_EQ(_outConnectionStatus, e_Icb_PSIMConnectionStatus_connectedHome);
        ASSERT_EQ(_outWirelessTechnology, e_Icb_WirelessTechnology_edge);
    };

    carModemService_.cbCarModemConnectionStatusResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMConnectionStatus(
                    _, _, _, Icb_OpPSIMConnectionStatus_Response_Create, Icb_OpPSIMConnectionStatus_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(false)));

    b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_PSIMConnectionStatus_t _outConnectionStatus,
        Icb_WirelessTechnology_t _outWirelessTechnology,
        std::shared_ptr<CarModemService::CarModemConnectionStatusRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
    };

    carModemService_.cbCarModemConnectionStatusResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, cbCarModemPSIMDataCounterResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMDataCounterRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMDataCounterRespCallback>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMDataCounter_Response resp = Icb_OpPSIMDataCounter_Response_Create(session);
    Icb_DateTime datetime = Icb_DateTime_Create(session);
    resp->byteCount = 55U;
    resp->counter = 0;
    datetime->year = 2010U;
    datetime->month = 9U;
    datetime->day = 8U;
    datetime->hour = 7U;
    datetime->minute = 6U;
    datetime->second = 5U;
    resp->dateTime = datetime;

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMDataCounter(
                    _, _, _, Icb_OpPSIMDataCounter_Response_Create, Icb_OpPSIMDataCounter_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_OpPSIMDataCounter_Response_t _outPSIMDataCounter,
        std::shared_ptr<CarModemService::CarModemPSIMDataCounterRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
        ASSERT_EQ(_outPSIMDataCounter.byteCount, 55U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->year, 2010U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->month, 9U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->day, 8U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->hour, 7U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->minute, 6U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->second, 5U);
    };

    carModemService_.cbCarModemPSIMDataCounterResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMDataCounter(
                    _, _, _, Icb_OpPSIMDataCounter_Response_Create, Icb_OpPSIMDataCounter_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(false)));

    b_callback_called = false;

    p_caller_data->callback = [&b_callback_called](
        Icb_OpPSIMDataCounter_Response_t _outPSIMDataCounter,
        std::shared_ptr<CarModemService::CarModemPSIMDataCounterRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
        ASSERT_EQ(_outPSIMDataCounter.byteCount, 0U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->year, 0U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->month, 0U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->day, 0U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->hour, 0U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->minute, 0U);
        ASSERT_EQ(_outPSIMDataCounter.dateTime->second, 0U);
    };

    carModemService_.cbCarModemPSIMDataCounterResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, cbCarModemPSIMResetDataCounterResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMResetDataCounterRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMResetDataCounterRespCallback>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMResetDataCounter_Response resp = Icb_OpPSIMResetDataCounter_Response_Create(session);
    resp->byteCount = 55U;
    resp->counter = 0;

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMResetDataCounter(
                    _, _, _, Icb_OpPSIMResetDataCounter_Response_Create, Icb_OpPSIMResetDataCounter_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_OpPSIMResetDataCounter_Response_t _outPSIMResetDataCounter,
        std::shared_ptr<CarModemService::CarModemPSIMResetDataCounterRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
        ASSERT_EQ(_outPSIMResetDataCounter.byteCount, 55U);
    };

    carModemService_.cbCarModemPSIMResetDataCounterResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, OnGetCarModemPSIMDataCounter)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMDataCounterRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMDataCounterRespCallback>());

    bool b_callback_called = false;
    p_caller_data->errorType = Connectivity::TransportServices::ErrorType::OK;

    gen_conn::CarModemStub::GetCarModemPSIMDataCounterReply_t callback = [&b_callback_called](
        ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_SIMDataCounter _outPSIMDataCounter) {
        b_callback_called = true;
    };

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_DateTime datetime = Icb_DateTime_Create(session);
    Icb_OpPSIMDataCounter_Response_t resp = {0, 0, datetime};

    carModemService_.OnGetCarModemPSIMDataCounter(resp, p_caller_data, callback);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, OnGetCarModemPSIMResetDataCounter)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMResetDataCounterRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMResetDataCounterRespCallback>());

    bool b_callback_called = false;
    p_caller_data->errorType = Connectivity::TransportServices::ErrorType::OK;

    gen_conn::CarModemStub::ResetCarModemPSIMDataCounterReply_t callback = [&b_callback_called](
        ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_ResetSIMDataCounter _outPSIMResetDataCounter) {
        b_callback_called = true;
    };

    Icb_OpPSIMResetDataCounter_Response_t resp = {0, 0};

    carModemService_.OnGetCarModemPSIMResetDataCounter(resp, p_caller_data, callback);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, OnGetCarModemConnectStatusResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemConnectionStatusRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemConnectionStatusRespCallback>());

    bool b_callback_called = false;
    p_caller_data->errorType = Connectivity::TransportServices::ErrorType::OK;

    gen_conn::CarModemStub::GetCarModemConnectionStatusReply_t callback = [&b_callback_called](
        ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_ConnectionStatus _outConnectionStatus) {
        b_callback_called = true;
    };

    carModemService_.OnGetCarModemConnectStatusResponse(
        e_Icb_PSIMConnectionStatus_connectedHome, e_Icb_WirelessTechnology_edge, p_caller_data, callback);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, OnGetCarModemPSIMStatusResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMStatusRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMStatusRespCallback>());

    bool b_callback_called = false;
    p_caller_data->errorType = Connectivity::TransportServices::ErrorType::OK;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    ASN_OctetString str_ptr = ASN_OctetString_Create(session);
    ASN_OctetString_SetText(str_ptr, session, const_cast<char *>("123456789"));

    Icb_SimStatus_t _outPSIMStatus;

    Icb_IMSINumber _outSimId = str_ptr;

    gen_conn::CarModemStub::GetCarModemPSIMStatusReply_t callback = [&b_callback_called](
        ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_SIMCardStatus _outSIMCardStatus) {
        b_callback_called = true;
    };

    carModemService_.OnGetCarModemPSIMStatusResponse(_outPSIMStatus, _outSimId, p_caller_data, callback);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, OnGetCarModemRegistrationStatus)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemRegistrationStatusRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemRegistrationStatusRespCallback>());

    bool b_callback_called = false;
    p_caller_data->errorType = Connectivity::TransportServices::ErrorType::OK;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMRegisterState_Response resp = Icb_OpPSIMRegisterState_Response_Create(session);
    resp->registerState = Icb_RegisterState_Create(session);
    resp->registerState->choice = e_Icb_RegisterState_registered;
    resp->registerState->registered = Icb_NetworkOperator_Create(session);
    ASN_OctetString_SetText(resp->registerState->registered->name, session, const_cast<char *>("Telenor"));

    gen_conn::CarModemStub::GetCarModemRegistrationStatusReply_t callback = [&b_callback_called](
        ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
        ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_RegistrationStatus _outRegistrationStatus) {
        b_callback_called = true;
    };

    carModemService_.OnGetCarModemRegistrationStatus(resp->registerState, p_caller_data, callback);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, cbCarModemRegistrationStatusNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMRegisterState_Response notify = Icb_OpPSIMRegisterState_Response_Create(session);
    notify->registerState = Icb_RegisterState_Create(session);
    notify->registerState->choice = e_Icb_RegisterState_registered;
    notify->registerState->registered = Icb_NetworkOperator_Create(session);
    ASN_OctetString_SetText(notify->registerState->registered->name, session, const_cast<char *>("Telenor"));

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMRegisterState(
                    _, _, Icb_OpPSIMRegisterState_Response_Create, Icb_OpPSIMRegisterState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notify), Return(true)));

    EXPECT_CALL(*p_car_modem_stub_,
                fireCarModemNetworkRegistrationStatusNotificationEvent(LambdaMatcher(
                    [](const ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_RegistrationStatus
                           &_outRegistrationStatus) {
                        EXPECT_EQ(gen_conn::IpCommandBusTypes::NetworkRegistrationStatus::Literal::kRegistered,
                                  _outRegistrationStatus.getRegistrationStatus());
                        EXPECT_EQ("Telenor", _outRegistrationStatus.getOperatorName());
                    })));
    carModemService_.cbCarModemRegistrationStatusNotification(msg);

    notify->registerState->choice = e_Icb_RegisterState_regRoaming;
    notify->registerState->regRoaming = Icb_NetworkOperator_Create(session);
    ASN_OctetString_SetText(notify->registerState->regRoaming->name, session, const_cast<char *>("Telenor"));

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMRegisterState(
                    _, _, Icb_OpPSIMRegisterState_Response_Create, Icb_OpPSIMRegisterState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notify), Return(true)));

    EXPECT_CALL(*p_car_modem_stub_,
                fireCarModemNetworkRegistrationStatusNotificationEvent(LambdaMatcher(
                    [](const ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_RegistrationStatus
                           &_outRegistrationStatus) {
                        EXPECT_EQ(gen_conn::IpCommandBusTypes::NetworkRegistrationStatus::Literal::kRegistered_Roaming,
                                  _outRegistrationStatus.getRegistrationStatus());
                        EXPECT_EQ("Telenor", _outRegistrationStatus.getOperatorName());
                    })));
    carModemService_.cbCarModemRegistrationStatusNotification(msg);

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMRegisterState(
                    _, _, Icb_OpPSIMRegisterState_Response_Create, Icb_OpPSIMRegisterState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notify), Return(false)));

    EXPECT_CALL(*p_car_modem_stub_, fireCarModemNetworkRegistrationStatusNotificationEvent(_)).Times(0);
    carModemService_.cbCarModemRegistrationStatusNotification(msg);
}

TEST_F(CarModemServiceFixture, cbCarModemRegistrationStatusResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemRegistrationStatusRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemRegistrationStatusRespCallback>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMRegisterState_Response resp = Icb_OpPSIMRegisterState_Response_Create(session);
    resp->registerState = Icb_RegisterState_Create(session);
    resp->registerState->choice = e_Icb_RegisterState_registered;
    resp->registerState->registered = Icb_NetworkOperator_Create(session);
    ASN_OctetString_SetText(resp->registerState->registered->name, session, const_cast<char *>("Telenor"));

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMRegisterState(
                    _, _, _, Icb_OpPSIMRegisterState_Response_Create, Icb_OpPSIMRegisterState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_RegisterState registrationState,
        std::shared_ptr<CarModemService::CarModemRegistrationStatusRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
        ASSERT_EQ(registrationState->choice, e_Icb_RegisterState_registered);
    };

    carModemService_.cbCarModemRegistrationStatusResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);

    resp->registerState->choice = e_Icb_RegisterState_regRoaming;
    resp->registerState->regRoaming = Icb_NetworkOperator_Create(session);
    ASN_OctetString_SetText(resp->registerState->regRoaming->name, session, const_cast<char *>("Telenor"));

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMRegisterState(
                    _, _, _, Icb_OpPSIMRegisterState_Response_Create, Icb_OpPSIMRegisterState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_RegisterState registrationState,
        std::shared_ptr<CarModemService::CarModemRegistrationStatusRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
        ASSERT_EQ(registrationState->choice, e_Icb_RegisterState_regRoaming);
    };
    carModemService_.cbCarModemRegistrationStatusResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMRegisterState(
                    _, _, _, Icb_OpPSIMRegisterState_Response_Create, Icb_OpPSIMRegisterState_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(false)));

    b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_RegisterState registrationState,
        std::shared_ptr<CarModemService::CarModemRegistrationStatusRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
    };

    carModemService_.cbCarModemRegistrationStatusResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, EnterCarModemPSIMCode)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors capture_error =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK;

    carModemService_.Initialize(&messageDispatcherMock_);

    gen_conn::IpCommandBusTypes::PinPukCommand_data data = std::string("1234");
    gen_conn::IpCommandBusTypes::CarModem_PinPukCommand command(
        gen_conn::IpCommandBusTypes::PinPukCommand_Choice::kEnterPin, data);

    EXPECT_CALL(carModemService_,
                SendSetRequestOpPSIMEnterCode(_,
                                              _,
                                              _,
                                              VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity,
                                              VccIpCmd::OperationId::PSIMEnterCode,
                                              Message::Ecu::TEM,
                                              _))
        .Times(1);

    carModemService_.EnterCarModemPSIMCode(
        clientIdPtr, command, [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error) {
            capture_error = _error;
        });
}

TEST_F(CarModemServiceFixture, cbCarModemPSIMEnterCodeResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMEnterCodeRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMEnterCodeRespCallback>());

    bool b_callback_called = false;
    p_caller_data->callback =
        [&b_callback_called](std::shared_ptr<CarModemService::CarModemPSIMEnterCodeRespCallback> ipcb_callback_obj) {
            b_callback_called = true;
        };

    carModemService_.cbCarModemPSIMEnterCodeResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, OnEnterCarModemPSIMCode)
{
    std::shared_ptr<CarModemService::CarModemPSIMEnterCodeRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMEnterCodeRespCallback>());

    bool b_callback_called = false;
    p_caller_data->errorType = Connectivity::TransportServices::ErrorType::OK;

    gen_conn::CarModemStub::EnterCarModemPSIMCodeReply_t callback =
        [&b_callback_called](::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Errors _error) {
            b_callback_called = true;
        };

    carModemService_.OnEnterCarModemPSIMCode(p_caller_data, callback);

    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, GetSetCarModemRoamingConfig)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;

    EXPECT_CALL(
        carModemService_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMRoamingConfiguration, Message::Ecu::TEM, _))
        .Times(1);
    carModemService_.GetCarModemRoamingConfig(
        clientIdPtr, [](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error, bool _roamingStatus) {});

    Icb_OpPSIMRoamingConfiguration_SetRequest_t setRequest;

    EXPECT_CALL(carModemService_,
                SendSetRequestOpPSIMRoamingConfiguration(_,
                                                         _,
                                                         _,
                                                         VccIpCmd::ServiceId::Connectivity,
                                                         VccIpCmd::OperationId::PSIMRoamingConfiguration,
                                                         Message::Ecu::TEM,
                                                         _))
        .WillOnce(SaveArgPointee<0>(&setRequest));
    carModemService_.SetCarModemRoamingConfig(
        clientIdPtr, true, [](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error, bool _roamingStatus) {});
    EXPECT_TRUE(setRequest.roamingAllowed);
}

TEST_F(CarModemServiceFixture, cbCarModemPSIMRoamingConfigurationResponse)
{
    bool called = false;
    bool resp_capture = false;

    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMRoamingConfigurationRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMRoamingConfigurationRespCallback>());
    p_caller_data->callback = [&](Icb_OpPSIMRoamingConfiguration_Response_t response,
                                  std::shared_ptr<CarModemService::CarModemPSIMRoamingConfigurationRespCallback>) {
        called = true;
        resp_capture = response.roamingAllowed;
    };

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMRoamingConfiguration_Response resp = Icb_OpPSIMRoamingConfiguration_Response_Create(session);
    resp->roamingAllowed = true;

    EXPECT_CALL(
        carModemService_,
        DecodeResponsePSIMRoamingConfiguration(
            _, _, _, Icb_OpPSIMRoamingConfiguration_Response_Create, Icb_OpPSIMRoamingConfiguration_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    carModemService_.cbCarModemPSIMRoamingConfigurationResponse(msg, p_caller_data);
    EXPECT_TRUE(called);
    EXPECT_TRUE(resp_capture);

    called = false;
    resp_capture = false;
    EXPECT_CALL(
        carModemService_,
        DecodeResponsePSIMRoamingConfiguration(
            _, _, _, Icb_OpPSIMRoamingConfiguration_Response_Create, Icb_OpPSIMRoamingConfiguration_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(false)));

    carModemService_.cbCarModemPSIMRoamingConfigurationResponse(msg, p_caller_data);
    EXPECT_TRUE(called);
    EXPECT_FALSE(resp_capture);
}

TEST_F(CarModemServiceFixture, OnCarModemRoamingConfigurationResponse)
{
    Icb_OpPSIMRoamingConfiguration_Response_t response{true};
    std::shared_ptr<CarModemService::CarModemPSIMRoamingConfigurationRespCallback> ipcb_callback_obj =
        std::make_shared<CarModemService::CarModemPSIMRoamingConfigurationRespCallback>();

    bool called = false;
    bool captured_status = false;
    carModemService_.OnCarModemRoamingConfigurationResponse(
        response,
        ipcb_callback_obj,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error, bool _roamingStatus) {
            captured_status = _roamingStatus;
            called = true;
        });
    EXPECT_TRUE(called);
    EXPECT_TRUE(captured_status);
}

TEST_F(CarModemServiceFixture, cbCarModemPSIMMessageSetRequest)
{
    const std::string kNotifyID = "00851";
    Message msg;
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMMessage_SetRequest sr = Icb_OpPSIMMessage_SetRequest_Create(session);

    std::string sender = "";
    std::string number = "1234";
    std::string body = "MessageBody";

    // Notification succeeds
    ASN_UTF8String_SetText(sr->body, session, const_cast<char *>(body.c_str()));
    ASN_UTF8String_SetText(sr->number, session, const_cast<char *>(number.c_str()));
    ASN_UTF8String_SetText(sr->sender, session, const_cast<char *>(sender.c_str()));

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMMessageSetRequest(
                    _, _, Icb_OpPSIMMessage_SetRequest_Create, Icb_OpPSIMMessage_SetRequest_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(sr), Return(true)));

    EXPECT_CALL(mockNotifySysUI_, NotifySMS(kNotifyID, sender, number, body)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(ip_service_,
                SendGenericResponse(
                    VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMMessage, Message::Ecu::TEM, _, _))
        .Times(1);
    carModemService_.cbCarModemPSIMMessageSetRequest(msg);

    // Decoding fail
    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMMessageSetRequest(
                    _, _, Icb_OpPSIMMessage_SetRequest_Create, Icb_OpPSIMMessage_SetRequest_Decode))
        .WillOnce(Return(false));

    EXPECT_CALL(mockNotifySysUI_, NotifySMS(_, _, _, _)).Times(0);
    EXPECT_CALL(ip_service_,
                SendGenericError(
                    _, VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMMessage, Message::Ecu::TEM, _))
        .Times(1);
    carModemService_.cbCarModemPSIMMessageSetRequest(msg);

    // Notification fail
    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMMessageSetRequest(
                    _, _, Icb_OpPSIMMessage_SetRequest_Create, Icb_OpPSIMMessage_SetRequest_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(sr), Return(true)));

    EXPECT_CALL(mockNotifySysUI_, NotifySMS(kNotifyID, sender, number, body)).Times(1).WillOnce(Return(false));
    EXPECT_CALL(ip_service_,
                SendGenericError(
                    _, VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::PSIMMessage, Message::Ecu::TEM, _))
        .Times(1);
    carModemService_.cbCarModemPSIMMessageSetRequest(msg);
}

TEST_F(CarModemServiceFixture, GetSetCarModemPSIMSecurity)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;

    EXPECT_CALL(carModemService_,
                SendGenericRequest(VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity,
                                   VccIpCmd::OperationId::PSIMSecurity,
                                   Message::Ecu::TEM,
                                   _))
        .Times(1);
    carModemService_.GetCarModemPSIMSecurity(
        clientIdPtr,
        [](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
           gen_conn::IpCommandBusTypes::CarModem_SIMSecuritySetting _outsetting) {});

    Icb_OpPSIMSecurity_SetRequest_t setRequest;

    EXPECT_CALL(carModemService_,
                SendSetRequestOpPSIMSecurity(_,
                                             _,
                                             _,
                                             VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity,
                                             VccIpCmd::OperationId::PSIMSecurity,
                                             Message::Ecu::TEM,
                                             _))
        .WillOnce(SaveArgPointee<0>(&setRequest));
    carModemService_.SetCarModemPSIMSecurity(
        clientIdPtr,
        "0000",
        gen_conn::IpCommandBusTypes::CarModem_SIMSecuritySetting::Literal::kOn,
        [](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
           gen_conn::IpCommandBusTypes::CarModem_SIMSecuritySetting _outsetting) {});
    EXPECT_EQ(setRequest.setting, e_Icb_SimSecuritySetting_on);
}

TEST_F(CarModemServiceFixture, cbCarModemPSIMSecurityResponse)
{
    bool called = false;
    Icb_SimSecuritySetting resp_capture = e_Icb_SimSecuritySetting_auto;

    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMSecurityRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMSecurityRespCallback>());
    p_caller_data->callback = [&](Icb_SimSecuritySetting response,
                                  std::shared_ptr<CarModemService::CarModemPSIMSecurityRespCallback>) {
        called = true;
        resp_capture = response;
    };

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMSecurity_Response resp = Icb_OpPSIMSecurity_Response_Create(session);
    resp->setting = e_Icb_SimSecuritySetting_off;

    EXPECT_CALL(
        carModemService_,
        DecodeResponsePSIMSecurity(_, _, _, Icb_OpPSIMSecurity_Response_Create, Icb_OpPSIMSecurity_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    carModemService_.cbCarModemPSIMSecurityResponse(msg, p_caller_data);
    EXPECT_TRUE(called);
    EXPECT_EQ(resp_capture, e_Icb_SimSecuritySetting_off);

    called = false;
    resp_capture = e_Icb_SimSecuritySetting_auto;
    EXPECT_CALL(
        carModemService_,
        DecodeResponsePSIMSecurity(_, _, _, Icb_OpPSIMSecurity_Response_Create, Icb_OpPSIMSecurity_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(false)));

    carModemService_.cbCarModemPSIMSecurityResponse(msg, p_caller_data);
    EXPECT_TRUE(called);
    EXPECT_EQ(resp_capture, e_Icb_SimSecuritySetting_on);  // Dummy value used when decoding fails...
}

TEST_F(CarModemServiceFixture, OnCarModemPSIMSecurityResponse)
{
    Icb_SimSecuritySetting response{e_Icb_SimSecuritySetting_on};
    std::shared_ptr<CarModemService::CarModemPSIMSecurityRespCallback> ipcb_callback_obj =
        std::make_shared<CarModemService::CarModemPSIMSecurityRespCallback>();

    bool called = false;
    gen_conn::IpCommandBusTypes::CarModem_SIMSecuritySetting captured_status =
        gen_conn::IpCommandBusTypes::CarModem_SIMSecuritySetting::Literal::kOff;
    carModemService_.OnCarModemPSIMSecurityResponse(
        response,
        ipcb_callback_obj,
        [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
            gen_conn::IpCommandBusTypes::CarModem_SIMSecuritySetting _outsetting) {
            captured_status = _outsetting;
            called = true;
        });
    EXPECT_TRUE(called);
    EXPECT_EQ(captured_status, gen_conn::IpCommandBusTypes::CarModem_SIMSecuritySetting::Literal::kOn);
}

TEST_F(CarModemServiceFixture, SendCarModemUssdRequest)
{
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;

    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors capture_error =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK;

    carModemService_.Initialize(&messageDispatcherMock_);
    EXPECT_CALL(carModemService_,
                SendSetRequestOpPSIMUssd(_,
                                         _,
                                         _,
                                         VccIpCmd::ServiceId::CommonPSIMPhoneConnectivity,
                                         VccIpCmd::OperationId::PSIMUssd,
                                         Message::Ecu::TEM,
                                         _))
        .Times(1);

    carModemService_.SendCarModemUssdRequest(clientIdPtr,
                                             "*111#",
                                             "eng",
                                             [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                 gen_conn::CarModem::CarModem_UssdStatus ussdStatus,
                                                 std::string ussdReturnCode) { capture_error = _error; });
}

TEST_F(CarModemServiceFixture, OnSendCarModemUssdResponse)
{
    std::shared_ptr<CarModemService::CarModemPSIMUssdRespCallback> ipcb_callback_obj =
        std::make_shared<CarModemService::CarModemPSIMUssdRespCallback>();

    Icb_UssdStatus ussdStatus = e_Icb_UssdStatus_successful;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
    Icb_UssdReturnCode ussdReturnCode = Icb_UssdReturnCode_Create(session);
    ASN_OctetString_SetText(ussdReturnCode->ussdReturnCode, session, const_cast<char *>("bla bla bla"));

    bool called = false;

    carModemService_.onSendCarModemUssdResponse(ussdStatus,
                                                ussdReturnCode,
                                                ipcb_callback_obj,
                                                [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors _error,
                                                    gen_conn::CarModem::CarModem_UssdStatus _ussdStatus,
                                                    std::string _ussdReturnCode) {
                                                    called = true;
                                                    EXPECT_EQ(e_Icb_UssdStatus_successful, _ussdStatus);
                                                    EXPECT_EQ("bla bla bla", _ussdReturnCode);
                                                });
    EXPECT_TRUE(called);
}

TEST_F(CarModemServiceFixture, cbCarModemUssdResponse)
{
    Message msg;
    std::shared_ptr<CarModemService::CarModemPSIMUssdRespCallback> p_caller_data(
        std::make_shared<CarModemService::CarModemPSIMUssdRespCallback>());

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMUssd_Response resp = Icb_OpPSIMUssd_Response_Create(session);
    resp->ussdStatus = e_Icb_UssdStatus_successful;
    resp->ussd = Icb_UssdReturnCode_Create(session);
    ASN_OctetString_SetText(resp->ussd->ussdReturnCode, session, const_cast<char *>("bla bla bla"));

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMUssd(_, _, _, Icb_OpPSIMUssd_Response_Create, Icb_OpPSIMUssd_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(true)));

    bool b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_UssdStatus _status,
        Icb_UssdReturnCode _outPSIMUssd,
        std::shared_ptr<CarModemService::CarModemPSIMUssdRespCallback> ipcb_callback_obj) { b_callback_called = true; };

    carModemService_.cbCarModemUssdResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);

    EXPECT_CALL(ip_service_,
                DecodeResponsePSIMUssd(_, _, _, Icb_OpPSIMUssd_Response_Create, Icb_OpPSIMUssd_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(resp), Return(false)));

    b_callback_called = false;
    p_caller_data->callback = [&b_callback_called](
        Icb_UssdStatus _status,
        Icb_UssdReturnCode _outPSIMUssd,
        std::shared_ptr<CarModemService::CarModemPSIMUssdRespCallback> ipcb_callback_obj) {
        b_callback_called = true;
        EXPECT_EQ(e_Icb_UssdStatus_timeout, _status);
    };

    carModemService_.cbCarModemUssdResponse(msg, p_caller_data);
    ASSERT_EQ(b_callback_called, true);
}

TEST_F(CarModemServiceFixture, cbCarModemUssdNotification)
{
    Message msg;

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));

    Icb_OpPSIMUssd_Response resp = Icb_OpPSIMUssd_Response_Create(session);
    // resp->ussdStatus = ;
    resp->ussd = Icb_UssdReturnCode_Create(session);
    ASN_OctetString_SetText(resp->ussd->ussdReturnCode, session, const_cast<char *>("bla bla bla"));

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMUssd(_, _, Icb_OpPSIMUssd_Response_Create, Icb_OpPSIMUssd_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(resp), Return(true)));

    EXPECT_CALL(*p_car_modem_stub_, fireCarModemUssdNotificationEvent(_, _)).Times(1);
    carModemService_.cbCarModemUssdNotification(msg);

    EXPECT_CALL(ip_service_,
                DecodeMessagePSIMUssd(_, _, Icb_OpPSIMUssd_Response_Create, Icb_OpPSIMUssd_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(resp), Return(false)));

    EXPECT_CALL(*p_car_modem_stub_, fireCarModemUssdNotificationEvent(_, _)).Times(0);
    carModemService_.cbCarModemUssdNotification(msg);
}
}  // namespace
