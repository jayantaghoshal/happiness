#include "vcc/local_config_file_reader_with_fallback.h"

#define LOG_TAG "localconfig"
#include <cutils/log.h>

vcc::LocalConfigFileReaderWithFallback::LocalConfigFileReaderWithFallback(std::string production_path,
                                                                          std::string fallback_path)
    : base([production_path, fallback_path](Json::Value *value) {
      try
      {
        LoadFile(production_path, value);
      }
      catch (std::exception &ex)
      {
        ALOGW("Failed to open file %s, for engineering purposes will attempt to fall back to %s",
              production_path.c_str(), fallback_path.c_str());
        LoadFile(fallback_path, value);
      }
    })
{
}
