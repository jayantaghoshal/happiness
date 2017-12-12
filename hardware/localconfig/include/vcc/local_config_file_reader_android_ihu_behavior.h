/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_FILE_READER_WITH_FALLBACK_H_
#define VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_FILE_READER_WITH_FALLBACK_H_

#include "local_config_reader.h"

namespace vcc {
class LocalConfigFileReaderAndroidIhuBehavior : public LocalConfigReader {
    using base = LocalConfigReader;

  public:
    LocalConfigFileReaderAndroidIhuBehavior(std::string production_path, std::string fallback_path,
                                            const char* environment_override);

  private:
    static void LoadWithFallback(const std::string& production_path, const std::string& fallback_path,
                                 Json::Value* value);
};

} /* namespace vcc */

#endif /* VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_FILE_READER_WITH_FALLBACK_H_ */
