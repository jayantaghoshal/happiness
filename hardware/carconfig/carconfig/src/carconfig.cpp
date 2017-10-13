#include <sys/stat.h>
#include "carconfig.h"
#include "carconfig_reader.h"
#include "carconfig_base.h"

using namespace Carconfig_base;

namespace  // Anonymous namespace for carconfig internal stuff..
{
    CarConfigReader cc = CarConfigReader();
}  // namespace::anonymous

namespace carconfig
{
std::uint8_t getValue(std::uint32_t parameterNumber) { return cc.getValue(parameterNumber); }
std::uint8_t getRawValue(std::uint32_t parameterNumber) { return cc.getRawValue(parameterNumber); }
bool isConfigured() { return fileExists(carconfig_configured_filename); }
}  // namespace::carconfig
