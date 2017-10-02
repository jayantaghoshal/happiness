/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <vcc/local_config_file_reader_with_fallback.h>
#include <vcc/localconfig.h>

namespace
{
const char *kProductionFilePath = "/oem_config/localconfig/localconfig.json";
const char *kDefaultFilePath = "/vendor/etc/localconfig/localconfig.json";
}

namespace vcc
{
const vcc::LocalConfigReaderInterface *LocalConfigDefault()
{
  static vcc::LocalConfigFileReaderWithFallback instance(kProductionFilePath, kDefaultFilePath);
  return &instance;
}
}  // namespace vcc
