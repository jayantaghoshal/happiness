#pragma once

#include <functional>
#include <future>
#include <memory>
#include <string>

namespace Connectivity {
namespace EntryPointParser {
struct EntryPoint {
    std::string host;
    int port;
};
EntryPoint parse(const char* const data);
}
}  // namespace Connectivity