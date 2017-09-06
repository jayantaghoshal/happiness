/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <services/car_modem_apn_service.h>
#include <services/car_modem_service_stub.h>
#include <services/icar_modem_service.h>
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_car_modem_service_stub.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::SetArgPointee;

namespace gen_conn = ::v0::org::volvo::connectivity;

namespace Connectivity
{
class CarModemApnServiceFixture : public ::testing::Test
{
public:
    CarModemApnServiceFixture() : carModemApnService_(p_car_modem_stub_)
    {
        p_car_modem_stub_->initStubAdapter(stubAdapter);
        p_car_modem_stub_->setListeners(nullptr, &carModemApnService_);
        carModemApnService_.Initialize(&messageDispatcherMock_);
    }

    ~CarModemApnServiceFixture()
    {
        carModemApnService_.Uninitialize();
    }

    std::shared_ptr<NiceMock<MockCarModemStub>> p_car_modem_stub_ = std::make_shared<NiceMock<MockCarModemStub>>();
    std::shared_ptr<NiceMock<MockCarModemServiceStubAdapter>> stubAdapter =
        std::make_shared<NiceMock<MockCarModemServiceStubAdapter>>();
    CarModemApnService carModemApnService_;
    NiceMock<MockMessageDispatcher> messageDispatcherMock_;
    NiceMock<MockIpService>& ip_service_ = carModemApnService_;
    std::shared_ptr<CommonAPI::ClientId> clientIdPtr;
};

ACTION_P(ReplyWithCorrectApnConfiguration, messageResponseCb)
{
    messageResponseCb(arg6);
}

ACTION(ShortcutReplyWithCorrectApnConfiguration)
{
    arg6->callback(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, nullptr);
}

TEST_F(CarModemApnServiceFixture, RequestCarModemApnConfigurationForwardedFromCommonApiToIPCB)
{
    gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors capture_error =
        gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::TIMEOUT;

    const std::string expectedApnName = "rjlsfdfdsljkfsfd fsd dl";
    const std::string expectedUserName = "dsflkmßðklfmsdkläö åö€ シャンハイd	ذ 	خ bbb";
    const std::string expectedPassword = "dsflmdlksfmlldfs sdf#¤";

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
    Message msgDummy_TheDecodeMessageMockWillReturnValidData;
    Icb_OpPSIMNetworkConfiguration_Response notify = Icb_OpPSIMNetworkConfiguration_Response_Create(session);
    notify->selection->autoFailed = Icb_AutomaticAPNConfiguration_Create(session);
    notify->selection->choice = Icb_APNSelection_Choices::e_Icb_APNSelection_autoFailed;
    notify->selection->autoFailed->exists__optional__configuration = true;

    notify->selection->autoFailed->configuration->apnName =
        InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, expectedApnName);
    notify->selection->autoFailed->configuration->username =
        InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, expectedUserName);
    notify->selection->autoFailed->configuration->password =
        InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, expectedPassword);

    EXPECT_CALL(
        ip_service_,
        DecodeResponsePSIMNetworkConfiguration(
            _, _, _, Icb_OpPSIMNetworkConfiguration_Response_Create, Icb_OpPSIMNetworkConfiguration_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<2>(notify), Return(true)));

    std::function<void(std::shared_ptr<MessageDispatcher::CallerData> pCallerData)> hmm =
        [&](std::shared_ptr<MessageDispatcher::CallerData> pCallerData) {
            carModemApnService_.cbCarModemPSIMNetworkConfigurationResponse(
                msgDummy_TheDecodeMessageMockWillReturnValidData, pCallerData);
        };

    EXPECT_CALL(carModemApnService_,
                SendSetRequestOpPSIMPSIMNetworkConfiguration(_,
                                                             _,
                                                             _,
                                                             VccIpCmd::ServiceId::Connectivity,
                                                             VccIpCmd::OperationId::PSIMNetworkConfiguration,
                                                             Message::Ecu::TEM,
                                                             _))
        .WillOnce(ReplyWithCorrectApnConfiguration(hmm));

    gen_conn::IpCommandBusTypes::CarModem_ApnConfiguration _requestedApnConfiguration{
        gen_conn::IpCommandBusTypes::CarModem_ApnSelection::Manual, expectedApnName, expectedUserName, expectedPassword,
    };

    const auto reply = [&](gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors e) { capture_error = e; };

    p_car_modem_stub_->RequestCarModemApnConfiguration(clientIdPtr, _requestedApnConfiguration, reply);
    EXPECT_EQ(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Errors::OK, capture_error);
}

MATCHER_P5(CarModem_ApnConfigurationMatcher,
           expectedExistsOptional,
           expectedApnSelection,
           expectedApnName,
           expectedApnUserName,
           expectedApnPassword,
           "")
{
    EXPECT_TRUE(arg.getDataValid());
    EXPECT_EQ(expectedExistsOptional, arg.getExistsOptionalConfiguration());
    EXPECT_EQ(expectedApnSelection, arg.getConfiguration().getSelection());
    EXPECT_EQ(expectedApnName, arg.getConfiguration().getApnName());
    EXPECT_EQ(expectedApnUserName, arg.getConfiguration().getUserName());
    EXPECT_EQ(expectedApnPassword, arg.getConfiguration().getPassword());
    return true;
}

TEST_F(CarModemApnServiceFixture, ApnConfigurationNotificationFromIpcbForwardedToCommonApi)
{
    const gen_conn::IpCommandBusTypes::CarModem_ApnSelection expectedApnSelection =
        gen_conn::IpCommandBusTypes::CarModem_ApnSelection::AutoFailed;
    const std::string expectedApnName = "rjlsfdfdsljkfsdl";
    const std::
        string
            expectedUserName =
                "dsflkmßðklfmsdkläö åö€ シャンハイdlk 章, 聴, 光, 晶, 晄, 彬, 昶, 了, 秋良, 明楽, 日日日, 亜紀良, aaa "
                "غ 	ظ 	ض 	ذ 	خ bbb";
    const std::string expectedPassword = "dsflmdlksfmll#¤";

    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
    Message msgDummy_TheDecodeMessageMockWillReturnValidData;
    Icb_OpPSIMNetworkConfiguration_Response notify = Icb_OpPSIMNetworkConfiguration_Response_Create(session);
    notify->selection->autoFailed = Icb_AutomaticAPNConfiguration_Create(session);
    notify->selection->choice = Icb_APNSelection_Choices::e_Icb_APNSelection_autoFailed;
    notify->selection->autoFailed->exists__optional__configuration = true;

    notify->selection->autoFailed->configuration->apnName =
        InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, expectedApnName);
    notify->selection->autoFailed->configuration->username =
        InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, expectedUserName);
    notify->selection->autoFailed->configuration->password =
        InfotainmentIpService::Utils::CapiStringTypeToAsnOctetString(session, expectedPassword);

    EXPECT_CALL(
        ip_service_,
        DecodeMessagePSIMNetworkConfigurationResponse(
            _, _, Icb_OpPSIMNetworkConfiguration_Response_Create, Icb_OpPSIMNetworkConfiguration_Response_Decode))
        .WillOnce(DoAll(SetArgReferee<1>(notify), Return(true)));

    EXPECT_CALL((*stubAdapter),
                fireCarModemCurrentApnConfigurationAttributeChanged(CarModem_ApnConfigurationMatcher(
                    true, expectedApnSelection, expectedApnName, expectedUserName, expectedPassword)));

    carModemApnService_.cbCarModemPSIMNetworkConfigurationNotification(
        msgDummy_TheDecodeMessageMockWillReturnValidData);
}

}  // namespace
