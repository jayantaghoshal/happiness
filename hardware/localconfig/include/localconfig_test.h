/*===========================================================================*\
 * Copyright 2017 Volvo Car Corporation, All Rights Reserved.
 \*==========================================================================*/

#ifndef VCC_LOCALCONFIGTEST_H
#define VCC_LOCALCONFIGTEST_H

#include <string>
#include <vector>

namespace vcc
{
namespace localconfig
{

/**
   Initialize the use of localconfig in a test scenario

   @note This function should only be used in test scenarios.

   @note If the file can't be loaded a std::runtime_error exception will be thrown.

   @param filepath Absolute path to localconfig json file.
*/
void testInit(const char *filepath);

} // namespace localconfig
} // namespace vcc

#endif // VCC_LOCALCONFIG_H
