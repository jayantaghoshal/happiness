/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 \*===========================================================================*/

#ifndef VCC_LOCALCONFIG_H_
#define VCC_LOCALCONFIG_H_

#include <string>

namespace vcc {
namespace localconfig {

/**
 * Get the value of a local config string parameter.
 *
 * Usage:
 *   const std::string my_string = vcc::localconfig::GetValueString("MyLcfgValue1");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return String with the requested parameter value.
 */
std::string GetValueString(const std::string &key);

/**
 * Get the value of a local config int parameter.
 *
 * Usage:
 *   const int my_int = vcc::localconfig::GetValueInt("MyLcfgValue2");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return int with the requested parameter value.
 */
int GetValueInt(const std::string &key);

/**
 * Get the value of a local config bool parameter.
 *
 * Usage:
 *   const bool my_bool = vcc::localconfig::GetValueBool("MyLcfgValue3");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return bool with the requested parameter value.
 */
bool GetValueBool(const std::string &key);

/**
 * Get the value of a local config double parameter.
 *
 * Usage:
 *   const double my_double = vcc::localconfig::GetValueDouble("MyLcfgValue4");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return double with the requested parameter value.
 */
double GetValueDouble(const std::string &key);

}  // namespace localconfig
}  // namespace vcc

#endif  // VCC_LOCALCONFIG_H_
