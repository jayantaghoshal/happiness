#include <sys/stat.h>
#include <stdexcept>
#include "carconfig.h"
#include "carconfig_reader.h"
#include "carconfig_base.h"

namespace  // Anonymous namespace for carconfig internal stuff..
{
CarConfigFileReader cc = CarConfigFileReader();

bool fileExists(const std::string& filePath)
{
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}
}  // namespace::anonymous

namespace carconfig
{
std::uint8_t getValue(std::uint32_t parameterNumber) { return cc.getValue(parameterNumber); }
std::uint8_t getRawValue(std::uint32_t parameterNumber) { return cc.getRawValue(parameterNumber); }
bool isConfigured() { return emptyFileExists(carconfig_configured_filename); }
}  // namespace::carconfig
