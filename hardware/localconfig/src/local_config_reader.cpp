#include "vcc/local_config_reader.h"

#include <fstream>

using namespace vcc;

namespace
{
std::string GetKeyString(std::initializer_list<std::string> keys)
{
  std::stringstream key_string;
  for (auto &k : keys)
  {
    key_string << "[\"" << k << "\"]";
  }
  return key_string.str();
}
}

void vcc::LocalConfigReader::Preload()
{
  std::unique_lock<std::mutex> lock(mutex_);
  loader_(&root_);
}

std::string LocalConfigReader::GetString(std::initializer_list<std::string> keys) const
{
  std::unique_lock<std::mutex> lock(mutex_);
  const Json::Value &node_value = GetValue(keys);
  if (!node_value.isString()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a string.");

  return node_value.asString();
}

const Json::Value &LocalConfigReader::GetValue(std::initializer_list<std::string> keys) const
{
  if (root_.isNull()) loader_(&root_);

  const Json::Value *current_node = &root_;
  const Json::Value *node_value = nullptr;
  for (auto &key : keys)
  {
    node_value = &(*current_node)[key];
    if (node_value->isNull())
      throw std::runtime_error("Parameter " + GetKeyString(keys) + " not found in localconfig.");
    current_node = node_value;
  }
  return *node_value;
}

int LocalConfigReader::GetInt(std::initializer_list<std::string> keys) const
{
  std::unique_lock<std::mutex> lock(mutex_);
  const Json::Value &node_value = GetValue(keys);

  if (!node_value.isInt()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not an integer.");

  return node_value.asInt();
}

bool LocalConfigReader::GetBool(std::initializer_list<std::string> keys) const
{
  std::unique_lock<std::mutex> lock(mutex_);
  const Json::Value &node_value = GetValue(keys);

  if (!node_value.isBool()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a bool.");

  return node_value.asBool();
}

double LocalConfigReader::GetDouble(std::initializer_list<std::string> keys) const
{
  std::unique_lock<std::mutex> lock(mutex_);
  const Json::Value &node_value = GetValue(keys);

  if (!node_value.isDouble()) throw std::runtime_error("Parameter " + GetKeyString(keys) + " is not a double.");

  return node_value.asDouble();
}

std::vector<std::string> LocalConfigReader::GetStringArray(std::initializer_list<std::string> keys) const
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

void vcc::LocalConfigReader::LoadFile(std::string file_path, Json::Value *value)
{
  std::ifstream ifs(file_path);
  if (!ifs) throw std::runtime_error("JSON file " + file_path + " could not be opened.");

  Json::Reader reader;
  if (!reader.parse(ifs, *value))
    throw std::runtime_error("JSON file " + file_path + " could not be parsed, please check file content.");
}

vcc::LocalConfigFileReader::LocalConfigFileReader(std::string file_path)
    : base::LocalConfigReader([file_path](Json::Value *value) { LoadFile(file_path, value); })
{
}

vcc::LocalConfigStaticContentReader::LocalConfigStaticContentReader(std::string json)
    : base([json](Json::Value *value) {
      Json::Reader reader;
      if (!reader.parse(json, *value)) throw std::runtime_error("JSON text could not be parsed, please check content.");
    })
{
}
