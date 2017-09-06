/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <connectivity-sd/mock_time_provider.h>
#include <dataelements/ECD_dataelement.h>
#include "CommonAPI/CommonAPI.hpp"
#include "cedric/cedric_localconfig.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/MockMessageDispatcher.h"
#include "mocks/mock_commonapi_runtime.hpp"
#include "mocks/mock_diagnostics_client.h"
#include "mocks/mock_iplm_service_stub.h"
#include "services/iplm_service.h"
#include "util/local_config.h"

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
using ::testing::InSequence;
using ::testing::SetArgPointee;

const generated::IpLmService::ResourceGroup _rg1 = generated::IpLmService::ResourceGroup::kResourceGroup1;
const generated::IpLmService::ResourceGroup _rg3 = generated::IpLmService::ResourceGroup::kResourceGroup3;
const generated::IpLmService::ResourceGroupStatus _rgStatusUnavailable =
    generated::IpLmService::ResourceGroupStatus::kResourceGroupStatus_Unavailable;
const generated::IpLmService::ResourceGroupStatus _rgStatusPartlyAvailable =
    generated::IpLmService::ResourceGroupStatus::kResourceGroupStatus_PartlyAvailable;
const generated::IpLmService::ResourceGroupStatus _rgStatusAvailable =
    generated::IpLmService::ResourceGroupStatus::kResourceGroupStatus_Available;
const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ecu _vcmEcu =
    generated::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm;
const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ecu _temEcu =
    generated::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem;

LocalconfigStub local_config_stub;

namespace generated = v0::org::volvo::connectivity;

namespace Connectivity
{
ACTION_P(SetArg2ToInt32, value)
{
    *static_cast<uint32_t*>(arg2) = value;
}
class IplmServiceFixture : public ::testing::Test
{
public:
    IplmServiceFixture() : iplmService{timeProvider}
    {
    }
    virtual void SetUp()
    {
        iplmService.iplm_service_stub_ = p_iplm_service_stub_;
        iplmService.m_dispatcher = &mockMessageDispatcher;
    }

    virtual void TearDown()
    {
        iplmService.m_dispatcher = nullptr;
    }
    Connectivity::TimeProviderStub timeProvider;
    IpLmServiceStubImpl iplmService;
    NiceMock<MockMessageDispatcher> mockMessageDispatcher;
    NiceMock<MockDiagnosticsClient> mockDiagnosticsClient_;

    std::shared_ptr<NiceMock<MockIpLmServiceStub>> p_iplm_service_stub_ =
        std::make_shared<NiceMock<MockIpLmServiceStub>>(nullptr);

