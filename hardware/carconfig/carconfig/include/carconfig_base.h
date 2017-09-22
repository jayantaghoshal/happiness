#ifndef CARCONFIG_BASE_H
#define CARCONFIG_BASE_H

#include <string>

const int cc_no_of_variables = 1778;
const std::string shm_file_name = "carconfig";

typedef struct carConfigObject_
{
    uint8_t value = 0x00;
    uint8_t raw = 0x00;
} carConfigObject;

class CarConfigBase
{
   protected:
    int32_t bufferSize() { return sizeof(carConfigObject) * cc_no_of_variables; }
    char *addr = nullptr;
};

#endif
