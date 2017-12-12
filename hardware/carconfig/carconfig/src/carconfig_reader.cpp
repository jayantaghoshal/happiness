/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "carconfig_reader.h"
#include <cutils/log.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <stdexcept>
#include "carconfig_file_reader.h"

#undef LOG_TAG
#define LOG_TAG "CarConfigReader"

using namespace Carconfig_base;

CarConfigReader::CarConfigReader() { read(); }

// Get the so called substitue value, this is the function normally used by applications.
// Substitue value is the last received valid value
uint8_t CarConfigReader::getValue(uint32_t position) {
    if ((position < 1) || (position > cc_no_of_parameters)) {
        throw std::out_of_range{"Trying to access carconfig value " + std::to_string(position) +
                                ", which is out of range"};
    }
    return carConfigValues[(position - 1)].subs;
}

// Get the so called raw value.
// Raw value is the last received value, no validation check
uint8_t CarConfigReader::getRawValue(uint32_t position) {
    if ((position < 1) || (position > cc_no_of_parameters)) {
        throw std::out_of_range{"Trying to access carconfig raw value " + std::to_string(position) +
                                ", which is out of range"};
    }
    return carConfigValues[(position - 1)].raw;
}

ccStatus CarConfigReader::getStatus(uint32_t position) {
    if ((position < 1) || (position > cc_no_of_parameters)) {
        throw std::out_of_range{"Trying to access carconfig status value " + std::to_string(position) +
                                ", which is out of range"};
    }
    return carConfigValues[(position - 1)].status;
}

void CarConfigReader::read() {
    CarConfigFileReader ccFileReader;
    if (fileExists(carconfig_file_name)) {
        ALOGI("CarConfig uses car config file");
        ccFileReader.open(carconfig_file_name);
    } else {
        ALOGI("CarConfig uses default car config file, no config from flexray will be considered");
        ccFileReader.open(carconfig_default_file_name);
    }

    ccValue ccRes;

    carConfigValues.reserve(cc_no_of_parameters);
    for (uint16_t x = 0; x < cc_no_of_parameters; x++) {
        ccRes = ccFileReader.getValue(x + 1);
        carConfigValues[x].subs = ccRes.subs;
        carConfigValues[x].raw = ccRes.raw;
        carConfigValues[x].status = ccRes.status;
    }
}
