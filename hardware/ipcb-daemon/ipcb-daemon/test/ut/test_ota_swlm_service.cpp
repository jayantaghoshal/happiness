/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <functional>
#include <map>
#include <memory>
#include "CommonAPI/CommonAPI.hpp"
#include "ipcommandbus/Message.h"
#include "ipcommandbus/MessageDispatcher.h"
#include "ipcommandbus/VccIpCmdApi.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/cedric/mock_cedric_localconfig.h"
#include "mocks/mock_commonapi_runtime.hpp"
#include "mocks/mock_ip_service.h"
#include "services/ota_swlm_service.h"

extern bool g_ota_swlm_available_;
extern int g_ota_swlm_available_calls;
extern int g_ota_swlm_function_called;

namespace gen_conn = v0::org::volvo::connectivity;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgReferee;

MockLocalconfig *mock_local_config;

class OtaSwlmServiceFixture : public ::testing::Test
{
public:
    OtaSwlmServiceFixture()
    {
        mockRuntime = CommonAPI::MockRuntime::get();
    }

    virtual void SetUp()
    {
        fflush(stderr);
        ON_CALL(otaSwlmService_, RegisterServer(_)).WillByDefault(Return(true));
    }

    virtual void TearDown()
    {
        g_ota_swlm_available_calls = 0;
        g_ota_swlm_function_called = 0;
    }
    NiceMock<Connectivity::MockMessageDispatcher> dispatcher;
    std::shared_ptr<CommonAPI::MainLoopContext> common_api_main_loop_context_ =
        std::make_shared<CommonAPI::MainLoopContext>("InfotainmentIpServiceDbusConnection");
    CommonAPI::ProxyStatusEvent proxyStatus;
    Connectivity::OTASwlmService otaSwlmService_;
    std::shared_ptr<CommonAPI::MockRuntime> mockRuntime;
    void setOtaSwlmAvailable(bool b)
    {
        otaSwlmService_.ota_swlm_available_ = b;
    }

    void AvailabilityCallback(CommonAPI::AvailabilityStatus status)
    {
        return otaSwlmService_.AvailabilityCallback(status);
    }

    void addDeviceInstallSubscriber(const Connectivity::IpService::NotificationSubscriber &sub)
    {
        otaSwlmService_.m_DeviceInstallSubscribers.push_back(sub);
    }

    void addLocalStorageSubscriber(const Connectivity::IpService::NotificationSubscriber &sub)
    {
        otaSwlmService_.m_LocalStorageSubscribers.push_back(sub);
    }

    void DeviceInstallNotificationCallback(const v0_conn::OTATypes::InstallStatus &installStatus,
                                           const v0_conn::OTATypes::InstallProgress &installProgress,
                                           const std::vector<v0_conn::OTATypes::InstalledData> &installedList)
    {
        otaSwlmService_.DeviceInstallNotificationCallback(installStatus, installProgress, installedList);
    }

    void LocalStorageNotificationCallback(bool available)
    {
        otaSwlmService_.LocalStorageNotificationCallback(available);
    }
    void cbDeviceInstallSetReq(Connectivity::Message &msg)
    {
        otaSwlmService_.cbDeviceInstallSetReq(msg);
    }
    void cbDeviceInstallReq(Connectivity::Message &msg)
    {
        otaSwlmService_.cbDeviceInstallReq(msg);
    }
    void cbDeviceInstallNotificationReq(Connectivity::Message &msg)
    {
        otaSwlmService_.cbDeviceInstallNotificationReq(msg);
    }
    void sendDeviceInstallResponse(const CommonAPI::CallStatus &status,
                                   const v0_conn::OTATypes::InstallStatus &installStatus,
                                   const v0_conn::OTATypes::InstallProgress &installProgress,
                                   const std::vector<v0_conn::OTATypes::InstalledData> &installedList,
                                   const Connectivity::Message::Ecu msg_target_ecu,
                                   const Connectivity::VccIpCmd::SenderHandleId sender_handle_id)
    {
        otaSwlmService_.sendDeviceInstallResponse(
            status, installStatus, installProgress, installedList, msg_target_ecu, sender_handle_id);
    }
    void cbLocalStorageReq(Connectivity::Message &msg)
    {
        otaSwlmService_.cbLocalStorageReq(msg);
    }
    void LocalStorageRequestCallback(const CommonAPI::CallStatus &status,
                                     bool available,
                                     const Connectivity::Message::Ecu msg_target_ecu,
                                     const Connectivity::VccIpCmd::SenderHandleId sender_handle_id)
    {
        otaSwlmService_.LocalStorageRequestCallback(status, available, msg_target_ecu, sender_handle_id);
    }
    void cbLocalStorageNotificationReq(Connectivity::Message &msg)
    {
        otaSwlmService_.cbLocalStorageNotificationReq(msg);
    }
};

