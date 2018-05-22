/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "remotectrl_client_audioctrl.h"
#include "remotectrl_client_base.h"
#include "remotectrl_client_climatectrl.h"
#include "remotectrl_client_mediactrl.h"
#include "signals_def.h"

using namespace vcc::remotectrlsimclient;
using namespace vcc::remotectrlsim;
using namespace vcc::remotectrl;

int main(int argc, char* argv[]) {
    using namespace std::chrono_literals;

    if (argc < 2) {
        // Only expected to happen during development and not in production image
        std::cout << "Expects vsomeip configuration file location as argument" << std::endl;
        std::this_thread::sleep_for(1s);
        return EXIT_FAILURE;
    }

    const char* environment_variable = "VSOMEIP_CONFIGURATION";
    const char* environment_value = argv[1];
    std::cout << environment_variable << ":" << environment_value << std::endl;

    setenv(environment_variable, environment_value, 1 /*override*/);

    std::string path(argv[1]);

    int val = -1;

    while (1) {
        std::cout << "Load Module:" << std::endl
                  << "0. Exit" << std::endl
                  << "1. Climate" << std::endl
                  << "2. Audio" << std::endl
                  << "3. Media" << std::endl;
        std::cin >> val;

        std::cout << "" << val << std::endl;

        if (0 == val) {
            std::cout << "Exiting..." << std::endl;
            break;
        } else if (1 == val) {
            std::cout << "Climate" << std::endl;
            ClimateClient climate(path);
            climate.Run();
        } else if (2 == val) {
            std::cout << "Audio" << std::endl;
            AudioClient audio(path);
            audio.Run();
        } else if (3 == val) {
            std::cout << "Media" << std::endl;
            MediaClient media(path);
            media.Run();
        } else {
            std::cout << "No." << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
