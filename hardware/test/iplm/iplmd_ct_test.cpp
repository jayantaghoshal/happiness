/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <ECD_dataelement.h>
#include <ipcb_simulator.h>
#include <sys/types.h>
#include <csignal>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>

#include "gtest/gtest.h"
#include "src/LscMocker.h"

#undef LOG_TAG
#define LOG_TAG "iplm_daemon_test"
#include <cutils/log.h>

using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;

using ::vendor::volvocars::hardware::iplm::V1_0::IIplm;
using ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroup;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroupStatus;

constexpr auto NAMESPACE_PATH = "/dev/vendor/netns/vcc";
constexpr auto TCAM_INTERFACE_NAME = "tcam0_ac:0";
constexpr auto IHU_IP = "198.19.101.66";
constexpr auto BROADCAST_IP = "198.19.101.95";
constexpr auto TCAM_IP = "198.19.101.67";
constexpr auto NETMASK = "255.255.255.224";
constexpr uint16_t IHU_PORT = 50000;
constexpr uint16_t IHU_BROADCAST_PORT = 50001;
constexpr uint16_t TCAM_PORT = 50000;
constexpr uint16_t TCAM_BROADCAST_PORT = 50002;

std::vector<std::pair<Connectivity::Message::Ecu, Connectivity::ISocket::EcuAddress>> EcuMap() {
    using namespace Connectivity;
    return std::vector<std::pair<Message::Ecu, ISocket::EcuAddress>>{
            std::make_pair(Message::Ecu::IHU, ISocket::EcuAddress{IHU_IP, IHU_PORT}),
            std::make_pair(Message::Ecu::ALL, ISocket::EcuAddress{BROADCAST_IP, TCAM_BROADCAST_PORT}),
            std::make_pair(Message::Ecu::TCAM, ISocket::EcuAddress{TCAM_IP, TCAM_PORT})};
}

class IplmTestFixture : public ::testing::Test {
  private:
    static int system(const std::string& cmd) { return std::system(cmd.c_str()); }

  protected:
    static void SetUpTestCase() {
        ALOGD("+ %s", __func__);

        // Move test case into namespace for network mocks to work
        int file_descriptor;

        file_descriptor = open(NAMESPACE_PATH, O_RDONLY | O_CLOEXEC);
        if (file_descriptor > 0) {
            if (setns(file_descriptor, CLONE_NEWNET) != 0) {
                ASSERT_TRUE(false) << "Set NS failed!";
            } else {
                ALOGD("Namespace is: %s", NAMESPACE_PATH);
            }
        } else {
            ASSERT_TRUE(false) << "Open NS filedescriptor failed!";
        }
        close(file_descriptor);

        // loopback interface is needed, bring it up.
        system("/vendor/bin/ifconfig lo up");
        // Set up an alias for the TCAM simulation to bind to.
        system("/vendor/bin/ifconfig " + std::string(TCAM_INTERFACE_NAME) + " " + std::string(TCAM_IP) + " netmask " +
               std::string(NETMASK) + " up");
        // Mangle the packets to spoof TCAM sending broadcast packages.
        system("/vendor/bin/iptables -t nat -I POSTROUTING -s " + std::string(IHU_IP) + " -p udp --dport " +
               std::to_string(IHU_BROADCAST_PORT) + " -j SNAT --to-source " + std::string(TCAM_IP) + " -w");

        // Bridge broadcast ports.
        system("/vendor/bin/iptables -t nat -I OUTPUT -d " + std::string(BROADCAST_IP) + " -p udp --dport " +
               std::to_string(TCAM_BROADCAST_PORT) + " -j DNAT --to-destination " + std::string(BROADCAST_IP) + ":" +
               std::to_string(IHU_BROADCAST_PORT));
        system("/vendor/bin/iptables -t nat -I OUTPUT -d " + std::string(BROADCAST_IP) + " -p udp --dport " +
               std::to_string(IHU_BROADCAST_PORT) + " -j DNAT --to-destination " + std::string(BROADCAST_IP) + ":" +
               std::to_string(TCAM_BROADCAST_PORT));

        /*
         * NATing is only used for the first packet in a connection. For UDP a connection is defined as established as
         * long as the time out after the last received packet does not fire. Therefore, in order to enforce NATing on
         * every packet we set this time out to 0, 30 is the default.
         */
        system("sysctl -w net.netfilter.nf_conntrack_udp_timeout=0");

        ALOGD("- %s", __func__);
    }

