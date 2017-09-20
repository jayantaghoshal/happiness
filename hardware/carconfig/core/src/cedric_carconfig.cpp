#include <cedric_carconfig.h>
#include <sys/stat.h>
#include <stdexcept>
#include "carconfig_reader.h"

namespace  // Anonymous namespace for carconfig internal stuff..
{
CarConfigFileReader cc = CarConfigFileReader();

bool fileExists(std::string filePath)
{
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}
}  // namespace::anonymous

namespace cedric
{
namespace core
{
namespace carconfig
{
const std::string kConfiguredFileName = "/var/lib/carconfig/CARCONFIG_CONFIGURED";

namespace v1
{
void init()
{
    if (!cc.isRead())
    {
        cc.read();
    }
}
}

inline namespace v0
{
int init()
{
    try
    {
        v1::init();
    }
    catch (std::runtime_error const&)
    {
        return 1;
    }
    return 0;
}
}

std::uint8_t getValue(std::uint32_t parameterNumber) { return cc.getValue(parameterNumber); }
std::uint8_t getRawValue(std::uint32_t parameterNumber) { return cc.getRawValue(parameterNumber); }
bool isConfigured() { return fileExists(kConfiguredFileName); }
}  // namespace::carconfig
}  // namespace::core
}  // namespace::cedric
