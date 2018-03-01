/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <ECD_dataelement.h>
#include <cutils/log.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
#include <thread>

#define LOG_TAG "InjectDEForBrightness"

int main(int argc, char* argv[]) {
    std::string cmdarg = "";
    std::string cmdarg2 = "";
    std::string cmdarg3 = "";
    if (argc >= 1) {
        cmdarg = argv[1];
    }
    if (argc >= 2) {
        cmdarg2 = argv[2];
    }
    if (argc >= 3) {
        cmdarg3 = argv[3];
    }

    if (cmdarg.find("illtest") != std::string::npos) {
        ALOGD("illtest");
        autosar::TwliBriSts1 twliBriStsInjectSignal = autosar::TwliBriSts1::Day;
        if (argc >= 2) {
            if (cmdarg2.find("night") != std::string::npos) {
                twliBriStsInjectSignal = autosar::TwliBriSts1::Night;
            }
        }

        // inject signals
        ECDDataElement::DEInjector<autosar::TwliBriSts_info> twliBriSts;  // 1=Day 0=Night
        ECDDataElement::DEInjector<autosar::IntrBriSts_info> intrBriSts;  // Rheostat 0-15

        twliBriSts.inject(twliBriStsInjectSignal);
        ALOGD("TwliBriSts_info, :%d injected", twliBriStsInjectSignal);

        if (cmdarg3.find("noloop") != std::string::npos) {
            return 1;
        }
        for (int i = 0; i <= 2; i++) {
            ALOGD("increasing value of reostat from 0 -> 15");
            for (int j = 0; j <= 15; j++) {
                usleep(500000);  // sleep 0.5 sec for signal change to be picked up?
                intrBriSts.inject(j);
                ALOGD("IntrBriSts_info, Reostat: %d injected", j);
            }
        }
        return 1;
    } else if (cmdarg.find("csdmantest") != std::string::npos) {
        ALOGD("csdmantest ");
        ECDDataElement::DEInjector<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1_info;  // IgnitionState
        autosar::VehModMngtGlbSafe1 vehModMngtGlbSafe1Value;
        vehModMngtGlbSafe1Value.CarModSts1_ = autosar::CarModSts1::CarModNorm;
        if (cmdarg2.find("UsgModCnvinc") != std::string::npos) {
            ALOGD("UsgModCnvinc");
            vehModMngtGlbSafe1Value.UsgModSts = autosar::UsgModSts1::UsgModCnvinc;
        } else if (cmdarg2.find("UsgModDrvg") != std::string::npos) {
            ALOGD("UsgModDrvg");
            vehModMngtGlbSafe1Value.UsgModSts = autosar::UsgModSts1::UsgModDrvg;
        } else {
            return -1;
        }
        ALOGD("vehModMngtGlbSafe1Value, :%d injected", (int)vehModMngtGlbSafe1Value.UsgModSts);
        vehModMngtGlbSafe1_info.inject(vehModMngtGlbSafe1Value);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 1;
}