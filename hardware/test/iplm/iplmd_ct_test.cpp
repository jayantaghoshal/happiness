/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <ECD_dataelement.h>
#include <cutils/log.h>
#include <ipcb_simulator.h>
#include <signal.h>
#include <sys/types.h>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>

#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>

#include "gtest/gtest.h"
#include "src/LscMocker.h"

#define LOG_TAG "iplm_daemon_test"

using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;

using ::vendor::volvocars::hardware::iplm::V1_0::IIplm;
using ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroup;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroupStatus;

static int new_ipcb_pid = -1;
static int new_iplm_pid = -1;

static bool setup_test_case_successful = false;

class IplmTestFixture : public ::testing::Test {
  protected:
    IplmTestFixture() {}

    static void SetUpTestCase() {
        ALOGD("+ SetUpTestCase ");

        // Expected that all tests would be aborted here, but that is not the case.
        // For now, abort all tests manually
        ASSERT_TRUE(fileExists("/data/local/tmp/localconfig.json"));

        // Stop IpcbD serving IplmD
        ALOGD("SetUpTestCase: Stopping ipcbd-iplm");
        getCmdOut("stop ipcbd-iplm");

        // Stop conflicting IplmD
        ALOGD("SetUpTestCase: Stopping iplmd");
        getCmdOut("stop iplmd");

        // Start IpcbD for test with mocked localconfig
        std::string new_ipcb_pid_str = getCmdOut(
                "VCC_LOCALCONFIG_PATH=/data/local/tmp/localconfig.json /system/bin/ip netns exec vcc "
                "/vendor/bin/hw/ipcbd iplm UDPB "
                "& echo $!");

        std::string::size_type sz;  // alias of size_t
        new_ipcb_pid = std::stoi(new_ipcb_pid_str, &sz);

        // Wait for service to start
        uint8_t count = 0;
        while (NULL == IVehicleCom::getService("iplm").get()) {
            usleep(100000);

            if (!processExists(new_ipcb_pid)) {
                ASSERT_TRUE(false) << "PID lost while waiting for service to be registered";
            }

            if (20 == ++count) {
                ASSERT_TRUE(false) << "Timed out while waiting for service to be registered";
            }
        }

        // Start IpcbD for test with mocked localconfig
        std::string new_iplm_pid_str = getCmdOut(
                "VCC_LOCALCONFIG_PATH=/data/local/tmp/localconfig.json /vendor/bin/hw/iplmd "
                "& echo $!");

        new_iplm_pid = std::stoi(new_iplm_pid_str, &sz);

        // Wait for service to start
        count = 0;
        while (NULL == IIplm::getService().get()) {
            usleep(100000);

            if (!processExists(new_iplm_pid)) {
                ASSERT_TRUE(false) << "PID lost while waiting for service to be registered";
            }

            if (20 == ++count) {
                ASSERT_TRUE(false) << "Timed out while waiting for service to be registered";
            }
        }

        // Move test case into namespace for network mocks to work
        int fileDescriptor;
        std::string nameSpace = "/var/run/netns/vcc";

        fileDescriptor = open(nameSpace.c_str(), O_RDONLY);
        if (fileDescriptor > 0) {
            if (setns(fileDescriptor, CLONE_NEWNET)) {
                ASSERT_TRUE(false) << "Set NS failed!";
            } else {
                ALOGD("+ SetUpTestCase - Namespace is: %s", nameSpace.c_str());
            }
        } else {
            ASSERT_TRUE(false) << "Open NS filedescriptor failed!";
        }

        setup_test_case_successful = true;

        ALOGD("- SetUpTestCase ");
    }

    static bool processExists(int pid) {
        // Calling kill with signal 0 will just return 0 if prcess is running
        return (0 == kill(pid, 0));
    }

    static bool fileExists(const std::string& name) {
        if (FILE* file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        } else {
            return false;
        }
    }

