/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <vcc/localconfig.h>
#include <vcc/localconfig_test.h>

#include <json/json.h>

#include <fstream>
#include <stdexcept>
#include <string>

namespace {
const char *kDefaultFilePath = "/oem_config/localconfig/localconfig.json";

// Singleton for accessing localconfig.
//
// Exctracting values is thread safe. Loads default configuration when first accessed. Tests may
// load a different configuration. This is not thread safe at the moment.
class LocalConfig {
 public:
  LocalConfig() { Load(kDefaultFilePath); }

  static LocalConfig &Instance() {
    static LocalConfig instance;
    return instance;
  }

  void Load(const std::string &file_path) {
    std::ifstream ifs(file_path);
    if (!ifs) {
      throw std::runtime_error("JSON file " + file_path + " could not be opened.");
    }

    Json::Reader reader;
    if (!reader.parse(ifs, root_)) {
      throw std::runtime_error("JSON file " + file_path + " could not be parsed, please check file content.");
    }
  }

  const Json::Value &GetValue(const std::string &key) const {
    const Json::Value &value = root_[key];
    if (value.isNull()) {
      throw std::runtime_error("Parameter " + key + " not found in localconfig.");
    }
    return value;
  }

 private:
  Json::Value root_;
};
}  // namespace

namespace vcc {
namespace localconfig {

void TestInit(const std::string &file_path) { LocalConfig::Instance().Load(file_path); }

std::string GetString(const std::string &key) {
  const Json::Value &value = LocalConfig::Instance().GetValue(key);

  if (!value.isString()) throw std::runtime_error("Parameter " + key + " is not a string.");

  return value.asString();
}

int GetInt(const std::string &key) {
  const Json::Value &value = LocalConfig::Instance().GetValue(key);

  if (!value.isInt()) throw std::runtime_error("Parameter " + key + " is not an integer.");

  return value.asInt();
}

bool GetBool(const std::string &key) {
  const Json::Value &value = LocalConfig::Instance().GetValue(key);

  if (!value.isBool()) throw std::runtime_error("Parameter " + key + " is not a bool.");

  return value.asBool();
}

double GetDouble(const std::string &key) {
  const Json::Value &value = LocalConfig::Instance().GetValue(key);

  if (!value.isDouble()) throw std::runtime_error("Parameter " + key + " is not a double.");

  return value.asDouble();
}

}  // namespace localconfig
}  // namespace vcc
