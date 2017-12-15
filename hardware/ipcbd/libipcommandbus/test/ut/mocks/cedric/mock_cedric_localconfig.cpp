/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "mock_cedric_localconfig.h"

#include <string>

MockLocalconfig::MockLocalconfig() {}

namespace cedric {
namespace core {
namespace localconfig {

bool init() { return g_mock_local_config->init(); }

template <>
const int* getValue(const std::string& key) {
    return g_mock_local_config->getValueInt(key);
}

template <>
const double* getValue(const std::string& key) {
    return g_mock_local_config->getValueDouble(key);
}

template <>
const std::string* getValue(const std::string& key) {
    return g_mock_local_config->getValueString(key);
}
}
}
}