TEST_F(OtaSwlmServiceFixture, TestInitialize)
{
    mockRuntime->available = true;

    EXPECT_CALL(otaSwlmService_, RegisterServer(_)).WillOnce(Return(false));
    EXPECT_FALSE(otaSwlmService_.Initialize(&dispatcher, common_api_main_loop_context_));

    EXPECT_CALL(otaSwlmService_, RegisterServer(_)).WillOnce(Return(true));
    EXPECT_TRUE(otaSwlmService_.Initialize(&dispatcher, common_api_main_loop_context_));
}

TEST_F(OtaSwlmServiceFixture, TestAvailabilityCallback)
{
    mockRuntime->available = true;
    EXPECT_TRUE(otaSwlmService_.Initialize(&dispatcher, common_api_main_loop_context_));

    g_ota_swlm_available_ = false;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::NOT_AVAILABLE);
    EXPECT_EQ(g_ota_swlm_available_calls, 0);

    g_ota_swlm_available_ = false;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(g_ota_swlm_available_calls, 1);

    g_ota_swlm_available_ = true;
    this->AvailabilityCallback(CommonAPI::AvailabilityStatus::AVAILABLE);
    EXPECT_EQ(g_ota_swlm_available_calls, 2);
}

TEST_F(OtaSwlmServiceFixture, TestDeviceInstallNotificationCallback)
{
    v0_conn::OTATypes::InstallStatus installStatus = v0_conn::OTATypes::InstallStatus::installing;

    otaSwlmService_.Initialize(&dispatcher, common_api_main_loop_context_);

    std::vector<v0_conn::OTATypes::InstalledData> installedList;
    v0_conn::OTATypes::PartIdentifier identifier("someId");
    v0_conn::OTATypes::FileInstallationStatus fileStatus(v0_conn::OTATypes::FileInstallationStatus::Literal::pending);
    v0_conn::OTATypes::InstalledData installedData(identifier, fileStatus);
    installedList.push_back(installedData);

    EXPECT_CALL(otaSwlmService_, sendMessage(_, _)).Times(1);  // VCM is default subscriber
    this->DeviceInstallNotificationCallback(installStatus, 1, installedList);

    Connectivity::IpService::NotificationSubscriber sub(Connectivity::VccIpCmd::ServiceId::TVM,
                                                        Connectivity::VccIpCmd::OperationId::DeviceInstall,
                                                        Connectivity::Message::Ecu::IHU);
    this->addDeviceInstallSubscriber(sub);

    EXPECT_CALL(otaSwlmService_, sendMessage(_, _)).Times(2);
    this->DeviceInstallNotificationCallback(installStatus, 1, installedList);
}