    static int getProcIdByName(std::string procName) {
        int pid = -1;

        // Open the /proc directory
        DIR* dp = opendir("/proc");
        if (dp != NULL) {
            // Enumerate all entries in directory until process found
            struct dirent* dirp;
            while (pid < 0 && (dirp = readdir(dp))) {
                // Skip non-numeric entries
                int id = atoi(dirp->d_name);
                if (id > 0) {
                    // Read contents of virtual /proc/{pid}/cmdline file
                    std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                    std::ifstream cmdFile(cmdPath.c_str());
                    std::string cmdLine;
                    getline(cmdFile, cmdLine);
                    replace(cmdLine.begin(), cmdLine.end(), '\0', ' ');
                    if (!cmdLine.empty()) {
                        cmdLine.erase(cmdLine.end() - 1);  // Remove the last character
                        if (procName == cmdLine) pid = id;
                    }
                }
            }
        }

        closedir(dp);

        return pid;
    }

    static std::string getCmdOut(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            ALOGE("popen() FAILED");
        }
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != NULL) {
                result += buffer.data();
                break;  // break it since we just interested to get process id
            }
        }
        return result;
    }

    static void TearDownTestCase() {
        ALOGD("+ TearDownTestCase");

        // Clean up, kill started processes
        if (-1 != new_ipcb_pid) {
            kill(new_ipcb_pid, SIGTERM);
        }

        if (-1 != new_iplm_pid) {
            kill(new_iplm_pid, SIGTERM);
        }

        // Restart IpcbD for IplmD
        ALOGD("TearDownTestCase: Starting ipcbd-iplm");
        getCmdOut("start ipcbd-iplm");

        // Restart IplmD
        ALOGD("TearDownTestCase: Starting iplmd");
        getCmdOut("start iplmd");

        ALOGD("- TearDownTestCase");
    }

    void SetUp() {
        ALOGD("+ SetUp ");

        lscMocker = new LscMocker();
        onNodeStatusCallbackCounter = 0;
        onResourceGroupStatusCallbackCounter = 0;

        // Make sure Test Case Setup was executed correctly
        ASSERT_TRUE(setup_test_case_successful) << "Setup Test Case failed, failing test";

        // Make sure IpcbD is still running
        ASSERT_TRUE(processExists(new_ipcb_pid)) << "IpcbD is not running anymore, did it crash?";

        // Make sure IplmD is still running
        ASSERT_TRUE(processExists(new_iplm_pid)) << "IplmD is not running anymore, did it crash?";
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to through exceptions from here if need be
    }

    ~IplmTestFixture() {}

    // put in any custom data members that you need
    sp<LscMocker> lscMocker;
    int onNodeStatusCallbackCounter;
    int onResourceGroupStatusCallbackCounter;
    std::future_status status;
    std::promise<bool> pOnResource;
    std::promise<bool> pOnNodeStatus;
    std::future<bool> fOnResource = pOnResource.get_future();
    std::future<bool> fOnNodeStatus = pOnNodeStatus.get_future();
};

/**
 Test that IplmD can receive an activity message
 **/
