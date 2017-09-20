#ifndef CARCONFIG_READER_H
#define CARCONFIG_READER_H

#include "carconfig_base.h"

class CarConfigFileReader : CarConfigBase
{
   public:
    void read(void);
    uint8_t getValue(uint32_t position);
    uint8_t getRawValue(uint32_t position);
    uint8_t isRead();
};

#endif
