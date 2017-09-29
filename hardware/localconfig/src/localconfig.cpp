/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <vcc/local_config_reader.h>
#include <vcc/localconfig.h>

#include <json/json.h>

namespace
{
const char *kDefaultFilePath = "/oem_config/localconfig/localconfig.json";
}

namespace vcc
{
const vcc::LocalConfigReaderInterface *LocalConfigDefault()
{
  static vcc::LocalConfigFileReader instance(kDefaultFilePath);
  return &instance;
}
}  // namespace vcc