    static void TearDownTestCase() {
        ALOGD("+ %s", __func__);

        // Clean up network configurations.
        // Delete tcam alias
        system("/vendor/bin/ifconfig " + std::string(TCAM_INTERFACE_NAME) + " down");
        // Delete mangle rules for spoofing TCAM sending broadcast packages.
        system("/vendor/bin/iptables -t nat -D POSTROUTING -s " + std::string(IHU_IP) + " -p udp --dport " +
               std::to_string(IHU_BROADCAST_PORT) + " -j SNAT --to-source " + std::string(TCAM_IP) + " -w");

        // Delete bridge for broadcast ports.
        system("/vendor/bin/iptables -t nat -D OUTPUT -d " + std::string(BROADCAST_IP) + " -p udp --dport " +
               std::to_string(TCAM_BROADCAST_PORT) + " -j DNAT --to-destination " + std::string(BROADCAST_IP) + ":" +
               std::to_string(IHU_BROADCAST_PORT));
        system("/vendor/bin/iptables -t nat -D OUTPUT -d " + std::string(BROADCAST_IP) + " -p udp --dport " +
               std::to_string(IHU_BROADCAST_PORT) + " -j DNAT --to-destination " + std::string(BROADCAST_IP) + ":" +
               std::to_string(TCAM_BROADCAST_PORT));

        // Restore UDP connection timeout
        system("sysctl -w net.netfilter.nf_conntrack_udp_timeout=30");

        // Restart CUTs.
        system("stop iplmd");
        system("stop ipcbd-iplm");
        system("start ipcbd-iplm");
        system("start iplmd");

        ALOGD("- %s", __func__);
    }

    void SetUp() override {
        ALOGD("+ %s", __func__);

        system("stop iplmd");
        system("stop ipcbd-iplm");
        system("start ipcbd-iplm");
        system("start iplmd");

        lsc_mocker_ = new LscMocker();
        ALOGD("- %s", __func__);
    }

    void TearDown() override {
        // code here will be called just after the test completes
        // ok to throw exceptions from here if need be
        ALOGD("+ %s", __func__);

        lsc_mocker_.clear();

        ALOGD("- %s", __func__);
    }

    // put in any custom data members that you need
    sp<LscMocker> lsc_mocker_;
};

/**
 Test that IplmD can receive an activity message
 **/
TEST_F(IplmTestFixture, ReceiveActivityMessage) {
    ALOGD("+ ReceiveActivityMessage");
    ALOGD("Starting test");
    bool promise_complete = false;

    vcc::ipcb::testing::IpcbSimulator ipcb_simulator(EcuMap());
    ipcb_simulator.Initialize(Connectivity::Message::Ecu::TCAM, "UDPB");

    ALOGD("Simulator created..");

    // Create activity message
    Connectivity::Pdu temp_pdu;
    temp_pdu.createHeader(0xFFFF,
                          0xFF01,
                          Connectivity::IpCmdTypes::OperationType::NOTIFICATION_CYCLIC,
                          Connectivity::IpCmdTypes::DataType::NOT_ENCODED,
                          1);
    temp_pdu.header.protocol_version = 3;
    temp_pdu.setPayload(std::vector<uint8_t>({1, 0, 0, 0}));
    Connectivity::Message msg;
    msg.ecu = Connectivity::Message::Ecu::ALL;
    msg.pdu = temp_pdu;

    ALOGD("Message created...");
    {
        ALOGD("Promise");
        std::promise<void> promise;
        std::future<void> future = promise.get_future();
        ALOGD("After Future");

        lsc_mocker_->RegisterLSC("iplmd-test");
        ALOGD("Registered");
        lsc_mocker_->onNodeStatusCallback = [&](Ecu ecuType, bool ecuStatus) {
            ALOGD("+ onNodeStatusCallback [%hhu] ecuStatus: %s, completed: %s",
                  ecuType,
                  ecuStatus ? "true" : "false",
                  promise_complete ? "true" : "false");
            if (ecuStatus && !promise_complete) {
                promise_complete = true;
                promise.set_value();
            }
            ALOGD("- onNodeStatusCallback");
        };

        ipcb_simulator.SendMessage(std::move(msg));

        std::future_status status = future.wait_for(std::chrono::milliseconds(3000));
        ASSERT_NE(std::future_status::deferred, status) << "Promise deferred exiting";
        ASSERT_NE(std::future_status::timeout, status)
                << "Timeout: Didn't recive any gnss message within timelimit. Exiting";
        ASSERT_EQ(std::future_status::ready, status) << "Unsuccessful";
    }
    lsc_mocker_->UnregisterLSC("iplmd-test");
    ALOGD("- ReceiveActivityMessage");
}

