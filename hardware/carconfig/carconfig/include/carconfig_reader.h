#pragma once

#include <vector>
#include "carconfig_base.h"

class CarConfigReader
{
public:
    CarConfigReader();
    uint8_t getValue(uint32_t position);
    uint8_t getRawValue(uint32_t position);
    ccStatus getStatus(uint32_t position);

private:
    void read(void);
    std::vector<ccValue> carConfigValues;
};
