/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "cedric_localconfig.h"

#include <string>

LocalconfigStub::LocalconfigStub()
{
}

namespace cedric
{
namespace core
{
namespace localconfig
{
bool init()
{
    return true;
}

template <>
const int* getValue(const std::string& key)
{
    (void)key;
    return local_config_stub.getInt;
}

template <>
const double* getValue(const std::string& key)
{
    (void)key;
    return local_config_stub.getDouble;
}

template <>
const std::string* getValue(const std::string& key)
{
    (void)key;
    return const_cast<const std::string*>(&(local_config_stub.getString));
}
}
}
}
