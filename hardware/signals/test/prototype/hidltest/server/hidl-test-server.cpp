#undef LOG_TAG
#define LOG_TAG "hidl-test-server@1.0-service"
#include <SignalsImpl.h>
#include <android/log.h>
#include <hidl/HidlTransportSupport.h>

#include <iostream>

//#include <vhal_v2_0/VehicleHalManager.h>
//#include <vhal_v2_0/EmulatedVehicleHal.h>

using namespace android;
using namespace android::hardware;
using namespace vcc::vcctest::V1_0::implementation;

// using namespace android::hardware::automotive::vehicle::V2_0;

int main(int /* argc */, char* /* argv */ [])
{
  auto service = std::make_unique<SignalsImpl>();

  // Configure a thread pool of 1, this means that the HIDL
  // server calls is done from one thread. If you want to have
  // more threads use can for example use configureRpcThreadpool(4, true /* callerWillJoin */);
  // But then the server impl need to deal with that methods can be
  // called from several threads.
  configureRpcThreadpool(1, true /* callerWillJoin */);

  ALOGI("Registering as service...");
  service->registerAsService("vcc_hidl_test_service");

  ALOGI("Ready");
  joinRpcThreadpool();
}
