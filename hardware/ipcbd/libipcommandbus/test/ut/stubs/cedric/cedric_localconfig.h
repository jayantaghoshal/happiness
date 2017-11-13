/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef CEDRIC_LOCALCONFIG_STUB_H
#define CEDRIC_LOCALCONFIG_STUB_H

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

#endif  // MOCK_CEDRIC_LOCALCONFIG_STUB_H
