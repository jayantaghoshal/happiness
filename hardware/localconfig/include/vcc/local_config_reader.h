#ifndef VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_SRC_LOCAL_CONFIG_READER_H_
#define VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_SRC_LOCAL_CONFIG_READER_H_

#include "local_config_reader_interface.h"

#include <json/json.h>
#include <fstream>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace vcc
{
// Extracting values is thread safe. Loads default configuration when first accessed. Tests may
// load a different configuration. This should be done prior to accessing any node_value if loading of
// default configuration is to be avoided.
class LocalConfigReader : public LocalConfigReaderInterface
{
 public:
  void TestInit(const std::string &file_path)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    Load(file_path);
  }

  std::string GetString(const std::initializer_list<std::string> &keys);
  int GetInt(const std::initializer_list<std::string> &keys);
  bool GetBool(const std::initializer_list<std::string> &keys);
  double GetDouble(const std::initializer_list<std::string> &keys);
  std::vector<std::string> GetStringArray(const std::initializer_list<std::string> &keys);

 private:
  void Load(const std::string &file_path);

  const Json::Value &GetValue(const std::initializer_list<std::string> &keys);

  std::mutex mutex_;
  Json::Value root_;
};
}
#endif /* VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_SRC_LOCAL_CONFIG_READER_H_ */
