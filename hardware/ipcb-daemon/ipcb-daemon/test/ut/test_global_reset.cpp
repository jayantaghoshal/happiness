/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <NodeStateTypes.h>
#include <connectivity-sd/mock_time_provider.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <services/global_reset.h>
#include <memory>
#include "mocks/MockMessageDispatcher.h"

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
class GlobalResetFixture : public ::testing::Test
{
public:
    GlobalResetFixture()
    {
    }

    ~GlobalResetFixture()
    {
    }
    virtual void SetUp()
    {
        globalReset_.onEcuAvailabilityChange(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem, true);
        globalReset_.onEcuAvailabilityChange(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm, true);
    }

    GlobalReset globalReset_;
    NiceMock<MockMessageDispatcher> messageDispatcherMock_;
    NiceMock<MockIpService>& ip_service_ = globalReset_;
};

MATCHER_P(LambdaMatcher, func, "")
{
    func(arg);
    return true;
}

TEST_F(GlobalResetFixture, Initialize)
{
    TimeProviderStub timeProvider;
    EXPECT_CALL(ip_service_, RegisterClient(_)).Times(1).WillOnce(Return(false));

    EXPECT_CALL(ip_service_, RegisterClient(_)).Times(1).WillOnce(Return(true));

    globalReset_.Uninitialize();
}

TEST_F(GlobalResetFixture, onSessionStateChangedUnhandledSessionStateName_NoRequestShallBeSent)
{
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::VCM, _))
        .Times(0);
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::TEM, _))
        .Times(0);

    globalReset_.onSessionStateChanged("SomeOtherSession", NsmSeat_Driver, 0);
    globalReset_.onSessionStateChanged(
        "StorageReset", NsmSeat_Driver, 1);  // 1 = RESET_INACTIVE, should not generate messages to VCM and TEM
    globalReset_.onSessionStateChanged(
        "StorageReset", NsmSeat_Driver, 2);  // 2 = RESET_PROFILE, should not generate messages to VCM and TEM
    globalReset_.onSessionStateChanged(
        "StorageReset", NsmSeat_Driver, 3);  // 3 = RESET_ALL_PROFILES, should not generate messages to VCM and TEM
}

TEST_F(GlobalResetFixture, onSessionStateChangedResetCar_SendRequestToVCMAndTEM)
{
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::VCM, _))
        .Times(1);
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::TEM, _))
        .Times(1);

    globalReset_.onSessionStateChanged("StorageReset", NsmSeat_Driver, 4);  // 4 = RESET_CAR
}

TEST_F(GlobalResetFixture, onSessionStateChangedResetFactory_SendRequestToVCMAndTEM)
{
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::VCM, _))
        .Times(1);
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::TEM, _))
        .Times(1);

    globalReset_.onSessionStateChanged("StorageReset", NsmSeat_Driver, 5);  // 5 = RESET_FACTORY
}

TEST_F(GlobalResetFixture, onEcuAvailabilityChangeFalse_NoRequestShallBeSentUntilAvailableAgain)
{
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::VCM, _))
        .Times(0);
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::TEM, _))
        .Times(0);

    globalReset_.onEcuAvailabilityChange(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem, false);
    globalReset_.onEcuAvailabilityChange(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm, false);

    globalReset_.onSessionStateChanged("StorageReset", NsmSeat_Driver, 5);  // 5 = RESET_FACTORY

    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::VCM, _))
        .Times(1);
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::TEM, _))
        .Times(0);

    globalReset_.onEcuAvailabilityChange(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm, true);
    globalReset_.onEcuAvailabilityChange(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem, false);

    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::VCM, _))
        .Times(0);
    EXPECT_CALL(
        globalReset_,
        SendGenericRequest(
            VccIpCmd::ServiceId::Connectivity, VccIpCmd::OperationId::FactoryDefaultRestore, Message::Ecu::TEM, _))
        .Times(1);

    globalReset_.onEcuAvailabilityChange(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm, true);
    globalReset_.onEcuAvailabilityChange(gen_conn::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem, true);
}

}  // namespace
