#ifndef VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_SRC_LOCAL_CONFIG_READER_H_
#define VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_SRC_LOCAL_CONFIG_READER_H_

#include "local_config_reader_interface.h"

#include <json/json.h>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>

namespace vcc {
// Extracting values is thread safe. Loads default configuration when first accessed. Tests may
// load a different configuration. This should be done prior to accessing any node_value if loading of
// default configuration is to be avoided.
class LocalConfigReader : public LocalConfigReaderInterface {
  public:
    using LazyLoader = std::function<void(Json::Value *root)>;

    explicit LocalConfigReader(LazyLoader loader) : loader_(std::move(loader)) {}
    LocalConfigReader(const LocalConfigReader &) = delete;
    const LocalConfigReader &operator=(const LocalConfigReader &) = delete;

    std::string GetString(std::initializer_list<std::string> keys) const override;
    int GetInt(std::initializer_list<std::string> keys) const override;
    bool GetBool(std::initializer_list<std::string> keys) const override;
    double GetDouble(std::initializer_list<std::string> keys) const override;
    std::vector<std::string> GetStringArray(std::initializer_list<std::string> keys) const override;

    void Preload();

  protected:
    static void LoadFile(const std::string &file_path, Json::Value *value);

  private:
    const Json::Value &GetJsonValue(std::initializer_list<std::string> keys) const;

    LazyLoader const loader_;
    mutable std::mutex mutex_;
    mutable Json::Value root_;
};

class LocalConfigFileReader : public LocalConfigReader {
    using base = LocalConfigReader;

  public:
    explicit LocalConfigFileReader(std::string file_path);
};

class LocalConfigStaticContentReader : public LocalConfigReader {
    using base = LocalConfigReader;

  public:
    explicit LocalConfigStaticContentReader(std::string json);
};
}  // namespace vcc

#endif /* VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_SRC_LOCAL_CONFIG_READER_H_ */
