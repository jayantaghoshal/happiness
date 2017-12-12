/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef MOCK_CEDRIC_LOCALCONFIG_H
#define MOCK_CEDRIC_LOCALCONFIG_H

#include <gmock/gmock.h>
#include <string>

class MockLocalconfig {
  public:
    MockLocalconfig();

    MOCK_METHOD0(init, bool());

    MOCK_METHOD1(getValueInt, const int*(const std::string& key));
    MOCK_METHOD1(getValueDouble, const double*(const std::string& key));
    MOCK_METHOD1(getValueString, const std::string*(const std::string& key));
};

extern MockLocalconfig* g_mock_local_config;

namespace cedric {
namespace core {
namespace localconfig {

bool init();

template <typename T>
const T* getValue(const std::string& key);
}
}
}

#endif  // MOCK_CEDRIC_LOCALCONFIG_H
