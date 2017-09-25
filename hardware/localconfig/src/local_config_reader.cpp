#include "vcc/local_config_reader.h"

using namespace vcc;

namespace
{
const char *kDefaultFilePath = "/oem_config/localconfig/localconfig.json";

std::string GetKeyString(const std::initializer_list<std::string> &keys)
{
  std::stringstream key_string;
  for (auto &k : keys)
  {
    key_string << "[\"" << k << "\"]";
  }
  return key_string.str();
}
}

std::string LocalConfigReader::GetString(const std::initializer_list<std::string> &keys)
{
  std::unique_lock<std::mutex> lock(mutex_);
  const Json::Value &node_value = GetValue(keys);
  if (!node_value.isString()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a string.");

  return node_value.asString();
}

void LocalConfigReader::Load(const std::string &file_path)
{
  std::ifstream ifs(file_path);
  if (!ifs) throw std::runtime_error("JSON file " + file_path + " could not be opened.");

  Json::Reader reader;
  if (!reader.parse(ifs, root_))
    throw std::runtime_error("JSON file " + file_path + " could not be parsed, please check file content.");
}

const Json::Value &LocalConfigReader::GetValue(const std::initializer_list<std::string> &keys)
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

int LocalConfigReader::GetInt(const std::initializer_list<std::string> &keys)
{
  std::unique_lock<std::mutex> lock(mutex_);
  const Json::Value &node_value = GetValue(keys);

  if (!node_value.isInt()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not an integer.");

  return node_value.asInt();
}

bool LocalConfigReader::GetBool(const std::initializer_list<std::string> &keys)
{
  std::unique_lock<std::mutex> lock(mutex_);
  const Json::Value &node_value = GetValue(keys);

  if (!node_value.isBool()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a bool.");

  return node_value.asBool();
}

double LocalConfigReader::GetDouble(const std::initializer_list<std::string> &keys)
{
  std::unique_lock<std::mutex> lock(mutex_);
  const Json::Value &node_value = GetValue(keys);

  if (!node_value.isDouble()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a double.");

  return node_value.asDouble();
}

std::vector<std::string> LocalConfigReader::GetStringArray(const std::initializer_list<std::string> &keys)
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
