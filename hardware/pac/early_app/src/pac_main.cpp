/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cstdlib>

#include <utils/StrongPointer.h>

#include <libdbg.h>
#include "pac_early_app.h"

using android::hardware::automotive::evs::V1_0::vcc_implementation::PacEarlyApp;
using android::sp;

int main() {
    sp<PacEarlyApp> pac_early_app = new PacEarlyApp();
    // Try to initialize
    if (!pac_early_app->Start()) {
        // TODO(ihu) Add controlled retry until initialization is successful.
        return EXIT_FAILURE;
    }

    dbgI("Exiting ...");
    return EXIT_SUCCESS;
}
