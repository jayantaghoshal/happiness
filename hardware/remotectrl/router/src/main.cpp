/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/properties.h>
#include <new>
#include <stdexcept>
#include "router.h"

#define LOG_TAG "ConvApi_Router"
#include <cutils/log.h>

using namespace vcc::remotectrl::router;

// TODO (Abhi): setup signal handlers
int main() {
    try {
        Router router;
        property_set("remotectrl_routerd.startup_completed", "1");
        router.Start();
    } catch (const std::runtime_error& e) {
        ALOGE("%s", e.what());
        return EXIT_FAILURE;
    } catch (const std::bad_alloc& e) {
        ALOGE("%s", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