    Connectivity::Message PrepareMsgCbLmBroadcast()
    {
        std::vector<uint8_t> payload{Connectivity::Message::Ecu::UNKNOWN,
                                     Connectivity::IpLmServiceStubImpl::Prio::PRIO_NORM};

        Pdu pdu;
        pdu.createHeader(Connectivity::VccIpCmd::ServiceId::Connectivity,
                         Connectivity::VccIpCmd::OperationId::ConnectivityStatus,
                         Connectivity::VccIpCmd::OperationType::REQUEST,
                         VccIpCmd::DataType::ENCODED,
                         1);
        pdu.setPayload(std::move(payload));
        Connectivity::Message msg(std::move(pdu));

        return msg;
    }
};

TEST_F(IplmServiceFixture, Initialize)
{
    // EXPECT_CALL(mockMessageDispatcher, registerNotificationCyclicCallback(_, _, _)).Times(1);
    // EXPECT_CALL(iplmService, setDispatcher(&mockMessageDispatcher)).Times(1);
    // EXPECT_TRUE(iplmService.Initialize(&mockMessageDispatcher));
}

TEST_F(IplmServiceFixture, Uninitialize)
{
    // EXPECT_CALL(mockMessageDispatcher, registerNotificationCyclicCallback(_, _, _)).Times(1);
    // EXPECT_CALL(iplmService, setDispatcher(&mockMessageDispatcher)).Times(1);
    // EXPECT_TRUE(iplmService.Initialize(&mockMessageDispatcher));

    // iplmService.Uninitialize();
}

TEST_F(IplmServiceFixture, GetStub)
{
    EXPECT_NE(iplmService.GetStub(), nullptr);
}

TEST_F(IplmServiceFixture, Request)
{
    int is_called = 0;

    for (int rg = static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup1);
         rg < static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup8) + 1;
         rg++)
    {
        for (int rp = static_cast<int>(generated::IpLmService::ResourcePrio::kPrioNormal);
             rp < static_cast<int>(generated::IpLmService::ResourcePrio::kPrioHigh) + 1;
             rp++)
        {
            is_called = 0;
            iplmService.Request(nullptr,
                                "testService0",
                                static_cast<generated::IpLmService::ResourceGroup::Literal>(rg),
                                static_cast<generated::IpLmService::ResourcePrio::Literal>(rp),
                                [&]() { is_called++; });
            iplmService.Release(
                nullptr, "testService0", static_cast<generated::IpLmService::ResourceGroup::Literal>(rg), [&]() {
                    is_called++;
                });
            EXPECT_EQ(is_called, 2);
        }
    }

    bool service_called_capture;
    for (int rg = static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup1);
         rg < static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup8) + 1;
         rg++)
    {
        for (int rp = static_cast<int>(generated::IpLmService::ResourcePrio::kPrioNormal);
             rp < static_cast<int>(generated::IpLmService::ResourcePrio::kPrioHigh) + 1;
             rp++)
        {
            is_called = 0;
            // Register a service
            iplmService.RegisterService(nullptr, "testService1", [&]() { service_called_capture = true; });
            iplmService.Request(nullptr,
                                "testService1",
                                static_cast<generated::IpLmService::ResourceGroup::Literal>(rg),
                                static_cast<generated::IpLmService::ResourcePrio::Literal>(rp),
                                [&]() { is_called++; });
            iplmService.Release(
                nullptr, "testService1", static_cast<generated::IpLmService::ResourceGroup::Literal>(rg), [&]() {
                    is_called++;
                });
            iplmService.UnRegisterService(nullptr, "testService1", [&]() { service_called_capture = true; });
            EXPECT_EQ(is_called, 2);
        }
    }

    for (int rg = static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup1);
         rg < static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup8) + 1;
         rg++)
    {
        for (int rp = static_cast<int>(generated::IpLmService::ResourcePrio::kPrioNormal);
             rp < static_cast<int>(generated::IpLmService::ResourcePrio::kPrioHigh) + 1;
             rp++)
        {
            is_called = 0;
            // Register a service
            iplmService.RegisterService(nullptr, "testService1", [&]() { service_called_capture = true; });
            iplmService.Request(nullptr,
                                "testService1",
                                static_cast<generated::IpLmService::ResourceGroup::Literal>(rg),
                                static_cast<generated::IpLmService::ResourcePrio::Literal>(rp),
                                [&]() { is_called++; });
            iplmService.UnRegisterService(nullptr, "testService1", [&]() { service_called_capture = true; });
            EXPECT_EQ(is_called, 1);
        }
    }
}

TEST_F(IplmServiceFixture, Request_normprio_with_hiprio_already_requested)
{
    int is_called = 0;
    iplmService.RegisterService(nullptr, "testService1_hiprio", []() {});
    iplmService.Request(nullptr,
                        "testService1_hiprio",
                        generated::IpLmService::ResourceGroup::kResourceGroup1,
                        generated::IpLmService::ResourcePrio::kPrioHigh,
                        [&]() { is_called++; });

    iplmService.RegisterService(nullptr, "testService2_normprio", []() {});
    iplmService.Request(nullptr,
                        "testService2_normprio",
                        generated::IpLmService::ResourceGroup::kResourceGroup1,
                        generated::IpLmService::ResourcePrio::kPrioNormal,
                        [&]() { is_called++; });
    EXPECT_EQ(is_called, 2);
}

