/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <json/json.h>
#include <localconfig.h>
#include <fstream>
#include <stdexcept>
#include <string>

namespace  // Anonymous namespace for localconfig internal stuff..
{
// TODO: Change to non hardcoded filepath
//       Perhaps use getenv() like the
//       now deprecated ihu_local_config does
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
}

namespace vcc {
namespace localconfig {

void testInit(const char *filepath) { LocalConfig::Instance().Load(filepath); }

const std::string getValueString(const std::string &key) { return LocalConfig::Instance().GetValue(key).asString(); }

int getValueInt(const std::string &key) { return LocalConfig::Instance().GetValue(key).asInt(); }

bool getValueBool(const std::string &key) { return LocalConfig::Instance().GetValue(key).asBool(); }

double getValueDouble(const std::string &key) { return LocalConfig::Instance().GetValue(key).asDouble(); }

}  // namespace localconfig
}  // namespace vcc
