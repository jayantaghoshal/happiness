#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_VPNENTRYPOINTPARSER_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_VPNENTRYPOINTPARSER_H

#include <functional>
#include <future>
#include <memory>
#include <string>

namespace Connectivity {
namespace VPNEntryPointParser {
struct EntryPoint {
    std::string host;
    int port;
};
EntryPoint parse(const char* const data);
}
}  // namespace Connectivity
#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_VPNENTRYPOINTPARSER_H