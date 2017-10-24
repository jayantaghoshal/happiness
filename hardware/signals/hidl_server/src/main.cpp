#include <cutils/log.h>
#include "signals_server.h"
#include <hidl/HidlTransportSupport.h>

#undef LOG_TAG
#define LOG_TAG "SignalsHidlServer"

using namespace vendor::volvocars::hardware::signals::V1_0::implementation;

int main(int argc, char* argv[])
{
  SignalsServer service;
  
  // Configure a thread pool of 1, this means that the HIDL
  // server calls is done from one thread. 
  android::hardware::configureRpcThreadpool(1, true /* callerWillJoin */);

  ALOGI("Registering as service...");
  service.registerAsService();

  ALOGI("Registering as service done.");
  android::hardware::joinRpcThreadpool();

}