/*
 Register and Unregister Local software Components.
 */
TEST_F(IplmTestFixture, RegisterUnRegisterLSC) {
    ALOGD("+ RegisterUnRegisterLSC");

    std::promise<void> pOnResource;
    std::future<void> fOnResource = pOnResource.get_future();

    std::promise<void> pOnNodeStatus;
    std::future<void> fOnNodeStatus = pOnNodeStatus.get_future();

    int onResourceGroupStatusCallbackCounter = 0;
    int onNodeStatusCallbackCounter = 0;

    lsc_mocker_->onResourceGroupStatusCallback = [&pOnResource, &onResourceGroupStatusCallbackCounter](
            ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) {
        (void)resourceGroupStatus;
        ALOGV("+ onResourceGroupStatusCallback");
        EXPECT_TRUE(resourceGroup == ResourceGroup::ResourceGroup1 || resourceGroup == ResourceGroup::ResourceGroup3);
        onResourceGroupStatusCallbackCounter++;
        if (onResourceGroupStatusCallbackCounter == 1) {
            pOnResource.set_value();
        }

        ALOGV("- onResourceGroupStatusCallback");
    };

    lsc_mocker_->onNodeStatusCallback = [&pOnNodeStatus, &onNodeStatusCallbackCounter](Ecu ecuType, bool ecuStatus) {
        (void)ecuStatus;
        ALOGV("+ onNodeStatusCallback");
        EXPECT_TRUE(ecuType == Ecu::TEM || ecuType == Ecu::VCM);
        onNodeStatusCallbackCounter++;
        if (onNodeStatusCallbackCounter == 1) {
            pOnNodeStatus.set_value();
        }
        ALOGV("- onNodeStatusCallback");
    };

    ALOGD("registerService iplmdtest");
    lsc_mocker_->RegisterLSC("iplmd-test");

    std::future_status status = fOnResource.wait_for(std::chrono::seconds(2));
    ASSERT_EQ(status, std::future_status::ready) << "Timout or deferred!";

    status = fOnNodeStatus.wait_for(std::chrono::seconds(2));
    ASSERT_EQ(status, std::future_status::ready) << "Timout or deferred!";

    ALOGD("unregisterService iplmdtest");
    lsc_mocker_->UnregisterLSC("iplmd-test");
    int onResourceGroupStatusCallbackCounterBefore = onResourceGroupStatusCallbackCounter;
    int onNodeStatusCallbackCounterBefore = onNodeStatusCallbackCounter;

    ALOGD("Sleeping for 2 sec");
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);

    // Shouldnt have increased counter since we have unregistered the LSC
    EXPECT_TRUE(onResourceGroupStatusCallbackCounter == onResourceGroupStatusCallbackCounterBefore);
    EXPECT_TRUE(onNodeStatusCallbackCounter == onNodeStatusCallbackCounterBefore);

    ALOGD("- RegisterUnRegisterLSC");
}

