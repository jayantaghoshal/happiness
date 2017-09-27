#ifndef VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_
#define VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_

#include <initializer_list>
#include <string>
#include <vector>

namespace vcc
{
/*
 * Describes contract for using LCFG without relying on global state
 */
struct LocalConfigReaderInterface
{
  virtual ~LocalConfigReaderInterface() = default;

  // actual backend reaching methods

  /**
   * Get the value of a local config string parameter.
   *
   * Usage:
   *   const std::string my_string = _lcfg->GetString("MyLcfgValue1");
   *
   * If the key is not found a std::runtime_error exception will be thrown.
   *
   * @param key string that identifies a local config value
   * @return String with the requested parameter value.
   */
  virtual std::string GetString(const std::initializer_list<std::string> &keys_path) = 0;

  /**
   * Get the value of a local config int parameter.
   *
   * Usage:
   *   const int my_int = _lcfg->GetInt("MyLcfgValue2");
   *
   * If the key is not found a std::runtime_error exception will be thrown.
   *
   * @param key string that identifies a local config value
   * @return int with the requested parameter value.
   */
  virtual int GetInt(const std::initializer_list<std::string> &keys_path) = 0;

  /**
   * Get the value of a local config bool parameter.
   *
   * Usage:
   *   const bool my_bool = _lcfg->GetBool("MyLcfgValue3");
   *
   * If the key is not found a std::runtime_error exception will be thrown.
   *
   * @param key string that identifies a local config value
   * @return bool with the requested parameter value.
   */
  virtual bool GetBool(const std::initializer_list<std::string> &keys_path) = 0;

  /**
   * Get the value of a local config double parameter.
   *
   * Usage:
   *   const double my_double = _lcfg->GetDouble("MyLcfgValue4");
   *
   * If the key is not found a std::runtime_error exception will be thrown.
   *
   * @param key string that identifies a local config value
   * @return double with the requested parameter value.
   */
  virtual double GetDouble(const std::initializer_list<std::string> &keys_path) = 0;

  /**
   * Get the value of a local config array of string parameter.
   *
   * Usage:
   *   const std::vector<string> my_strings = _lcfg->GetStringArray("MyLcfgValue5");
   *
   * If the key is not found a std::runtime_error exception will be thrown.
   *
   * @param key string that identifies a local config value
   * @return vector of strings with requested parameter value
   */
  virtual std::vector<std::string> GetStringArray(const std::initializer_list<std::string> &keys_path) = 0;

  // conveniance adapters

  template <class... T>
  std::string GetString(const T &... keys)
  {
    return GetString({keys...});
  }

  template <class... T>
  int GetInt(const T &... keys)
  {
    return GetInt({keys...});
  }

  template <class... T>
  bool GetBool(const T &... keys)
  {
    return GetBool({keys...});
  }

  template <class... T>
  double GetDouble(const T &... keys)
  {
    return GetDouble({keys...});
  }

  template <class... T>
  std::vector<std::string> GetStringArray(const T &... keys)
  {
    return GetStringArray({keys...});
  }
};

} /* namespace vcc */

#endif /* VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_ */