TEST_F(IplmServiceFixture, Request_normprio_with_hiprio_already_requested_by_other_node)
{
    int is_called = 0;

    std::vector<uint8_t> payload = {IpLmServiceStubImpl::ResourceGroup::RG_1, IpLmServiceStubImpl::Prio::PRIO_HIGH};
    Message m;
    m.ecu = Message::Ecu::VCM;
    m.pdu.payload = std::move(payload);
    iplmService.CbLmBroadcast(m);

    iplmService.RegisterService(nullptr, "testService2_normprio", []() {});
    iplmService.Request(nullptr,
                        "testService2_normprio",
                        generated::IpLmService::ResourceGroup::kResourceGroup1,
                        generated::IpLmService::ResourcePrio::kPrioNormal,
                        [&]() { is_called++; });
    EXPECT_EQ(is_called, 1);
}

// TEST_F(IplmServiceFixture, SetNsmSessionState)
// {
//     // "Initialize" an SD-Bus
//     iplmService.bus_ = reinterpret_cast<sd_bus*>(4);

//     EXPECT_CALL(mockLibSdBus_, sd_bus_call_method(_, _, _, _, _, _, _, _)).WillOnce(Return(0));
//     EXPECT_CALL(mockLibSdBus_, sd_bus_message_read_basic(_, _, _)).WillOnce(Return(0));

//     EXPECT_EQ(iplmService.SetNsmSessionState(generated::IpLmService::ResourceGroup::kResourceGroup1,
//                                              Connectivity::IpLmServiceStubImpl::RGActivationState::PrioNormal),
//               true);
// }

// TEST_F(IplmServiceFixture, SetNsmSessionState_Uninitialized_sdbus)
// {
//     EXPECT_EQ(iplmService.SetNsmSessionState(generated::IpLmService::ResourceGroup::kResourceGroup2,
//                                              Connectivity::IpLmServiceStubImpl::RGActivationState::PrioNormal),
//               false);
// }

// TEST_F(IplmServiceFixture, SetNsmSessionState_Invalid_Rg)
// {
//     // "Initialize" an SD-Bus
//     iplmService.bus_ = reinterpret_cast<sd_bus*>(4);

//     EXPECT_EQ(iplmService.SetNsmSessionState(generated::IpLmService::ResourceGroup::kResourceGroup2,
//                                              Connectivity::IpLmServiceStubImpl::RGActivationState::PrioNormal),
//               false);
// }

// TEST_F(IplmServiceFixture, SetNsmSessionState_sd_bus_call_method_Error)
// {
//     // "Initialize" an SD-Bus
//     iplmService.bus_ = reinterpret_cast<sd_bus*>(4);

//     EXPECT_CALL(mockLibSdBus_, sd_bus_call_method(_, _, _, _, _, _, _, _)).WillOnce(Return(-1));

//     EXPECT_EQ(iplmService.SetNsmSessionState(generated::IpLmService::ResourceGroup::kResourceGroup1,
//                                              Connectivity::IpLmServiceStubImpl::RGActivationState::PrioNormal),
//               false);
// }

// TEST_F(IplmServiceFixture, SetNsmSessionState_sd_bus_message_read_basic_Error)
// {
//     // "Initialize" an SD-Bus
//     iplmService.bus_ = reinterpret_cast<sd_bus*>(4);

//     EXPECT_CALL(mockLibSdBus_, sd_bus_call_method(_, _, _, _, _, _, _, _)).WillOnce(Return(0));
//     EXPECT_CALL(mockLibSdBus_, sd_bus_message_read_basic(_, _, _)).WillOnce(Return(-1));

//     EXPECT_EQ(iplmService.SetNsmSessionState(generated::IpLmService::ResourceGroup::kResourceGroup1,
//                                              Connectivity::IpLmServiceStubImpl::RGActivationState::PrioNormal),
//               false);
// }

// TEST_F(IplmServiceFixture, SetNsmSessionState_sd_bus_message_read_basic_ErrorValue)
// {
//     // "Initialize" an SD-Bus
//     iplmService.bus_ = reinterpret_cast<sd_bus*>(4);

//     EXPECT_CALL(mockLibSdBus_, sd_bus_call_method(_, _, _, _, _, _, _, _)).WillOnce(Return(0));

//     EXPECT_CALL(mockLibSdBus_, sd_bus_message_read_basic(_, _, _)).WillOnce(DoAll(SetArg2ToInt32(2), Return(0)));

//     EXPECT_EQ(iplmService.SetNsmSessionState(generated::IpLmService::ResourceGroup::kResourceGroup1,
//                                              Connectivity::IpLmServiceStubImpl::RGActivationState::PrioNormal),
//               false);
// }

