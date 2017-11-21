/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

// Test program that expectes one input signal and then send an output signal with the same value as the input.
#include <cutils/log.h>
#include <chrono>
#include <iostream>
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
    DESender<DrvrHmiForHmiCen_info> outsig;
    DrvrHmiForHmiCen value;

    ALOGI("FlexrayTest started");

    uint8_t x = 0;
    uint8_t offset = 5;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now() + std::chrono::seconds(30);
    while (std::chrono::steady_clock::now() < end) {
        for (size_t i = 0; i < value.size(); i++) {
            value[i] = offset + x;
            x++;
        }
        offset++;
        outsig.send(value);
        ALOGI("Next pattern starting with offset=%d", offset);
        std::this_thread::sleep_for(100ms);
    }
    return 0;
}
