/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <vcc/localconfig.h>
#include <vcc/localconfig_test.h>

#include <json/json.h>

#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>

namespace
{
const char *kDefaultFilePath = "/oem_config/localconfig/localconfig.json";

// Singleton for accessing localconfig.
//
// Exctracting values is thread safe. Loads default configuration when first accessed. Tests may
// load a different configuration. This should be done prior to accessing any value if loading of
// default configuration is to be avoided.
class LocalConfig
{
 public:
  static LocalConfig &Instance()
  {
    static LocalConfig instance;
    return instance;
  }

  void TestInit(const std::string &file_path)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    Load(file_path);
  }

  std::string GetString(const std::string &key)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &value = GetValue(key);

    if (!value.isString()) throw std::runtime_error("Parameter " + key + " is not a string.");

    return value.asString();
  }

  int GetInt(const std::string &key)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &value = GetValue(key);

    if (!value.isInt()) throw std::runtime_error("Parameter " + key + " is not an integer.");

    return value.asInt();
  }

  bool GetBool(const std::string &key)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &value = GetValue(key);

    if (!value.isBool()) throw std::runtime_error("Parameter " + key + " is not a bool.");

    return value.asBool();
  }

  double GetDouble(const std::string &key)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &value = GetValue(key);

    if (!value.isDouble()) throw std::runtime_error("Parameter " + key + " is not a double.");

    return value.asDouble();
  }

 private:
  void Load(const std::string &file_path)
  {
    std::ifstream ifs(file_path);
    if (!ifs)
      throw std::runtime_error("JSON file " + file_path + " could not be opened.");

    Json::Reader reader;
    if (!reader.parse(ifs, root_))
      throw std::runtime_error("JSON file " + file_path + " could not be parsed, please check file content.");
  }

  const Json::Value &GetValue(const std::string &key)
  {
    if (root_.isNull()) Load(kDefaultFilePath);

    const Json::Value &value = root_[key];
    if (value.isNull())
      throw std::runtime_error("Parameter " + key + " not found in localconfig.");

    return value;
  }

  std::mutex mutex_;
  Json::Value root_;
};
}  // namespace

namespace vcc
{
namespace localconfig
{
void TestInit(const std::string &file_path) { LocalConfig::Instance().TestInit(file_path); }

std::string GetString(const std::string &key) { return LocalConfig::Instance().GetString(key); }

int GetInt(const std::string &key) { return LocalConfig::Instance().GetInt(key); }

bool GetBool(const std::string &key) { return LocalConfig::Instance().GetBool(key); }

double GetDouble(const std::string &key) { return LocalConfig::Instance().GetDouble(key); }

}  // namespace localconfig
}  // namespace vcc
