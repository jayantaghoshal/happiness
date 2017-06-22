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
// Holds localconfig values that has been read from a JSON file.
Json::Value json_root;

// TODO: Change to non hardcoded filepath
//       Perhaps use getenv() like the
//       now deprecated ihu_local_config does
const char *default_filepath = "/oem_config/localconfig/localconfig.json";
}

namespace vcc {
namespace localconfig {
void init() { return initWithFilepath(default_filepath); }

void initWithFilepath(const char *filepath) {
  std::ifstream ifs(filepath);
  if (!ifs) {
    throw std::runtime_error{"JSON file " + std::string(filepath) + " could not be found."};
  }

  std::string doc;
  std::getline(ifs, doc, (char)EOF);
  Json::Reader reader;
  bool ok = reader.parse(doc, json_root);
  if (!ok) {
    throw std::runtime_error{"JSON file " + std::string(filepath) + " could not be parsed, please check file content."};
  }
}

const Json::Value getValue(const std::string &key) {
  const Json::Value value = json_root[key];
  if (value.isNull()) {
    throw std::runtime_error{"Parameter " + key + " not found in localconfig."};
  }
  return value;
}

const std::string getValueString(const std::string &key) { return getValue(key).asString(); }

int getValueInt(const std::string &key) { return getValue(key).asInt(); }

bool getValueBool(const std::string &key) { return getValue(key).asBool(); }

double getValueDouble(const std::string &key) { return getValue(key).asDouble(); }

}  // namespace localconfig
}  // namespace vcc
