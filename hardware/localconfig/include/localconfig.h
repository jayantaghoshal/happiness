/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 \*===========================================================================*/

#ifndef VCC_LOCALCONFIG_H
#define VCC_LOCALCONFIG_H

#include <string>
#include <vector>

namespace vcc {
namespace localconfig {

/**
   Initialize the use of localconfig in a test scenario

   @note This function should only be used in test scenarios,
         normally the init() function should be used.

   @note If the file can't be loaded a std::runtime_error exception will be thrown.

   @param filepath Absolute path to localconfig json file.
*/
void initWithFilepath(const char *filepath);

/**
 * Get the value of a local config string parameter.
 *
 * Usage:
 *   const std::string myString = vcc::localconfig::getValueString("MyLcfgValue1");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return String with the requested parameter value.
 */
const std::string getValueString(const std::string &key);

/**
 * Get the value of a local config int parameter.
 *
 * Usage:
 *   const int myInt = vcc::localconfig::getValueInt("MyLcfgValue2");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return int with the requested parameter value.
 */
int getValueInt(const std::string &key);

/**
 * Get the value of a local config bool parameter.
 *
 * Usage:
 *   const bool myBool = vcc::localconfig::getValueBool("MyLcfgValue3");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return bool with the requested parameter value.
 */
bool getValueBool(const std::string &key);

/**
 * Get the value of a local config double parameter.
 *
 * Usage:
 *   const double myDouble = vcc::localconfig::getValueDouble("MyLcfgValue4");
 *
 * If the key is not found a std::runtime_error exception will be thrown.
 *
 * @param key string that identifies a local config value
 * @return double with the requested parameter value.
 */
double getValueDouble(const std::string &key);

}  // namespace localconfig
}  // namespace vcc

#endif  // VCC_LOCALCONFIG_H
