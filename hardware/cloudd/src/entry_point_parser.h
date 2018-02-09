/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <functional>
#include <future>
#include <memory>
#include <string>

namespace Connectivity {
namespace EntryPointParser {
struct EntryPoint {
    std::string client_uri;
    std::string host;
    int port;
};
/*
 * Tries to parse out an EntryPoint from a XML document. Throws runtime error if it fails.
 * @parm data XML data containing the EntryPoint
 * @return An EntryPoint populated with the data contained within the XML data.
 */
EntryPoint parse(const char* const data) throw(std::runtime_error);
}
}  // namespace Connectivity