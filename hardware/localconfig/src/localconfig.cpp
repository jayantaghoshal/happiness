/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <vcc/local_config_reader.h>
#include <vcc/localconfig.h>
#include <vcc/localconfig_test.h>

namespace
{
vcc::LocalConfigReader &Instance()
{
  static vcc::LocalConfigReader instance;
  return instance;
}
}

namespace vcc
{
namespace localconfig
{
const vcc::LocalConfigReaderInterface *Default() { return &Instance(); }

void TestInit(const std::string &file_path) { Instance().TestInit(file_path); }

std::string GetString(const std::initializer_list<std::string> &keys) { return Instance().GetString(keys); }
int GetInt(const std::initializer_list<std::string> &keys) { return Instance().GetInt(keys); }
bool GetBool(const std::initializer_list<std::string> &keys) { return Instance().GetBool(keys); }
double GetDouble(const std::initializer_list<std::string> &keys) { return Instance().GetDouble(keys); }
std::vector<std::string> GetStringArray(const std::initializer_list<std::string> &keys)
{
  return Instance().GetStringArray(keys);
}

}  // namespace localconfig
}  // namespace vcc
