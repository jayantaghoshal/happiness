/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "entry_point_parser.h"
#include <tinyxml2.h>
#include <string>
#include <utility>
#include <vector>

#define LOG_TAG "CloudD"
#include <cutils/log.h>

namespace Connectivity {
namespace EntryPointParser {
EntryPointParser::EntryPoint parse(const char* const data_nullterminated) {
    // TODO: Verify the XML signature, [IHU-14218]

    tinyxml2::XMLDocument doc;

    tinyxml2::XMLElement* client_uri_element = nullptr;
    tinyxml2::XMLElement* host_uri_element = nullptr;
    tinyxml2::XMLElement* port_element = nullptr;
    tinyxml2::XMLElement* signal_service_uri_element = nullptr;

    ALOGV(LOG_TAG, "Parsing entrypoint XML data");
    doc.Parse(data_nullterminated);
    tinyxml2::XMLElement* entry_point_element = doc.GetDocument()->FirstChildElement("entry_point");
    if (entry_point_element != nullptr) {
        ALOGV(LOG_TAG, "Found entrypoint element in XML data");

        client_uri_element = entry_point_element->FirstChildElement("client_uri");
        if (client_uri_element == nullptr) {
            ALOGE(LOG_TAG, "Client URI element not found in entrypoint XML data");
        }

        host_uri_element = entry_point_element->FirstChildElement("host");
        if (host_uri_element == nullptr) {
            ALOGE(LOG_TAG, "Host URI element not found in entrypoint XML data");
        }

        port_element = entry_point_element->FirstChildElement("port");
        if (port_element == nullptr) {
            ALOGE(LOG_TAG, "Port element not found in entrypoint XML data");
        }

        signal_service_uri_element = entry_point_element->FirstChildElement("signal_service_uri");
        if (client_uri_element == nullptr) {
            ALOGD(LOG_TAG, "Signal Service URI element not found in entrypoint XML data");
        }
    }

    return EntryPointParser::EntryPoint{
            client_uri_element != nullptr ? client_uri_element->GetText() : "",
            host_uri_element != nullptr ? host_uri_element->GetText() : "",
            port_element != nullptr ? std::stoi(port_element->GetText()) : -1,
            signal_service_uri_element != nullptr ? signal_service_uri_element->GetText() : ""};
}
}
}
