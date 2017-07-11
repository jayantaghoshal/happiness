
#define LOG_TAG "hidl-test-client@1.0"
#include <android/log.h>
#include <hidl/HidlTransportSupport.h>
#include <vcc/vcctest/1.0/ISignals.h>

#include <iostream>

using namespace android;
using namespace android::hardware;
using namespace vcc::vcctest::V1_0;

class ServerCallback : public ISignalsCallback
{
 public:
  virtual ::android::hardware::Return<void> signalChanged(const ::android::hardware::hidl_string& signalName) override
  {
    ALOGI("main(): Got callback from testserver with name %s", toString(signalName).c_str());
    return Void();
  }
};

int main(int /* argc */, char* /* argv */ [])
{
  // Connect to test service
  auto signalServer = ISignals::getService("vcc_hidl_test_service");

  // Install callback
  ServerCallback cb;
  signalServer.get()->subscribe(&cb);

  hidl_string param("Test String from client");
  uint32_t result = signalServer.get()->set(param);

  ALOGI("main(): Result from test server %d", result);
}