/*
IPLM Inform other LM modules across the vehicle internal IP network when the ECU has all its services available.
Following test will register 3 Local Software components and will expect broadcast messages from IPLM
*/
TEST_F(IplmTestFixture, DoSendBroadcast) {
    ALOGD("+ DoSendBroadcast");

    std::promise<void> promise;
    std::future<void> future = promise.get_future();

    vcc::ipcb::testing::IpcbSimulator ipcb_simulator(EcuMap());
    ipcb_simulator.Initialize(Connectivity::Message::Ecu::TCAM, "UDPB");
    uint64_t register_id = ipcb_simulator.RegisterMessageCallback(
            0xFFFF,
            0xFF01,
            Connectivity::IpCmdTypes::OperationType::NOTIFICATION_CYCLIC,
            [&promise, &register_id](Connectivity::Message& msg, uint64_t& id) -> bool {
                (void)msg;
                EXPECT_EQ(register_id, id);
                promise.set_value();
                return true;
            });

    std::future_status status = future.wait_for(std::chrono::seconds(10));

    ASSERT_NE(std::future_status::deferred, status) << "Promise deferred exiting";
    ASSERT_NE(std::future_status::timeout, status)
            << "Timeout: Didn't recive any gnss message within timelimit. Exiting";
    ASSERT_EQ(std::future_status::ready, status) << "Unsuccessful";

    ALOGD("- DoSendBroadcast");
}

// Request services by Resource Group 1 and Prio Normal
TEST_F(IplmTestFixture, RequestResourceGroup_RG1_NORM) {
    // Need to register atleast three LSCs in order IPLM to star broadcasting message
    ALOGD("+ RequestResourceGroup_RG1_NORM");

    std::promise<void> pOnResource;
    std::future<void> fOnResource = pOnResource.get_future();

    std::promise<void> pOnNodeStatus;
    std::future<void> fOnNodeStatus = pOnNodeStatus.get_future();

    int onResourceGroupStatusCallbackCounter = 0;
    int onNodeStatusCallbackCounter = 0;

    lsc_mocker_->onResourceGroupStatusCallback = [&pOnResource, &onResourceGroupStatusCallbackCounter](
            ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) {
        (void)resourceGroupStatus;
        ALOGV("+ onResourceGroupStatusCallback");
        EXPECT_TRUE(resourceGroup == ResourceGroup::ResourceGroup1 || resourceGroup == ResourceGroup::ResourceGroup3);
        onResourceGroupStatusCallbackCounter++;
        if (onResourceGroupStatusCallbackCounter == 1) {
            pOnResource.set_value();
        }

        ALOGV("- onResourceGroupStatusCallback");
    };

    lsc_mocker_->onNodeStatusCallback = [&pOnNodeStatus, &onNodeStatusCallbackCounter](Ecu ecuType, bool ecuStatus) {
        (void)ecuStatus;
        ALOGV("+ onNodeStatusCallback");
        EXPECT_TRUE(ecuType == Ecu::TEM || ecuType == Ecu::VCM);
        onNodeStatusCallbackCounter++;
        if (onNodeStatusCallbackCounter == 1) {
            pOnNodeStatus.set_value();
        }
        ALOGV("- onNodeStatusCallback");
    };

    lsc_mocker_->RegisterLSC("VOC");
    lsc_mocker_->RegisterLSC("OTA");
    lsc_mocker_->RegisterLSC("CSB");

    std::future_status status = fOnResource.wait_for(std::chrono::seconds(2));
    ASSERT_EQ(status, std::future_status::ready) << "Timout or deferred!";

    status = fOnNodeStatus.wait_for(std::chrono::seconds(2));
    ASSERT_EQ(status, std::future_status::ready) << "Timout or deferred!";

    const hidl_string& lscName("VOC");
    ResourceGroup _rg = ResourceGroup::ResourceGroup1;
    ResourceGroupPrio _prio = ResourceGroupPrio::Normal;
    Connectivity::Pdu pdu;

    // Request resource group
    lsc_mocker_->RequestResourceGroup(lscName, _rg, _prio);

    vcc::ipcb::testing::IpcbSimulator ipcb_simulator(EcuMap());
    ipcb_simulator.Initialize(Connectivity::Message::Ecu::TCAM, "UDPB");

    // Check the comming 5 PDUs to see if payload is correct
    std::promise<void> promise;
    std::future<void> f_pdu_received = promise.get_future();
    int count = 0;

    ipcb_simulator.RegisterMessageCallback(
            0xFFFF,
            0xFF01,
            Connectivity::IpCmdTypes::OperationType::NOTIFICATION_CYCLIC,
            [&_rg, &_prio, &promise, &count](Connectivity::Message& msg, uint64_t& id) -> bool {
                (void)id;
                std::vector<uint8_t> payload(msg.pdu.payload);

                // get resource group bit which is stored in first bit
                uint8_t received_action = payload[0];
                uint8_t received_prio = payload[1];

                if (((received_action & _rg) == static_cast<int8_t>(_rg)) &&
                    ((received_prio & _prio) == static_cast<int8_t>(_prio))) {
                    count++;
                    if (5 == count) {
                        promise.set_value();
                    }
                }
                return true;
            });

    status = f_pdu_received.wait_for(std::chrono::seconds(10));
    ASSERT_NE(std::future_status::deferred, status) << "Promise deferred exiting";
    ASSERT_NE(std::future_status::timeout, status)
            << "Timeout: Didn't recive any gnss message within timelimit. Exiting";
    ASSERT_EQ(std::future_status::ready, status) << "Unsuccessful";

    // Unregistering LSCs
    lsc_mocker_->UnregisterLSC("VOC");
    lsc_mocker_->UnregisterLSC("OTA");
    lsc_mocker_->UnregisterLSC("CSB");

    lsc_mocker_->onResourceGroupStatusCallback = nullptr;
    lsc_mocker_->onNodeStatusCallback = nullptr;

    ALOGD("- RequestResourceGroup_RG1_NORM");
}

