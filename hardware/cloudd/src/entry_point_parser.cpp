#include "entry_point_parser.h"
#include <tinyxml2.h>
#include <string>
#include <utility>
#include <vector>

namespace Connectivity {
namespace EntryPointParser {
EntryPointParser::EntryPoint parse(const char* const data_nullterminated) {
    // TODO: Verify the XML signature, [IHU-14218]

    tinyxml2::XMLDocument doc;
    doc.Parse(data_nullterminated);
    tinyxml2::XMLElement* entry_point_element = doc.GetDocument()->FirstChildElement("entry_point");
    if (entry_point_element == nullptr) throw std::runtime_error("entry_point not found");
    tinyxml2::XMLElement* host_uri_element = entry_point_element->FirstChildElement("host");
    if (host_uri_element == nullptr) throw std::runtime_error("host not found");
    tinyxml2::XMLElement* port_element = entry_point_element->FirstChildElement("port");
    if (port_element == nullptr) throw std::runtime_error("port not found");
    const std::string port_str = std::string{port_element->GetText()};
    const int port = std::stoi(port_str);

    return EntryPointParser::EntryPoint{std::string{host_uri_element->GetText()}, port};
}
}
}
