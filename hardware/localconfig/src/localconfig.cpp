/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <vcc/localconfig.h>

#include "vcc/local_config_file_reader_android_ihu_behavior.h"

namespace
{
const char *kProductionFilePath = "/oem_config/localconfig/localconfig.json";
const char *kDefaultFilePath = "/vendor/etc/localconfig/localconfig.json";
}

namespace vcc
{
const vcc::LocalConfigReaderInterface *LocalConfigDefault()
{
  // supports environment override, and if it is missing tries production value, with fallback to vendor/engineering
  static vcc::LocalConfigFileReaderAndroidIhuBehavior instance(kProductionFilePath, kDefaultFilePath,
                                                               "VCC_LOCALCONFIG_PATH");
  return &instance;
}
}  // namespace vcc
