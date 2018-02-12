/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <ECD_dataelement.h>
#include <cutils/log.h>
#include <string.h>
#include <unistd.h>

#define LOG_TAG "InjectFlexRayIllTest"

int main(int argc, char* argv[]) {
    autosar::TwliBriSts1 twliBriStsInjectSignal = autosar::TwliBriSts1::Day;
    std::string cmdarg = "";
    if (argc > 1) {
        cmdarg = argv[1];
        if (cmdarg.find("night") != std::string::npos) {
            twliBriStsInjectSignal = autosar::TwliBriSts1::Night;
        }
    }

    // inject signals
    ECDDataElement::DEInjector<autosar::TwliBriSts_info> twliBriSts;  // 1=Day 0=Night
    ECDDataElement::DEInjector<autosar::IntrBriSts_info> intrBriSts;  // Rheostat 0-15

    twliBriSts.inject(twliBriStsInjectSignal);
    ALOGD("TwliBriSts_info, :%d injected", twliBriStsInjectSignal);

    while (true) {
        ALOGD("increasing value of reostat from 0 -> 15");
        for (int j = 0; j <= 15; j++) {
            usleep(500000);  // sleep 0.5 sec for signal change to be picked up?
            intrBriSts.inject(j);
            ALOGD("IntrBriSts_info, Reostat: %d injected", j);
        }
    }
}