/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <chrono>
#include <thread>
#include "Application_dataelement.h"

#undef LOG_TAG
#define LOG_TAG "FlexrayTest"

using namespace ApplicationDataElement;
using namespace autosar;
using namespace std::chrono_literals;

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    size_t max_nr_of_signals = 300;
    std::chrono::milliseconds sleep_between_signals_during_init = 200ms;
    std::chrono::milliseconds sleep_between_signals_during_burst = 0ms;
    if (argc == 4) {
        max_nr_of_signals = std::stoul(argv[1]);
        sleep_between_signals_during_init = std::chrono::milliseconds(std::stoul(argv[2]));
        sleep_between_signals_during_burst = std::chrono::milliseconds(std::stoul(argv[3]));
    }

    DESender<AntithftRednReq_info> trigger_signal;
    trigger_signal.send(1);

    std::vector<std::function<void(const OnOffPen& deValue)>> senders;

    DESender<RoadFricIndcnActv_info> s2;
    DESender<RoadSgnInfoActv_info> s3;
    DESender<SpdAlrmActvForRoadSgnInfo_info> s4;
    DESender<SpdCameraInfoSeldForRoadSgnInfo_info> s5;
    DESender<SteerAssiActvForAutDrv_info> s6;
    DESender<TrfcRglnSeldForRoadSgnInfo_info> s7;
    DESender<TrlrLampChkAutReq_info> s8;
    DESender<UnlckFbVisReq_info> s10;
    DESender<WiprReAutReq_info> s11;
    DESender<AccAdprSpdLimActv_info> s12;
    DESender<AccAdprTurnSpdActv_info> s13;
    DESender<CllsnFwdWarnActv_info> s14;

    senders.emplace_back([&](const OnOffPen& x) { s2.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s3.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s4.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s5.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s6.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s7.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s8.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s10.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s11.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s12.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s13.send(x); });
    senders.emplace_back([&](const OnOffPen& x) { s14.send(x); });

    for (size_t i = 0; i < senders.size() && i < max_nr_of_signals; i++) {
        auto send_func = senders[i];
        OnOffPen toSend;
        toSend.Sts = OnOff1::Off;
        toSend.Pen = IdPen::ProfUkwn;
        send_func(toSend);
        std::this_thread::sleep_for(sleep_between_signals_during_init);
    }

    trigger_signal.send(2);
    // Sleep to give host side of test time to assert
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (size_t i = 0; i < senders.size() && i < max_nr_of_signals; i++) {
        auto send_func = senders[i];
        OnOffPen toSend;
        toSend.Sts = OnOff1::On;
        toSend.Pen = IdPen::Prof6;
        send_func(toSend);
        std::this_thread::sleep_for(sleep_between_signals_during_burst);
    }

    // Sleep to avoid trigger signal from being affected by burst
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    trigger_signal.send(3);

    return 1;
}
