/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <vcc/localconfig.h>
#include <vcc/localconfig_test.h>

#include <json/json.h>

#include <fstream>
#include <initializer_list>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
const char *kDefaultFilePath = "/oem_config/localconfig/localconfig.json";

// Singleton for accessing localconfig.
//
// Exctracting values is thread safe. Loads default configuration when first accessed. Tests may
// load a different configuration. This should be done prior to accessing any node_value if loading of
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

  std::string GetString(const std::initializer_list<std::string> &keys)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &node_value = GetValue(keys);

    if (!node_value.isString()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a string.");

    return node_value.asString();
  }

  int GetInt(const std::initializer_list<std::string> &keys)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &node_value = GetValue(keys);

    if (!node_value.isInt()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not an integer.");

    return node_value.asInt();
  }

  bool GetBool(const std::initializer_list<std::string> &keys)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &node_value = GetValue(keys);

    if (!node_value.isBool()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a bool.");

    return node_value.asBool();
  }

  double GetDouble(const std::initializer_list<std::string> &keys)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &node_value = GetValue(keys);

    if (!node_value.isDouble()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a double.");

    return node_value.asDouble();
  }

  std::vector<std::string> GetStringArray(const std::initializer_list<std::string> &keys)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    const Json::Value &array = GetValue(keys);

    if (!array.isArray()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not an array.");

    std::vector<std::string> values;
    for (const Json::Value &val : array)
    {
      if (!val.isString())
        throw std::runtime_error("List node_value for parameter " + GetKeyString(keys) + " is not a string.");
      values.push_back(val.asString());
    }

    return values;
  }

 private:
  void Load(const std::string &file_path)
  {
    std::ifstream ifs(file_path);
    if (!ifs) throw std::runtime_error("JSON file " + file_path + " could not be opened.");

    Json::Reader reader;
    if (!reader.parse(ifs, root_))
      throw std::runtime_error("JSON file " + file_path + " could not be parsed, please check file content.");
  }

  const Json::Value &GetValue(const std::initializer_list<std::string> &keys)
  {
    if (root_.isNull()) Load(kDefaultFilePath);

    Json::Value *current_node = &root_;
    Json::Value *node_value = nullptr;
    for (auto &key : keys)
    {
      node_value = &(*current_node)[key];
      if (node_value->isNull())
        throw std::runtime_error("Parameter " + GetKeyString(keys) + " not found in localconfig.");
      current_node = node_value;
    }
    return *node_value;
  }

  static std::string GetKeyString(const std::initializer_list<std::string> &keys)
  {
    std::stringstream key_string;
    for (auto &k : keys)
    {
      key_string << "[\"" << k << "\"]";
    }
    return key_string.str();
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
std::string GetString(const std::initializer_list<std::string> &keys)
{
  return LocalConfig::Instance().GetString(keys);
}

int GetInt(const std::initializer_list<std::string> &keys) { return LocalConfig::Instance().GetInt(keys); }
bool GetBool(const std::initializer_list<std::string> &keys) { return LocalConfig::Instance().GetBool(keys); }
double GetDouble(const std::initializer_list<std::string> &keys) { return LocalConfig::Instance().GetDouble(keys); }
std::vector<std::string> GetStringArray(const std::initializer_list<std::string> &keys)
{
  return LocalConfig::Instance().GetStringArray(keys);
}

}  // namespace localconfig
}  // namespace vcc
