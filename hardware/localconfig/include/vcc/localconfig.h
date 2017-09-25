/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 \*===========================================================================*/

#ifndef VCC_LOCALCONFIG_H_
#define VCC_LOCALCONFIG_H_

#include <vcc/local_config_reader_interface.h>
#include <string>
#include <vector>

namespace vcc
{
namespace localconfig
{
/**
 * Get default instance of LocalConfigReader
 *
 * This method allows to inject LocalConfig dependency cleanly, by keeping interface as a member,
 * and initializing it with ObjectDependingOnLcfg(LocalConfigReaderInterface * lcfg = vcc::localconfig::Default())
 *
 * But also allows to easilly inject a Mock of the interface, to prevent FileSystem and global data dependency to
 * spread across all unit tests of LCFG dependent components.
 */
const vcc::LocalConfigReaderInterface *Default();

/**
 * Get the value of a local config string parameter.
 *
 * Usage:
 *   const std::string my_string = vcc::localconfig::GetString("MyLcfgValue1");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return String with the requested parameter value.
 */
std::string GetString(const std::initializer_list<std::string> &keys);

template <class... T>
std::string GetString(const T &... keys)
{
  return GetString({keys...});
}

/**
 * Get the value of a local config int parameter.
 *
 * Usage:
 *   const int my_int = vcc::localconfig::GetInt("MyLcfgValue2");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return int with the requested parameter value.
 */
int GetInt(const std::initializer_list<std::string> &keys);

template <class... T>
int GetInt(const T &... keys)
{
  return GetInt({keys...});
}

/**
 * Get the value of a local config bool parameter.
 *
 * Usage:
 *   const bool my_bool = vcc::localconfig::GetBool("MyLcfgValue3");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return bool with the requested parameter value.
 */
bool GetBool(const std::initializer_list<std::string> &keys);

template <class... T>
bool GetBool(const T &... keys)
{
  return GetBool({keys...});
}

/**
 * Get the value of a local config double parameter.
 *
 * Usage:
 *   const double my_double = vcc::localconfig::GetDouble("MyLcfgValue4");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return double with the requested parameter value.
 */
double GetDouble(const std::initializer_list<std::string> &keys);

template <class... T>
double GetDouble(const T &... keys)
{
  return GetDouble({keys...});
}

/**
 * Get the value of a local config array of string parameter.
 *
 * Usage:
 *   const std::vector<string> my_strings = vcc::localconfig::GetStringArray("MyLcfgValue5");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return vector of strings with requested parameter value
 */

std::vector<std::string> GetStringArray(const std::initializer_list<std::string> &keys);

template <class... T>
std::vector<std::string> GetStringArray(const T &... keys)
{
  return GetStringArray({keys...});
}

}  // namespace localconfig
}  // namespace vcc

#endif  // VCC_LOCALCONFIG_H_