TEST_F(IplmServiceFixture, SendFlexrayWakeup)
{
    // EXPECT_EQ(iplmService.SendFlexrayWakeup(generated::IpLmService::ResourceGroup::kResourceGroup1,
    //                                         generated::IpLmService::ResourcePrio::kPrioHigh),
    //           true);
}

TEST_F(IplmServiceFixture, FlexrayWakeupTimeout)
{
    // TODO: Fix real checks. Left here for coverage only.
    // iplmService.FlexrayWakeupTimeout(InfotainmentIpService::Utils::BaseTimer::ReasonCode::TIMER_CANCELLED, 0);
}

TEST_F(IplmServiceFixture, Release)
{
    int is_called;
    for (int rg = static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup1);
         rg < static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup8) + 1;
         rg++)
    {
        is_called = 0;
        iplmService.Release(
            nullptr, "testService0", static_cast<generated::IpLmService::ResourceGroup::Literal>(rg), [&]() {
                is_called++;
            });
        EXPECT_GT(is_called, 0);
    }

    bool service_called_capture;
    // Register a service
    iplmService.RegisterService(nullptr, "testService1", [&]() { service_called_capture = true; });
    for (int rg = static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup1);
         rg < static_cast<int>(generated::IpLmService::ResourceGroup::kResourceGroup8) + 1;
         rg++)
    {
        is_called = 0;
        iplmService.Request(nullptr,
                            "testService1",
                            generated::IpLmService::ResourceGroup::kResourceGroup1,
                            generated::IpLmService::ResourcePrio::kPrioHigh,
                            [&]() { is_called++; });
        iplmService.Release(
            nullptr, "testService1", static_cast<generated::IpLmService::ResourceGroup::Literal>(rg), [&]() {
                is_called++;
            });
        EXPECT_GT(is_called, 1);
    }
}

TEST_F(IplmServiceFixture, RegisterService)
{
    bool is_called = false;

    g_nrOfLocalSoftwareComponents = 0;
    iplmService.RegisterService(nullptr, "testService0", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);

    is_called = false;
    g_nrOfLocalSoftwareComponents = 2;
    iplmService.RegisterService(nullptr, "testService1", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);

    is_called = false;
    g_nrOfLocalSoftwareComponents = 2;
    iplmService.RegisterService(nullptr, "testService1", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);

    is_called = false;
    g_nrOfLocalSoftwareComponents = 4;
    iplmService.RegisterService(nullptr, "testService2", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);
}

TEST_F(IplmServiceFixture, UnRegisterService)
{
    bool is_called = false;

    g_nrOfLocalSoftwareComponents = 1;
    iplmService.RegisterService(nullptr, "testService0", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);

    is_called = false;
    g_nrOfLocalSoftwareComponents = 2;
    iplmService.RegisterService(nullptr, "testService1", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);

    is_called = false;
    iplmService.UnRegisterService(nullptr, "testService1", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);

    is_called = false;
    iplmService.UnRegisterService(nullptr, "testService1", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);

    is_called = false;
    iplmService.UnRegisterService(nullptr, "testService_notfound", [&]() { is_called = true; });
    EXPECT_EQ(is_called, true);
}