// Request services by Resource Group 3 and Prio High
TEST_F(IplmTestFixture, RequestResourceGroup_RG3_HIGH) {
    ALOGD("+ RequestResourceGroup_RG3_HIGH");

    std::promise<void> pOnResource;
    std::future<void> fOnResource = pOnResource.get_future();

    std::promise<void> pOnNodeStatus;
    std::future<void> fOnNodeStatus = pOnNodeStatus.get_future();

    int onResourceGroupStatusCallbackCounter = 0;
    int onNodeStatusCallbackCounter = 0;

    ALOGV("Registering callbacks..");

    lsc_mocker_->onResourceGroupStatusCallback = [&pOnResource, &onResourceGroupStatusCallbackCounter](
            ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) {
        (void)resourceGroupStatus;
        ALOGV("+ onResourceGroupStatusCallback");
        EXPECT_TRUE(resourceGroup == ResourceGroup::ResourceGroup1 || resourceGroup == ResourceGroup::ResourceGroup3);
        onResourceGroupStatusCallbackCounter++;
        if (onResourceGroupStatusCallbackCounter == 1) {
            pOnResource.set_value();
        }
        ALOGV("- onResourceGroupStatusCallback");
    };

    lsc_mocker_->onNodeStatusCallback = [&pOnNodeStatus, &onNodeStatusCallbackCounter](Ecu ecuType, bool ecuStatus) {
        (void)ecuStatus;
        ALOGV("+ onNodeStatusCallback");
        EXPECT_TRUE(ecuType == Ecu::TEM || ecuType == Ecu::VCM);
        onNodeStatusCallbackCounter++;
        if (onNodeStatusCallbackCounter == 1) {
            pOnNodeStatus.set_value();
        }
        ALOGV("- onNodeStatusCallback");
    };

    ALOGV("Setup complete..");

    // Need to register atleast three LSCs in order IPLM to start broadcasting message
    lsc_mocker_->RegisterLSC("VOC");
    lsc_mocker_->RegisterLSC("OTA");
    lsc_mocker_->RegisterLSC("CSB");

    ALOGD("registerService iplmdtest");
    lsc_mocker_->RegisterLSC("iplmd-test");

    std::future_status status = fOnResource.wait_for(std::chrono::seconds(2));
    ASSERT_EQ(status, std::future_status::ready) << "Timout or deferred!";

    status = fOnNodeStatus.wait_for(std::chrono::seconds(2));
    ASSERT_EQ(status, std::future_status::ready) << "Timout or deferred!";

    const hidl_string& lscName("VOC");
    ResourceGroup _rg = ResourceGroup::ResourceGroup3;
    ResourceGroupPrio _prio = ResourceGroupPrio::High;
    Connectivity::Pdu pdu;

    lsc_mocker_->RequestResourceGroup(lscName, _rg, _prio);

    ALOGV("Initializing simulator...");

    vcc::ipcb::testing::IpcbSimulator ipcb_simulator(EcuMap());
    ipcb_simulator.Initialize(Connectivity::Message::Ecu::TCAM, "UDPB");

    // Check the comming 5 PDUs to see if payload is correct
    std::promise<void> promise;
    std::future<void> f_pdu_received = promise.get_future();
    int count = 0;

    ipcb_simulator.RegisterMessageCallback(
            0xFFFF,
            0xFF01,
            Connectivity::IpCmdTypes::OperationType::NOTIFICATION_CYCLIC,
            [&_rg, &_prio, &promise, &count](Connectivity::Message& msg, uint64_t& id) -> bool {
                ALOGV("Recevied a call...");
                (void)id;
                std::vector<uint8_t> payload(msg.pdu.payload);

                // get resource group bit which is stored in first bit
                uint8_t received_action = payload[0];
                uint8_t received_prio = payload[1];

                if (((received_action & _rg) == static_cast<int8_t>(_rg)) &&
                    ((received_prio & _prio) == static_cast<int8_t>(_prio))) {
                    count++;
                    if (5 == count) {
                        promise.set_value();
                    }
                }
                return true;
            });

    ALOGV("Waiting for 5 calls..");

    status = f_pdu_received.wait_for(std::chrono::seconds(10));
    ASSERT_NE(std::future_status::deferred, status) << "Promise deferred exiting";
    ASSERT_NE(std::future_status::timeout, status)
            << "Timeout: Didn't recive any gnss message within timelimit. Exiting";
    ASSERT_EQ(std::future_status::ready, status) << "Unsuccessful";

    // Unregistering LSCs
    lsc_mocker_->UnregisterLSC("VOC");
    lsc_mocker_->UnregisterLSC("OTA");
    lsc_mocker_->UnregisterLSC("CSB");

    lsc_mocker_->onResourceGroupStatusCallback = nullptr;
    lsc_mocker_->onNodeStatusCallback = nullptr;

    ALOGD("- RequestResourceGroup_RG3_HIGH");
}

