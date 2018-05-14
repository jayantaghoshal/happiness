/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "carconfig_reader.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Error, number of input argument must be 1\n");
        return 1;
    }
    int ccParam = std::stoi(argv[1]);

    CarConfigReader ccReader;
    std::array<Carconfig_base::ccValue, Carconfig_base::cc_no_of_parameters> ccParameters;
    Carconfig_base::ccValue value;

    std::cout << std::to_string(ccReader.getValue(ccParam)) << std::endl;

    return 0;
}
