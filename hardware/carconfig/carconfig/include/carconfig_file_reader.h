#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "carconfig_base.h"

class CarConfigFileReader
{
   public:
    CarConfigFileReader();
    void open(const std::string path);
    ccValue getValue(int id);


   private:
    std::unique_ptr<std::fstream> ccFile_;
    std::unique_ptr<std::stringstream> fileBuf_;
    std::string filePath_;
    int filePos_;
    bool validateValue(int unvalidatedValue, uint8_t &validatedValue);
    bool validateStatus(int unvalidatedStatus, ccStatus &validatedStatus);
};
