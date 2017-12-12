/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "localconfig.h"

#include "vcc/local_config_file_reader_android_ihu_behavior.h"

namespace {
const char *kProductionFilePath = "/oem_config/localconfig/localconfig.json";
const char *kDefaultFilePath = "/vendor/etc/localconfig/localconfig.json";
const char *kEnvironmentVariableOverridingPath = "VCC_LOCALCONFIG_PATH";
}

namespace vcc {
const vcc::LocalConfigReaderInterface *LocalConfigDefault() {
    // supports environment override, and if it is missing tries production value, with fallback to vendor/engineering
    static vcc::LocalConfigFileReaderAndroidIhuBehavior instance(kProductionFilePath, kDefaultFilePath,
                                                                 kEnvironmentVariableOverridingPath);
    return &instance;
}
}  // namespace vcc
