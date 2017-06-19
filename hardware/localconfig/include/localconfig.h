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
   Initialize the use of localconfig
   Done once per application
   @return true if OK, false if failed
*/
bool init();

/**
   Initialize the use of localconfig in a test scenario
   Done once per application

   @note This function should only be used in test scenarios,
         normally the init() function should be used.

   @param filepath Absolute path to localconfig json file.
   @return true if OK, false if failed
*/
bool initWithFilepath(const char *filepath);

/**
 * Get the value of a local config parameter
 *
 * Usage:
 *   const std::string *myVal1 = vcc::localconfig::getValue<std::string>("MyLcfgValue1");
 *   ...
 *   const int *myVal2 = vcc::localconfig::getValue<int>("MyLcfgValue2");
 *
 * The template function supports the following types: string, int, bool, double
 *
 * @param key string that identifies a local config value
 * @return A pointer to the requested parameter value (in the type that was used to instantiate the function)
 *         or nullptr if the key does not exist among local config parameters.
 *         The object pointed to is owned by localconfig so there is no need to delete or manage the object pointed to.
 */
template <typename T>
const T *getValue(const std::string &key);

/**
 * Get (nested) array of values of local config parameters
 *
 * Usage:
 *
 * "NETWORK_HANDLER_FW":
 * {
 *   "MANGLE_TABLE":
 *   {
 *     "CHAINS":[
 *       "INCOMMING",
 *       "SCTP_scan",
 *       "SYNFLOOD",
 *       "UDP_scan"
 *     ]
 *   }
 * }
 *
 * auto temp =
 * vcc::localconfig::getVector<std::string>("NETWORK_HANDLER_FW","MANGLE_TABLE", "CHAINS");
 * for(std::string it : temp)
 * {
 *   std::cout << it << std::endl;
 * }
 *
 * The template function supports the following types: string, int, bool, double
 *
 * @param key string depth_1
 * @param key string depth_2
 * @param key string depth_3
 * @return A vector of requested type. Empty vector will be returned in case of an error.
 */
template <typename T>
const std::vector<T> getVector(const std::string &key1, const std::string &key2, const std::string &key3);

/**
 * Get array of values of local config parameters
 *
 * Usage (example):
 *
 * "my_array": [1,2,3,4]
 *
 * std::vector<int> temp = vcc::localconfig::getVector<int>("my_array");
 * for(auto it : temp)
 * {
 *   std::cout << it << std::endl;
 * }
 *
 * The template function supports the following types: string, int, bool, double
 *
 * @param key The key for the requested local config array
 * @return A vector of requested type. Empty vector will be returned in case of an error.
 */

template <typename T>
const std::vector<T> getVector(const std::string &key);

/**
 * Extract an array of JSON objects.
 *
 * Usage:
 *
 * "MyArray":
 * [
 *     {
 *         "Name": "John Doe",
 *         "Age" : 31
 *     },
 *     {
 *         "Name": "Jane Doe",
 *         "Age" : 55
 *     }
 * ]
 *
 * auto result = vcc::localconfig::getArray<std::tuple<std::string, int>("MyArray", "Name", "Age");
 * for (auto x: result)
 * {
 *     std::string name;
 *     int         age;
 *     std::tie(name, age) = x;
 *     std::cout << name << " is " << x << " years old\n";
 * }
 *
 * There are also two ways to skip parts of objects. Say we only want a list of names. We could either fetch only
 * the names:
 *
 * auto result = vcc::localconfig::getArray<std::tuple<std::string>("MyArray", "Name");
 *
 * or, just fetch everything and ignore the age field with std::tie>
 *
 * for (auto x: result)
 * {
 *     std::string name;
 *     std::tie(name, std::ignore) = x;
 *     std::cout << name << std::endl;
 * }
 *
 * @param key The key for the requested local config array
 * @param elements A comma separated list of the element names in the local config object
 * @return a vector of tuples of user specified types.
 */
template <typename T, typename... Elements>
auto getArray(std::string &&key, Elements... elements) -> std::vector<T> const;
}  // namecpace localconfig
}  // namespace vcc

#endif  // VCC_LOCALCONFIG_H
