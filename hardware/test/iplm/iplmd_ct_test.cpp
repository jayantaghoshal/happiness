#include <ECD_dataelement.h>
#include <cutils/log.h>
#include <ipcb_simulator.h>
#include <signal.h>
#include <sys/types.h>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include "gtest/gtest.h"
#include "src/LscMocker.h"

#define LOG_TAG "iplm_daemon_test"

using ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroup;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroupStatus;

static int pid;

class IplmTestFixture : public ::testing::Test {
 protected:
  IplmTestFixture() {}

  void SetUp() {
    lscMocker = new LscMocker();
    onNodeStatusCallbackCounter = 0;
    onResourceGroupStatusCallbackCounter = 0;
  }

  static void SetUpTestCase() {
    ALOGD("+ SetUpTestCase ");

    // run a process and create a streambuf that reads its stdout and stderr
    std::string pid_str = getCmdOut(
        "VCC_LOCALCONFIG_PATH=/data/local/tmp/localconfig.json /system/bin/ip netns exec vcc /vendor/bin/hw/ipcbd iplm "
        "UDPB & echo $!");

    std::string::size_type sz;  // alias of size_t
    pid = std::stoi(pid_str, &sz);

    ALOGD("\n Process int id: %d \n", pid);

    int fileDescriptor;
    std::string nameSpace = "/var/run/netns/vcc";

    fileDescriptor = open(nameSpace.c_str(), O_RDONLY);
    if (fileDescriptor > 0) {
      if (setns(fileDescriptor, CLONE_NEWNET))
        ALOGE("Set NS failed!");
      else
        ALOGD("+ SetUpTestCase - Namespace is: %s", nameSpace.c_str());
    } else
      ALOGE("Open NS filedescriptor failed!");

    ALOGD("- SetUpTestCase ");
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
    int result = kill(pid, SIGTERM);
    ALOGD("- TearDownTestCase: %d \n", result);
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

/*
 Register and Unregister Local software Components.
 */
TEST_F(IplmTestFixture, RegisterUnRegisterLSC) {
  ALOGD("+ RegisterUnRegisterLSC");

  lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) {
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
Following test will register 3 Local Software components and will exepect broad case messages from IPLM
*/
TEST_F(IplmTestFixture, RecieveBroadCast_SUCCESS) {
  ALOGD("+ RecieveBroadCast_SUCCESS");

  lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) {
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
  IpcbSimulator* ipcbSimulator = new IpcbSimulator("198.18.255.255", 60000, 70000, 1);

  Pdu pdu;

  bool result = false;
  for (int i = 0; i < 10; i++) {
    // TODO: PDU valid or not should be handled in seperate tests within libipcommandbus
    std::future<bool> fut = std::async([&]() { return ipcbSimulator->ReceivePdu(pdu); });

    std::chrono::milliseconds span(3000);
    if (fut.wait_for(span) == std::future_status::timeout) {
      ipcbSimulator->CloseSocket();
      fut.get();
      delete ipcbSimulator;
      break;
    } else {
      result = fut.get();
    }
  }

  for (int i = 0; i < 3; i++) {
    lscMocker->UnregisterLSC("lsc-" + std::to_string(i));
    ALOGD("UnregisterLSC - %d", i);
  }

  if (result) {
    ipcbSimulator->CloseSocket();
    delete ipcbSimulator;
  }

  EXPECT_EQ(result, true);
  ALOGD("- RecieveBroadCast_SUCCESS");
}

// IPLM broadcast messages if less than 3 LSc registered
TEST_F(IplmTestFixture, RecieveBroadCast_FAIL) {
  ALOGD("+ RecieveBroadCast_FAIL");
  // TODO: Implemenet test case where less than 3 LSCs are registered, in this case IPLM shall not broadcast message.
  bool flag = false;
  EXPECT_EQ(flag, false);
  ALOGD("- RecieveBroadCast_FAIL");
}

// Request services by Resource Group 1 and Prio Normal
TEST_F(IplmTestFixture, RequestResourceGroup_RG1_NORM) {
  // Need to register atleast three LSCs in order IPLM to star broadcasting message
  ALOGD("+ RequestResourceGroup_RG1_NORM");

  lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) {
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
  IpcbSimulator IpcbSimulator("198.18.255.255", 60000, 70000, 1);
  bool flag = false;
  Pdu pdu;

  lscMocker->RequestResourceGroup(lscName, _rg, _prio);

  // TODO: PDU valid or not should be handled in seperate tests within libipcommandbus
  bool IsPduRecevied = IpcbSimulator.ReceivePdu(pdu);

  if (IsPduRecevied) {
    std::vector<uint8_t> payload(pdu.payload);

    // get resource group bit which is stored in first bit
    uint8_t received_action = payload[0];
    uint8_t received_prio = payload[1];

    if (((received_action & _rg) == (int8_t)_rg) && ((received_prio & _prio) == (int8_t)_prio)) {
      flag = true;
    }
  }

  // Unregistering LSCs
  lscMocker->UnregisterLSC("VOC");
  lscMocker->UnregisterLSC("OTA");
  lscMocker->UnregisterLSC("CSB");

  EXPECT_EQ(flag, true);
}

// Request services by Resource Group 3 and Prio High
TEST_F(IplmTestFixture, RequestResourceGroup_RG3_HIGH) {
  ALOGD("+ RequestResourceGroup_RG3_HIGH");

  lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus) {
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

  // Need to register atleast three LSCs in order IPLM to star broadcasting message
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
  IpcbSimulator IpcbSimulator("198.18.255.255", 60000, 70000, 1);
  bool flag = false;
  Pdu pdu;

  lscMocker->RequestResourceGroup(lscName, _rg, _prio);

  // TODO: PDU valid or not should be handled in seperate tests within libipcommandbus
  bool IsPduRecevied = IpcbSimulator.ReceivePdu(pdu);

  if (IsPduRecevied) {
    std::vector<uint8_t> payload(pdu.payload);

    // get resource group bit which is stored in first bit
    uint8_t received_action = payload[0];
    uint8_t received_prio = payload[1];

    if (((received_action & _rg) == (int8_t)_rg) && ((received_prio & _prio) == (int8_t)_prio)) {
      flag = true;
    }
  }

  // Unregistering LSCs
  lscMocker->UnregisterLSC("VOC");
  lscMocker->UnregisterLSC("OTA");
  lscMocker->UnregisterLSC("CSB");

  EXPECT_EQ(flag, true);
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