TEST_F(IplmTestFixture, ReciveActivityMessage) {
    ALOGD("+ ReciveActivityMessage");

    // Setup iplm. Wait for internal properties to clear.
    {
        std::promise<int> promise;
        std::future<int> future = promise.get_future();

        lscMocker->RegisterLSC("iplmd-test");
        lscMocker->onNodeStatusCallback = [&](Ecu ecuType, bool ecuStatus) {
            ALOGD("+ onNodeStatusCallback");
            onResourceGroupStatusCallbackCounter++;
            if (!ecuStatus && onResourceGroupStatusCallbackCounter == 1) {
                promise.set_value(true);
            }
            ALOGD("- onNodeStatusCallback");
        };

        std::future_status status = future.wait_for(std::chrono::milliseconds(3000));
        if (status == std::future_status::deferred) {
            ASSERT_TRUE(false) << "Preconditions not met. Promise deferred exiting";
        } else if (status == std::future_status::timeout) {
            ASSERT_TRUE(false)
                    << "Preconditions not met. Timeout: Didn't recive any keep alive message within timelimit. Exiting";
        }
        ALOGD("- Setup complete");
    }

    ALOGD("- Starting test");
    bool promise_complete = false;
    Pdu temp_pdu;

    // Create activity message
    IpcbSimulator ipcbSimulator("198.18.255.255", 60000, 70000, 1);
    temp_pdu.createHeader(
            0xFFFF, 0xFF01, IpCmdTypes::OperationType::NOTIFICATION_CYCLIC, IpCmdTypes::DataType::NOT_ENCODED, 1);
    temp_pdu.header.protocol_version = 2;
    temp_pdu.setPayload(std::vector<uint8_t>({0x01, (uint8_t)0x00, 0, 0}));
    ipcbSimulator.SendPdu(temp_pdu);

    {
        std::promise<int> promise;
        std::future<int> future = promise.get_future();

        lscMocker->RegisterLSC("iplmd-test");
        lscMocker->onNodeStatusCallback = [&](Ecu ecuType, bool ecuStatus) {
            ALOGD("+ onNodeStatusCallback ecuStatus: %s, completed: %s",
                  ecuStatus ? "true" : "false",
                  promise_complete ? "true" : "false");
            if (ecuStatus && !promise_complete) {
                promise_complete = true;
                promise.set_value(true);
            }

            ALOGD("- onNodeStatusCallback");
        };

        std::future_status status = future.wait_for(std::chrono::milliseconds(3000));
        if (status == std::future_status::deferred) {
            ASSERT_TRUE(false) << "Promise deferred exiting";
        } else if (status == std::future_status::timeout) {
            ASSERT_TRUE(false) << "Timeout:Didn't recive any keep alive message within timelimit. Exiting";
        } else if (status == std::future_status::ready) {
            ALOGD("Success!");
        }
    }
    lscMocker->UnregisterLSC("iplmd-test");
    ALOGD("- ReciveActivityMessage");
}

/*
 Register and Unregister Local software Components.
 */
TEST_F(IplmTestFixture, RegisterUnRegisterLSC) {
    ALOGD("+ RegisterUnRegisterLSC");

    lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup,
                                                   ResourceGroupStatus resourceGroupStatus) {
        ALOGD("+ onResourceGroupStatusCallback");
        EXPECT_TRUE(resourceGroup == ResourceGroup::ResourceGroup1 || resourceGroup == ResourceGroup::ResourceGroup3);
        onResourceGroupStatusCallbackCounter++;
        if (onResourceGroupStatusCallbackCounter == 1) {
            pOnResource.set_value(true);
        }

        ALOGD("- onResourceGroupStatusCallback");
    };

    lscMocker->onNodeStatusCallback = [&](Ecu ecuType, bool ecuStatus) {
        ALOGD("+ onNodeStatusCallback");
        EXPECT_TRUE(ecuType == Ecu::TEM || ecuType == Ecu::VCM);
        onNodeStatusCallbackCounter++;
        if (onNodeStatusCallbackCounter == 1) {
            pOnNodeStatus.set_value(true);
        }
        ALOGD("- onNodeStatusCallback");
    };

    ALOGD("registerService iplmdtest");
    lscMocker->RegisterLSC("iplmd-test");
    status = fOnResource.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool calledOnResource = fOnResource.get();
    ALOGD("Called fOnResource %d", calledOnResource);
    EXPECT_TRUE(calledOnResource);

    status = fOnNodeStatus.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool calledNodeStatus = fOnNodeStatus.get();
    ALOGD("Called fOnNodeStatus %d", calledNodeStatus);
    EXPECT_TRUE(calledNodeStatus);

    ALOGD("unregisterService iplmdtest");
    lscMocker->UnregisterLSC("iplmd-test");
    int onResourceGroupStatusCallbackCounterBefore = onResourceGroupStatusCallbackCounter;
    int onNodeStatusCallbackCounterBefore = onNodeStatusCallbackCounter;

    ALOGD("Sleeping for 2 sec");
    usleep(1000000 * 2);

    // Shouldnt have increased counter since we have unregistered the LSC
    EXPECT_TRUE(onResourceGroupStatusCallbackCounter == onNodeStatusCallbackCounterBefore);
    EXPECT_TRUE(onNodeStatusCallbackCounter == onNodeStatusCallbackCounterBefore);

    ALOGD("- RegisterUnRegisterLSC");
}

