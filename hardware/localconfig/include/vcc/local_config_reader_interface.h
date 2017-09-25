#ifndef VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_
#define VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_

namespace vcc
{
/*
 * Describes contract for using LCFG without relying on global state
 */
struct LocalConfigReaderInterface
{
 public:
  virtual ~LocalConfigReaderInterface() = default;
};

} /* namespace vcc */

#endif /* VENDOR_VOLVOCARS_HARDWARE_LOCALCONFIG_INCLUDE_VCC_LOCAL_CONFIG_READER_INTERFACE_H_ */
