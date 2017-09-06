/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef CEDRIC_LOCALCONFIG_H
#define CEDRIC_LOCALCONFIG_H

#include <string>

namespace cedric
{
namespace core
{
namespace localconfig
{
/**
   Initialize the use of cedric localconfig
   Done once per application
   @return true if OK, false if failed
*/
bool init();

template <typename T>
const T *getValue(const std::string &key);

}
}
}

#endif  // CEDRIC_LOCALCONFIG_H