/*
IPLM Inform other LM modules across the vehicle internal IP network when the ECU has all its services available.
Following test will register 3 Local Software components and will expect broadcast messages from IPLM
*/
TEST_F(IplmTestFixture, DoSendBroadcast) {
    ALOGD("+ DoSendBroadcast");

    lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup,
                                                   ResourceGroupStatus resourceGroupStatus) {
        EXPECT_TRUE(resourceGroup == ResourceGroup::ResourceGroup1 || resourceGroup == ResourceGroup::ResourceGroup3);
        onResourceGroupStatusCallbackCounter++;
        if (onResourceGroupStatusCallbackCounter == 1) {
            pOnResource.set_value(true);
        }
    };

    lscMocker->onNodeStatusCallback = [&](Ecu ecuType, bool ecuStatus) {
        EXPECT_TRUE(ecuType == Ecu::TEM || ecuType == Ecu::VCM);
        onNodeStatusCallbackCounter++;
        if (onNodeStatusCallbackCounter == 1) {
            pOnNodeStatus.set_value(true);
        }
    };

    // Lets register 3 services for IPLM to start broadcasting messages from iplm
    for (int i = 0; i < 3; i++) {
        lscMocker->RegisterLSC("lsc-" + std::to_string(i));
        ALOGD("RegisterLSC: %d", i);
    }

    status = fOnResource.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool calledOnResource = fOnResource.get();
    ALOGD("Called fOnResource %d", calledOnResource);
    EXPECT_TRUE(calledOnResource);

    status = fOnNodeStatus.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool calledNodeStatus = fOnNodeStatus.get();
    ALOGD("Called fOnNodeStatus %d", calledNodeStatus);
    EXPECT_TRUE(calledNodeStatus);

    // After 3 registered LSCs we should receive broadcast messages.
    IpcbSimulator ipcbSimulator("198.18.255.255", 60000, 70000, 1);

    Pdu pdu;

    bool result = ipcbSimulator.ReceivePdu(pdu, 3);

    for (int i = 0; i < 3; i++) {
        lscMocker->UnregisterLSC("lsc-" + std::to_string(i));
        ALOGD("UnregisterLSC - %d", i);
    }

    EXPECT_EQ(result, true);
    ALOGD("- DoSendBroadcast");
}

// IPLM should not broadcast messages if less than 3 LSCs registered
// NOTE! This will fail once the "correct" handling is in where IplmD should never
// stop sending Activity messages once it has started.... At that point, this testcase
// needs to be fixed or removed
TEST_F(IplmTestFixture, DoNotSendBroadcast) {
    ALOGD("+ DoNotSendBroadcast");

    // Start simulator to receive broadcasts
    IpcbSimulator ipcbSimulator("198.18.255.255", 60000, 70000, 1);

    Pdu pdu;

    // Read off all old PDUs, to make sure inbuffer is empty
    ipcbSimulator.ReceivePdu(pdu, 3);

    // Expect no new PDUs to be received by simulator
    EXPECT_FALSE(ipcbSimulator.ReceivePdu(pdu, 5));

    ALOGD("- DoNotSendBroadcast");
}

