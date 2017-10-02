#ifndef VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_FILE_READER_WITH_FALLBACK_H_
#define VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_FILE_READER_WITH_FALLBACK_H_

#include "local_config_reader.h"

namespace vcc
{
class LocalConfigFileReaderWithFallback : public LocalConfigReader
{
  typedef LocalConfigReader base;

 public:
  LocalConfigFileReaderWithFallback(std::string production_path, std::string fallback_path);
};

} /* namespace vcc */

#endif /* VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_FILE_READER_WITH_FALLBACK_H_ */