TEST_F(IplmServiceFixture, ToString)
{
    // EXPECT_STREQ(iplmService.ToString(Connectivity::IpLmServiceStubImpl::ResourceGroup::RG_Reserved),
    //              "RG_Reserved");
    // EXPECT_STREQ(iplmService.ToString(Connectivity::IpLmServiceStubImpl::ResourceGroup::RG_1).c_str(), "RG_1");
    // EXPECT_STREQ(iplmService.ToString(Connectivity::IpLmServiceStubImpl::ResourceGroup::RG_3).c_str(), "RG_3");
    // EXPECT_STREQ(iplmService.ToString(static_cast<Connectivity::IpLmServiceStubImpl::ResourceGroup>(4)).c_str(), "");

    // EXPECT_STREQ(iplmService.ToString(Connectivity::IpLmServiceStubImpl::ACTION_AVAILABLE).c_str(), "AVAILABLE ");
    // EXPECT_STREQ(
    //     iplmService
    //         .ToString(static_cast<Connectivity::IpLmServiceStubImpl::Action>(Connectivity::IpLmServiceStubImpl::RG_1))
    //         .c_str(),
    //     "RG_1 ");
    // EXPECT_STREQ(
    //     iplmService
    //         .ToString(static_cast<Connectivity::IpLmServiceStubImpl::Action>(Connectivity::IpLmServiceStubImpl::RG_3))
    //         .c_str(),
    //     "RG_3 ");
    // EXPECT_STREQ(
    //     iplmService
    //         .ToString(Connectivity::IpLmServiceStubImpl::ACTION_AVAILABLE | Connectivity::IpLmServiceStubImpl::RG_1)
    //         .c_str(),
    //     "AVAILABLE RG_1 ");
    // EXPECT_STREQ(
    //     iplmService
    //         .ToString(Connectivity::IpLmServiceStubImpl::ACTION_AVAILABLE | Connectivity::IpLmServiceStubImpl::RG_3)
    //         .c_str(),
    //     "AVAILABLE RG_3 ");
    // EXPECT_STREQ(
    //     iplmService.ToString(Connectivity::IpLmServiceStubImpl::RG_1 |
    //     Connectivity::IpLmServiceStubImpl::RG_3).c_str(),
    //     "RG_1 RG_3 ");
    // EXPECT_STREQ(iplmService
    //                  .ToString(Connectivity::IpLmServiceStubImpl::RG_1 | Connectivity::IpLmServiceStubImpl::RG_3 |
    //                            Connectivity::IpLmServiceStubImpl::ACTION_AVAILABLE)
    //                  .c_str(),
    //              "AVAILABLE RG_1 RG_3 ");
    // EXPECT_STREQ(iplmService.ToString(static_cast<Connectivity::IpLmServiceStubImpl::Action>(0)).c_str(), "");

    // EXPECT_STREQ(iplmService.ToString(Connectivity::IpLmServiceStubImpl::Prio::PRIO_NORM), "NORM");
    // EXPECT_STREQ(iplmService.ToString(Connectivity::IpLmServiceStubImpl::Prio::PRIO_HIGH), "HIGH");
    // EXPECT_STREQ(iplmService.ToString(static_cast<Connectivity::IpLmServiceStubImpl::Prio>(100)), "");

    // EXPECT_STREQ(
    //     iplmService.ToString(generated::IpLmService::ResourcePrio(generated::IpLmService::ResourcePrio::kPrioNormal)),
    //     "NORM");
    // EXPECT_STREQ(
    //     iplmService.ToString(generated::IpLmService::ResourcePrio(generated::IpLmService::ResourcePrio::kPrioHigh)),
    //     "HIGH");
    // EXPECT_STREQ(iplmService
    //                  .ToString(generated::IpLmService::ResourcePrio(
    //                      static_cast<generated::IpLmService::ResourcePrio::Literal>(2))),
    //              "");
}

