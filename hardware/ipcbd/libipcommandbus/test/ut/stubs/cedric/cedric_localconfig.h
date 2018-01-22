/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <string>

class LocalconfigStub {
  public:
    LocalconfigStub();

    const int* getInt = 0;
    const double* getDouble = 0;
    std::string getString = "";
};

extern LocalconfigStub local_config_stub;

namespace cedric {
namespace core {
namespace localconfig {

bool init();

template <typename T>
const T* getValue(const std::string& key);
}
}
}