// Request services by Resource Group 1 and Prio Normal
TEST_F(IplmTestFixture, RequestResourceGroup_RG1_NORM) {
    // Need to register atleast three LSCs in order IPLM to star broadcasting message
    ALOGD("+ RequestResourceGroup_RG1_NORM");

    lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup,
                                                   ResourceGroupStatus resourceGroupStatus) {
        ALOGD("+ onResourceGroupStatusCallback");
        EXPECT_TRUE(resourceGroup == ResourceGroup::ResourceGroup1 || resourceGroup == ResourceGroup::ResourceGroup3);
        onResourceGroupStatusCallbackCounter++;
        if (onResourceGroupStatusCallbackCounter == 1) {
            pOnResource.set_value(true);
        }

        ALOGD("- onResourceGroupStatusCallback");
    };

    lscMocker->onNodeStatusCallback = [&](Ecu ecuType, bool ecuStatus) {
        ALOGD("+ onNodeStatusCallback");
        EXPECT_TRUE(ecuType == Ecu::TEM || ecuType == Ecu::VCM);
        onNodeStatusCallbackCounter++;
        if (onNodeStatusCallbackCounter == 1) {
            pOnNodeStatus.set_value(true);
        }
        ALOGD("- onNodeStatusCallback");
    };

    lscMocker->RegisterLSC("VOC");
    lscMocker->RegisterLSC("OTA");
    lscMocker->RegisterLSC("CSB");

    status = fOnResource.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool calledOnResource = fOnResource.get();
    ALOGD("Called fOnResource %d", calledOnResource);
    EXPECT_TRUE(calledOnResource);

    status = fOnNodeStatus.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool calledNodeStatus = fOnNodeStatus.get();
    ALOGD("Called fOnNodeStatus %d", calledNodeStatus);
    EXPECT_TRUE(calledNodeStatus);

    const hidl_string& lscName("VOC");
    ResourceGroup _rg = ResourceGroup::ResourceGroup1;
    ResourceGroupPrio _prio = ResourceGroupPrio::Normal;
    IpcbSimulator ipcbSimulator("198.18.255.255", 60000, 70000, 1);
    Pdu pdu;

    // Request resource group
    lscMocker->RequestResourceGroup(lscName, _rg, _prio);

    // Check the comming 5 PDUs to see if payload is correct
    std::future<bool> f_pdu_received = std::async([&]() {
        for (int i = 1; i < 6; i++) {
            bool IsPduRecevied = ipcbSimulator.ReceivePdu(pdu, 2);
            ALOGD("Received PDU number %d", i);

            if (IsPduRecevied) {
                std::vector<uint8_t> payload(pdu.payload);

                // get resource group bit which is stored in first bit
                uint8_t received_action = payload[0];
                uint8_t received_prio = payload[1];

                if (((received_action & _rg) == (int8_t)_rg) && ((received_prio & _prio) == (int8_t)_prio)) {
                    ALOGD("Payload is correct, ending loop");
                    return true;
                }
            }
        }
        ALOGD("Payload was not correct after five tries, return false");
        return false;
    });

    if (f_pdu_received.wait_for(std::chrono::milliseconds(10000)) == std::future_status::timeout) ASSERT_TRUE(false);
    EXPECT_TRUE(f_pdu_received.get());

    // Unregistering LSCs
    lscMocker->UnregisterLSC("VOC");
    lscMocker->UnregisterLSC("OTA");
    lscMocker->UnregisterLSC("CSB");

    ALOGD("- RequestResourceGroup_RG1_NORM");
}