TEST_F(IplmServiceFixture, CbLmBroadcast)
{
    /*    Connectivity::Message msg = PrepareMsgCbLmBroadcast();*/

    //// Verify that we can handle OperationType is ERROR.
    // iplmService.action_[Connectivity::Message::Ecu::UNKNOWN] = 5;
    // msg.pdu.header.operation_type = Connectivity::VccIpCmd::OperationType::ERROR;
    // iplmService.CbLmBroadcast(msg);
    // EXPECT_EQ(iplmService.action_[Connectivity::Message::Ecu::UNKNOWN], 5);

    //// Verify that we can handle OperationType REQUEST, current ECU is not VCM nor TEM.
    // iplmService.action_[Connectivity::Message::Ecu::UNKNOWN] = 5;
    // msg.pdu.header.operation_type = Connectivity::VccIpCmd::OperationType::REQUEST;
    // iplmService.CbLmBroadcast(msg);
    // EXPECT_EQ(iplmService.action_[Connectivity::Message::Ecu::UNKNOWN], 0);

    //// Verify that we can handle a request for an ECU, which is not the current ECU that we handle.
    // IpLmServiceStubImpl::Prio prio = iplmService.prio_[Connectivity::Message::Ecu::UNKNOWN];
    // std::bitset<static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Max)> current_rg1_availability_status =
    // iplmService.rg1_availabilityStatus_;
    // std::bitset<static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Max)> current_rg3_availability_status =
    // iplmService.rg3_availabilityStatus_;
    // msg.ecu = Connectivity::Message::Ecu::UNKNOWN;
    // iplmService.CbLmBroadcast(msg);
    // EXPECT_EQ(current_rg1_availability_status, iplmService.rg1_availabilityStatus_);
    // EXPECT_EQ(current_rg3_availability_status, iplmService.rg3_availabilityStatus_);
    // EXPECT_EQ(prio, iplmService.prio_[Connectivity::Message::Ecu::UNKNOWN]);

    //// Verify that we can handle a request for VCM, which is the current ECU that we handle.
    //// But, priority has not been changed.
    // iplmService.prio_[Connectivity::Message::Ecu::VCM] = IpLmServiceStubImpl::Prio::PRIO_NORM;
    // prio = iplmService.prio_[Connectivity::Message::Ecu::VCM];
    // iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 0;
    // iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 0;
    // msg.ecu = Connectivity::Message::Ecu::VCM;
    // iplmService.CbLmBroadcast(msg);
    // EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
    // EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
    // EXPECT_EQ(prio, iplmService.prio_[Connectivity::Message::Ecu::VCM]);

    //// Verify that we can handle a request for VCM, which is the current ECU that we handle.
    //// Priority has been changed.
    // iplmService.prio_[Connectivity::Message::Ecu::VCM] = IpLmServiceStubImpl::Prio::PRIO_HIGH;
    // prio = iplmService.prio_[Connectivity::Message::Ecu::VCM];
    // iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 0;
    // iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 0;
    // msg.pdu.payload[1] = Connectivity::IpLmServiceStubImpl::Prio::PRIO_NORM;
    // iplmService.CbLmBroadcast(msg);
    // EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
    // EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
    // EXPECT_NE(prio, iplmService.prio_[Connectivity::Message::Ecu::VCM]);

    //// Verify that we can handle a request for TEM, which is the current ECU that we handle.
    //// But, priority has not been changed.
    // iplmService.prio_[Connectivity::Message::Ecu::VCM] = IpLmServiceStubImpl::Prio::PRIO_NORM;
    // prio = iplmService.prio_[Connectivity::Message::Ecu::TEM];
    // iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Tem)] = 0;
    // iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Tem)] = 0;
    // msg.ecu = Connectivity::Message::Ecu::TEM;
    // iplmService.CbLmBroadcast(msg);
    // EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Tem)], 1);
    // EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Tem)], 1);
    // EXPECT_EQ(prio, iplmService.prio_[Connectivity::Message::Ecu::TEM]);

    //// Verify that we can handle a request for TEM, which is the current ECU that we handle.
    //// Priority has been changed.
    // iplmService.prio_[Connectivity::Message::Ecu::VCM] = IpLmServiceStubImpl::Prio::PRIO_NORM;
    // prio = iplmService.prio_[Connectivity::Message::Ecu::TEM];
    // iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Tem)] = 0;
    // iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Tem)] = 0;
    // msg.pdu.payload[1] = Connectivity::IpLmServiceStubImpl::Prio::PRIO_HIGH;
    // iplmService.CbLmBroadcast(msg);
    // EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Tem)], 1);
    // EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Tem)], 1);
    /*EXPECT_NE(prio, iplmService.prio_[Connectivity::Message::Ecu::TEM]);*/
}