TEST_F(OtaSwlmServiceFixture, TestcbDeviceInstallSetReq)
{
    mockRuntime->available = true;
    EXPECT_TRUE(otaSwlmService_.Initialize(&dispatcher, common_api_main_loop_context_));

    setOtaSwlmAvailable(false);
    EXPECT_CALL(otaSwlmService_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::OTA,
                                 Connectivity::VccIpCmd::OperationId::DeviceInstall,
                                 Connectivity::Message::Ecu::IHU,
                                 8));
    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::IHU;
    msg.pdu.header.sender_handle_id = 8;
    this->cbDeviceInstallSetReq(msg);

    setOtaSwlmAvailable(true);
    ASN_BYTE session_buffer[ASN_SESSION_SIZE + 2048];
    ASN_Session session = ASN_Session_Create(session_buffer, sizeof(session_buffer));
    Icb_OpDeviceInstall_SetRequest req = Icb_OpDeviceInstall_SetRequest_Create(session);
    req->installCommand = 0;

    Icb_ArrayOf_InstallationData installList = Icb_ArrayOf_InstallationData_Create(session);
    Icb_ArrayOf_InstallationData_SetSize(installList, session, 1);
    char c[] = "whatever";
    ASN_UTF8String_AssociateText(installList->data_array[0]->identifier->partID, c);
    ASN_UTF8String_AssociateText(installList->data_array[0]->name->uri, c);
    installList->data_array[0]->rebootNeeded = false;
    req->installList = installList;
    EXPECT_CALL(otaSwlmService_, DecodeMessageDeviceInstallSetRequest(_, _, _, _))
        .WillOnce(DoAll(SetArgReferee<1>(req), Return(true)));
    this->cbDeviceInstallSetReq(msg);

    msg.pdu.header.sender_handle_id = 9;
    EXPECT_CALL(otaSwlmService_, DecodeMessageDeviceInstallSetRequest(_, _, _, _)).WillOnce(Return(false));
    EXPECT_CALL(otaSwlmService_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::OTA,
                                 Connectivity::VccIpCmd::OperationId::DeviceInstall,
                                 Connectivity::Message::Ecu::IHU,
                                 9));
    this->cbDeviceInstallSetReq(msg);
}

