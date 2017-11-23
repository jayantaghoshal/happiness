#include <cutils/log.h>
#include "Application_dataelement.h"
using namespace ApplicationDataElement;
using namespace autosar;
#include <chrono>
#include <thread>

#undef LOG_TAG
#define LOG_TAG "SignalsSendTest"

int main() {
    // Arbitarily chosen signals, just one FR and one LIN, for happy testing of both buses
    DESender<ClimaTmrStsRqrd_info> flexraySender;
    DESender<ActvnOfSwtIllmnCen_info> linSender;
    bool linValueOut = false;
    OnOff1 outValue = OnOff1::Off;

    while (true) {
        outValue = (outValue == OnOff1::Off) ? OnOff1::On : OnOff1::Off;
        linValueOut = !linValueOut;
        ALOGD("Sending FR.ClimaTmrStsRqrd=%d, LIN.ActvnOfSwtIllmnCen=%d", static_cast<int>(outValue), linValueOut);
        flexraySender.send(outValue);
        linSender.send(linValueOut);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}