// TODO: This test is total crap, it assumes the timers are started by forcing calls to ActivityTimeout
// when in fact the iplmService is in an invalid state (NsmSessionState_Unregistered == session_state), in Initialize()
// ...in fact Initialize() isn't even called and that's where timers are set up..
TEST_F(IplmServiceFixture, ActivityTimeout)
{
    {
        EXPECT_CALL(iplmService, getNewSequenceId()).WillRepeatedly(Return(0));
        EXPECT_CALL(iplmService, broadcastMessage(_)).Times(1);

        EXPECT_CALL(*p_iplm_service_stub_, fireRgStatusEvent(_rg1, _rgStatusUnavailable)).Times(1);
        EXPECT_CALL(*p_iplm_service_stub_, fireRgStatusEvent(_rg3, _rgStatusUnavailable)).Times(1);
        EXPECT_CALL(*p_iplm_service_stub_, fireNodeStatusEvent(_vcmEcu, _)).Times(1);
        EXPECT_CALL(*p_iplm_service_stub_, fireNodeStatusEvent(_temEcu, _)).Times(1);
        g_nrOfLocalSoftwareComponents = 0;
        iplmService.ActivityTimeout();  // TODO: Replace with sleep()
    }
    // Verify that fireRgStatusEvent is called, as ReasonCode and userData (local ECU) are as required
    // for calling that function.
    // Try with another number of g_nrOfLocalSoftwareComponents to see that it still works as expected.
    EXPECT_CALL(iplmService, broadcastMessage(_)).Times(1);
    bool is_called = false;
    EXPECT_CALL(*p_iplm_service_stub_, fireRgStatusEvent(_rg1, _rgStatusUnavailable)).Times(1);
    EXPECT_CALL(*p_iplm_service_stub_, fireRgStatusEvent(_rg3, _rgStatusUnavailable)).Times(1);
    EXPECT_CALL(*p_iplm_service_stub_, fireNodeStatusEvent(_vcmEcu, _)).Times(1);
    EXPECT_CALL(*p_iplm_service_stub_, fireNodeStatusEvent(_temEcu, _)).Times(1);
    g_nrOfLocalSoftwareComponents = 4;
    iplmService.RegisterService(nullptr, "testService0", [&]() { is_called = true; });
    iplmService.RegisterService(nullptr, "testService1", [&]() { is_called = true; });
    iplmService.RegisterService(nullptr, "testService2", [&]() { is_called = true; });
    iplmService.RegisterService(nullptr, "testService3", [&]() { is_called = true; });
    iplmService.ActivityTimeout();  // TODO: Replace with sleep()
}

// TEST_F(IplmServiceFixture, RequestMonitoringTimeoutVCM)
//{
//// Verify that availibilitystatus and action are not set when we're processing
//// an unknown ecu.
// iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.action_[Message::Ecu::VCM] = 5;
// iplmService.RequestMonitoringTimeoutVCM(InfotainmentIpService::Utils::BaseTimer::ReasonCode::TIMER_CANCELLED,
// Message::Ecu::UNKNOWN);
// EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
// EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
// EXPECT_EQ(iplmService.action_[Message::Ecu::VCM], 5);

//// Verify that availibilitystatus and action are not set when timer has
//// expired
// iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.action_[Message::Ecu::VCM] = 5;
// iplmService.RequestMonitoringTimeoutVCM(InfotainmentIpService::Utils::BaseTimer::ReasonCode::TIMER_CANCELLED,
// Message::Ecu::VCM);
// EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
// EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
// EXPECT_EQ(iplmService.action_[Message::Ecu::VCM], 5);

//// Verify that availibilitystatus and action are not set when we're processing
//// an unknown ecu and the timer has expired.
// iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.action_[Message::Ecu::VCM] = 5;
// iplmService.RequestMonitoringTimeoutVCM(InfotainmentIpService::Utils::BaseTimer::ReasonCode::TIMER_EXPIRED,
// Message::Ecu::UNKNOWN);
// EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
// EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 1);
// EXPECT_EQ(iplmService.action_[Message::Ecu::VCM], 5);

//// Verify that availibilitystatus and action are set when we're processing
//// a VCM timeout.
// iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.action_[Message::Ecu::VCM] = 5;
// iplmService.RequestMonitoringTimeoutVCM(InfotainmentIpService::Utils::BaseTimer::ReasonCode::TIMER_EXPIRED,
// Message::Ecu::VCM);
// EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 0);
// EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 0);
// EXPECT_EQ(iplmService.action_[Message::Ecu::VCM], 0);

//// Verify that availibilitystatus and action are set when we're processing
//// a VCM timeout.
// iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.action_[Message::Ecu::VCM] = 5;
// iplmService.action_[Message::Ecu::SELF] = IpLmServiceStubImpl::ResourceGroup::RG_1;
// iplmService.RequestMonitoringTimeoutVCM(InfotainmentIpService::Utils::BaseTimer::ReasonCode::TIMER_EXPIRED,
// Message::Ecu::VCM);
// EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 0);
// EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 0);
// EXPECT_EQ(iplmService.action_[Message::Ecu::VCM], 0);