TEST_F(OtaSwlmServiceFixture, TestCbDeviceInstallReq)
{
    mockRuntime->available = true;
    EXPECT_TRUE(otaSwlmService_.Initialize(&dispatcher, common_api_main_loop_context_));

    setOtaSwlmAvailable(false);
    EXPECT_CALL(otaSwlmService_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::OTA,
                                 Connectivity::VccIpCmd::OperationId::DeviceInstall,
                                 Connectivity::Message::Ecu::TEM,
                                 2));

    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::TEM;
    msg.pdu.header.sender_handle_id = 2;
    this->cbDeviceInstallReq(msg);

    setOtaSwlmAvailable(true);
    this->cbDeviceInstallReq(msg);
    EXPECT_EQ(1, g_ota_swlm_function_called);
}
TEST_F(OtaSwlmServiceFixture, TestCbDeviceInstallNotificationReq)
{
    setOtaSwlmAvailable(false);
    EXPECT_CALL(otaSwlmService_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::OTA,
                                 Connectivity::VccIpCmd::OperationId::DeviceInstall,
                                 Connectivity::Message::Ecu::UNKNOWN,
                                 1));

    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::UNKNOWN;
    msg.pdu.header.sender_handle_id = 1;
    this->cbDeviceInstallNotificationReq(msg);

    setOtaSwlmAvailable(true);
    EXPECT_CALL(otaSwlmService_,
                SendResponseDeviceInstall(_,
                                          _,
                                          _,
                                          Connectivity::VccIpCmd::ServiceId::OTA,
                                          Connectivity::VccIpCmd::OperationId::DeviceInstall,
                                          Connectivity::Message::Ecu::UNKNOWN,
                                          1));
    this->cbDeviceInstallNotificationReq(msg);
}
TEST_F(OtaSwlmServiceFixture, TestSendDeviceInstallResponse)
{
    // same response shared by DeviceInstallRequestCallback and DeviceInstallSetRequestCallback
    std::vector<v0_conn::OTATypes::InstalledData> installedList;
    v0_conn::OTATypes::PartIdentifier identifier("someId");
    v0_conn::OTATypes::FileInstallationStatus fileStatus(v0_conn::OTATypes::FileInstallationStatus::Literal::pending);
    v0_conn::OTATypes::InstalledData installedData(identifier, fileStatus);
    installedList.push_back(installedData);

    EXPECT_CALL(otaSwlmService_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::OTA,
                                 Connectivity::VccIpCmd::OperationId::DeviceInstall,
                                 Connectivity::Message::Ecu::VCM,
                                 1));
    this->sendDeviceInstallResponse(CommonAPI::CallStatus::NOT_AVAILABLE,
                                    v0_conn::OTATypes::InstallStatus::installing,
                                    1,
                                    installedList,
                                    Connectivity::Message::Ecu::VCM,
                                    1);

    EXPECT_CALL(otaSwlmService_,
                SendResponseDeviceInstall(_,
                                          _,
                                          _,
                                          Connectivity::VccIpCmd::ServiceId::OTA,
                                          Connectivity::VccIpCmd::OperationId::DeviceInstall,
                                          Connectivity::Message::Ecu::UNKNOWN,
                                          3));
    this->sendDeviceInstallResponse(CommonAPI::CallStatus::SUCCESS,
                                    v0_conn::OTATypes::InstallStatus::installing,
                                    1,
                                    installedList,
                                    Connectivity::Message::Ecu::UNKNOWN,
                                    3);
}
TEST_F(OtaSwlmServiceFixture, TestLocalStorageNotificationCallback)
{
    EXPECT_CALL(otaSwlmService_, sendMessage(_, _)).Times(1);  // VCM is default subscriber
    this->LocalStorageNotificationCallback(false);

    Connectivity::IpService::NotificationSubscriber sub(Connectivity::VccIpCmd::ServiceId::TVM,
                                                        Connectivity::VccIpCmd::OperationId::LocalStorage,
                                                        Connectivity::Message::Ecu::IHU);
    this->addLocalStorageSubscriber(sub);

    EXPECT_CALL(otaSwlmService_, sendMessage(_, _)).Times(2);
    this->LocalStorageNotificationCallback(true);
}
TEST_F(OtaSwlmServiceFixture, TestCbLocalStorageReq)
{
    mockRuntime->available = true;
    EXPECT_TRUE(otaSwlmService_.Initialize(&dispatcher, common_api_main_loop_context_));

    setOtaSwlmAvailable(false);
    EXPECT_CALL(otaSwlmService_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::OTA,
                                 Connectivity::VccIpCmd::OperationId::LocalStorage,
                                 Connectivity::Message::Ecu::TEM,
                                 2));

    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::TEM;
    msg.pdu.header.sender_handle_id = 2;
    this->cbLocalStorageReq(msg);

    setOtaSwlmAvailable(true);
    this->cbLocalStorageReq(msg);
    EXPECT_EQ(1, g_ota_swlm_function_called);
}
TEST_F(OtaSwlmServiceFixture, TestLocalStorageRequestCallback)
{
    EXPECT_CALL(otaSwlmService_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::OTA,
                                 Connectivity::VccIpCmd::OperationId::LocalStorage,
                                 Connectivity::Message::Ecu::VCM,
                                 1));
    this->LocalStorageRequestCallback(CommonAPI::CallStatus::NOT_AVAILABLE, false, Connectivity::Message::Ecu::VCM, 1);

    EXPECT_CALL(otaSwlmService_,
                SendResponseLocalStorage(_,
                                         _,
                                         _,
                                         Connectivity::VccIpCmd::ServiceId::OTA,
                                         Connectivity::VccIpCmd::OperationId::LocalStorage,
                                         Connectivity::Message::Ecu::UNKNOWN,
                                         3));
    this->LocalStorageRequestCallback(CommonAPI::CallStatus::SUCCESS, true, Connectivity::Message::Ecu::UNKNOWN, 3);
}
TEST_F(OtaSwlmServiceFixture, TestCbLocalStorageNotificationReq)
{
    setOtaSwlmAvailable(false);
    EXPECT_CALL(otaSwlmService_,
                SendGenericError(_,
                                 Connectivity::VccIpCmd::ServiceId::OTA,
                                 Connectivity::VccIpCmd::OperationId::LocalStorage,
                                 Connectivity::Message::Ecu::UNKNOWN,
                                 1));

    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::UNKNOWN;
    msg.pdu.header.sender_handle_id = 1;
    this->cbLocalStorageNotificationReq(msg);

    setOtaSwlmAvailable(true);
    EXPECT_CALL(otaSwlmService_,
                SendResponseLocalStorage(_,
                                         _,
                                         _,
                                         Connectivity::VccIpCmd::ServiceId::OTA,
                                         Connectivity::VccIpCmd::OperationId::LocalStorage,
                                         Connectivity::Message::Ecu::UNKNOWN,
                                         1));
    this->cbLocalStorageNotificationReq(msg);
    ;
}
