// Test program that expectes one input signal and then send an output signal with the same value as the input.

#include <cutils/log.h>
#include <chrono>
#include <thread>
#include "Application_dataelement.h"

#undef LOG_TAG
#define LOG_TAG "FlexrayTest"

#define REC_SIGNAL AccAutResuWarnReq_info
#define SEND_SIGNAL ClimaTmrStsRqrd_info

using namespace ApplicationDataElement;
using namespace autosar;
using namespace std::chrono_literals;

int main(int argc, char** argv)
{
  DEReceiver<REC_SIGNAL> insig;
  DESender<SEND_SIGNAL> outsig;

  ALOGI("FlexrayTest started");
  ALOGI("Listening on %s", REC_SIGNAL::Name());
  bool signalReceived = false;

  insig.subscribe([&]() {
    const auto signal = insig.get();
    if (signal.isOk())
    {
      const auto value = signal.value();
      const char* onoffstr = (value == OnOff1::On) ? "ON " : "OFF";
      ALOGI("Received %s:%s ,  ", REC_SIGNAL::Name(), onoffstr);
      ALOGI("Send %s, %s ", SEND_SIGNAL::Name(), onoffstr);
      outsig.send({value});
      signalReceived = true;
    }
    else
    {
      ALOGI("Received %s with ERROR %d\n", REC_SIGNAL::Name(), signal.errorCode());
    }
  });

  // Fallback timeout. Program should be killed anyway from the test case
  std::this_thread::sleep_for(10s);
  if (signalReceived)
  {
    ALOGI("Signal %s received at least once, exit test program", REC_SIGNAL::Name());
    return 0;
  }

  ALOGE("Did not receive %s", REC_SIGNAL::Name());
  return 1;
}
