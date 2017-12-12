/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <android/log.h>
#include "Application_dataelement.h"
#include "ECD_dataelement.h"

#undef LOG_TAG
#define LOG_TAG "DataelementsSignalEcho"

using namespace ApplicationDataElement;
using namespace ECDDataElement;
using namespace autosar;

int main(int argc, char** argv) {
    DEReceiver<DefrstDurgClimaEnad_info> insig_receiver;
    DEInjector<DefrstDurgClimaEnad_info> insig_injector;

    OnOff1 recSignalValue;
    recSignalValue = OnOff1::On;

    uint16_t numberOfCallbackCalls = 0;

    ALOGI("Signalecho started");
    ALOGI("Listening/injecting on %s, sending %s", PassSeatMassgFct_info::Name(), MassgFctActv_info::Name());

    insig_receiver.subscribe([&]() {
        const auto signal = insig_receiver.get();
        if (signal.isOk()) {
            const auto value = signal.value();
            const char* onoffstr = (value == OnOff1::On) ? "ON " : "OFF";
            // Do not print all callbacks, then logcat feels bad.
            if (numberOfCallbackCalls % 1000 == 0) {
                ALOGI("Received %s:%s ,  ", PassSeatMassgFct_info::Name(), onoffstr);
            }
            numberOfCallbackCalls++;
        } else {
            ALOGI("Received %s with ERROR %d\n", PassSeatMassgFct_info::Name(), signal.errorCode());
        }
    });

    const uint16_t expectedNumberOfCalls = 10000;
    for (int i = 0; i < expectedNumberOfCalls; i++) {
        // Inject a toggeling value and count the callbacks
        recSignalValue = recSignalValue == OnOff1::On ? OnOff1::Off : OnOff1::On;
        insig_injector.inject(recSignalValue);

        // Needed to not starve callback handling from hidl
        usleep(0);
    }
    usleep(10);

    if (numberOfCallbackCalls != expectedNumberOfCalls) {
        ALOGE("numberOfCallbackCalls: %d did not meet the expected number of calls: %d", numberOfCallbackCalls,
              expectedNumberOfCalls);
        return 1;
    }

    return 0;
}
