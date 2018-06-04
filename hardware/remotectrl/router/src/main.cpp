/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <router.h>

#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <thread>

#include <cutils/properties.h>

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_Routerd"
#include <cutils/log.h>

using namespace vcc::someip::router;

int main(int argc, char* argv[]) {
    using namespace std::chrono_literals;

    if (argc < 2) {
        // Only expected to happen during development and not in production image
        ALOGE("Expects vsomeip configuration file location as argument");
        std::this_thread::sleep_for(1s);
        return EXIT_FAILURE;
    }

    const char* router_name = argv[0];
    const char* environment_value = argv[1];

    try {
        Router router(router_name, environment_value);

        property_set("remotectrl_routerd.startup_completed", "1");

        router.Start();
    } catch (const std::exception& e) {
        ALOGE("%s", e.what());
        std::this_thread::sleep_for(1s);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
