#pragma once

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include "carconfig_base.h"

class CarConfigFileReader {
 public:
  CarConfigFileReader();
  void open(const std::string path);
  Carconfig_base::ccValue getValue(int id);

 private:
  std::unique_ptr<std::fstream> ccFile_;
  std::unique_ptr<std::stringstream> fileBuf_;
  std::string filePath_;
  int filePos_;
  bool validateValue(int unvalidatedValue, uint8_t &validatedValue);
  bool validateStatus(int unvalidatedStatus, Carconfig_base::ccStatus &validatedStatus);
};
