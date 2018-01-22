/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <string>

const int cc_no_of_variables = 504;
const std::string shm_file_name = "carconfig";

typedef struct carConfigObject_ {
    uint8_t value = 0x00;
    uint8_t raw = 0x00;
} carConfigObject;

class CarConfigBase {
  protected:
    carConfigObject ccCache[cc_no_of_variables];
    int32_t bufferSize() { return sizeof(carConfigObject) * cc_no_of_variables; }
};