// TODO(Ali): Verify if this test case is producing correct result.
TEST_F(IplmTestFixture, WakeUpOnFlexRayTest) {
    ALOGD("+ WakeUpOnFlexRayTest");

    std::promise<void> pFlexrayWakeUp;
    std::future<void> fFlexrayWakeUp = pFlexrayWakeUp.get_future();

    unsigned int count = 0;
    ECDDataElement::DESink<autosar::NetHdActvt_info> flexraySink;
    flexraySink.subscribe([&pFlexrayWakeUp, &count, &flexraySink]() {
        auto value = flexraySink.get().value();
        // EXPECT_EQ(value.Prio, autosar::PrioHighNormal::PrioNormal);
        // EXPECT_EQ(value.ResourceGroup, 0 /*IplmService::ResourceGroup::RG_Reserved*/);
        ALOGD("ResourceGroup %d", value.ResourceGroup);
        ALOGD("Prio %d", value.Prio);
        if (value.Prio == autosar::PrioHighNormal::PrioNormal &&
            value.ResourceGroup == 2 /*IplmService::ResourceGroup::RG_1*/) {
            if (1 == ++count) {
                pFlexrayWakeUp.set_value();
            }
        }
        return Void();
    });

    ALOGD("registerService iplmdtest1");
    lsc_mocker_->RegisterLSC("iplmd-test1");
    ALOGD("registerService iplmdtest2");
    lsc_mocker_->RegisterLSC("iplmd-test2");
    ALOGD("registerService iplmdtest3");
    lsc_mocker_->RegisterLSC("iplmd-test3");

    using namespace std::chrono_literals;
    std::future_status status = fFlexrayWakeUp.wait_for(5s);

    ASSERT_NE(std::future_status::deferred, status) << "Promise deferred exiting";
    ASSERT_NE(std::future_status::timeout, status)
            << "Timeout: Didn't recive any flexray wakeup message within timelimit. Exiting";
    ASSERT_EQ(std::future_status::ready, status) << "Unsuccessful";

    ALOGD("unregisterService iplmd-test1");
    lsc_mocker_->UnregisterLSC("iplmd-test1");
    ALOGD("unregisterService iplmd-test2");
    lsc_mocker_->UnregisterLSC("iplmd-test2");
    ALOGD("unregisterService iplmd-test3");
    lsc_mocker_->UnregisterLSC("iplmd-test3");

    ALOGD("- WakeUpOnFlexRayTest");
}