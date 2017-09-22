#ifndef CARCONFIG_WRITER_H
#define CARCONFIG_WRITER_H
#include <string>
#include "carconfig_base.h"

class CarConfigFileWriter : CarConfigBase
{
   public:
    CarConfigFileWriter(std::string fileposition);
    int commitToSharedMemory(void);

   private:
    std::string fPos;
};

#endif
