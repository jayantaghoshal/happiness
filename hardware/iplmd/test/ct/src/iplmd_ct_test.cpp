#include <gtest/gtest.h>
#include "LscMocker.h"
#include <cutils/log.h>
#include <ECD_dataelement.h>
#include <iostream>
#include <future>
#include <thread>

#define LOG_TAG "iplm_daemon_test"

using ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroup;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroup;
using ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroupStatus;

TEST(ImplmdTest, RegisterUnRegisterLSCTest){
  LscMocker* lscMocker = new LscMocker();
  int onNodeStatusCallbackCounter = 0;
  int onResourceGroupStatusCallbackCounter = 0;
  std::promise<bool> pOnResource;
  std::promise<bool> pOnNodeStatus;
  std::future<bool> fOnResource = pOnResource.get_future();
  std::future<bool> fOnNodeStatus = pOnNodeStatus.get_future();

  lscMocker->onResourceGroupStatusCallback = [&](ResourceGroup resourceGroup, ResourceGroupStatus resourceGroupStatus)
  {
    EXPECT_TRUE(resourceGroup == ResourceGroup::ResourceGroup1 || resourceGroup == ResourceGroup::ResourceGroup3);
    onResourceGroupStatusCallbackCounter++;
    if(onResourceGroupStatusCallbackCounter == 1){
      pOnResource.set_value(true);
    }
  };
  lscMocker->onNodeStatusCallback = [&](Ecu ecuType, bool ecuStatus){
    EXPECT_TRUE(ecuType == Ecu::TEM || ecuType == Ecu::VCM);
    onNodeStatusCallbackCounter++;
    if(onNodeStatusCallbackCounter == 1){
      pOnNodeStatus.set_value(true);
    }
  };

  ALOGD("registerService iplmdtest");
  lscMocker->RegisterLSC("iplmd-test");

  fOnResource.wait_for(std::chrono::seconds(2));
  bool calledOnResource = fOnResource.get();
  ALOGD("Called fOnResource %d",calledOnResource);
  EXPECT_TRUE(calledOnResource);

  fOnNodeStatus.wait_for(std::chrono::seconds(2));
  bool calledNodeStatus = fOnNodeStatus.get();
  ALOGD("Called fOnNodeStatus %d",calledNodeStatus);
  EXPECT_TRUE(calledNodeStatus);

  ALOGD("unregisterService iplmdtest");
  lscMocker->UnregisterLSC("iplmd-test");
  int onResourceGroupStatusCallbackCounterBefore = onResourceGroupStatusCallbackCounter;
  int onNodeStatusCallbackCounterBefore = onNodeStatusCallbackCounter;

  ALOGD("Sleeping for 2 sec");
  usleep(1000000*2);

  //Shouldnt have increased counter since we have unregistered the LSC
  EXPECT_TRUE(onResourceGroupStatusCallbackCounter == onNodeStatusCallbackCounterBefore);
  EXPECT_TRUE(onNodeStatusCallbackCounter == onNodeStatusCallbackCounterBefore);
}

/* Needs packet injector tool, will enable later  */
/*
TEST(ImplmdTest, WakeUpOnFlexRayTest){

  std::promise<bool> pFlexrayWakeUp;
  std::future<bool> fFlexrayWakeUp = pFlexrayWakeUp.get_future();

  LscMocker* lscMocker = new LscMocker();
  ECDDataElement::DESink<autosar::NetHdActvt_info> flexraySink;
  flexraySink.subscribe(
    [&](){
    auto value = flexraySink.get().value();
    EXPECT_TRUE(value.Prio== autosar::PrioHighNormal::PrioNormal);
    EXPECT_TRUE(value.ResourceGroup == 2);
    ALOGD("ResourceGroup %d",value.ResourceGroup);
    ALOGD("Prio %d",value.Prio);
    pFlexrayWakeUp.set_value(true);
    return Void();
  });

  ALOGD("registerService iplmdtest1");
  lscMocker->RegisterLSC("iplmd-test1");
  ALOGD("registerService iplmdtest2");
  lscMocker->RegisterLSC("iplmd-test2");
  ALOGD("registerService iplmdtest3");
  lscMocker->RegisterLSC("iplmd-test3");

  fFlexrayWakeUp.wait_for(std::chrono::seconds(2));
  bool called = fFlexrayWakeUp.get();
  ALOGD("Called fFlexrayWakeUp %d",called);

  ALOGD("unregisterService iplmd-test1");
  lscMocker->UnregisterLSC("iplmd-test1");
  ALOGD("unregisterService iplmd-test2");
  lscMocker->UnregisterLSC("iplmd-test2");
  ALOGD("unregisterService iplmd-test3");
  lscMocker->UnregisterLSC("iplmd-test3");
}*/