//// Verify that availibilitystatus and action are set when we're processing
//// a VCM timeout.
// iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.action_[Message::Ecu::VCM] = 5;
// iplmService.action_[Message::Ecu::SELF] = IpLmServiceStubImpl::ResourceGroup::RG_3;
// iplmService.RequestMonitoringTimeoutVCM(InfotainmentIpService::Utils::BaseTimer::ReasonCode::TIMER_EXPIRED,
// Message::Ecu::VCM);
// EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 0);
// EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 0);
// EXPECT_EQ(iplmService.action_[Message::Ecu::VCM], 0);

//// Verify that availibilitystatus and action are set when we're processing
//// a VCM timeout.
// iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)] = 1;
// iplmService.action_[Message::Ecu::VCM] = 5;
// iplmService.action_[Message::Ecu::SELF] =
// IpLmServiceStubImpl::ResourceGroup::RG_3 | IpLmServiceStubImpl::ResourceGroup::RG_3;
// iplmService.RequestMonitoringTimeoutVCM(InfotainmentIpService::Utils::BaseTimer::ReasonCode::TIMER_EXPIRED,
// Message::Ecu::VCM);
// EXPECT_EQ(iplmService.rg1_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 0);
// EXPECT_EQ(iplmService.rg3_availabilityStatus_[static_cast<int>(IpLmServiceStubImpl::EcuId::ECU_Vcm)], 0);
// EXPECT_EQ(iplmService.action_[Message::Ecu::VCM], 0);
//}

TEST_F(IplmServiceFixture, GetEcuAvailibility)
{
    // uint8_t action = 0;
    // uint8_t prio = 0;

    // // Test unknown/out-of-bounds ecu
    // action = prio = 1;
    // EXPECT_FALSE(iplmService.GetEcuAvailibility(Connectivity::Message::Ecu::UNKNOWN, action, prio));
    // EXPECT_EQ(action, 0);
    // EXPECT_EQ(prio, 0);

    // action = prio = 1;
    // EXPECT_FALSE(iplmService.GetEcuAvailibility(
    //     static_cast<Connectivity::Message::Ecu>(static_cast<int>(Connectivity::Message::Ecu::SELF) + 1), action,
    //     prio));
    // EXPECT_EQ(action, 0);
    // EXPECT_EQ(prio, 0);

    // // Request RG1, AVAILABLE, HIGH_PRIO from VCM
    // std::vector<uint8_t> payload = {IpLmServiceStubImpl::ResourceGroup::RG_1 | IpLmServiceStubImpl::ACTION_AVAILABLE,
    //                                 IpLmServiceStubImpl::Prio::PRIO_HIGH};

    // action = prio = 0;
    // Message m;
    // m.ecu = Message::Ecu::VCM;
    // m.pdu.payload = payload;
    // iplmService.CbLmBroadcast(m);

    // EXPECT_TRUE(iplmService.GetEcuAvailibility(Connectivity::Message::Ecu::VCM, action, prio));
    // EXPECT_EQ(action, IpLmServiceStubImpl::ResourceGroup::RG_1 | IpLmServiceStubImpl::ACTION_AVAILABLE);
    // EXPECT_EQ(prio, IpLmServiceStubImpl::Prio::PRIO_HIGH);

    // payload = {IpLmServiceStubImpl::ResourceGroup::RG_3, IpLmServiceStubImpl::Prio::PRIO_NORM};

    // action = prio = 0;
    // m.ecu = Message::Ecu::TEM;
    // m.pdu.payload = payload;
    // iplmService.CbLmBroadcast(m);

    // EXPECT_TRUE(iplmService.GetEcuAvailibility(Connectivity::Message::Ecu::TEM, action, prio));
    // EXPECT_EQ(action, IpLmServiceStubImpl::ResourceGroup::RG_3);
    // EXPECT_EQ(prio, IpLmServiceStubImpl::Prio::PRIO_NORM);
}

}  // namespace
