/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <ECD_dataelement.h>
#include <cutils/log.h>
#include <unistd.h>
#include <chrono>
#include <string>
#include <thread>

#define LOG_TAG "InjectDEForBrightness"

int main(int argc, char* argv[]) {
    std::string cmdarg = "";
    std::string cmdarg2 = "";
    if (argc > 1) {
        cmdarg = argv[1];
    }
    if (argc > 2) {
        cmdarg2 = argv[2];
    }

    if (cmdarg == "ill") {
        // inject signals
        ECDDataElement::DEInjector<autosar::IntrBriSts_info> intrBriSts;  // Rheostat 0-15

        int rheoval = 0;
        if (cmdarg2 != "") {
            rheoval = std::stoi(cmdarg2);
        }
        ALOGD("ill=%d", rheoval);
        intrBriSts.inject(rheoval);
    } else if (cmdarg == "csdmantest") {
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
    } else if (cmdarg == "day") {
        ALOGD("day");
        ECDDataElement::DEInjector<autosar::TwliBriSts_info> twliBriSts;  // 1=Day 0=Night
        twliBriSts.inject(autosar::TwliBriSts1::Day);
    } else if (cmdarg == "night") {
        ALOGD("night");
        ECDDataElement::DEInjector<autosar::TwliBriSts_info> twliBriSts;  // 1=Day 0=Night
        twliBriSts.inject(autosar::TwliBriSts1::Night);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return 1;
}