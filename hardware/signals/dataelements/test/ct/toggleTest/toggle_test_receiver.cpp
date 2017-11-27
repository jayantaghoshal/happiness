/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <chrono>
#include <thread>
#include "Application_dataelement.h"

#undef LOG_TAG
#define LOG_TAG "FlexrayToggleTest"

using namespace ApplicationDataElement;
using namespace autosar;
using namespace std::chrono_literals;

int main(int argc, char** argv) {
    std::chrono::milliseconds test_timeout;
    int expected_dt = 300;
    int nr_of_toggles_left = 350;
    if (argc == 4) {
        nr_of_toggles_left = std::stoi(argv[1]);
        expected_dt = std::stoi(argv[2]);
        test_timeout = std::chrono::milliseconds(std::stoi(argv[3]));
    }
    const int max_dt = static_cast<int>((expected_dt * 2.0f));
    ALOGI("FlexrayTest started, argc=%d, Nr of toggles: %d, expected_dt: %d ms, max_dt: %d, timeout: %lld ms",
          argc,
          nr_of_toggles_left,
          expected_dt,
          max_dt,
          test_timeout.count());

    bool any_errors = false;

    {
        int64_t prev = 0;
        DEReceiver<AdjSpdLimnSts_info> insig;
        AdjSpdLimnSts2 nextExpected = AdjSpdLimnSts2::Off;

        insig.subscribe([&]() {
            const auto signal = insig.get();
            if (signal.isOk()) {
                const auto value = signal.value();
                if (value == nextExpected) {
                    if (prev != 0) {
                        const auto dt = static_cast<int>(signal.timestamp() - prev);
                        if (dt > max_dt) {
                            any_errors = true;
                            ALOGE("Toggle dt: %d ms <<< Error", dt);
                        } else {
                            ALOGI("Toggle dt: %d ms", dt);
                        }
                    }
                    prev = signal.timestamp();

                    nr_of_toggles_left--;
                    nextExpected = (nextExpected == AdjSpdLimnSts2::Off) ? AdjSpdLimnSts2::Stb : AdjSpdLimnSts2::Off;
                }
            } else {
                ALOGI("Received error code: %d\n", signal.errorCode());
            }
        });

        const auto deadline = std::chrono::steady_clock::now() + test_timeout;
        while (nr_of_toggles_left > 0 && std::chrono::steady_clock::now() < deadline) {
            std::this_thread::sleep_for(1s);
        }
    }

    if (nr_of_toggles_left > 0) {
        ALOGE("Did not receive all expected toggles, %d left", nr_of_toggles_left);
        any_errors = true;
    }

    if (any_errors) {
        ALOGE("Errors found");
        return 1;
    }
    return 0;
}
