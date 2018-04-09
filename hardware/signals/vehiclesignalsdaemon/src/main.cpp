/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <vsm.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "desip_handler.h"


// DO NOT CHANGE THE ORDER!
#define DTS_PRIVATE_MODEL Tr_Declare_App(VehicleSignalsDaemon) 
#include "DTS_GenericPrivateModel.h"
#include "DTS_DeclarePrivateModel.h"
// END OF: DO NOT CHANGE THE ORDER!

#undef LOG_TAG
#define LOG_TAG "VSD"

int main(int argc, char* argv[]) {
    bool startOnlyDBUS = false;

    if (2 > argc) {
        ALOGE("Serial port not specified when starting the Vehicle Signals Manager.");
        return EXIT_FAILURE;
    } else if (2 < argc) {
        ALOGE("Wrong number of parameters specified when starting "
              "Flexray/LIN vehicle signals communication service.");
        return EXIT_FAILURE;
    }

    ALOGI("VSM communication service is starting....");

    // Initialize the VSM module. After this function is called the VSM
    // will be able to handle injected messages from lower layers
    vsm_init();

    // Init desip with path to UART diver and UART speed
    bool result = initDesip(argv[1]);

    if (!result) {
        ALOGE("Failed to initialize DESIP with uart=%s", argv[1]);
        return EXIT_FAILURE;
    }

    // Start the VSM module. After this function is called
    // the VSM module will sink messages to lower layers
    vsm_start();

    ALOGI("Flexray/LIN vehicle signals communication service calls app.exec().");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