// Request services by Resource Group 3 and Prio High
TEST_F(IplmTestFixture, RequestResourceGroup_RG3_HIGH) {
    ALOGD("+ RequestResourceGroup_RG3_HIGH");

    lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup,
                                                   ResourceGroupStatus resourceGroupStatus) {
        ALOGD("+ onResourceGroupStatusCallback");
        EXPECT_TRUE(resourceGroup == ResourceGroup::ResourceGroup1 || resourceGroup == ResourceGroup::ResourceGroup3);
        onResourceGroupStatusCallbackCounter++;
        if (onResourceGroupStatusCallbackCounter == 1) {
            pOnResource.set_value(true);
        }
        ALOGD("- onResourceGroupStatusCallback");
    };

    lscMocker->onNodeStatusCallback = [&](Ecu ecuType, bool ecuStatus) {
        ALOGD("+ onNodeStatusCallback");
        EXPECT_TRUE(ecuType == Ecu::TEM || ecuType == Ecu::VCM);
        onNodeStatusCallbackCounter++;
        if (onNodeStatusCallbackCounter == 1) {
            pOnNodeStatus.set_value(true);
        }
        ALOGD("- onNodeStatusCallback");
    };

    // Need to register atleast three LSCs in order IPLM to start broadcasting message
    lscMocker->RegisterLSC("VOC");
    lscMocker->RegisterLSC("OTA");
    lscMocker->RegisterLSC("CSB");

    status = fOnResource.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool calledOnResource = fOnResource.get();
    ALOGD("Called fOnResource %d", calledOnResource);
    EXPECT_TRUE(calledOnResource);

    status = fOnNodeStatus.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool calledNodeStatus = fOnNodeStatus.get();
    ALOGD("Called fOnNodeStatus %d", calledNodeStatus);
    EXPECT_TRUE(calledNodeStatus);

    const hidl_string& lscName("VOC");
    ResourceGroup _rg = ResourceGroup::ResourceGroup3;
    ResourceGroupPrio _prio = ResourceGroupPrio::High;
    IpcbSimulator ipcbSimulator("198.18.255.255", 60000, 70000, 1);
    Pdu pdu;

    lscMocker->RequestResourceGroup(lscName, _rg, _prio);

    // Check the comming 5 PDUs to see if payload is correct
    std::future<bool> f_pdu_received = std::async([&]() {
        for (int i = 1; i < 6; i++) {
            bool IsPduRecevied = ipcbSimulator.ReceivePdu(pdu, 2);
            ALOGD("Received PDU number %d", i);

            if (IsPduRecevied) {
                std::vector<uint8_t> payload(pdu.payload);

                // get resource group bit which is stored in first bit
                uint8_t received_action = payload[0];
                uint8_t received_prio = payload[1];

                if (((received_action & _rg) == (int8_t)_rg) && ((received_prio & _prio) == (int8_t)_prio)) {
                    ALOGD("Payload is correct, ending loop");
                    return true;
                }
            }
        }
        ALOGD("Payload was not correct after five tries, return false");
        return false;
    });

    if (f_pdu_received.wait_for(std::chrono::milliseconds(10000)) == std::future_status::timeout) ASSERT_TRUE(false);
    EXPECT_TRUE(f_pdu_received.get());

    // Unregistering LSCs
    lscMocker->UnregisterLSC("VOC");
    lscMocker->UnregisterLSC("OTA");
    lscMocker->UnregisterLSC("CSB");

    ALOGD("- RequestResourceGroup_RG3_HIGH");
}

// TODO: Verify if this test case is producing correct result.
TEST_F(IplmTestFixture, WakeUpOnFlexRayTest) {
    std::promise<bool> pFlexrayWakeUp;
    std::future<bool> fFlexrayWakeUp = pFlexrayWakeUp.get_future();

    ECDDataElement::DESink<autosar::NetHdActvt_info> flexraySink;
    flexraySink.subscribe([&]() {
        auto value = flexraySink.get().value();
        EXPECT_TRUE(value.Prio == autosar::PrioHighNormal::PrioNormal);
        // EXPECT_TRUE(value.ResourceGroup == 2);
        ALOGD("ResourceGroup %d", value.ResourceGroup);
        ALOGD("Prio %d", value.Prio);
        pFlexrayWakeUp.set_value(true);
        return Void();
    });

    ALOGD("registerService iplmdtest1");
    lscMocker->RegisterLSC("iplmd-test1");
    ALOGD("registerService iplmdtest2");
    lscMocker->RegisterLSC("iplmd-test2");
    ALOGD("registerService iplmdtest3");
    lscMocker->RegisterLSC("iplmd-test3");

    status = fFlexrayWakeUp.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ALOGE("Timout or deferred!");
        ASSERT_TRUE(false);
    }

    bool called = fFlexrayWakeUp.get();
    ALOGD("Called fFlexrayWakeUp %d", called);

    ALOGD("unregisterService iplmd-test1");
    lscMocker->UnregisterLSC("iplmd-test1");
    ALOGD("unregisterService iplmd-test2");
    lscMocker->UnregisterLSC("iplmd-test2");
    ALOGD("unregisterService iplmd-test3");
    lscMocker->UnregisterLSC("iplmd-test3");
}